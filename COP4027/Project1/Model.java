public enum Model
{
	COMPACT, INTERMEDIATE, FULLSIZED, SUV;
	
	public String toString()
	{
		if (this == COMPACT)
		{
			return "Compact";
		}
		else if (this == INTERMEDIATE)
		{
			return "Intermediate";
		}
		else if (this == FULLSIZED)
		{
			return "Full Sized";
		}
		else
		{
			return "SUV";
		}
	}
	
	public static Model fromString(String from) throws RuntimeException
	{
		switch (from.toUpperCase())
		{
		case "COMPACT":
			return COMPACT;
		
		case "INTERMEDIATE":
			return INTERMEDIATE;
		
		case "FULLSIZED":
		case "FULL SIZED":
		case "FULL-SIZED":
			return FULLSIZED;
		
		case "SUV":
			return SUV;
		
		default:
			throw new RuntimeException("Unknown model '" + from + "'.");
		}
	}
	
	public double getMinimumWeightForClass()
	{
		if (this == COMPACT)
		{
			return 1500.0d;
		}
		else if (this == INTERMEDIATE)
		{
			return 2000.0d;
		}
		else
		{
			return 2500.0d;
		}
	}
	
	public double getMaximumWeightForClass()
	{
		if (this == COMPACT)
		{
			return 2000.0d;
		}
		else if (this == INTERMEDIATE)
		{
			return 2500.0d;
		}
		else
		{
			return 4000.0d;
		}
	}
}
