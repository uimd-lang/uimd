package uimd;

import java.io.IOException;
import java.util.ArrayList;
import java.util.Comparator;
import java.util.LinkedHashSet;
import java.util.List;
import java.util.Set;

import com.fasterxml.jackson.core.JsonProcessingException;
import com.fasterxml.jackson.databind.JsonNode;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.node.ArrayNode;
import com.fasterxml.jackson.databind.node.NullNode;
import com.fasterxml.jackson.databind.node.ObjectNode;


public final class McpController
{
    private static final String JSON_RPC_VERSION = "2.0";
    private static final String MCP_PROTOCOL_VERSION = "2024-11-05";
    private static final int JSON_RPC_ERROR_CODE = -32000;
    private static final int RENDER_WAIT_TIMEOUT_MILLISECONDS = 2000;
    private static final ObjectMapper JSON = new ObjectMapper();
    private static final Set<String> BUILTIN_TOOL_NAMES = Set.of(
        "activate_element",
        "check",
        "clear_text",
        "click_element",
        "copy_selection",
        "cut_selection",
        "enter_edit_mode",
        "exit_edit_mode",
        "focus_element",
        "get_accessibility_snapshot",
        "get_cursor",
        "get_edit_mode",
        "get_element",
        "get_elements",
        "get_focused_element",
        "get_image_render_info",
        "get_options",
        "get_render_cell",
        "get_render_frame",
        "get_render_rect",
        "get_render_snapshot",
        "get_render_snapshot_compact",
        "get_schema",
        "get_selection",
        "get_source",
        "get_state",
        "get_text",
        "get_text_snapshot",
        "get_value",
        "get_viewport",
        "get_window",
        "mouse_click",
        "mouse_drag",
        "mouse_move",
        "mouse_press",
        "mouse_release",
        "move_cursor",
        "paste_text",
        "press_key",
        "repaint",
        "replace_selection",
        "scroll",
        "select_all",
        "select_option",
        "select_text",
        "set_checked",
        "set_cursor",
        "set_selection",
        "set_text",
        "set_value",
        "set_viewport",
        "type_text",
        "ui.activate",
        "ui.check",
        "ui.clear",
        "ui.click",
        "ui.copy_selection",
        "ui.cut_selection",
        "ui.fill",
        "ui.focus",
        "ui.get_element",
        "ui.get_elements",
        "ui.get_focused_element",
        "ui.get_options",
        "ui.get_schema",
        "ui.get_source",
        "ui.get_value",
        "ui.get_window",
        "ui.press",
        "ui.render_snapshot",
        "ui.select_option",
        "ui.select_text",
        "ui.set_checked",
        "ui.set_cursor",
        "ui.set_selection",
        "ui.set_value",
        "ui.snapshot",
        "ui.text_snapshot",
        "ui.uncheck",
        "uncheck");


    private final GeneratedWindowStack windowStack;
    private final McpRuntimeConfig config;
    private final Object uiLock = new Object();
    private Rect viewport;
    private Point mouseSelectionStart;
    private long renderGeneration;
    private long terminalRenderGeneration;

    public McpController(
        GeneratedWindowBase window,
        GeneratedWindowRuntimeOptions options,
        McpRuntimeConfig config)
    {
        if (window == null)
        {
            throw new IllegalArgumentException("window must not be null");
        }
        GeneratedWindowRuntimeOptions resolvedOptions = options == null
            ? window.runtimeOptions()
            : options;
        GeneratedWindowStack resolvedStack = resolvedOptions.windowStack();
        if (resolvedStack == null)
        {
            resolvedStack = new GeneratedWindowStack();
            resolvedOptions.setWindowStack(resolvedStack);
        }
        if (resolvedStack.rootFrame() == null)
        {
            resolvedStack.setRootFrame(GeneratedWindowRuntime.createFrame(window, resolvedOptions));
        }
        windowStack = resolvedStack;
        this.config = config == null ? new McpRuntimeConfig() : config;
        viewport = this.config.viewport();
    }

    Object uiLock()
    {
        return uiLock;
    }

    long mutationGeneration()
    {
        synchronized (uiLock)
        {
            return renderGeneration;
        }
    }

    Rect currentViewport()
    {
        synchronized (uiLock)
        {
            return viewport;
        }
    }

    void terminalRendered()
    {
        synchronized (uiLock)
        {
            ++terminalRenderGeneration;
            uiLock.notifyAll();
        }
    }

    public boolean shouldCloseRequested()
    {
        synchronized (uiLock)
        {
            GeneratedWindowStackFrame root = windowStack.rootFrame();
            return windowStack.empty()
                && root != null
                && root.options().shouldClose() != null
                && root.options().shouldClose().getAsBoolean();
        }
    }

    public JsonNode callTool(String name, JsonNode arguments)
    {
        synchronized (uiLock)
        {
            long previousMutationGeneration = renderGeneration;
            long previousTerminalRenderGeneration = terminalRenderGeneration;
            String toolName = resolveToolName(name == null ? "" : name);
            ObjectNode object = arguments instanceof ObjectNode value
                ? value
                : JSON.createObjectNode();
            JsonNode result = switch (toolName)
            {
                case "get_window" -> toolGetWindow();
                case "get_elements" -> toolGetElements();
                case "get_element" -> snapshot(requireElement(stringField(object, "element_id")));
                case "get_value" -> object("value", elementValue(requireElement(
                    stringField(object, "element_id"))));
                case "get_options" -> toolGetOptions(object);
                case "get_schema" -> toolGetSchema();
                case "get_source" -> toolGetSource();
                case "get_focused_element" -> toolGetFocusedElement();
                case "get_edit_mode" -> object("edit_mode", activeFrame().editMode());
                case "get_state", "get_accessibility_snapshot" -> toolGetState();
                case "get_render_frame" -> toolGetRenderFrame();
                case "get_render_snapshot" -> toolGetRenderSnapshot(object);
                case "get_render_snapshot_compact" -> toolGetRenderSnapshotCompact(object);
                case "get_render_cell" -> toolGetRenderCell(object);
                case "get_image_render_info" -> toolGetImageRenderInfo(object);
                case "get_text_snapshot" -> toolGetTextSnapshot();
                case "get_viewport", "get_render_rect" -> toolGetViewport();
                case "set_viewport" -> toolSetViewport(object);
                case "repaint" -> toolRepaint();
                case "focus_element" -> toolFocusElement(object);
                case "enter_edit_mode" -> toolEnterEditMode(object);
                case "exit_edit_mode" -> toolExitEditMode();
                case "activate_element" -> toolActivateElement(object);
                case "click_element" -> toolClickElement(object);
                case "press_key" -> toolPressKey(object);
                case "set_text" -> toolSetText(object, false);
                case "paste_text" -> toolSetText(object, true);
                case "type_text", "replace_selection" -> toolTypeText(object);
                case "clear_text" -> toolClearText(object);
                case "set_value" -> toolSetValue(object);
                case "set_checked" -> toolSetChecked(object, booleanField(object, "checked"));
                case "check" -> toolSetChecked(object, true);
                case "uncheck" -> toolSetChecked(object, false);
                case "select_option" -> toolSelectOption(object);
                case "set_selection" -> toolSetSelection(object);
                case "move_cursor" -> toolMoveCursor(object);
                case "set_cursor" -> toolSetCursor(object);
                case "select_text" -> toolSelectText(object);
                case "select_all" -> toolSelectAll(object);
                case "copy_selection" -> toolCopySelection(object);
                case "cut_selection" -> toolCutSelection(object);
                case "get_text" -> toolGetText(object);
                case "get_selection" -> toolGetSelection(object);
                case "get_cursor" -> toolGetCursor(object);
                case "mouse_click" -> toolMouseClick(object);
                case "mouse_drag" -> toolMouseDrag(object);
                case "mouse_press" -> toolMousePress(object);
                case "mouse_move" -> toolMouseMove(object);
                case "mouse_release" -> toolMouseRelease(object);
                case "scroll" -> toolScroll(object);
                default -> toolCallAppTool(toolName, object);
            };
            if (renderGeneration != previousMutationGeneration)
            {
                waitForTerminalRender(previousTerminalRenderGeneration);
            }
            return result;
        }
    }

