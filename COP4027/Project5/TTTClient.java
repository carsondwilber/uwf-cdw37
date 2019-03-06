import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.net.Socket;
import java.util.Scanner;

public class TTTClient
{
    public static void main(String[] args)
    {
        try
        {
            Socket client = new Socket("localhost", TTTServer.SBAP_PORT);
            
            InputStream inputStream = client.getInputStream();
            OutputStream outputStream = client.getOutputStream();
            
            Scanner scnr = new Scanner(inputStream);
            PrintWriter pntr = new PrintWriter(outputStream);
            
            Scanner stdin = new Scanner(System.in);
            
            boolean exiting = false;
            
            while (!exiting)
            {
                String command = stdin.nextLine();
                
                pntr.println(command);
                pntr.flush();
                
                String commandResponse = scnr.nextLine();
                
                System.out.println(commandResponse.replace("\\n", "\n"));

                String continueMessage = scnr.nextLine();
                
                System.out.println(continueMessage.replace("\\n", "\n"));
                
                if (command.equals("quit"))
                {
                    exiting = true;
                }
            }
        }
        catch (IOException e)
        {
            e.printStackTrace();
        }
    }
}
