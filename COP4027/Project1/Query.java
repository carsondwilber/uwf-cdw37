import java.sql.Connection;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

public class Query {
	public static ResultSet execute(Connection conn, String query) throws RuntimeException
	{
		DBLogging.log("Attempting to execute query '" + query + "'");
		
		try
		{
			Statement stmt = conn.createStatement();
			
			ResultSet result = stmt.executeQuery(query);
			
			DBLogging.log("Successfully executed query.");
			
			return result;
		}
		catch (SQLException e)
		{
			DBLogging.log("Failed to execute query.");
			e.printStackTrace();
			throw new RuntimeException("Exception when attempting query: " + e.getMessage());
		}
	}
}
