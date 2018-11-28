public class Vehicle {
	private Make make;
	
	private Model model;
	
	private double weight;
	
	private double engineSize;
	
	private boolean isImport;
	
	public Vehicle(Make make, Model model, double weight, double engineSize) throws RuntimeException
	{
		setMake(make);
		setModelAndWeight(model, weight);
		setEngineSize(engineSize);
	}
	
	public void setMake(Make make)
	{
		this.make = make;
		this.isImport = this.make.isImport();
	}
	
	public void setModelAndWeight(Model model, double weight) throws RuntimeException
	{
		if (weight > 0.0d)
		{
			if (weight >= model.getMinimumWeightForClass())
			{
				if (weight < model.getMaximumWeightForClass())
				{
					this.model = model;
					this.weight = weight;
				}
				else
				{
					throw new RuntimeException("Weight must be less than the minimum weight for model/class.");
				}
			}
			else
			{
				throw new RuntimeException("Weight must be greater than or equal to the minimum weight for model/class.");
			}
		}
		else
		{
			throw new RuntimeException("Weight cannot be negative.");
		}
	}
	
	public void setEngineSize(double engineSize) throws RuntimeException
	{
		if (engineSize > 0.0d)
		{
			this.engineSize = engineSize;
		}
		else
		{
			throw new RuntimeException("Engine size cannot be negative.");
		}
	}
	
	public Make getMake()
	{
		return this.make;
	}
	
	public Model getModel()
	{
		return this.model;
	}
	
	public double getWeight()
	{
		return this.weight;
	}
	
	public double getEngineSize()
	{
		return this.engineSize;
	}
	
	public boolean getIsImport()
	{
		return this.isImport;
	}
}
