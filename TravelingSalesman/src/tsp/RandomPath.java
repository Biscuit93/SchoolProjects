package tsp;

public class RandomPath extends Solution
{
	@Override
	public void solve ( Map m )
	{
		super.setMap( m );
		int[ ] path = createOrder( super.getMap( ).getNumberOfCities( ) );
		path = super.shuffle( path );
		super.setSolution( path );
		super.setDistance( );
	}
}