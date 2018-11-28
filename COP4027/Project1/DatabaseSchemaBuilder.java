import java.lang.reflect.*;
import java.util.stream.*;
import java.util.ArrayList;

public class DatabaseSchemaBuilder {
	public static DatabaseSchema fromClasses(Class... classes) throws RuntimeException
	{
		return new DatabaseSchema(Stream.of(classes).map((clazz) -> fromClass(clazz)).toArray(TableSchema[]::new));
	}
	
	private static TableSchema fromClass(Class clazz) throws RuntimeException
	{
		if (clazz == null)
		{
			throw new RuntimeException("Cannot build a table schema from a null class.");
		}
		
		ArrayList<FieldSchema> fields = new ArrayList<>();
		
		ArrayList<FieldValueFilter> filters = new ArrayList<>();
		
		ArrayList<FieldSchema> filtersOn = new ArrayList<>();
		
		for (Field field : clazz.getDeclaredFields())
		{
			FieldSchema.FieldType fieldType = null;
			
			if (field.getType().equals(Integer.class))
			{
				fieldType = FieldSchema.FieldType.INT;
			}
			else if (field.getType().equals(Double.class) || field.getType().equals(double.class))
			{
				fieldType = FieldSchema.FieldType.DOUBLE;
			}
			else if (field.getType().equals(Boolean.class)|| field.getType().equals(boolean.class))
			{
				fieldType = FieldSchema.FieldType.BOOLEAN;
			}
			else if (field.getType().equals(String.class) || field.getType().isEnum())
			{
				fieldType = FieldSchema.FieldType.VARCHAR;
			}
			else
			{
				throw new RuntimeException("Supported types are int, double, boolean, and string.");
			}
			
			FieldSchema fieldSchema = new FieldSchema(clazz, field, field.getName(), fieldType);
			
			fields.add(fieldSchema);
			
			if (field.getType().isEnum())
			{
				DiscreteEnumFilter filter = new DiscreteEnumFilter(Stream.of(field.getType().getEnumConstants()).map((value) -> value.toString()).toArray(String[]::new));
				
				filters.add(filter);
				filtersOn.add(fieldSchema);
			}
		}
		
		return new TableSchema(clazz,
				clazz.getSimpleName(), 
				(FieldSchema[]) fields.toArray(new FieldSchema[fields.size()]), 
				(FieldValueFilter[]) filters.toArray(new FieldValueFilter[filters.size()]), 
				(FieldSchema[]) filtersOn.toArray(new FieldSchema[filtersOn.size()]));
	}
}
