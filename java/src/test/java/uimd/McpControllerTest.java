package uimd;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.ByteArrayInputStream;
import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.util.List;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.atomic.AtomicReference;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.node.ObjectNode;

import org.junit.jupiter.api.Test;

final class McpControllerTest
{
    private static final ObjectMapper JSON = new ObjectMapper();
    private static final int VIEWPORT_WIDTH = 24;
    private static final int VIEWPORT_HEIGHT = 8;
    private static final int NESTED_HOST_ROW = 1;
    private static final int NESTED_HOST_COL = 2;
    private static final int CONCURRENCY_TIMEOUT_SECONDS = 5;
    private static final int DRAIN_OBSERVATION_MILLISECONDS = 100;

    @Test
    void runtimeConfigParsesHeadlessTransportDelaysAndViewport()
    {
        McpRuntimeConfig config = McpRuntimeConfig.parse(new String[] {
            "--mcp-server",
            "--headless",
            "--mcp-fast",
            "--mcp-transport",
            "http",
            "--mcp-host",
            "0.0.0.0",
            "--mcp-port",
            "9001",
            "--viewport",
            "2,3,40,12",
            "--mcp-wait-render",
            "--mcp-controlled-render",
        });

        assertTrue(config.enabled());
        assertFalse(config.gui());
        assertEquals(McpRuntimeConfig.TRANSPORT_HTTP, config.transport());
        assertEquals("0.0.0.0", config.host());
        assertEquals(9001, config.port());
        assertEquals(0, config.actionDelayMilliseconds());
        assertEquals(0, config.typeDelayMilliseconds());
        assertEquals(new Rect(2, 3, 40, 12), config.viewport());
        assertTrue(config.waitRender());
        assertTrue(config.controlledRender());
    }

    @Test
    void jsonRpcAndUiAliasesShareTheGeneratedRuntimeState() throws JsonProcessingException
    {
        TestWindow window = new TestWindow("Root");
        McpController controller = controller(window);

        String initialize = controller.handleJsonRpcLine(
            "{\"jsonrpc\":\"2.0\",\"id\":1,\"method\":\"initialize\"}");
        JsonNode initializeResponse = JSON.readTree(initialize);
        assertEquals("2024-11-05", initializeResponse.path("result").path("protocolVersion").asText());

        ObjectNode fill = JSON.createObjectNode();
        fill.put("element_id", "query");
        fill.put("text", "hello");
        controller.callTool("ui.fill", fill);

        ObjectNode query = JSON.createObjectNode();
        query.put("element_id", "query");
        assertEquals("hello", controller.callTool("ui.get_value", query).path("value").asText());
        assertTrue(window.changedCount > 0);

        JsonNode appResult = controller.callTool("echo", JSON.createObjectNode());
        assertEquals("java", appResult.path("backend").asText());
        assertTrue(controller.listTools().path("tools").toString().contains("\"echo\""));
    }

    @Test
    void nestedElementPathsAndRenderSnapshotsExposeCanonicalState()
    {
        GeneratedWindowBase row = new GeneratedWindowBase("Row");
        TextInput field = row.addElement(new TextInput("field", "value", 0));
        row.setGeneratedLayout(List.of(layout("field", 0, 0, 8, 1)));

        GeneratedWindowBase window = new GeneratedWindowBase("List");
        ScrollView items = window.addElement(new ScrollView("items"));
        items.addChild(new ReusableElement("row", row));
        window.setGeneratedLayout(List.of(layout("items", 0, 0, 12, 3)));
        McpController controller = controller(window);

        ObjectNode arguments = JSON.createObjectNode();
        arguments.put("element_id", "items[0].field");
        JsonNode snapshot = controller.callTool("get_element", arguments);

        assertEquals(field.name(), snapshot.path("id").asText().substring("items[0].".length()));
        assertEquals("value", snapshot.path("value").asText());
        JsonNode render = controller.callTool("get_render_snapshot", JSON.createObjectNode());
        assertEquals(VIEWPORT_HEIGHT, render.path("text_lines").size());
        assertEquals(VIEWPORT_WIDTH, render.path("cells").path(0).size());
        assertTrue(render.path("cells").path(0).path(0).path("background").isNull());
        assertTrue(render.path("cells").path(VIEWPORT_HEIGHT - 1)
            .path(VIEWPORT_WIDTH - 1).path("background").isNull());
    }

