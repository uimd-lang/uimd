package uimd;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.net.Socket;
import java.nio.charset.StandardCharsets;
import java.util.LinkedHashMap;
import java.util.Locale;
import java.util.Map;

public final class McpHttpServer extends McpSocketServer
{
    private static final String ENDPOINT = "/mcp";
    private static final String APPLICATION_JSON = "application/json";
    private static final String TEXT_EVENT_STREAM = "text/event-stream";
    private static final String PROTOCOL_VERSION = "2024-11-05";
    private static final int MAXIMUM_HEADER_BYTES = 64 * 1024;

    private final McpController controller;

    public McpHttpServer(McpController controller, McpRuntimeConfig config)
    {
        super(config);
        this.controller = controller;
    }

    @Override
    protected String serverName()
    {
        return "uimd-java-mcp-http";
    }

    @Override
    protected void handleClient(Socket socket) throws IOException
    {
        HttpRequest request = readRequest(socket.getInputStream());
        byte[] response = responseFor(request).getBytes(StandardCharsets.UTF_8);
        OutputStream output = socket.getOutputStream();
        output.write(response);
        output.flush();
    }

    private String responseFor(HttpRequest request)
    {
        if (!ENDPOINT.equals(request.path()))
        {
            return response(404, "", Map.of());
        }
        if (!originAllowed(request.header("origin")))
        {
            return response(403, "", Map.of());
        }
        if (!protocolAllowed(request.header("mcp-protocol-version")))
        {
            return response(400, "", Map.of());
        }
        if (!"POST".equals(request.method()))
        {
            return response(405, "", Map.of());
        }
        if (!acceptAllowed(request.header("accept")))
        {
            return response(406, "", Map.of());
        }
        String body = controller.handleJsonRpcLine(request.body());
        if (body.isEmpty())
        {
            return response(202, "", Map.of());
        }
        return response(
            200,
            body,
            Map.of(
                "Content-Type", APPLICATION_JSON,
                "MCP-Protocol-Version", PROTOCOL_VERSION));
    }

    private static HttpRequest readRequest(InputStream input) throws IOException
    {
        ByteArrayOutputStream headerBytes = new ByteArrayOutputStream();
        int state = 0;
        while (headerBytes.size() < MAXIMUM_HEADER_BYTES)
        {
            int value = input.read();
            if (value < 0)
            {
                break;
            }
            headerBytes.write(value);
            state = switch (state)
            {
                case 0 -> value == '\r' ? 1 : 0;
                case 1 -> value == '\n' ? 2 : value == '\r' ? 1 : 0;
                case 2 -> value == '\r' ? 3 : 0;
                case 3 -> value == '\n' ? 4 : 0;
                default -> state;
            };
            if (state == 4)
            {
                break;
            }
        }
        if (state != 4)
        {
            throw new IOException("incomplete HTTP request headers");
        }
        String[] lines = headerBytes.toString(StandardCharsets.US_ASCII).split("\\r\\n");
        String[] requestLine = lines.length == 0 ? new String[0] : lines[0].split(" ", 3);
        if (requestLine.length < 2)
        {
            throw new IOException("invalid HTTP request line");
        }
        Map<String, String> headers = new LinkedHashMap<>();
        for (int index = 1; index < lines.length; ++index)
        {
            int colon = lines[index].indexOf(':');
            if (colon < 0)
            {
                continue;
            }
            headers.put(
                lines[index].substring(0, colon).trim().toLowerCase(Locale.ROOT),
                lines[index].substring(colon + 1).trim());
        }
        int contentLength;
        try
        {
            contentLength = Integer.parseInt(headers.getOrDefault("content-length", "0"));
        }
        catch (NumberFormatException exception)
        {
            throw new IOException("invalid HTTP Content-Length", exception);
        }
        if (contentLength < 0)
        {
            throw new IOException("negative HTTP Content-Length");
        }
        byte[] body = input.readNBytes(contentLength);
        if (body.length != contentLength)
        {
            throw new IOException("incomplete HTTP request body");
        }
        return new HttpRequest(
            requestLine[0],
            requestLine[1],
            headers,
            new String(body, StandardCharsets.UTF_8));
    }

    private static String response(int status, String body, Map<String, String> headers)
    {
        byte[] bodyBytes = body.getBytes(StandardCharsets.UTF_8);
        StringBuilder result = new StringBuilder();
        result.append("HTTP/1.1 ").append(status).append(' ').append(statusText(status)).append("\r\n");
        result.append("Connection: close\r\n");
        result.append("Content-Length: ").append(bodyBytes.length).append("\r\n");
        for (Map.Entry<String, String> header : headers.entrySet())
        {
            result.append(header.getKey()).append(": ").append(header.getValue()).append("\r\n");
        }
        return result.append("\r\n").append(body).toString();
    }

    private static String statusText(int status)
    {
        return switch (status)
        {
            case 200 -> "OK";
            case 202 -> "Accepted";
            case 400 -> "Bad Request";
            case 403 -> "Forbidden";
            case 404 -> "Not Found";
            case 405 -> "Method Not Allowed";
            case 406 -> "Not Acceptable";
            default -> "Error";
        };
    }

    private static boolean acceptAllowed(String accept)
    {
        return accept.isEmpty()
            || accept.contains("*/*")
            || accept.contains(APPLICATION_JSON)
            || accept.contains(TEXT_EVENT_STREAM);
    }

    private static boolean protocolAllowed(String protocol)
    {
        return protocol.isEmpty()
            || PROTOCOL_VERSION.equals(protocol)
            || "2025-03-26".equals(protocol)
            || "2025-06-18".equals(protocol);
    }

    private static boolean originAllowed(String origin)
    {
        return origin.isEmpty()
            || "null".equals(origin)
            || origin.startsWith("http://127.0.0.1")
            || origin.startsWith("http://localhost")
            || origin.startsWith("https://127.0.0.1")
            || origin.startsWith("https://localhost");
    }

    private record HttpRequest(
        String method,
        String path,
        Map<String, String> headers,
        String body)
    {
        String header(String name)
        {
            return headers.getOrDefault(name, "");
        }
    }
}
