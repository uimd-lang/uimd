package uimd;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.nio.charset.StandardCharsets;

public final class McpStdioServer
{
    private final McpController controller;
    private final BufferedReader input;
    private final BufferedWriter output;

    public McpStdioServer(McpController controller)
    {
        this(controller, System.in, System.out);
    }

    McpStdioServer(McpController controller, InputStream input, OutputStream output)
    {
        this.controller = controller;
        this.input = new BufferedReader(new InputStreamReader(input, StandardCharsets.UTF_8));
        this.output = new BufferedWriter(new OutputStreamWriter(output, StandardCharsets.UTF_8));
    }

    public void serveForever()
    {
        try
        {
            String line;
            while ((line = input.readLine()) != null)
            {
                String response = controller.handleJsonRpcLine(line);
                if (!response.isEmpty())
                {
                    output.write(response);
                    output.newLine();
                    output.flush();
                }
            }
        }
        catch (IOException exception)
        {
            throw new IllegalStateException("Java MCP stdio transport failed", exception);
        }
    }
}