    public ObjectNode listTools()
    {
        synchronized (uiLock)
        {
            Set<String> names = new LinkedHashSet<>(BUILTIN_TOOL_NAMES);
            for (GeneratedAppToolMetadata metadata : activeWindow().mcpAppTools())
            {
                names.add(metadata.name());
            }
            List<String> sorted = new ArrayList<>(names);
            sorted.sort(Comparator.naturalOrder());
            ArrayNode tools = JSON.createArrayNode();
            for (String name : sorted)
            {
                String resolved = resolveToolName(name);
                GeneratedAppToolMetadata appTool = appToolMetadata(resolved);
                ObjectNode definition = JSON.createObjectNode();
                definition.put("name", name);
                definition.put("description", appTool == null ? "" : appTool.description());
                definition.set(
                    "inputSchema",
                    appTool == null
                        ? toolInputSchema(resolved)
                        : parseSchema(appTool.inputSchemaJson()));
                if (appTool != null && !appTool.outputSchemaJson().isEmpty())
                {
                    definition.set("outputSchema", parseSchema(appTool.outputSchemaJson()));
                }
                tools.add(definition);
            }
            ObjectNode result = JSON.createObjectNode();
            result.set("tools", tools);
            return result;
        }
    }

    public String handleJsonRpcLine(String line)
    {
        try
        {
            JsonNode request = JSON.readTree(line);
            JsonNode response = handleJsonRpc(request);
            return response == null ? "" : JSON.writeValueAsString(response);
        }
        catch (IOException | RuntimeException exception)
        {
            try
            {
                return JSON.writeValueAsString(jsonRpcError(NullNode.instance, exception.getMessage()));
            }
            catch (JsonProcessingException serializationException)
            {
                throw new IllegalStateException("cannot serialize MCP error", serializationException);
            }
        }
    }

    private JsonNode handleJsonRpc(JsonNode request)
    {
        if (request == null)
        {
            return null;
        }
        if (request.isArray())
        {
            ArrayNode responses = JSON.createArrayNode();
            for (JsonNode item : request)
            {
                JsonNode response = handleJsonRpcObject(item);
                if (response != null)
                {
                    responses.add(response);
                }
            }
            return responses.isEmpty() ? null : responses;
        }
        return handleJsonRpcObject(request);
    }

    private JsonNode handleJsonRpcObject(JsonNode request)
    {
        boolean hasIdentifier = request != null && request.has("id");
        JsonNode identifier = hasIdentifier ? request.get("id").deepCopy() : NullNode.instance;
        try
        {
            JsonNode result = handleRequest(request);
            if (!hasIdentifier)
            {
                return null;
            }
            ObjectNode response = JSON.createObjectNode();
            response.put("jsonrpc", JSON_RPC_VERSION);
            response.set("id", identifier);
            response.set("result", result == null ? NullNode.instance : result);
            return response;
        }
        catch (RuntimeException exception)
        {
            return jsonRpcError(identifier, exception.getMessage());
        }
    }

    private JsonNode handleRequest(JsonNode request)
    {
        String method = request.path("method").asText("");
        JsonNode parameters = request.path("params");
        if ("initialize".equals(method))
        {
            ObjectNode result = JSON.createObjectNode();
            result.put("protocolVersion", MCP_PROTOCOL_VERSION);
            result.set("serverInfo", object("name", "ui-java-runtime", "version", "0.1"));
            result.set("capabilities", object("tools", JSON.createObjectNode()));
            return result;
        }
        if ("tools/list".equals(method))
        {
            return listTools();
        }
        if ("tools/call".equals(method))
        {
            String name = parameters.path("name").asText("");
            JsonNode arguments = parameters.path("arguments");
            JsonNode result = callTool(name, arguments);
            ObjectNode content = JSON.createObjectNode();
            content.put("type", "text");
            content.put("text", jsonText(result));
            ObjectNode response = JSON.createObjectNode();
            response.set("content", JSON.createArrayNode().add(content));
            return response;
        }
        return callTool(method, parameters);
    }

    private ObjectNode toolGetWindow()
    {
        GeneratedWindowBase window = activeWindow();
        Size size = reportedWindowSize(window);
        return object(
            "title", window.title(),
            "class", activeWindowClassName(),
            "backend", "java",
            "mode", windowMode(window),
            "description", window.mcpWindowDescription(),
            "width", size.width(),
            "height", size.height(),
            "mcp_enabled", window.mcpEnabled());
    }

    private ArrayNode toolGetElements()
    {
        ArrayNode result = JSON.createArrayNode();
        for (ElementReference reference : elementReferences())
        {
            if (elementExposed(reference))
            {
                result.add(snapshot(reference));
            }
        }
        return result;
    }

    private ObjectNode toolGetOptions(ObjectNode arguments)
    {
        Element element = requireElement(stringField(arguments, "element_id"));
        List<String> options = elementOptions(element);
        if (options == null)
        {
            throw new IllegalArgumentException("Element has no selectable options: " + element.name());
        }
        return object("options", JSON.valueToTree(options));
    }

    private ObjectNode toolGetSchema()
    {
        ArrayNode elements = JSON.createArrayNode();
        for (ElementReference reference : elementReferences())
        {
            if (!elementExposed(reference))
            {
                continue;
            }
            ObjectNode item = snapshot(reference);
            item.set("capabilities", elementCapabilities(reference.element()));
            elements.add(item);
        }
        return object("window", toolGetWindow(), "elements", elements);
    }

    private ObjectNode toolGetSource()
    {
        GeneratedWindowBase window = activeWindow();
        return object(
            "class", activeWindowClassName(),
            "mcp_enabled", window.mcpEnabled(),
            "source_markdown", window.mcpSourceMarkdown());
    }

    private JsonNode toolGetFocusedElement()
    {
        GeneratedWindowStackFrame frame = activeFrame();
        List<Element> focusable = GeneratedWindowRuntime.focusableElements(
            frame.window(),
            frame.activeScrollView());
        if (frame.focusedIndex() < 0 || frame.focusedIndex() >= focusable.size())
        {
            return NullNode.instance;
        }
        ElementReference reference = referenceForElement(focusable.get(frame.focusedIndex()));
        return reference == null || !elementExposed(reference)
            ? NullNode.instance
            : snapshot(reference);
    }

    private ObjectNode toolGetState()
    {
        return object(
            "window", toolGetWindow(),
            "focused_element", toolGetFocusedElement(),
            "edit_mode", activeFrame().editMode(),
            "elements", toolGetElements());
    }

    private ObjectNode toolGetTextSnapshot()
    {
        ArrayNode lines = JSON.createArrayNode();
        for (List<TerminalCell> row : renderActiveWindow())
        {
            lines.add(renderedText(row));
        }
        return object("lines", lines);
    }

    private ObjectNode toolGetRenderFrame()
    {
        List<List<TerminalCell>> content = renderFullSurface();
        return object(
            "width", content.isEmpty() ? 0 : content.get(0).size(),
            "height", content.size(),
            "cells", renderCompactCells(content));
    }

    private ObjectNode toolGetRenderSnapshot(ObjectNode arguments)
    {
        return RenderHelpers.withRenderTimeOverride(
            optionalLongField(arguments, "snapshot_time_ms"),
            () ->
        {
            List<List<TerminalCell>> content = renderForScope(arguments);
            ArrayNode lines = JSON.createArrayNode();
            for (List<TerminalCell> row : content)
            {
                lines.add(renderedText(row));
            }
            return object(
                "ansi_lines", lines.deepCopy(),
                "text_lines", lines,
                "cells", renderCells(content));
        });
    }

    private ObjectNode toolGetRenderSnapshotCompact(ObjectNode arguments)
    {
        return RenderHelpers.withRenderTimeOverride(
            optionalLongField(arguments, "snapshot_time_ms"),
            () ->
        {
            return object(
                "format", "render-cells-v1",
                "cells", renderCompactCells(renderForScope(arguments)));
        });
    }

    private ObjectNode toolGetRenderCell(ObjectNode arguments)
    {
        return RenderHelpers.withRenderTimeOverride(
            optionalLongField(arguments, "snapshot_time_ms"),
            () ->
        {
            List<List<TerminalCell>> content = renderForScope(arguments);
            int row = integerField(arguments, "y", 0);
            int col = integerField(arguments, "x", 0);
            if (row < 0 || row >= content.size())
            {
                throw new IllegalArgumentException("render cell y out of range: " + row);
            }
            if (col < 0 || col >= content.get(row).size())
            {
                throw new IllegalArgumentException("render cell x out of range: " + col);
            }
            ObjectNode result = renderCell(content.get(row).get(col));
            result.put("x", col);
            result.put("y", row);
            return result;
        });
    }

