package uimd;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;
import java.net.Socket;
import java.nio.charset.StandardCharsets;

public final class McpTcpServer extends McpSocketServer
{
    private final McpController controller;

    public McpTcpServer(McpController controller, McpRuntimeConfig config)
    {
        super(config);
        this.controller = controller;
    }

    @Override
    protected String serverName()
    {
        return "uimd-java-mcp-tcp";
    }

    @Override
    protected void handleClient(Socket socket) throws IOException
    {
        BufferedReader reader = new BufferedReader(
            new InputStreamReader(socket.getInputStream(), StandardCharsets.UTF_8));
        String line = reader.readLine();
        if (line == null || line.isEmpty())
        {
            return;
        }
        String response = controller.handleJsonRpcLine(line);
        if (response.isEmpty())
        {
            return;
        }
        BufferedWriter writer = new BufferedWriter(
            new OutputStreamWriter(socket.getOutputStream(), StandardCharsets.UTF_8));
        writer.write(response);
        writer.newLine();
        writer.flush();
    }
}