    @Test
    void elementAndImageInfoBoundsUseCanonicalEdgeSchema()
    {
        GeneratedWindowBase window = new GeneratedWindowBase("Image bounds");
        window.setMode("fullscreen");
        window.addElement(new Image("photo"));
        window.setGeneratedLayout(List.of(layout("photo", 1, 2, 4, 3)));
        McpController controller = controller(window);
        ObjectNode arguments = JSON.createObjectNode();
        arguments.put("element_id", "photo");

        JsonNode imageInfo = controller.callTool("get_image_render_info", arguments);
        JsonNode element = controller.callTool("get_element", arguments);
        JsonNode bounds = imageInfo.path("bounds");

        assertEquals(element.path("bounds"), bounds);
        assertTrue(bounds.has("top"));
        assertTrue(bounds.has("left"));
        assertTrue(bounds.has("bottom"));
        assertTrue(bounds.has("right"));
        assertTrue(bounds.has("width"));
        assertTrue(bounds.has("height"));
        assertFalse(bounds.has("x"));
        assertFalse(bounds.has("y"));
        assertEquals(bounds.path("left").asInt() + bounds.path("width").asInt(),
            bounds.path("right").asInt());
        assertEquals(bounds.path("top").asInt() + bounds.path("height").asInt(),
            bounds.path("bottom").asInt());
    }

    @Test
    void generatedRootScrollViewUsesItsReusableHostInPublicElementPaths()
    {
        GeneratedWindowBase row = new GeneratedWindowBase("Row");
        TextInput field = row.addElement(new TextInput("field", "value", 0));
        row.setGeneratedLayout(List.of(layout("field", 0, 0, 8, 1)));

        GeneratedScrollViewBase list = new GeneratedScrollViewBase("List");
        ScrollView generatedScrollView = list.addElement(new ScrollView("__scrollview"));
        generatedScrollView.addChild(new ReusableElement("row", row));
        list.setGeneratedScrollView(generatedScrollView);
        list.setGeneratedLayout(List.of(layout("__scrollview", 0, 0, 12, 3)));

        GeneratedWindowBase window = new GeneratedWindowBase("Host");
        window.addElement(new ReusableElement("items", list));
        window.setGeneratedLayout(List.of(layout("items", 0, 0, 12, 3)));
        McpController controller = controller(window);

        ObjectNode arguments = JSON.createObjectNode();
        arguments.put("element_id", "items[0].field");
        JsonNode snapshot = controller.callTool("get_element", arguments);

        assertEquals("value", snapshot.path("value").asText());
        assertEquals("items[0]." + field.name(), snapshot.path("id").asText());
        assertFalse(controller.callTool("get_elements", null).toString().contains("__scrollview"));
    }