    private ObjectNode toolGetImageRenderInfo(ObjectNode arguments)
    {
        String elementId = stringField(arguments, "element_id");
        ElementReference reference = requireReference(elementId);
        if (!(reference.element() instanceof Image image))
        {
            throw new IllegalArgumentException("Element is not an image: " + elementId);
        }
        renderFullSurface();
        Rect bounds = image.frame();
        ImageRenderInfo info = image.renderInfo(
            new Size(
                Math.max(GeneratedWindowRuntime.MINIMUM_RENDERABLE_SIZE, bounds.width()),
                Math.max(GeneratedWindowRuntime.MINIMUM_RENDERABLE_SIZE, bounds.height())),
            elementRenderState(image));
        int imageRight = info.imageLeft() + info.imageWidth();
        int imageBottom = info.imageTop() + info.imageHeight();
        int visibleRight = info.visibleLeft() + info.visibleWidth();
        int visibleBottom = info.visibleTop() + info.visibleHeight();
        return object(
            "id", elementId,
            "source", info.source(),
            "fit", info.fit(),
            "configured_render_mode", info.configuredRenderMode(),
            "resolved_render_mode", info.resolvedRenderMode(),
            "source_loaded", info.sourceLoaded(),
            "source_width", info.sourceWidth(),
            "source_height", info.sourceHeight(),
            "element_width", info.elementWidth(),
            "element_height", info.elementHeight(),
            "cell_pixel_width", info.cellPixelWidth(),
            "cell_pixel_height", info.cellPixelHeight(),
            "image_left", info.imageLeft(),
            "image_top", info.imageTop(),
            "image_width", info.imageWidth(),
            "image_height", info.imageHeight(),
            "image_right", imageRight,
            "image_bottom", imageBottom,
            "visible_left", info.visibleLeft(),
            "visible_top", info.visibleTop(),
            "visible_width", info.visibleWidth(),
            "visible_height", info.visibleHeight(),
            "visible_right", visibleRight,
            "visible_bottom", visibleBottom,
            "raw_expected", info.rawExpected(),
            "raw_present", info.rawPresent(),
            "sample_signature", JSON.valueToTree(info.sampleSignature()),
            "bounds", rectNode(bounds),
            "absolute_image_left", bounds.col() + info.imageLeft(),
            "absolute_image_top", bounds.row() + info.imageTop(),
            "absolute_image_right", bounds.col() + imageRight,
            "absolute_image_bottom", bounds.row() + imageBottom,
            "absolute_visible_left", bounds.col() + info.visibleLeft(),
            "absolute_visible_top", bounds.row() + info.visibleTop(),
            "absolute_visible_right", bounds.col() + visibleRight,
            "absolute_visible_bottom", bounds.row() + visibleBottom);
    }

    private ObjectNode toolGetViewport()
    {
        Rect value = effectiveViewport();
        return object(
            "row", value.row(),
            "col", value.col(),
            "width", value.width(),
            "height", value.height());
    }

    private ObjectNode toolSetViewport(ObjectNode arguments)
    {
        viewport = new Rect(
            Math.max(0, integerField(arguments, "row", 0)),
            Math.max(0, integerField(arguments, "col", 0)),
            Math.max(1, integerField(arguments, "width", 1)),
            Math.max(1, integerField(arguments, "height", 1)));
        ++renderGeneration;
        return toolGetViewport();
    }

    private ObjectNode toolRepaint()
    {
        ++renderGeneration;
        return object("ok", true);
    }

    private ObjectNode toolFocusElement(ObjectNode arguments)
    {
        actionDelay();
        ElementReference reference = requireReference(stringField(arguments, "element_id"));
        GeneratedWindowRuntime.focusFrameElement(activeFrame(), reference.element());
        ++renderGeneration;
        return snapshot(reference);
    }

    private ObjectNode toolEnterEditMode(ObjectNode arguments)
    {
        actionDelay();
        Element element;
        if (arguments.has("element_id") && arguments.get("element_id").isTextual())
        {
            GeneratedWindowRuntime.commitFrameElementEdit(activeFrame());
            ElementReference reference = requireReference(arguments.get("element_id").asText());
            element = reference.element();
        }
        else
        {
            element = focusedElement();
        }
        if (element == null || !GeneratedWindowRuntime.editableElement(element))
        {
            throw new IllegalArgumentException("Focused element is not editable");
        }
        GeneratedWindowRuntime.beginFrameElementEdit(activeFrame(), element);
        ++renderGeneration;
        return object("edit_mode", activeFrame().editMode());
    }

    private ObjectNode toolExitEditMode()
    {
        actionDelay();
        GeneratedWindowStackFrame frame = activeFrame();
        for (int attempt = 0; attempt < 2 && frame.editMode(); ++attempt)
        {
            dispatch(Event.key("Escape"));
            frame = activeFrame();
        }
        if (frame.editMode())
        {
            frame.setEditMode(false);
            frame.setActiveScrollView(null);
            frame.setActiveScrollViewEditElement(null);
            frame.setEditSnapshot(null);
        }
        ++renderGeneration;
        return object("edit_mode", frame.editMode());
    }

    private JsonNode toolActivateElement(ObjectNode arguments)
    {
        actionDelay();
        String elementId = stringField(arguments, "element_id");
        ElementReference reference = requireReference(elementId);
        if (!reference.element().enabled())
        {
            return snapshot(reference);
        }
        GeneratedWindowStackFrame frame = activeFrame();
        GeneratedWindowRuntime.commitFrameElementEdit(frame);
        GeneratedWindowRuntime.focusFrameElement(frame, reference.element());
        dispatch(Event.key("Enter"));
        ++renderGeneration;
        ElementReference active = findReference(elementId);
        return active == null
            ? object("ok", true, "element_id", reference.element().name())
            : snapshot(active);
    }

    private JsonNode toolClickElement(ObjectNode arguments)
    {
        actionDelay();
        String elementId = stringField(arguments, "element_id");
        renderFullSurface();
        ElementReference reference = requireReference(elementId);
        if (!reference.element().enabled())
        {
            return snapshot(reference);
        }
        Point point = GeneratedWindowRuntime.mouseClickPoint(reference.element());
        dispatch(Event.mouse(EventType.MOUSE_PRESS, point));
        dispatch(Event.mouse(EventType.MOUSE_RELEASE, point));
        ++renderGeneration;
        ElementReference active = findReference(elementId);
        return active == null
            ? object("ok", true, "element_id", reference.element().name())
            : snapshot(active);
    }

    private JsonNode toolPressKey(ObjectNode arguments)
    {
        actionDelay();
        String key = stringField(arguments, "key");
        if ("cmd_c".equals(key))
        {
            copyFocusedText();
        }
        else if ("cmd_v".equals(key))
        {
            Element element = focusedElement();
            if (element instanceof TextInput input)
            {
                GeneratedWindowRuntime.prepareFrameElementEdit(activeFrame(), input);
                input.insertText(RuntimeClipboard.runtimeClipboardText());
                GeneratedWindowRuntime.notifyFrameChanged(activeFrame(), input);
            }
        }
        else
        {
            dispatch(Event.key(key));
        }
        ++renderGeneration;
        return toolGetState();
    }

    private JsonNode toolSetText(ObjectNode arguments, boolean paste)
    {
        actionDelay();
        String elementId = stringField(arguments, "element_id");
        String text = textField(arguments, "text");
        ElementReference reference = requireReference(elementId);
        Element element = reference.element();
        GeneratedWindowStackFrame frame = activeFrame();
        GeneratedWindowRuntime.prepareFrameElementEdit(frame, element);
        if (element instanceof TextInput input)
        {
            if (paste)
            {
                input.insertText(text);
                GeneratedWindowRuntime.notifyFrameChanged(frame, input);
            }
            else
            {
                input.setValue("");
                input.setCursor(0);
                GeneratedWindowRuntime.notifyFrameChanged(frame, input);
                for (String character : codePoints(text))
                {
                    typeDelay();
                    input.insertText(character);
                    GeneratedWindowRuntime.notifyFrameChanged(frame, input);
                }
            }
        }
        else if (element instanceof NumberInput input)
        {
            if (paste)
            {
                input.setValue(text.isEmpty() ? 0.0 : Double.parseDouble(text));
                GeneratedWindowRuntime.notifyFrameChanged(frame, input);
            }
            else
            {
                input.setEditText("");
                GeneratedWindowRuntime.notifyFrameChanged(frame, input);
                for (String character : codePoints(text))
                {
                    typeDelay();
                    input.handleKey(character);
                    GeneratedWindowRuntime.notifyFrameChanged(frame, input);
                }
                input.commitEdit();
                input.setValue(text.isEmpty() ? 0.0 : Double.parseDouble(text));
                input.setEditText(text);
                GeneratedWindowRuntime.notifyFrameChanged(frame, input);
            }
        }
        else
        {
            throw new IllegalArgumentException("Element is not editable text: " + element.name());
        }
        ++renderGeneration;
        ElementReference active = findReference(elementId);
        return snapshot(active == null ? reference : active);
    }

