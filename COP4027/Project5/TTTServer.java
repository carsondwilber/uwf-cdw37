import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

public class TTTServer
{
    public final static int SBAP_PORT = 8889;
    
    private static ServerSocket server;
    
    private static List<TicTacToe> waitingGames;
    
    public static void main(String[] args)
    {
        waitingGames = new ArrayList<TicTacToe>();
        
        try
        {
            server = new ServerSocket(SBAP_PORT);
            
            System.out.println("Waiting for clients to connect...");
            
            while (true)
            {
                if (waitingGames.size() == 0)
                {
                    waitingGames.add(new TicTacToe());
                }
                
                waitForPlayer(nextPlayerNeeded(), waitingGames.get(0));
            }
        }
        catch (IOException e)
        {
            e.printStackTrace();
        }
    }
    
    private static void waitForPlayer(int player, TicTacToe ttt)
    {
        try
        {
            while (!ttt.playerReady(player))
            {
                Socket socket = server.accept();
                
                InputStream inputStream = socket.getInputStream();
                
                Scanner scnr = new Scanner(inputStream);
                
                if (scnr.hasNext())
                {
                    if (scnr.next().equals("join"))
                    {
                        if (scnr.hasNext())
                        {
                            String playerName = scnr.next();
                            
                            System.out.printf("Client %d ('%s') connected.\n", player, playerName);
                            
                            OutputStream outputStream = socket.getOutputStream();
                            
                            PrintWriter pntr = new PrintWriter(outputStream);
                            
                            pntr.printf("Hello %s! You are Player %d.\n", playerName, player);
                            pntr.flush();
                            
                            Runnable service = new TTTService(ttt, socket, player);
                            Thread thread = new Thread(service);
                            thread.start();
                            
                            ttt.readyPlayer(player);
                            
                            if (ttt.playerReady(1) && ttt.playerReady(2))
                            {
                                ttt.startGame();
                                waitingGames.remove(ttt);
                            }
                        }
                    }
                }
            }
        }
        catch (IOException e)
        {
            e.printStackTrace();
        }
    }
    
    private static int nextPlayerNeeded()
    {
        int player = 1;
        
        for (TicTacToe ttt : waitingGames)
        {
            if (ttt.playerReady(1))
            {
                player = 2;
            }
        }
        
        return player;
    }
    
    public static void waitOnGame(TicTacToe ttt)
    {
        if (ttt.readyPlayer(1) || ttt.readyPlayer(2))
        {
            waitingGames.add(ttt);
        }
        else
        {
            waitingGames.remove(ttt);
        }
    }
}