    @Test
    void renderSnapshotToolsUseAndRestoreTheRequestedGradientTime()
    {
        GeneratedWindowBase window = new GeneratedWindowBase("Gradient");
        window.setMode("fullscreen");
        Label label = window.addElement(new Label("animated", "AB"));
        TextGradient gradient = new TextGradient();
        gradient.setIntervalMs(100);
        gradient.setStep(1);
        gradient.setSegmentSize(1);
        gradient.colors().addAll(List.of(
            new Color("#ff0000"),
            new Color("#00ff00"),
            new Color("#0000ff")));
        Style style = new Style();
        style.setTextColorGradient(gradient);
        label.setStyle(style);
        GeneratedLayoutEntry animatedLayout = layout("animated", 0, 0, 2, 1);
        animatedLayout.setCellWidth(AxisDimension.expanded());
        animatedLayout.setCellHeight(AxisDimension.expanded());
        window.setGeneratedLayout(List.of(animatedLayout));
        McpController controller = controller(window);
        RenderHelpers.setRenderTimeOverrideMs(700L);

        try
        {
            ObjectNode fullArguments = snapshotArguments(0);
            JsonNode full = controller.callTool("get_render_snapshot", fullArguments);
            Point fullPosition = findRenderedCell(full.path("cells"), "A", false);
            assertEquals("#ff0000", full.path("cells")
                .path(fullPosition.row()).path(fullPosition.col())
                .path("foreground").asText());
            assertEquals(700L, RenderHelpers.renderTimeOverrideMs());

            ObjectNode compactArguments = snapshotArguments(100);
            JsonNode compact = controller.callTool(
                "get_render_snapshot_compact",
                compactArguments);
            Point compactPosition = findRenderedCell(compact.path("cells"), "A", true);
            assertEquals("#0000ff", compact.path("cells")
                .path(compactPosition.row()).path(compactPosition.col()).path(1).asText());
            assertEquals(700L, RenderHelpers.renderTimeOverrideMs());

            ObjectNode cellArguments = snapshotArguments(200);
            cellArguments.put("x", fullPosition.col());
            cellArguments.put("y", fullPosition.row());
            JsonNode cell = controller.callTool("get_render_cell", cellArguments);
            assertEquals("#00ff00", cell.path("foreground").asText());
            assertEquals(700L, RenderHelpers.renderTimeOverrideMs());
        }
        finally
        {
            RenderHelpers.setRenderTimeOverrideMs(null);
        }
    }

    @Test
    void numberInputSetAndPasteTextCommitTheNumericValue()
    {
        GeneratedWindowBase window = new GeneratedWindowBase("Numbers");
        NumberInput amount = window.addElement(new NumberInput("amount", 3.0, 1.0));
        amount.setEditStyle(Style.fromProperties(
            "color", "#ffffff",
            "background", "#2d456d"));
        amount.setCursorStyle(Style.fromProperties(
            "color", "#111827",
            "background", "#facc15"));
        window.setGeneratedLayout(List.of(layout("amount", 0, 0, 8, 1)));
        McpController controller = controller(window);
        ObjectNode arguments = JSON.createObjectNode();
        arguments.put("element_id", "amount");
        arguments.put("text", 0);

        JsonNode replaced = controller.callTool("set_text", arguments);

        assertEquals(0.0, replaced.path("value").asDouble());
        assertEquals(0.0, amount.value());

        JsonNode editState = controller.callTool("enter_edit_mode", arguments);
        assertTrue(editState.path("edit_mode").asBoolean());
        ElementRenderState renderState = new ElementRenderState();
        renderState.setFocused(true);
        renderState.setEditMode(true);
        TerminalCell zeroCell = amount.render(new Size(8, 1), renderState).get(0).get(0);
        assertEquals(new Color("#111827"), zeroCell.foreground());
        assertEquals(new Color("#facc15"), zeroCell.background());

        arguments.put("text", "4.5");
        JsonNode pasted = controller.callTool("paste_text", arguments);

        assertEquals(4.5, pasted.path("value").asDouble());
        assertEquals(4.5, amount.value());
    }

    @Test
    void consecutiveTextToolsPreserveCursorAndSelection()
    {
        TestWindow window = new TestWindow("Text sequence");
        McpController controller = controller(window);
        ObjectNode arguments = JSON.createObjectNode();
        arguments.put("element_id", "query");
        arguments.put("text", "Grace Hopper");
        controller.callTool("set_text", arguments);

        arguments.remove("text");
        arguments.put("offset", 0);
        controller.callTool("set_cursor", arguments);
        arguments.remove("offset");
        arguments.put("text", "Dr. ");
        JsonNode prefixed = controller.callTool("type_text", arguments);

        assertEquals("Dr. Grace Hopper", prefixed.path("value").asText());

        arguments.remove("text");
        arguments.put("start", 0);
        arguments.put("end", 3);
        controller.callTool("select_text", arguments);
        arguments.remove("start");
        arguments.remove("end");
        arguments.put("text", "Adm.");
        JsonNode replaced = controller.callTool("type_text", arguments);

        assertEquals("Adm. Grace Hopper", replaced.path("value").asText());
    }

