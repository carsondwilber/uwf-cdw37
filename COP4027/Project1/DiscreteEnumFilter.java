import java.util.stream.Stream;

public class DiscreteEnumFilter implements FieldValueFilter<String>
{
	private String[] validValues;
	
	public DiscreteEnumFilter(String[] validValues)
	{
		this.validValues = Stream.of(validValues).map((value) -> { return "'"+value+"'"; }).toArray(String[]::new);
	}
	
	@Override
	public boolean test(String value)
	{
		for (String validValue : validValues)
		{
			if (value.equals(validValue))
			{
				return true;
			}
		}
		
		return false;
	}
	
	@Override
	public String asConstraintOn(FieldSchema field)
	{
		return "CONSTRAINT chk_" + field.getName() + " CHECK (" + field.getName() + " IN (" + String.join(", ", validValues) + "))";
	}
}
