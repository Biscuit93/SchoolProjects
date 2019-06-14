package tsp;

public class NearestNeighbour extends Solution
{
	@Override
	public void solve ( Map m )
	{
		super.setMap( m );

		int[ ] path = new int[ m.getNumberOfCities( ) ];
		path[ 0 ] = ( int ) ( Math.random( ) * m.getNumberOfCities( ) );
		boolean[ ] visited = new boolean[ path.length ];
		visited[ path[ 0 ] ] = true;

		int closest;
		double currentDistance, shortestDistance;

		for ( int i = 1; i < path.length; i++ )
		{
			closest = -1;
			shortestDistance = Double.MAX_VALUE;

			for ( int j = 0; j < path.length; j++ )
			{
				if ( visited[ j ] )
					continue;

				currentDistance = euclideanDistanceBetweenCities( path[ i - 1 ], j );
				if ( currentDistance < shortestDistance )
				{
					closest = j;
					shortestDistance = currentDistance;
				}
			}
			
			path[ i ] = closest;
			visited[ path[ i ] ] = true;
		}

		super.setSolution( path );
		super.setDistance( );
	}
}