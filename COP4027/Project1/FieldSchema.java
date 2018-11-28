import java.lang.reflect.Field;

public class FieldSchema
{
	public enum FieldType
	{
		INT, BOOLEAN, DOUBLE, VARCHAR;
		
		public int getDefaultSize()
		{
			int size;
			
			switch (this)
			{
			case INT:
			case DOUBLE:
				size = 11;
				break;
			
			case BOOLEAN:
				size = 1;
				break;
			
			case VARCHAR:
				size = 32;
				break;
			
			default:
				size = 0;
				break;
			}
			
			return size;
		}
		
		@Override
		public String toString()
		{
			String ret;
			
			switch (this)
			{
			case INT:
				ret = "int";
				break;
			
			case DOUBLE:
				ret = "double";
				break;
			
			case BOOLEAN:
				ret = "int";
				break;
			
			case VARCHAR:
				ret = "varchar";
				break;
			
			default:
				ret = "";
				break;
			}
			
			return ret;
		}
		
		public static FieldType fromString(String from) throws RuntimeException
		{
			switch (from.toUpperCase())
			{
			case "INT":
			case "INTEGER":
				return INT;
			
			case "DOUBLE":
				return DOUBLE;
			
			case "BOOL":
			case "BOOLEAN":
				return BOOLEAN;
			
			case "VARCHAR":
			case "STRING":
				return VARCHAR;
			
			default:
				throw new RuntimeException("Cannot ascertain field type from unknown string.");
			}
		}
		
		public Class asClass()
		{
			Class clazz;
			
			switch (this)
			{
			default:
				clazz = null;
				break;
			
			case INT:
				clazz = Integer.class;
				break;
			
			case DOUBLE:
				clazz = Double.class;
				break;
			
			case BOOLEAN:
				clazz = Boolean.class;
				break;
			
			case VARCHAR:
				clazz = String.class;
				break;
			}
			
			return clazz;
		}
	}
	
	private Class clazz;
	
	private Field field;
	
	private String name;
	
	private FieldType type;
	
	private int size;
	
	private boolean optional;
	
	private boolean unique;
	
	private boolean key;
	
	public FieldSchema(Class clazz, Field field, String name, FieldType type, int size, boolean optional, boolean unique, boolean key) throws RuntimeException
	{
		if (clazz == null)
		{
			throw new RuntimeException("Cannot create a field schema with a null class.");
		}
		
		if (field == null)
		{
			throw new RuntimeException("Cannot create a field schema with a null field.");
		}
		
		boolean fieldExists = false;
		
		for (Field declaredField : clazz.getDeclaredFields())
		{
			if (field.equals(declaredField))
			{
				fieldExists = true;
			}
		}
		
		if (!fieldExists)
		{
			throw new RuntimeException("Cannot create a field schema with a field that does not belong to the corresponding class.");
		}
		
		if (name == null)
		{
			throw new RuntimeException("Cannot create a field schema with a name that is null.");
		}
		
		if (name.matches("[^a-zA-Z0-9]"))
		{
			throw new RuntimeException("Cannot create a field schema with a non-alphanumeric name or a name with whitespace.");
		}
		
		if (name.charAt(0) >= '0' && name.charAt(0) <= '9')
		{
			throw new RuntimeException("Cannot create a field schema with a number as the first character in the name.");
		}
		
		if (type == null)
		{
			throw new RuntimeException("Cannot create a field schema with a type that is null.");
		}
		
		this.clazz = clazz;
		this.field = field;
		this.name = name;
		this.type = type;
		this.size = size;
		this.optional = optional;
		this.unique = unique;
		this.key = key;
	}
	
	public FieldSchema(Class clazz, Field field, String name, FieldType type, int size) throws RuntimeException
	{
		this(clazz, field, name, type, size, true, false, false);
	}
	
	public FieldSchema(Class clazz, Field field, String name, FieldType type) throws RuntimeException
	{
		this(clazz, field, name, type, type.getDefaultSize());
	}
	
	public String getName()
	{
		return this.name;
	}
	
	public FieldType getType()
	{
		return this.type;
	}
	
	public int getSize()
	{
		return this.size;
	}
	
	public Class getSchemaClass()
	{
		return this.clazz;
	}
	
	public Field getSchemaField()
	{
		return this.field;
	}
	
	public boolean isOptional()
	{
		return this.optional;
	}
	
	public boolean isUnique()
	{
		return this.unique;
	}
	
	public boolean isKey()
	{
		return this.key;
	}
	
	@Override
	public String toString()
	{
		String ret = this.name + " " + this.type.toString();
		
		if (this.type == FieldType.VARCHAR)
		{
			ret += "(" + Integer.toString(this.size) + ")";
		}
		
		if (this.key)
		{
			ret += " KEY";
		}
		else if (this.unique)
		{
			ret += " UNIQUE";
		}
		
		if (!this.optional) {
			ret += " NOT NULL";
		}
		
		return ret;
	}
}
