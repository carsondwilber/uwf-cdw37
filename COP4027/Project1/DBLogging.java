import java.io.PrintStream;
import java.io.IOException;
import java.io.FileOutputStream;

public class DBLogging {
	private static String file;
	
	private static PrintStream out;
	
	public static void close()
	{
		if (out != null)
		{
			out.close();
		}
	}
	
	public static void openLog(String fileName) throws IOException
	{
		close();
		
		file = fileName;
		
		out = new PrintStream(new FileOutputStream(fileName));
	}
	
	public static void log(String info)
	{
		if (out != null)
		{
			out.printf("[%d] %s\n", System.currentTimeMillis(), info);
		}
	}
	
	public static String getFileName()
	{
		return file;
	}
}
