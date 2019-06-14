package tsp;

public class PairwiseExchange extends Solution
{
	@Override
	public void solve ( Map m )
	{
		super.setMap( m );
		
		int[ ] path = createOrder( super.getMap( ).getNumberOfCities( ) );
		int[ ] tempPath;
		path = super.shuffle( path );

		int improve = 0;
		int iteration = 0;
		double tempDistance, bestDistance;

		while ( improve < 500 )
		{
			bestDistance = calculateDistance( path );

			for ( int i = 1; i < path.length - 1; i++ )
				for ( int k = i + 1; k < path.length; k++ )
				{
					tempPath = twoOptSwap( path, i, k );
					tempDistance = calculateDistance( tempPath );
					iteration++;

					if ( !( tempDistance < bestDistance ) )
						continue;

					improve = 0;
					path = tempPath;
					bestDistance = tempDistance;
				}

			improve++;
		}

		super.setSolution( path );
		super.setDistance( );
	}

	private int[ ] twoOptSwap ( int[ ] path, int i, int k )
	{
		int[ ] result = new int[ path.length ];

		for ( int j = 0; j < i; j++ )
			result[ j ] = path[ j ];
		int d = 0;
		for ( int j = i; j <= k; j++ )
		{
			result[ j ] = path[ k - d ];
			d++;
		}
		for ( int j = k + 1; j < path.length; j++ )
			result[ j ] = path[ j ];

		return result;
	}
}