    private JsonNode toolTypeText(ObjectNode arguments)
    {
        actionDelay();
        String elementId = stringField(arguments, "element_id");
        String text = textField(arguments, "text");
        ElementReference reference = requireReference(elementId);
        Element element = reference.element();
        GeneratedWindowStackFrame frame = activeFrame();
        GeneratedWindowRuntime.prepareFrameElementEdit(frame, element);
        if (!(element instanceof TextInput) && !(element instanceof NumberInput))
        {
            throw new IllegalArgumentException("Element is not editable text: " + element.name());
        }
        for (String character : codePoints(text))
        {
            typeDelay();
            element.handleKey(character);
            GeneratedWindowRuntime.notifyFrameChanged(frame, element);
        }
        ++renderGeneration;
        ElementReference active = findReference(elementId);
        return snapshot(active == null ? reference : active);
    }

    private JsonNode toolClearText(ObjectNode arguments)
    {
        actionDelay();
        String elementId = stringField(arguments, "element_id");
        ElementReference reference = requireReference(elementId);
        Element element = reference.element();
        GeneratedWindowStackFrame frame = activeFrame();
        GeneratedWindowRuntime.prepareFrameElementEdit(frame, element);
        if (element instanceof TextInput input)
        {
            input.selectRange(0, input.value().length());
            input.handleKey("Backspace");
        }
        else if (element instanceof NumberInput input)
        {
            input.setEditText("");
        }
        else
        {
            throw new IllegalArgumentException("Element is not editable text: " + element.name());
        }
        GeneratedWindowRuntime.notifyFrameChanged(frame, element);
        ++renderGeneration;
        return snapshot(reference);
    }

    private JsonNode toolSetValue(ObjectNode arguments)
    {
        actionDelay();
        String elementId = stringField(arguments, "element_id");
        ElementReference reference = requireReference(elementId);
        Element element = reference.element();
        JsonNode value = requiredField(arguments, "value");
        GeneratedWindowStackFrame frame = activeFrame();
        if (element instanceof TextInput input)
        {
            input.setValue(value.asText());
            input.setCursor(input.value().length());
            GeneratedWindowRuntime.notifyFrameChanged(frame, input);
        }
        else if (element instanceof NumberInput input)
        {
            input.setValue(value.isNumber() ? value.asDouble() : Double.parseDouble(value.asText()));
            GeneratedWindowRuntime.notifyFrameChanged(frame, input);
        }
        else if (element instanceof CheckBox checkBox)
        {
            if (!value.isBoolean())
            {
                throw new IllegalArgumentException(
                    "Checkbox value must be boolean: " + element.name());
            }
            checkBox.setChecked(value.asBoolean());
            GeneratedWindowRuntime.notifyFrameChanged(frame, checkBox);
        }
        else if (element instanceof ComboBox || element instanceof ListBox && !value.isArray())
        {
            setOptionValue(frame, element, value.asText());
        }
        else if (element instanceof ListBox && value.isArray())
        {
            ObjectNode selection = JSON.createObjectNode();
            selection.put("element_id", elementId);
            selection.set("values", value);
            return toolSetSelection(selection);
        }
        else
        {
            throw new IllegalArgumentException(
                "Element value cannot be set directly: " + element.name());
        }
        ++renderGeneration;
        ElementReference active = findReference(elementId);
        return snapshot(active == null ? reference : active);
    }

    private JsonNode toolSetChecked(ObjectNode arguments, boolean checked)
    {
        actionDelay();
        String elementId = stringField(arguments, "element_id");
        ElementReference reference = requireReference(elementId);
        if (!(reference.element() instanceof CheckBox checkBox))
        {
            throw new IllegalArgumentException(
                "Element is not a checkbox: " + reference.element().name());
        }
        if (checkBox.checked() != checked)
        {
            checkBox.setChecked(checked);
            GeneratedWindowRuntime.notifyFrameChanged(activeFrame(), checkBox);
        }
        ++renderGeneration;
        return snapshot(reference);
    }

    private JsonNode toolSelectOption(ObjectNode arguments)
    {
        actionDelay();
        ElementReference reference = requireReference(stringField(arguments, "element_id"));
        setOptionValue(activeFrame(), reference.element(), textField(arguments, "value"));
        ++renderGeneration;
        return snapshot(reference);
    }

    private JsonNode toolSetSelection(ObjectNode arguments)
    {
        actionDelay();
        ElementReference reference = requireReference(stringField(arguments, "element_id"));
        if (!(reference.element() instanceof ListBox listBox))
        {
            throw new IllegalArgumentException(
                "Element is not a listbox: " + reference.element().name());
        }
        JsonNode valuesNode = requiredField(arguments, "values");
        if (!valuesNode.isArray())
        {
            throw new IllegalArgumentException(
                "ListBox selection values must be an array: " + listBox.name());
        }
        List<String> values = new ArrayList<>();
        for (JsonNode value : valuesNode)
        {
            String option = value.asText();
            if (!listBox.options().contains(option))
            {
                throw new IllegalArgumentException(
                    "Unknown option for " + listBox.name() + ": " + option);
            }
            values.add(option);
        }
        if (!listBox.multiple() && values.size() > 1)
        {
            throw new IllegalArgumentException("ListBox is not multi-select: " + listBox.name());
        }
        listBox.setSelectedValues(values);
        GeneratedWindowRuntime.notifyFrameChanged(activeFrame(), listBox);
        ++renderGeneration;
        return snapshot(reference);
    }

    private JsonNode toolMoveCursor(ObjectNode arguments)
    {
        actionDelay();
        ElementReference reference = requireReference(stringField(arguments, "element_id"));
        Element element = reference.element();
        GeneratedWindowRuntime.prepareFrameElementEdit(activeFrame(), element);
        String key = cursorKey(stringField(arguments, "direction"));
        if (key.isEmpty())
        {
            throw new IllegalArgumentException(
                "Unsupported cursor direction: " + stringField(arguments, "direction"));
        }
        int count = Math.max(1, integerField(arguments, "count", 1));
        for (int index = 0; index < count; ++index)
        {
            element.handleKey(key);
        }
        ++renderGeneration;
        return snapshot(reference);
    }

    private JsonNode toolSetCursor(ObjectNode arguments)
    {
        actionDelay();
        ElementReference reference = requireReference(stringField(arguments, "element_id"));
        if (!(reference.element() instanceof TextInput input))
        {
            throw new IllegalArgumentException(
                "Element has no text cursor: " + reference.element().name());
        }
        GeneratedWindowRuntime.prepareFrameElementEdit(activeFrame(), input);
        input.setCursor(integerField(arguments, "offset", 0));
        ++renderGeneration;
        return snapshot(reference);
    }

    private JsonNode toolSelectText(ObjectNode arguments)
    {
        actionDelay();
        ElementReference reference = requireReference(stringField(arguments, "element_id"));
        int start = integerField(arguments, "start", 0);
        int end = integerField(arguments, "end", 0);
        if (reference.element() instanceof TextInput input)
        {
            GeneratedWindowRuntime.prepareFrameElementEdit(activeFrame(), input);
            input.selectRange(start, end);
        }
        else if (reference.element() instanceof Label label)
        {
            label.selectRange(start, end);
        }
        else
        {
            throw new IllegalArgumentException(
                "Element does not support text selection: " + reference.element().name());
        }
        ++renderGeneration;
        return selectionSnapshot(reference.element());
    }

