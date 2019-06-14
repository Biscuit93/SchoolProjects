package tsp;

public abstract class Solution
{
	private Map map;
	private int[ ] solution;
	private double distance;

	abstract void solve ( Map m );

	public double euclideanDistanceBetweenCities( int c1, int c2 )
	{
		double result = 0;
		double[ ] city1 = map.getCity( c1 );
		double[ ] city2 = map.getCity( c2 );
		
		for ( int i = 0; i < city1.length; i++ )
			result += ( city1[ i ] - city2[ i ] ) * ( city1[ i ] - city2[ i ] );

		return Math.sqrt( result );
	}

	public void setDistance ( )
	{
		distance = calculateDistance( solution );
	}

	public double calculateDistance ( int[ ] path )
	{
		double result = 0;

		for ( int i = 0; i < path.length; i++ )
			result += euclideanDistanceBetweenCities( path[ i ], path[ ( i + 1 ) % path.length ] );

		return result;
	}

	public int[ ] createOrder ( int n )
	{
		int[ ] result = new int[ n ];

		for ( int i = 0; i < result.length; i++ )
			result[ i ] = i;

		return result;
	}

	public int[ ] shuffle ( int[ ] n )
	{
		int[ ] result = n;
		int temp, swap;

		for ( int i = result.length - 1; i > 0; i-- )
		{
			swap = ( int ) ( Math.random( ) * i );
			temp = result[ i ];
			result[ i ] = result[ swap ];
			result[ swap ] = temp;
		}

		return result;
	}

	public double getDistance ( ) { return distance; }
	public int[ ] getSolution ( ) { return solution; }
	public void setSolution ( int[ ] s ) { solution = s; }
	public Map getMap ( ) { return map; }
	public void setMap ( Map m ) { map = m; }
}