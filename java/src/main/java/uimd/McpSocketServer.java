package uimd;

import java.io.IOException;
import java.net.InetSocketAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.SocketException;
import java.util.concurrent.CountDownLatch;
import java.util.concurrent.TimeUnit;

abstract class McpSocketServer implements AutoCloseable
{
    private static final int LISTEN_BACKLOG = 16;
    private static final int START_TIMEOUT_SECONDS = 5;
    private static final int CLIENT_DRAIN_TIMEOUT_SECONDS = 5;

    private final String host;
    private final int requestedPort;
    private final CountDownLatch started = new CountDownLatch(1);
    private final Object clientMonitor = new Object();
    private volatile boolean running;
    private volatile RuntimeException failure;
    private volatile ServerSocket serverSocket;
    private int activeClients;
    private Thread serverThread;

    McpSocketServer(McpRuntimeConfig config)
    {
        host = config.host();
        requestedPort = config.port();
    }

    public final void start()
    {
        if (serverThread != null)
        {
            return;
        }
        running = true;
        serverThread = new Thread(this::serveSafely, serverName());
        serverThread.setDaemon(true);
        serverThread.start();
        awaitStart();
    }

    public final void serveForever()
    {
        running = true;
        serveSafely();
        if (failure != null)
        {
            throw failure;
        }
    }

    public final int port()
    {
        ServerSocket current = serverSocket;
        return current == null ? requestedPort : current.getLocalPort();
    }

    public final void stop()
    {
        running = false;
        ServerSocket current = serverSocket;
        if (current != null)
        {
            try
            {
                current.close();
            }
            catch (IOException ignored)
            {
                // Closing a stopped listener is best effort.
            }
        }
        Thread currentThread = serverThread;
        if (currentThread != null && currentThread != Thread.currentThread())
        {
            try
            {
                currentThread.join(TimeUnit.SECONDS.toMillis(START_TIMEOUT_SECONDS));
            }
            catch (InterruptedException exception)
            {
                Thread.currentThread().interrupt();
            }
        }
        awaitClientDrain();
    }

    @Override
    public final void close()
    {
        stop();
    }

    protected abstract String serverName();

    protected abstract void handleClient(Socket socket) throws IOException;

    private void serveSafely()
    {
        try (ServerSocket listener = new ServerSocket())
        {
            listener.setReuseAddress(true);
            listener.bind(new InetSocketAddress(host, requestedPort), LISTEN_BACKLOG);
            serverSocket = listener;
            started.countDown();
            while (running)
            {
                Socket client;
                try
                {
                    client = listener.accept();
                }
                catch (SocketException exception)
                {
                    if (!running)
                    {
                        break;
                    }
                    throw exception;
                }
                Thread worker = new Thread(
                    () -> handleClientSafely(client),
                    serverName() + "-client");
                worker.setDaemon(true);
                clientStarted();
                try
                {
                    worker.start();
                }
                catch (RuntimeException exception)
                {
                    clientFinished();
                    throw exception;
                }
            }
        }
        catch (IOException exception)
        {
            if (running)
            {
                failure = new IllegalStateException(
                    "cannot start " + serverName() + " on " + host + ":" + requestedPort,
                    exception);
            }
        }
        finally
        {
            running = false;
            started.countDown();
        }
    }

    private void handleClientSafely(Socket client)
    {
        try (Socket socket = client)
        {
            handleClient(socket);
        }
        catch (IOException ignored)
        {
            // A disconnected MCP client must not terminate the server.
        }
        finally
        {
            clientFinished();
        }
    }

    private void clientStarted()
    {
        synchronized (clientMonitor)
        {
            ++activeClients;
        }
    }

    private void clientFinished()
    {
        synchronized (clientMonitor)
        {
            --activeClients;
            clientMonitor.notifyAll();
        }
    }

    private void awaitClientDrain()
    {
        long remainingNanoseconds = TimeUnit.SECONDS.toNanos(CLIENT_DRAIN_TIMEOUT_SECONDS);
        long deadline = System.nanoTime() + remainingNanoseconds;
        synchronized (clientMonitor)
        {
            while (activeClients > 0 && remainingNanoseconds > 0L)
            {
                try
                {
                    TimeUnit.NANOSECONDS.timedWait(clientMonitor, remainingNanoseconds);
                }
                catch (InterruptedException exception)
                {
                    Thread.currentThread().interrupt();
                    return;
                }
                remainingNanoseconds = deadline - System.nanoTime();
            }
        }
    }

    private void awaitStart()
    {
        try
        {
            if (!started.await(START_TIMEOUT_SECONDS, TimeUnit.SECONDS))
            {
                stop();
                throw new IllegalStateException(serverName() + " did not start in time");
            }
        }
        catch (InterruptedException exception)
        {
            Thread.currentThread().interrupt();
            stop();
            throw new IllegalStateException(serverName() + " start interrupted", exception);
        }
        if (failure != null)
        {
            throw failure;
        }
    }
}
