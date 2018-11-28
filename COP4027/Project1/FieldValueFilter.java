public interface FieldValueFilter <FieldType>
{
	public boolean test(FieldType value);
	
	public String asConstraintOn(FieldSchema field);
}