    private JsonNode toolSelectAll(ObjectNode arguments)
    {
        ElementReference reference = requireReference(stringField(arguments, "element_id"));
        int length = textOf(reference.element()).length();
        ObjectNode selection = JSON.createObjectNode();
        selection.put("element_id", reference.id());
        selection.put("start", 0);
        selection.put("end", length);
        return toolSelectText(selection);
    }

    private JsonNode toolCopySelection(ObjectNode arguments)
    {
        Element element = requireElement(stringField(arguments, "element_id"));
        String selected = selectedTextOf(element);
        RuntimeClipboard.copyTextToClipboard(selected);
        return object("text", selected);
    }

    private JsonNode toolCutSelection(ObjectNode arguments)
    {
        ElementReference reference = requireReference(stringField(arguments, "element_id"));
        String selected = selectedTextOf(reference.element());
        if (reference.element() instanceof TextInput input && !selected.isEmpty())
        {
            input.handleKey("Backspace");
            GeneratedWindowRuntime.notifyFrameChanged(activeFrame(), input);
        }
        ++renderGeneration;
        return object("text", selected);
    }

    private JsonNode toolGetText(ObjectNode arguments)
    {
        Element element = requireElement(stringField(arguments, "element_id"));
        return object("text", textOf(element));
    }

    private JsonNode toolGetSelection(ObjectNode arguments)
    {
        Element element = requireElement(stringField(arguments, "element_id"));
        return selectionSnapshot(element);
    }

    private JsonNode toolGetCursor(ObjectNode arguments)
    {
        Element element = requireElement(stringField(arguments, "element_id"));
        return object(
            "cursor",
            element instanceof TextInput input ? input.cursor() : NullNode.instance);
    }

    private JsonNode toolMouseClick(ObjectNode arguments)
    {
        actionDelay();
        renderFullSurface();
        Point point = mousePoint(arguments);
        dispatch(Event.mouse(EventType.MOUSE_PRESS, point));
        dispatch(Event.mouse(EventType.MOUSE_RELEASE, point));
        ++renderGeneration;
        return toolGetState();
    }

    private JsonNode toolMouseDrag(ObjectNode arguments)
    {
        actionDelay();
        renderFullSurface();
        Point from = viewportPoint(
            integerField(arguments, "from_y", 0),
            integerField(arguments, "from_x", 0));
        Point to = viewportPoint(
            integerField(arguments, "to_y", 0),
            integerField(arguments, "to_x", 0));
        dispatch(Event.mouse(EventType.MOUSE_PRESS, from));
        dispatch(Event.mouse(EventType.MOUSE_DRAG, to));
        dispatch(Event.mouse(EventType.MOUSE_RELEASE, to));
        mouseSelectionStart = null;
        ++renderGeneration;
        return toolGetState();
    }

    private JsonNode toolMousePress(ObjectNode arguments)
    {
        actionDelay();
        renderFullSurface();
        mouseSelectionStart = mousePoint(arguments);
        dispatch(Event.mouse(EventType.MOUSE_PRESS, mouseSelectionStart));
        ++renderGeneration;
        return toolGetState();
    }

    private JsonNode toolMouseMove(ObjectNode arguments)
    {
        Point point = mousePoint(arguments);
        if (mouseSelectionStart != null)
        {
            dispatch(Event.mouse(EventType.MOUSE_DRAG, point));
        }
        ++renderGeneration;
        return toolGetState();
    }

    private JsonNode toolMouseRelease(ObjectNode arguments)
    {
        Point point = mousePoint(arguments);
        dispatch(Event.mouse(EventType.MOUSE_RELEASE, point));
        mouseSelectionStart = null;
        ++renderGeneration;
        return toolGetState();
    }

    private JsonNode toolScroll(ObjectNode arguments)
    {
        actionDelay();
        ElementReference reference = requireReference(stringField(arguments, "element_id"));
        int delta = integerField(arguments, "delta", 0);
        ScrollView scrollView = reference.element() instanceof ScrollView value
            ? value
            : reference.element() instanceof ReusableElement reusable
                && reusable.child() != null
                ? reusable.child().generatedScrollView()
                : null;
        if (scrollView != null)
        {
            scrollView.scrollLines(
                delta,
                new Size(
                    Math.max(1, scrollView.frame().width()),
                    Math.max(1, scrollView.frame().height())));
            scrollView.consumeTerminalScrollDelta();
        }
        else if (reference.element() instanceof ListBox listBox)
        {
            listBox.scrollLines(delta);
        }
        else if (reference.element() instanceof TextInput input && input.multiline())
        {
            input.scrollByRows(delta, Math.max(1, input.frame().height()));
        }
        else
        {
            throw new IllegalArgumentException(
                "Element is not scrollable: " + reference.element().name());
        }
        ++renderGeneration;
        return snapshot(reference);
    }

    private JsonNode toolCallAppTool(String name, ObjectNode arguments)
    {
        GeneratedAppToolMetadata metadata = appToolMetadata(name);
        if (metadata == null)
        {
            throw new IllegalArgumentException("Unknown MCP UI tool: " + name);
        }
        try
        {
            String result = activeWindow().callAppTool(name, JSON.writeValueAsString(arguments));
            JsonNode parsed = result == null || result.isEmpty()
                ? JSON.createObjectNode()
                : JSON.readTree(result);
            ++renderGeneration;
            return parsed;
        }
        catch (JsonProcessingException exception)
        {
            throw new IllegalStateException("App MCP tool returned invalid JSON", exception);
        }
    }

    private void setOptionValue(
        GeneratedWindowStackFrame frame,
        Element element,
        String value)
    {
        List<String> options = elementOptions(element);
        if (options == null)
        {
            throw new IllegalArgumentException(
                "Element is not a supported selection control: " + element.name());
        }
        int index = options.indexOf(value);
        if (index < 0)
        {
            throw new IllegalArgumentException(
                "Unknown option for " + element.name() + ": " + value);
        }
        if (element instanceof ComboBox comboBox)
        {
            comboBox.setSelectedIndex(index);
        }
        else if (element instanceof ListBox listBox)
        {
            if (listBox.multiple())
            {
                listBox.setSelectedValues(List.of(value));
            }
            else
            {
                listBox.setSelectedIndex(index);
            }
        }
        GeneratedWindowRuntime.notifyFrameChanged(frame, element);
    }

    private void dispatch(Event event)
    {
        if (windowStack.topFrame() == null)
        {
            GeneratedWindowRuntime.dispatchFrameEvent(windowStack.rootFrame(), event);
        }
        else
        {
            GeneratedWindowRuntime.dispatchWindowStackEvent(windowStack, event);
        }
    }

    private GeneratedWindowStackFrame activeFrame()
    {
        GeneratedWindowStackFrame frame = windowStack.activeFrame();
        if (frame == null)
        {
            throw new IllegalStateException("MCP window stack is empty");
        }
        return frame;
    }

    private GeneratedWindowBase activeWindow()
    {
        return activeFrame().window();
    }

    private String activeWindowClassName()
    {
        String className = activeFrame().options().className();
        if (className.isEmpty())
        {
            className = activeWindow().mcpClassName();
        }
        if (!className.isEmpty())
        {
            return className;
        }
        StringBuilder result = new StringBuilder();
        boolean uppercase = true;
        for (char character : activeWindow().title().toCharArray())
        {
            if (!Character.isLetterOrDigit(character))
            {
                uppercase = true;
                continue;
            }
            result.append(uppercase ? Character.toUpperCase(character) : character);
            uppercase = false;
        }
        return result.toString();
    }

    private Element focusedElement()
    {
        GeneratedWindowStackFrame frame = activeFrame();
        List<Element> focusable = GeneratedWindowRuntime.focusableElements(
            frame.window(),
            frame.activeScrollView());
        return frame.focusedIndex() >= 0 && frame.focusedIndex() < focusable.size()
            ? focusable.get(frame.focusedIndex())
            : null;
    }

    private List<ElementReference> elementReferences()
    {
        List<ElementReference> result = new ArrayList<>();
        appendWindowReferences(activeWindow(), "", null, result);
        return result;
    }

