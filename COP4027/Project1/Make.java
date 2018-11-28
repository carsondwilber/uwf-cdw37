public enum Make {
	CHEVY, FORD, TOYOTA, NISSAN, HYUNDAI;
	
	public String toString()
	{
		String ret;
		
		switch (this)
		{
		case CHEVY:
			ret = "Chevy";
			break;
			
		case FORD:
			ret = "Ford";
			break;
			
		case TOYOTA:
			ret = "Toyota";
			break;
			
		case NISSAN:
			ret = "Nissan";
			break;
			
		case HYUNDAI:
			ret = "Hyundai";
			break;
			
		default:
			ret = "";
			break;
		}
		
		return ret;
	}
	
	public static Make fromString(String from) throws RuntimeException
	{
		switch (from.toUpperCase())
		{
		case "CHEVY":
			return CHEVY;
		
		case "FORD":
			return FORD;
			
		case "TOYOTA":
			return TOYOTA;
			
		case "NISSAN":
			return NISSAN;
			
		case "HYUNDAI":
			return HYUNDAI;
		
		default:
			throw new RuntimeException("Unknown make '" + from + "'.");
		}
	}
	
	public boolean isImport()
	{
		switch (this)
		{
		case TOYOTA:
		case NISSAN:
		case HYUNDAI:
			return true;
		
		default:
			return false;
		}
	}
}