    @Test
    void mcpAndDirectRuntimeShareClipboardText()
    {
        TestWindow window = new TestWindow("Clipboard");
        McpController controller = controller(window);
        ObjectNode arguments = JSON.createObjectNode();
        arguments.put("element_id", "query");
        arguments.put("text", "abcdef");
        controller.callTool("set_text", arguments);

        arguments.remove("text");
        arguments.put("start", 4);
        arguments.put("end", 6);
        controller.callTool("select_text", arguments);
        JsonNode copied = controller.callTool("copy_selection", arguments);
        assertEquals("ef", copied.path("text").asText());
        assertEquals("ef", RuntimeClipboard.runtimeClipboardText());

        arguments.remove("start");
        arguments.remove("end");
        arguments.put("text", "abcdX");
        controller.callTool("set_text", arguments);
        arguments.remove("text");
        arguments.put("key", "cmd_v");
        JsonNode pasted = controller.callTool("press_key", arguments);

        assertEquals("abcdXef", window.query.value());
        assertTrue(pasted.path("edit_mode").asBoolean());
    }

    @Test
    void cuttingSelectionDoesNotReplaceTheSharedClipboard()
    {
        RuntimeClipboard.copyTextToClipboard("clipboard sentinel");
        TestWindow window = new TestWindow("Cut selection");
        McpController controller = controller(window);
        ObjectNode arguments = JSON.createObjectNode();
        arguments.put("element_id", "query");
        arguments.put("text", "abcdef");
        controller.callTool("set_text", arguments);

        arguments.remove("text");
        arguments.put("start", 1);
        arguments.put("end", 3);
        controller.callTool("select_text", arguments);
        JsonNode cut = controller.callTool("cut_selection", arguments);

        assertEquals("bc", cut.path("text").asText());
        assertEquals("adef", window.query.value());
        assertEquals("clipboard sentinel", RuntimeClipboard.runtimeClipboardText());
    }

    @Test
    void activatingButtonAfterEditingTextEndsTheEditAndDispatchesTheButton()
    {
        TestWindow window = new TestWindow("Edit activation");
        McpController controller = controller(window);
        ObjectNode arguments = JSON.createObjectNode();
        arguments.put("element_id", "query");
        arguments.put("text", "ready");
        controller.callTool("set_text", arguments);
        assertTrue(controller.callTool("get_edit_mode", null).path("edit_mode").asBoolean());

        arguments.remove("text");
        arguments.put("element_id", "action");
        controller.callTool("activate_element", arguments);

        assertEquals(1, window.activationCount);
        assertFalse(controller.callTool("get_edit_mode", null).path("edit_mode").asBoolean());
        assertEquals("ready", window.query.value());
    }