    private static void appendWindowReferences(
        GeneratedWindowBase window,
        String prefix,
        String generatedScrollViewAlias,
        List<ElementReference> output)
    {
        ScrollView generatedScrollView = window.generatedScrollView();
        for (Element element : window.elements())
        {
            String elementId = generatedScrollViewAlias != null && element == generatedScrollView
                ? generatedScrollViewAlias
                : prefix + element.name();
            output.add(new ElementReference(elementId, element, window));
            if (element instanceof ReusableElement reusable && reusable.child() != null)
            {
                String childScrollViewAlias = reusable.child().generatedScrollView() == null
                    ? null
                    : elementId;
                appendWindowReferences(
                    reusable.child(),
                    elementId + ".",
                    childScrollViewAlias,
                    output);
            }
            if (!(element instanceof ScrollView scrollView))
            {
                continue;
            }
            for (int index = 0; index < scrollView.children().size(); ++index)
            {
                Element child = scrollView.children().get(index);
                if (child instanceof ReusableElement reusable && reusable.child() != null)
                {
                    appendWindowReferences(
                        reusable.child(),
                        elementId + "[" + index + "].",
                        null,
                        output);
                }
            }
        }
    }

    private ElementReference requireReference(String elementId)
    {
        ElementReference reference = findReference(elementId);
        if (reference == null)
        {
            throw new IllegalArgumentException("Unknown element: " + elementId);
        }
        return reference;
    }

    private Element requireElement(String elementId)
    {
        return requireReference(elementId).element();
    }

    private ElementReference findReference(String elementId)
    {
        if (elementId == null || elementId.isEmpty())
        {
            return null;
        }
        List<ElementReference> references = elementReferences();
        for (ElementReference reference : references)
        {
            if (elementId.equals(reference.id()))
            {
                return reference;
            }
        }
        for (ElementReference reference : references)
        {
            if (elementId.equals(reference.element().name()))
            {
                return reference;
            }
        }
        return null;
    }

    private ElementReference referenceForElement(Element element)
    {
        for (ElementReference reference : elementReferences())
        {
            if (reference.element() == element)
            {
                return reference;
            }
        }
        return null;
    }

    private static boolean elementExposed(ElementReference reference)
    {
        GeneratedElementMetadata metadata = elementMetadata(reference.owner(), reference.element());
        return metadata == null || metadata.expose();
    }

    private static GeneratedElementMetadata elementMetadata(
        GeneratedWindowBase window,
        Element element)
    {
        for (GeneratedElementMetadata metadata : window.mcpElementMetadata())
        {
            if (metadata.name().equals(element.name()))
            {
                return metadata;
            }
        }
        return null;
    }

    private ObjectNode snapshot(Element element)
    {
        ElementReference reference = referenceForElement(element);
        if (reference == null)
        {
            reference = new ElementReference(element.name(), element, activeWindow());
        }
        return snapshot(reference);
    }

    private ObjectNode snapshot(ElementReference reference)
    {
        Element element = reference.element();
        GeneratedElementMetadata metadata = elementMetadata(reference.owner(), element);
        Element focused = focusedElement();
        boolean isFocused = focused == element;
        ObjectNode result = object(
            "id", reference.id(),
            "type", elementType(element),
            "role", elementRole(element),
            "description", metadata == null ? "" : metadata.description(),
            "focused", isFocused,
            "visible", true,
            "enabled", element.enabled(),
            "bounds", rectNode(element.frame()),
            "style_state", isFocused ? activeFrame().editMode() ? "edit" : "focus" : "base",
            "value", elementValue(element));
        List<String> options = elementOptions(element);
        if (options != null)
        {
            result.set("options", JSON.valueToTree(options));
        }
        if (element instanceof TextInput input)
        {
            result.put("cursor", input.cursor());
            result.set("selection", selectionSnapshot(input));
        }
        return result;
    }

    private static String elementType(Element element)
    {
        if (element instanceof TextArea)
        {
            return "textarea";
        }
        if (element instanceof TextInput)
        {
            return "textinput";
        }
        if (element instanceof NumberInput)
        {
            return "numberinput";
        }
        if (element instanceof Button)
        {
            return "button";
        }
        if (element instanceof CheckBox)
        {
            return "checkbox";
        }
        if (element instanceof ComboBox)
        {
            return "combobox";
        }
        if (element instanceof ListBox)
        {
            return "listbox";
        }
        if (element instanceof Image)
        {
            return "image";
        }
        if (element instanceof InfoLabel)
        {
            return "infolabel";
        }
        if (element instanceof MessageTable)
        {
            return "messagetable";
        }
        if (element instanceof Label)
        {
            return "label";
        }
        if (element instanceof ScrollView)
        {
            return "scrollview";
        }
        if (element instanceof ViewHost)
        {
            return "viewhost";
        }
        return "element";
    }

    private static String elementRole(Element element)
    {
        String type = elementType(element);
        if ("textinput".equals(type) || "textarea".equals(type) || "numberinput".equals(type))
        {
            return "input";
        }
        if ("button".equals(type))
        {
            return "action";
        }
        if ("checkbox".equals(type))
        {
            return "toggle";
        }
        if ("combobox".equals(type) || "listbox".equals(type))
        {
            return "selection";
        }
        return "text";
    }

    private static JsonNode elementValue(Element element)
    {
        if (element instanceof TextInput input)
        {
            return JSON.valueToTree(input.value());
        }
        if (element instanceof NumberInput input)
        {
            return JSON.valueToTree(input.value());
        }
        if (element instanceof CheckBox checkBox)
        {
            return JSON.valueToTree(checkBox.checked());
        }
        if (element instanceof ComboBox comboBox)
        {
            return JSON.valueToTree(comboBox.selectedText());
        }
        if (element instanceof ListBox listBox)
        {
            return JSON.valueToTree(listBox.selectedValues());
        }
        if (element instanceof Label label)
        {
            return JSON.valueToTree(label.text());
        }
        if (element instanceof Image image)
        {
            return JSON.valueToTree(image.source());
        }
        if (element instanceof Button button)
        {
            return JSON.valueToTree(button.title());
        }
        return NullNode.instance;
    }

    private static List<String> elementOptions(Element element)
    {
        if (element instanceof ComboBox comboBox)
        {
            return comboBox.options();
        }
        if (element instanceof ListBox listBox)
        {
            return listBox.options();
        }
        return null;
    }

    private static ArrayNode elementCapabilities(Element element)
    {
        ArrayNode result = JSON.createArrayNode().add("ui.get_element");
        if (!elementValue(element).isNull())
        {
            result.add("ui.get_value");
        }
        if (element instanceof TextInput || element instanceof NumberInput)
        {
            result.add("ui.set_value");
            result.add("ui.fill");
            result.add("ui.clear");
            result.add("ui.focus");
        }
        else if (element instanceof Button)
        {
            result.add("ui.activate");
            result.add("ui.click");
            result.add("ui.focus");
        }
        else if (element instanceof CheckBox)
        {
            result.add("ui.set_value");
            result.add("ui.check");
            result.add("ui.uncheck");
            result.add("ui.set_checked");
            result.add("ui.activate");
            result.add("ui.click");
            result.add("ui.focus");
        }
        else if (element instanceof ComboBox)
        {
            result.add("ui.set_value");
            result.add("ui.select_option");
            result.add("ui.get_options");
            result.add("ui.focus");
            result.add("ui.press");
        }
        else if (element instanceof ListBox)
        {
            result.add("ui.set_value");
            result.add("ui.select_option");
            result.add("ui.set_selection");
            result.add("ui.get_options");
            result.add("ui.focus");
            result.add("ui.press");
        }
        return result;
    }

    private List<List<TerminalCell>> renderFullSurface()
    {
        List<List<TerminalCell>> content = GeneratedWindowRuntime.renderRuntimeStack(
            windowStack,
            viewportSize());
        return content;
    }

    private void waitForTerminalRender(long previousGeneration)
    {
        if (!config.waitRender() || !config.gui())
        {
            return;
        }
        long deadline = System.nanoTime()
            + java.util.concurrent.TimeUnit.MILLISECONDS.toNanos(
                RENDER_WAIT_TIMEOUT_MILLISECONDS);
        while (terminalRenderGeneration == previousGeneration)
        {
            long remainingNanos = deadline - System.nanoTime();
            if (remainingNanos <= 0)
            {
                return;
            }
            try
            {
                java.util.concurrent.TimeUnit.NANOSECONDS.timedWait(uiLock, remainingNanos);
            }
            catch (InterruptedException exception)
            {
                Thread.currentThread().interrupt();
                return;
            }
        }
    }

