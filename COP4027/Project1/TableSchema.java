import java.lang.reflect.Field;
import java.sql.Connection;
import java.sql.Statement;

public class TableSchema
{
	private Class clazz;
	
	private String name;
	
	private FieldSchema[] fields;
	
	private FieldValueFilter[] filters;
	
	private FieldSchema[] filtersOn;
	
	public TableSchema(Class clazz, String name, FieldSchema[] fields, FieldValueFilter[] filters, FieldSchema[] filtersOn)
	{
		if (clazz == null)
		{
			throw new RuntimeException("Cannot create a table schema with a class that is null.");
		}
		
		this.clazz = clazz;
		
		if (name == null)
		{
			throw new RuntimeException("Cannot create a table schema with a name that is null.");
		}
		
		if (name.matches("[^a-zA-Z0-9]"))
		{
			throw new RuntimeException("Cannot create a table schema with a non-alphanumeric name or a name with whitespace.");
		}
		
		if (name.charAt(0) >= '0' && name.charAt(0) <= '9')
		{
			throw new RuntimeException("Cannot create a table schema with a number as the first character in the name.");
		}
		
		this.name = name;
		
		for (FieldSchema field : fields)
		{
			if (field == null)
			{
				throw new RuntimeException("Cannot create a table schema from a null field schema.");
			}
			
			if (!field.getSchemaClass().equals(clazz))
			{
				throw new RuntimeException("Cannot create a table schema from a field schema that does not exist in the table's class.");
			}
		}
		
		this.fields = fields;
				
		if (filters != null)
		{
			if (filtersOn != null)
			{
				if (filters.length == filtersOn.length)
				{
					for (FieldSchema field : filtersOn)
					{
						if (field == null)
						{
							throw new RuntimeException("Cannot create a table schema from a null field schema.");
						}
						
						boolean known = false;
						
						for (FieldSchema knownField : fields)
						{
							if (field.equals(knownField))
							{
								known = true;
								break;
							}
						}
						
						if (!known)
						{
							throw new RuntimeException("Cannot create a table schema with a filter that applies to a class not in the table's class.");
						}
					}
					
					this.filters = filters;
					this.filtersOn = filtersOn;
				}
				else
				{
					throw new RuntimeException("Cannot create a table schema with a filter that does not apply to any field.");
				}
			}
			else
			{
				throw new RuntimeException("Cannot create a table schema with a filter that does not apply to any field.");
			}
		}
	}
	
	public TableSchema(Class clazz, String name, FieldSchema[] fields) throws RuntimeException
	{
		this(clazz, name, fields, null, null);
	}
	
	public boolean build(Connection conn)
	{
		try
		{
			Statement stmt = conn.createStatement();
			
			String query = "CREATE TABLE " + this.name + " (";
			
			for (int i = 0; i < this.fields.length - 1; i++)
			{
				query += this.fields[i].toString();
				query += ", ";
			}
			
			query += this.fields[this.fields.length - 1].toString();
			
			if (this.filters != null)
			{
				if (this.filters.length > 0)
				{
					query += ", ";
					
					for (int i = 0; i < this.filters.length - 1; ++i)
					{
						query += this.filters[i].asConstraintOn(this.filtersOn[i]);
						query += ", ";
					}
					
					query += this.filters[this.filters.length - 1].asConstraintOn(this.filtersOn[this.filtersOn.length - 1]);
				}
			}
			
			query += ")";
			
			DBLogging.log("Attempting to create table with query '" + query + "'");
						
			stmt.execute(query);
			
			DBLogging.log("Successfully created table.");
			
			stmt.close();
			
			return true;
		}
		catch (Exception e)
		{
			DBLogging.log("Failed to create table.");
			e.printStackTrace();
			return false;
		}
	}
	
	public boolean destroy(Connection conn)
	{
		try
		{
			Statement stmt = conn.createStatement();
			
			String query = "DROP TABLE " + this.name;
			
			DBLogging.log("Attempted to delete table with query '" + query + "'");
			
			stmt.execute(query);
			
			DBLogging.log("Successfully deleted table.");
			
			stmt.close();
			
			return true;
		}
		catch (Exception e)
		{
			e.printStackTrace();
			return false;
		}
	}
	
	public Class getSchemaClass()
	{
		return this.clazz;
	}
	
	public String getInsertionQueryFor(Object obj) throws RuntimeException
	{
		if (obj.getClass() != this.clazz)
		{
			throw new RuntimeException("Cannot generate insertion query for object that is not of table's class.");
		}
		
		String query = "INSERT INTO " + this.name + " (";
		
		String values = " VALUES (";
		
		try
		{
			for (Field field : obj.getClass().getDeclaredFields())
			{
				field.setAccessible(true);
				
				if (field.isAccessible())
				{
					for (FieldSchema schema : this.fields)
					{
						if (schema.getSchemaField().equals(field))
						{
							if (field.get(obj) == null && !schema.isOptional())
							{
								throw new RuntimeException("Cannot insert row with field value that is null for non-optional column.");
							}
							
							query += schema.getName() + ", ";
						}
					}
					
					if (field.getType().equals(String.class) || field.getType().isEnum())
					{
						values += "'" + field.get(obj).toString() + "'";
					}
					else if (field.getType().equals(Boolean.class) || field.getType().equals(boolean.class))
					{
						values += ((Boolean) field.get(obj)).booleanValue() ? 1 : 0;
					}
					else
					{
						values += field.get(obj).toString();
					}
					
					values += ", ";
				}
				else
				{
					throw new RuntimeException("Could not suppress Java access checks.");
				}
			}
		}
		catch (Exception e)
		{
			e.printStackTrace();
			throw new RuntimeException("Could not generate insertion query: " + e.getMessage());
		}
		
		query = query.substring(0, query.length() - 2) + ")" + values.substring(0, values.length() - 2) + ")";
		
		return query;
	}
}