    @Test
    void repeatedNestedComboBoxClickRestoresItsScrollViewScope()
    {
        GeneratedWindowBase row = new GeneratedWindowBase("Row");
        ComboBox category = row.addElement(new ComboBox("category", List.of("A", "B")));
        row.setGeneratedLayout(List.of(layout("category", 0, 0, 8, 1)));

        GeneratedScrollViewBase list = new GeneratedScrollViewBase("List");
        ScrollView scrollView = list.addElement(new ScrollView("__scrollview"));
        scrollView.addChild(new ReusableElement("row", row));
        list.setGeneratedScrollView(scrollView);
        list.setGeneratedLayout(List.of(layout("__scrollview", 0, 0, 12, 3)));

        GeneratedWindowBase view = new GeneratedWindowBase("View");
        view.setGeneratedWindowStyle(Style.fromProperties("background", "#1f2937"));
        view.addElement(new ReusableElement("expenses", list));
        view.setGeneratedLayout(List.of(layout("expenses", 2, 0, 12, 3)));
        GeneratedWindowBase window = new GeneratedWindowBase("Host");
        window.setMode("fullscreen");
        window.setGeneratedWindowStyle(Style.fromProperties("background", "#1f2937"));
        ViewHost host = window.addElement(new ViewHost("main"));
        host.setView(view);
        window.setGeneratedLayout(List.of(layout(
            "main",
            NESTED_HOST_ROW,
            NESTED_HOST_COL,
            VIEWPORT_WIDTH - NESTED_HOST_COL * 2,
            VIEWPORT_HEIGHT - NESTED_HOST_ROW * 2)));
        GeneratedWindowRuntimeOptions runtimeOptions = window.runtimeOptions();
        McpRuntimeConfig config = McpRuntimeConfig.parse(new String[] {
            "--mcp-server",
            "--headless",
            "--mcp-fast",
            "--viewport",
            "0,0," + VIEWPORT_WIDTH + "," + VIEWPORT_HEIGHT,
        });
        McpController controller = new McpController(window, runtimeOptions, config);

        ObjectNode element = JSON.createObjectNode();
        element.put("element_id", "main.expenses[0].category");
        controller.callTool("click_element", element);
        Rect firstClickFrame = category.frame();
        ObjectNode key = JSON.createObjectNode();
        key.put("key", "Down");
        controller.callTool("press_key", key);
        key.put("key", "Enter");
        controller.callTool("press_key", key);
        element.put("value", "A");
        controller.callTool("set_value", element);
        element.remove("value");

        controller.callTool("click_element", element);

        assertEquals(firstClickFrame, category.frame());
        assertTrue(controller.callTool("get_edit_mode", null).path("edit_mode").asBoolean());
        GeneratedWindowStackFrame frame = runtimeOptions.windowStack().activeFrame();
        assertEquals(scrollView, frame.activeScrollView());
        assertFalse(frame.suppressActiveScrollViewScopeVisuals());
        JsonNode rendered = controller.callTool("get_render_snapshot", null);
        Point outside = new Point(NESTED_HOST_ROW, VIEWPORT_WIDTH - NESTED_HOST_COL - 1);
        assertFalse(scrollView.frame().contains(outside));
        assertEquals(
            "#131921",
            rendered.path("cells").path(outside.row()).path(outside.col()).path("background").asText());
    }

    @Test
    void controllerAlwaysRoutesToolsToTheTopModalWindow()
    {
        TestWindow root = new TestWindow("Root");
        McpController controller = controller(root);
        TestWindow modal = new TestWindow("Modal");

        root.showModal(modal);

        assertEquals("Modal", controller.callTool("get_window", null).path("title").asText());
        ObjectNode arguments = JSON.createObjectNode();
        arguments.put("element_id", "action");
        controller.callTool("activate_element", arguments);
        assertEquals(0, root.activationCount);
        assertEquals(1, modal.activationCount);
    }

    @Test
    void stdioAndTcpTransportsReturnOneJsonRpcResponsePerRequest() throws IOException
    {
        McpController controller = controller(new TestWindow("Transport"));
        String request = "{\"jsonrpc\":\"2.0\",\"id\":7,\"method\":\"get_window\"}";
        ByteArrayOutputStream stdioOutput = new ByteArrayOutputStream();
        McpStdioServer stdio = new McpStdioServer(
            controller,
            new ByteArrayInputStream((request + "\n").getBytes(StandardCharsets.UTF_8)),
            stdioOutput);

        stdio.serveForever();

        JsonNode stdioResponse = JSON.readTree(stdioOutput.toString(StandardCharsets.UTF_8));
        assertEquals(7, stdioResponse.path("id").asInt());
        assertEquals("Transport", stdioResponse.path("result").path("title").asText());

        McpRuntimeConfig tcpConfig = socketConfig();
        try (McpTcpServer tcp = new McpTcpServer(controller, tcpConfig))
        {
            tcp.start();
            String response = socketRequest(tcp.port(), request + "\n");
            JsonNode tcpResponse = JSON.readTree(response);
            assertEquals(7, tcpResponse.path("id").asInt());
            assertEquals("Transport", tcpResponse.path("result").path("title").asText());
        }
    }

