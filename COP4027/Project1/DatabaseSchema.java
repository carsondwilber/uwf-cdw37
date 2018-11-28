import java.sql.Connection;
import java.sql.Statement;

public class DatabaseSchema
{
	private TableSchema[] tables;
	
	public DatabaseSchema(TableSchema[] tables) throws RuntimeException
	{
		for (TableSchema table : tables)
		{
			if (table == null)
			{
				throw new RuntimeException("Cannot create a database schema with a null table schema.");
			}
		}
		
		this.tables = tables;
	}
	
	public boolean build(Connection conn)
	{
		try
		{
			for (TableSchema table : this.tables)
			{
				table.build(conn);
			}
			
			return true;
		}
		catch (Exception e)
		{
			e.printStackTrace();
			return false;
		}
	}
	
	public boolean destroy(Connection conn)
	{
		try
		{
			for (TableSchema table : this.tables)
			{
				table.destroy(conn);
			}
			
			return true;
		}
		catch (Exception e)
		{
			e.printStackTrace();
			return false;
		}
	}
	
	public TableSchema getTableSchemaFor(Class clazz) throws RuntimeException
	{
		for (TableSchema schema : this.tables)
		{
			if (schema.getSchemaClass().equals(clazz))
			{
				return schema;
			}
		}
		
		throw new RuntimeException("Table schema does not exist for class " + clazz.getName() + ".");
	}
}
