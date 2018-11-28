import java.util.Random;
import java.io.BufferedReader;
import java.io.FileReader;
import java.sql.Connection;
import java.sql.Statement;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;

public class VehicleDBTest
{
	public static void printTable(ResultSet rs) throws RuntimeException
	{
		try
		{
			ResultSetMetaData md = rs.getMetaData();
			
			for (int i = 1; i <= md.getColumnCount(); ++i)
			{
				System.out.printf("%20s", md.getColumnName(i));
			}
			
			System.out.println();
			
			while (rs.next())
			{
				for (int i = 1; i <= md.getColumnCount(); ++i)
				{
					switch (md.getColumnType(i))
					{
					case java.sql.Types.INTEGER:
						System.out.printf("%20d", rs.getInt(i));
						break;
					
					case java.sql.Types.DOUBLE:
						System.out.printf("%20.5f", rs.getDouble(i));
						break;
					
					case java.sql.Types.VARCHAR:
						System.out.printf("%20s", rs.getString(i));
						break;
					
					default:
						System.out.printf("%20s", rs.getObject(i).toString());
						break;
					}
				}
				
				System.out.println();
			}
			
			rs.close();
		}
		catch (Exception e)
		{
			e.printStackTrace();
			throw new RuntimeException("Exception while printing table: " + e.getMessage());
		}
	}
	
	public static void main(String[] args)
	{
		if (args.length != 1)
		{
			System.err.println("Must input database.properties file.");
			System.exit(-1);
		}
		
		Random random = new Random(System.nanoTime());
		
		Vehicle[] vehicles = new Vehicle[10];
		
		for (int i = 0; i < vehicles.length; ++i)
		{
			Make make = Make.values()[random.nextInt(Make.values().length)];
			
			Model model = Model.values()[random.nextInt(Model.values().length)];
			
			double weight = model.getMinimumWeightForClass() 
					+ (random.nextDouble() * 
							(model.getMaximumWeightForClass() 
									- model.getMinimumWeightForClass())
					);
			
			double engineSize = (0.5d + random.nextDouble()) * 2500.0d;
			
			vehicles[i] = new Vehicle(make, model, weight, engineSize);
		}
		
		try
		{
			DBLogging.openLog("dbOperations.log");
			
			SimpleDataSource.init(args[0]);
			
			Connection conn = SimpleDataSource.getConnection();
			
			Statement stmt = conn.createStatement();
			
			DatabaseSchema schema = DatabaseSchemaBuilder.fromClasses(Vehicle.class);
			
			schema.build(conn);
			
			TableSchema tSchema = schema.getTableSchemaFor(Vehicle.class);
			
			for (int i = 0; i < 10; ++i)
			{
				stmt.execute(tSchema.getInsertionQueryFor(vehicles[i]));
			}
			
			System.out.println("All vehicles in the database:");
			
			printTable(Query.execute(conn, "SELECT * FROM Vehicle"));
			
			System.out.println("All vehicles with weight greater than 2500 lb:");
			
			printTable(Query.execute(conn, "SELECT * FROM Vehicle WHERE weight > 2500"));
			
			System.out.println("All vehicles that are imported SUVs:");
			
			printTable(Query.execute(conn, "SELECT * FROM Vehicle WHERE Model = 'SUV' AND isImport = 1"));
			
			schema.destroy(conn);
			
			DBLogging.close();
			
			System.out.println("Log file:");
			
			BufferedReader in = new BufferedReader(new FileReader(DBLogging.getFileName()));
			
			String line;
			
			while ((line = in.readLine()) != null)
			{
				System.out.println(line);
			}
		}
		catch (Exception e)
		{
			e.printStackTrace();
			System.exit(-1);
		}
	}
}