    @Test
    void socketStopWaitsForAnAcceptedResponseToFlush() throws Exception
    {
        CountDownLatch requestAccepted = new CountDownLatch(1);
        CountDownLatch responseAllowed = new CountDownLatch(1);
        AtomicReference<String> response = new AtomicReference<>();
        AtomicReference<Throwable> requestFailure = new AtomicReference<>();
        try (DrainTestServer server = new DrainTestServer(
            socketConfig(),
            requestAccepted,
            responseAllowed))
        {
            server.start();
            Thread requester = new Thread(() ->
            {
                try
                {
                    response.set(socketRequest(server.port(), "request\n"));
                }
                catch (Throwable exception)
                {
                    requestFailure.set(exception);
                }
            }, "uimd-java-mcp-drain-request");
            requester.start();
            assertTrue(requestAccepted.await(CONCURRENCY_TIMEOUT_SECONDS, TimeUnit.SECONDS));

            CountDownLatch stopReturned = new CountDownLatch(1);
            Thread stopper = new Thread(() ->
            {
                server.stop();
                stopReturned.countDown();
            }, "uimd-java-mcp-drain-stop");
            stopper.start();
            try
            {
                assertFalse(stopReturned.await(
                    DRAIN_OBSERVATION_MILLISECONDS,
                    TimeUnit.MILLISECONDS));
            }
            finally
            {
                responseAllowed.countDown();
            }

            assertTrue(stopReturned.await(CONCURRENCY_TIMEOUT_SECONDS, TimeUnit.SECONDS));
            requester.join(TimeUnit.SECONDS.toMillis(CONCURRENCY_TIMEOUT_SECONDS));
            assertFalse(requester.isAlive());
            if (requestFailure.get() != null)
            {
                throw new AssertionError("MCP drain request failed", requestFailure.get());
            }
            assertEquals("drained", response.get());
        }
    }

    @Test
    void httpTransportEnforcesEndpointHeadersAndReturnsJsonRpc() throws IOException
    {
        McpController controller = controller(new TestWindow("HTTP"));
        McpRuntimeConfig httpConfig = socketConfig();
        String request = "{\"jsonrpc\":\"2.0\",\"id\":8,\"method\":\"get_window\"}";
        try (McpHttpServer http = new McpHttpServer(controller, httpConfig))
        {
            http.start();
            String response = socketRequest(
                http.port(),
                "POST /mcp HTTP/1.1\r\n"
                    + "Host: 127.0.0.1\r\n"
                    + "Accept: application/json\r\n"
                    + "MCP-Protocol-Version: 2024-11-05\r\n"
                    + "Content-Length: " + request.getBytes(StandardCharsets.UTF_8).length + "\r\n"
                    + "\r\n"
                    + request);

            assertTrue(response.startsWith("HTTP/1.1 200 OK\n"));
            assertTrue(response.contains("MCP-Protocol-Version: 2024-11-05\n"));
            JsonNode body = JSON.readTree(response.substring(response.indexOf("\n\n") + 2));
            assertEquals("HTTP", body.path("result").path("title").asText());

            String forbidden = socketRequest(
                http.port(),
                "POST /mcp HTTP/1.1\r\n"
                    + "Host: 127.0.0.1\r\n"
                    + "Origin: https://example.com\r\n"
                    + "Content-Length: 0\r\n\r\n");
            assertTrue(forbidden.startsWith("HTTP/1.1 403 Forbidden\n"));
        }
    }

    private static McpController controller(GeneratedWindowBase window)
    {
        McpRuntimeConfig config = McpRuntimeConfig.parse(new String[] {
            "--mcp-server",
            "--headless",
            "--mcp-fast",
            "--viewport",
            "0,0," + VIEWPORT_WIDTH + "," + VIEWPORT_HEIGHT,
        });
        return new McpController(window, window.runtimeOptions(), config);
    }

    private static McpRuntimeConfig socketConfig()
    {
        McpRuntimeConfig config = new McpRuntimeConfig();
        config.setHost(McpRuntimeConfig.DEFAULT_HOST);
        config.setPort(0);
        return config;
    }

    private static ObjectNode snapshotArguments(long timeMilliseconds)
    {
        ObjectNode arguments = JSON.createObjectNode();
        arguments.put("snapshot_time_ms", timeMilliseconds);
        arguments.put("render_scope", "active_window");
        return arguments;
    }

    private static Point findRenderedCell(JsonNode rows, String character, boolean compact)
    {
        for (int row = 0; row < rows.size(); ++row)
        {
            for (int col = 0; col < rows.path(row).size(); ++col)
            {
                JsonNode cell = rows.path(row).path(col);
                String rendered = compact ? cell.path(0).asText() : cell.path("char").asText();
                if (character.equals(rendered))
                {
                    return new Point(row, col);
                }
            }
        }
        throw new AssertionError("Rendered cell was not found: " + character);
    }