    private List<List<TerminalCell>> renderActiveWindow()
    {
        GeneratedWindowStackFrame frame = activeFrame();
        return GeneratedWindowRuntime.renderTerminalContent(
            frame.window(),
            viewportSize(),
            frame.focusedIndex(),
            frame.editMode(),
            frame.activeScrollView(),
            frame.activeScrollViewEditElement(),
            frame.suppressActiveScrollViewScopeVisuals());
    }

    private List<List<TerminalCell>> renderForScope(ObjectNode arguments)
    {
        String scope = arguments.path("render_scope").asText("full_surface");
        if (scope.isEmpty() || "full_surface".equals(scope))
        {
            return renderFullSurface();
        }
        if ("active_window".equals(scope))
        {
            return renderActiveWindow();
        }
        throw new IllegalArgumentException("unknown render_scope: " + scope);
    }

    private Size viewportSize()
    {
        Rect value = effectiveViewport();
        return new Size(
            Math.max(GeneratedWindowRuntime.MINIMUM_RENDERABLE_SIZE, value.width()),
            Math.max(GeneratedWindowRuntime.MINIMUM_RENDERABLE_SIZE, value.height()));
    }

    private Rect effectiveViewport()
    {
        if (viewport != null)
        {
            return viewport;
        }
        Size size = reportedWindowSize(activeWindow());
        return new Rect(0, 0, size.width(), size.height());
    }

    private Size reportedWindowSize(GeneratedWindowBase window)
    {
        Size natural = GeneratedWindowRuntime.generatedWindowContentSize(window);
        Style style = window.generatedWindowStyle();
        int width = natural.width() + nonNegative(style.borderWidthVertical()) * 2;
        int height = natural.height() + nonNegative(style.borderWidthHorizontal()) * 2;
        if (viewport == null)
        {
            return new Size(width, height);
        }
        String mode = windowMode(window);
        int availableWidth = Math.max(
            GeneratedWindowRuntime.MINIMUM_RENDERABLE_SIZE,
            viewport.width()
                - styleValue(style.marginLeft(), style.margin())
                - styleValue(style.marginRight(), style.margin()));
        int availableHeight = Math.max(
            GeneratedWindowRuntime.MINIMUM_RENDERABLE_SIZE,
            viewport.height()
                - styleValue(style.marginTop(), style.margin())
                - styleValue(style.marginBottom(), style.margin()));
        width = "fullscreen".equals(mode) || "expand_width".equals(mode)
            ? availableWidth
            : Math.min(width, availableWidth);
        height = "fullscreen".equals(mode) || "expand_height".equals(mode)
            ? availableHeight
            : Math.min(height, availableHeight);
        return new Size(width, height);
    }

    private static String windowMode(GeneratedWindowBase window)
    {
        boolean expandsWidth = false;
        boolean expandsHeight = false;
        for (GeneratedLayoutEntry entry : window.generatedLayout())
        {
            expandsWidth = expandsWidth || entry.cellWidth().mode() == DimensionMode.EXPANDED;
            expandsHeight = expandsHeight || entry.cellHeight().mode() == DimensionMode.EXPANDED;
        }
        if (expandsWidth && expandsHeight)
        {
            return "fullscreen";
        }
        if (expandsWidth)
        {
            return "expand_width";
        }
        if (expandsHeight)
        {
            return "expand_height";
        }
        return "normal";
    }

    private ElementRenderState elementRenderState(Element element)
    {
        boolean focused = focusedElement() == element;
        ElementRenderState state = new ElementRenderState();
        state.setFocused(focused);
        state.setEditMode(focused && activeFrame().editMode());
        return state;
    }

    private static ArrayNode renderCells(List<List<TerminalCell>> content)
    {
        ArrayNode rows = JSON.createArrayNode();
        for (List<TerminalCell> row : content)
        {
            ArrayNode cells = JSON.createArrayNode();
            for (TerminalCell cell : row)
            {
                cells.add(renderCell(cell));
            }
            rows.add(cells);
        }
        return rows;
    }

    private static ArrayNode renderCompactCells(List<List<TerminalCell>> content)
    {
        ArrayNode rows = JSON.createArrayNode();
        for (List<TerminalCell> row : content)
        {
            ArrayNode cells = JSON.createArrayNode();
            for (TerminalCell cell : row)
            {
                ArrayNode compact = JSON.createArrayNode();
                compact.add(cell.text().isEmpty() ? " " : cell.text());
                compact.add(colorNode(cell.foreground()));
                compact.add(colorNode(cell.background()));
                compact.add(JSON.createArrayNode());
                cells.add(compact);
            }
            rows.add(cells);
        }
        return rows;
    }

    private static ObjectNode renderCell(TerminalCell cell)
    {
        return object(
            "char", cell.text().isEmpty() ? " " : cell.text(),
            "foreground", colorNode(cell.foreground()),
            "background", colorNode(cell.background()),
            "attributes", JSON.createArrayNode());
    }

    private static JsonNode colorNode(Color color)
    {
        if (color == null || color.isTransparent() || color.toString().isEmpty())
        {
            return NullNode.instance;
        }
        return JSON.valueToTree(color.toString());
    }

    private static String renderedText(List<TerminalCell> row)
    {
        StringBuilder result = new StringBuilder();
        for (TerminalCell cell : row)
        {
            result.append(RenderHelpers.safeTerminalCellText(cell.text()));
        }
        return result.toString();
    }

    private Point mousePoint(ObjectNode arguments)
    {
        return viewportPoint(
            integerField(arguments, "y", 0),
            integerField(arguments, "x", 0));
    }

    private Point viewportPoint(int row, int col)
    {
        Rect value = effectiveViewport();
        return new Point(row - value.row(), col - value.col());
    }

    private static ObjectNode selectionSnapshot(Element element)
    {
        if (element instanceof TextInput input)
        {
            if (input.selectedText().isEmpty())
            {
                return object("start", NullNode.instance, "end", NullNode.instance, "text", "");
            }
            return object(
                "start", input.selectionStart(),
                "end", input.selectionEnd(),
                "text", input.selectedText());
        }
        if (element instanceof Label label)
        {
            if (label.selectedText().isEmpty())
            {
                return object("start", NullNode.instance, "end", NullNode.instance, "text", "");
            }
            return object(
                "start", label.selectionStart(),
                "end", label.selectionEnd(),
                "text", label.selectedText());
        }
        return object("start", NullNode.instance, "end", NullNode.instance, "text", "");
    }

    private static String selectedTextOf(Element element)
    {
        if (element instanceof TextInput input)
        {
            return input.selectedText();
        }
        if (element instanceof Label label)
        {
            return label.selectedText();
        }
        return "";
    }

    private static String textOf(Element element)
    {
        if (element instanceof TextInput input)
        {
            return input.value();
        }
        if (element instanceof NumberInput input)
        {
            return input.displayText();
        }
        if (element instanceof Label label)
        {
            return label.text();
        }
        return "";
    }

    private void copyFocusedText()
    {
        Element element = focusedElement();
        String selected = selectedTextOf(element);
        RuntimeClipboard.copyTextToClipboard(selected.isEmpty() ? textOf(element) : selected);
    }

    private static String resolveToolName(String name)
    {
        String local = name.startsWith("ui.") ? name.substring("ui.".length()) : name;
        return switch (local)
        {
            case "snapshot" -> "get_accessibility_snapshot";
            case "render_snapshot" -> "get_render_snapshot";
            case "text_snapshot" -> "get_text_snapshot";
            case "focus" -> "focus_element";
            case "activate" -> "activate_element";
            case "click" -> "click_element";
            case "press" -> "press_key";
            case "fill" -> "set_text";
            case "clear" -> "clear_text";
            default -> local;
        };
    }

