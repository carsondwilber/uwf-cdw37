import java.io.IOException;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.NoSuchElementException;
import java.util.Scanner;

public class TTTService implements Runnable
{
    private TicTacToe ttt;
    
    private Socket socket;
    
    private int player;
    
    public TTTService(TicTacToe ttt, Socket socket, int player)
    {
        this.ttt = ttt;
        this.socket = socket;
        this.player = player;
    }

    @Override
    public void run()
    {
        try
        {
            boolean exiting = false;
            
            Scanner scnr = new Scanner(socket.getInputStream());
            
            PrintWriter pntr = new PrintWriter(socket.getOutputStream());
            
            while (!exiting)
            {
                try
                {
                    while (!ttt.isPlayerTurn(player) && ttt.getWinner() == -1) {
                        try
                        {
                            Thread.sleep(100);
                        }
                        catch (InterruptedException e)
                        {
                            
                        }
                    }
                    
                    if (ttt.gameIsOver())
                    {
                        if (ttt.getWinner() == player)
                        {
                            pntr.println("You won!");
                        }
                        else
                        {
                            pntr.println("You lost!");
                        }
                        
                        pntr.flush();

                        pntr.println("Goodbye.");
                        pntr.flush();
                        
                        exiting = true;
                    }
                    else
                    {
                        pntr.println(ttt.getGameState().replace("\n", "\\n"));
                        pntr.flush();
                        
                        String clientCommand = scnr.nextLine();
                        
                        Scanner cmd = new Scanner(clientCommand);
                        
                        boolean validCommand = false;
                        
                        int player = -1, x = -1, y = -1;
                        
                        if (cmd.hasNext())
                        {
                            clientCommand = cmd.next();
                            
                            switch (clientCommand)
                            {
                            case "quit":
                            {
                                if (!cmd.hasNext())
                                {
                                    validCommand = true;
                                }
                                
                                break;
                            }
                            
                            case "choice":
                            {
                                if (cmd.hasNextInt())
                                {
                                    player = cmd.nextInt();
                                    
                                    if (cmd.hasNextInt())
                                    {
                                        x = cmd.nextInt();
                                        
                                        if (cmd.hasNextInt())
                                        {
                                            y = cmd.nextInt();
                                            
                                            if (!cmd.hasNext())
                                            {
                                                validCommand = true;
                                            }
                                        }
                                    }
                                }
                                
                                break;
                            }
                            }
                        }
                        
                        cmd.close();
                        
                        if (validCommand)
                        {
                            String serverResponse = "Unknown error";
                            
                            switch (clientCommand)
                            {
                            case "quit":
                            {
                                if (ttt.removePlayer(this.player))
                                {
                                    pntr.println("Game over");
                                    pntr.flush();
                                    serverResponse = "";
                                    exiting = true;
                                }
                                
                                break;
                            }
                            
                            case "choice":
                            {
                                if (this.player == player)
                                {
                                    int responseCode = this.ttt.tryMove(player, x, y);
                                    
                                    switch (responseCode)
                                    {
                                    case 0:
                                    {
                                        serverResponse = ttt.getGameState().replace("\n", "\\n");
                                        break;
                                    }
                                    
                                    case 1:
                                    {
                                        serverResponse = "Game is over. ";
                                        break;
                                    }
                                    
                                    case 2:
                                    {
                                        serverResponse = "Game is not in progress.";
                                        break;
                                    }
                                    
                                    case 3:
                                    {
                                        serverResponse = "We do not have two players yet.";
                                        break;
                                    }
                                    
                                    case 4:
                                    {
                                        serverResponse = String.format("Not Player %d turn", player);
                                        break;
                                    }
                                    
                                    case 5:
                                    {
                                        serverResponse = "Illegal board position";
                                        break;
                                    }
                                    
                                    case 6:
                                    {
                                        serverResponse = String.format("Position %d %d is taken, please try again", x, y);
                                        break;
                                    }
                                    }
                                }
                                else
                                {
                                    serverResponse = String.format("You are not player %d", player);
                                    break;
                                }
                                
                                break;
                            }
                            
                            default:
                            {
                                serverResponse = "Unknown command";
                                break;
                            }
                            }
                            
                            pntr.println(serverResponse);
                            pntr.flush();
                        }
                    }
                }
                catch (NoSuchElementException e)
                {
                    // Connection dropped
                    exiting = true;
                }
            }
        }
        catch (IOException e)
        {
            
        }
        
        try
        {
            if (socket != null)
            {
                if (!socket.isClosed())
                {
                    socket.close();
                }
                
                ttt.removePlayer(player);
                TTTServer.waitOnGame(ttt);
            }
        }
        catch (IOException e)
        {
            e.printStackTrace();
        }
    }
}