    private static String socketRequest(int port, String request) throws IOException
    {
        try (Socket socket = new Socket(McpRuntimeConfig.DEFAULT_HOST, port))
        {
            BufferedWriter writer = new BufferedWriter(
                new OutputStreamWriter(socket.getOutputStream(), StandardCharsets.UTF_8));
            writer.write(request);
            writer.flush();
            socket.shutdownOutput();
            BufferedReader reader = new BufferedReader(
                new InputStreamReader(socket.getInputStream(), StandardCharsets.UTF_8));
            StringBuilder result = new StringBuilder();
            String line;
            while ((line = reader.readLine()) != null)
            {
                if (!result.isEmpty())
                {
                    result.append('\n');
                }
                result.append(line);
            }
            return result.toString();
        }
    }

    private static GeneratedLayoutEntry layout(
        String name,
        int row,
        int col,
        int width,
        int height)
    {
        return new GeneratedLayoutEntry()
            .setName(name)
            .setRelative(new Rect(0, 0, width, height))
            .setSourceCell(new Rect(row, col, width, height))
            .setWidth(AxisDimension.auto(width))
            .setHeight(AxisDimension.auto(height))
            .setCellWidth(AxisDimension.auto(width))
            .setCellHeight(AxisDimension.auto(height));
    }

    private static final class TestWindow extends GeneratedWindowBase
    {
        private final TextInput query;
        private int changedCount;
        private int activationCount;

        TestWindow(String title)
        {
            super(title);
            query = addElement(new TextInput("query", "", 0));
            addElement(new Button("action", "Run"));
            setGeneratedLayout(List.of(
                layout("query", 0, 0, 10, 1),
                layout("action", 1, 0, 6, 1)));
            setMcpMetadata(
                true,
                title + "Window",
                "# " + title,
                "MCP test window",
                List.of(
                    new GeneratedElementMetadata("query", "Query", true),
                    new GeneratedElementMetadata("action", "Action", true)));
            setMcpAppTools(List.of(new GeneratedAppToolMetadata(
                "echo",
                "Returns the backend name",
                "{\"type\":\"object\",\"properties\":{}}",
                "{\"type\":\"object\"}")));
        }

        void showModal(GeneratedWindowBase modal)
        {
            openWindow(modal);
        }

        @Override
        public boolean handleGeneratedButton(String name)
        {
            if ("action".equals(name))
            {
                ++activationCount;
                return true;
            }
            return false;
        }

        @Override
        public boolean handleGeneratedTextChanged(String name, String value)
        {
            if (query.name().equals(name))
            {
                ++changedCount;
                return true;
            }
            return false;
        }

        @Override
        public String callAppTool(String name, String inputJson)
        {
            if ("echo".equals(name))
            {
                return "{\"backend\":\"java\"}";
            }
            return super.callAppTool(name, inputJson);
        }
    }

    private static final class DrainTestServer extends McpSocketServer
    {
        private final CountDownLatch requestAccepted;
        private final CountDownLatch responseAllowed;

        DrainTestServer(
            McpRuntimeConfig config,
            CountDownLatch requestAccepted,
            CountDownLatch responseAllowed)
        {
            super(config);
            this.requestAccepted = requestAccepted;
            this.responseAllowed = responseAllowed;
        }

        @Override
        protected String serverName()
        {
            return "uimd-java-mcp-drain-test";
        }

        @Override
        protected void handleClient(Socket socket) throws IOException
        {
            socket.getInputStream().read();
            requestAccepted.countDown();
            try
            {
                responseAllowed.await();
            }
            catch (InterruptedException exception)
            {
                Thread.currentThread().interrupt();
                throw new IOException("MCP drain test interrupted", exception);
            }
            BufferedWriter writer = new BufferedWriter(
                new OutputStreamWriter(socket.getOutputStream(), StandardCharsets.UTF_8));
            writer.write("drained");
            writer.newLine();
            writer.flush();
        }
    }
}