    private static ObjectNode toolInputSchema(String name)
    {
        ObjectNode properties = JSON.createObjectNode();
        ArrayNode required = JSON.createArrayNode();
        Set<String> elementOnly = Set.of(
            "get_element",
            "get_value",
            "get_options",
            "focus_element",
            "activate_element",
            "click_element",
            "clear_text",
            "select_all",
            "copy_selection",
            "cut_selection",
            "get_text",
            "get_selection",
            "get_cursor",
            "get_image_render_info",
            "check",
            "uncheck");
        if (elementOnly.contains(name))
        {
            addElementIdSchema(properties, required);
        }
        else if ("set_value".equals(name))
        {
            addElementIdSchema(properties, required);
            ArrayNode alternatives = JSON.createArrayNode();
            alternatives.add(typeSchema("string"));
            alternatives.add(typeSchema("number"));
            alternatives.add(typeSchema("boolean"));
            alternatives.add(object("type", "array", "items", typeSchema("string")));
            properties.set("value", object("oneOf", alternatives));
            required.add("value");
        }
        else if (Set.of("set_text", "paste_text", "type_text", "replace_selection")
            .contains(name))
        {
            addElementIdSchema(properties, required);
            properties.set("text", typeSchema("string"));
            required.add("text");
        }
        else if ("set_checked".equals(name))
        {
            addElementIdSchema(properties, required);
            properties.set("checked", typeSchema("boolean"));
            required.add("checked");
        }
        else if ("select_option".equals(name))
        {
            addElementIdSchema(properties, required);
            properties.set("value", typeSchema("string"));
            required.add("value");
        }
        else if ("set_selection".equals(name))
        {
            addElementIdSchema(properties, required);
            properties.set("values", object("type", "array", "items", typeSchema("string")));
            required.add("values");
        }
        else if ("press_key".equals(name))
        {
            properties.set("key", typeSchema("string"));
            required.add("key");
        }
        else if ("move_cursor".equals(name))
        {
            addElementIdSchema(properties, required);
            properties.set("direction", typeSchema("string"));
            properties.set("count", object("type", "integer", "minimum", 1));
            required.add("direction");
        }
        else if ("set_cursor".equals(name))
        {
            addElementIdSchema(properties, required);
            properties.set("offset", object("type", "integer", "minimum", 0));
            required.add("offset");
        }
        else if ("select_text".equals(name))
        {
            addElementIdSchema(properties, required);
            properties.set("start", object("type", "integer", "minimum", 0));
            properties.set("end", object("type", "integer", "minimum", 0));
            required.add("start");
            required.add("end");
        }
        else if ("set_viewport".equals(name))
        {
            for (String field : List.of("row", "col", "width", "height"))
            {
                properties.set(field, typeSchema("integer"));
                required.add(field);
            }
        }
        else if (Set.of("get_render_snapshot", "get_render_snapshot_compact").contains(name))
        {
            addRenderSnapshotSchema(properties);
        }
        else if ("get_render_cell".equals(name))
        {
            properties.set("x", typeSchema("integer"));
            properties.set("y", typeSchema("integer"));
            addRenderSnapshotSchema(properties);
            required.add("x");
            required.add("y");
        }
        else if ("repaint".equals(name))
        {
            properties.set("full", typeSchema("boolean"));
        }
        else if (Set.of("mouse_click", "mouse_press", "mouse_move", "mouse_release")
            .contains(name))
        {
            properties.set("x", typeSchema("integer"));
            properties.set("y", typeSchema("integer"));
            required.add("x");
            required.add("y");
        }
        else if ("mouse_drag".equals(name))
        {
            for (String field : List.of("from_x", "from_y", "to_x", "to_y"))
            {
                properties.set(field, typeSchema("integer"));
                required.add(field);
            }
        }
        else if ("scroll".equals(name))
        {
            addElementIdSchema(properties, required);
            properties.set("delta", typeSchema("integer"));
            required.add("delta");
        }

        ObjectNode schema = object("type", "object", "properties", properties);
        if (!required.isEmpty())
        {
            schema.set("required", required);
        }
        return schema;
    }

    private static void addElementIdSchema(ObjectNode properties, ArrayNode required)
    {
        properties.set("element_id", typeSchema("string"));
        required.add("element_id");
    }

    private static void addRenderSnapshotSchema(ObjectNode properties)
    {
        properties.set("snapshot_time_ms", typeSchema("integer"));
        properties.set(
            "render_scope",
            object(
                "type",
                "string",
                "enum",
                JSON.createArrayNode().add("full_surface").add("active_window")));
    }

    private static ObjectNode typeSchema(String type)
    {
        return object("type", type);
    }

    private static JsonNode parseSchema(String json)
    {
        if (json == null || json.isEmpty())
        {
            return object("type", "object", "properties", JSON.createObjectNode());
        }
        try
        {
            return JSON.readTree(json);
        }
        catch (JsonProcessingException exception)
        {
            throw new IllegalArgumentException("Invalid generated MCP schema", exception);
        }
    }

    private GeneratedAppToolMetadata appToolMetadata(String name)
    {
        for (GeneratedAppToolMetadata metadata : activeWindow().mcpAppTools())
        {
            if (metadata.name().equals(name))
            {
                return metadata;
            }
        }
        return null;
    }

    private static String cursorKey(String direction)
    {
        return switch (direction.toLowerCase(java.util.Locale.ROOT))
        {
            case "left" -> "Left";
            case "right" -> "Right";
            case "up" -> "Up";
            case "down" -> "Down";
            case "home" -> "Home";
            case "end" -> "End";
            default -> "";
        };
    }

    private static List<String> codePoints(String text)
    {
        List<String> result = new ArrayList<>();
        text.codePoints().forEach(value -> result.add(new String(Character.toChars(value))));
        return result;
    }

    private void actionDelay()
    {
        sleep(config.actionDelayMilliseconds());
    }

    private void typeDelay()
    {
        sleep(config.typeDelayMilliseconds());
    }

    private static void sleep(int milliseconds)
    {
        if (milliseconds <= 0)
        {
            return;
        }
        try
        {
            Thread.sleep(milliseconds);
        }
        catch (InterruptedException exception)
        {
            Thread.currentThread().interrupt();
            throw new IllegalStateException("MCP action interrupted", exception);
        }
    }

    private static String stringField(ObjectNode object, String name)
    {
        JsonNode value = object.get(name);
        return value == null || value.isNull() ? "" : value.asText();
    }

    private static JsonNode requiredField(ObjectNode object, String name)
    {
        JsonNode value = object.get(name);
        if (value == null || value.isNull())
        {
            throw new IllegalArgumentException("Missing required field: " + name);
        }
        return value;
    }

    private static String textField(ObjectNode object, String name)
    {
        return stringField(object, name);
    }

    private static int integerField(ObjectNode object, String name, int fallback)
    {
        JsonNode value = object.get(name);
        return value == null || value.isNull() ? fallback : value.asInt(fallback);
    }

    private static Long optionalLongField(ObjectNode object, String name)
    {
        JsonNode value = object.get(name);
        return value == null || value.isNull() ? null : value.asLong();
    }

    private static boolean booleanField(ObjectNode object, String name)
    {
        JsonNode value = object.get(name);
        return value != null && value.asBoolean(false);
    }

    private static int nonNegative(Integer value)
    {
        return value == null ? 0 : Math.max(0, value);
    }

    private static int styleValue(Integer value, Integer fallback)
    {
        return value == null ? nonNegative(fallback) : nonNegative(value);
    }

    private static ObjectNode rectNode(Rect rect)
    {
        return object(
            "top", rect.row(),
            "left", rect.col(),
            "bottom", rect.row() + rect.height(),
            "right", rect.col() + rect.width(),
            "width", rect.width(),
            "height", rect.height());
    }

    private static ObjectNode jsonRpcError(JsonNode identifier, String message)
    {
        return object(
            "jsonrpc", JSON_RPC_VERSION,
            "id", identifier == null ? NullNode.instance : identifier,
            "error", object(
                "code", JSON_RPC_ERROR_CODE,
                "message", message == null ? "MCP request failed" : message));
    }

    private static String jsonText(JsonNode value)
    {
        try
        {
            return JSON.writeValueAsString(value == null ? NullNode.instance : value);
        }
        catch (JsonProcessingException exception)
        {
            throw new IllegalStateException("cannot serialize MCP tool result", exception);
        }
    }

    private static ObjectNode object(Object... fields)
    {
        if (fields.length % 2 != 0)
        {
            throw new IllegalArgumentException("JSON object fields must be key/value pairs");
        }
        ObjectNode result = JSON.createObjectNode();
        for (int index = 0; index < fields.length; index += 2)
        {
            String key = String.valueOf(fields[index]);
            Object value = fields[index + 1];
            JsonNode node = value instanceof JsonNode jsonNode
                ? jsonNode
                : JSON.valueToTree(value);
            result.set(key, node == null ? NullNode.instance : node);
        }
        return result;
    }

    private record ElementReference(String id, Element element, GeneratedWindowBase owner)
    {
    }
}
