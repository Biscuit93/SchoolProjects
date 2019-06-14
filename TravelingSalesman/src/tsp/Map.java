package tsp;

import java.io.*;
import java.awt.*;

public class Map
{
	final private String PATH = "data/in/";

	private String name;
	private int numberOfCities = -1;
	private double[ ][ ] city;
	private double[ ] northWestCorner, southEastCorner;

	public Map ( String fileName )
	{
		loadData( fileName );
		findExtremes( );
	}

	private void loadData ( String fileName )
	{
		String line;
		String[ ] temp;

		FileReader fr = null;
		BufferedReader br = null;

		try
		{
			fr = new FileReader( PATH + fileName );
			br = new BufferedReader( fr );

			while ( ( line = br.readLine( ) ) != null )
			{
				if ( line.split( " " )[ 0 ].equalsIgnoreCase( "NAME:" ) )
					name = line.split( " " )[ 1 ];
				else if ( line.split( " " )[ 0 ].equalsIgnoreCase( "DIMENSION:" ) )
				{
					numberOfCities = Integer.parseInt( line.split( " " )[ 1 ] );
					city = new double[ numberOfCities ][ 2 ];
				}
				else if ( line.split( " " )[ 0 ].equalsIgnoreCase( "NODE_COORD_SECTION" ) )
					while ( !( line = br.readLine( ) ).equalsIgnoreCase( "EOF" ) )
					{
						temp = line.split( " " );
						city[ Integer.parseInt( temp[ 0 ] ) - 1 ][ 0 ] = Double.parseDouble( temp[ 1 ] );
						city[ Integer.parseInt( temp[ 0 ] ) - 1 ][ 1 ] = Double.parseDouble( temp[ 2 ] );
					}
			}

		} catch ( IOException e ) {
			e.printStackTrace( );
		}
	}

	private void findExtremes ( )
	{
		if ( city == null )
			return;

		northWestCorner = new double[ 2 ];
		southEastCorner = new double[ 2 ];
		double minX = Double.MAX_VALUE, 
		       minY = Double.MAX_VALUE,
		       maxX = Double.MIN_VALUE,
		       maxY = Double.MIN_VALUE;
		for ( double[ ] d : city )
		{
			if ( d[ 0 ] < minX ) minX = d[ 0 ];
			if ( d[ 0 ] > maxX ) maxX = d[ 0 ];
			if ( d[ 1 ] < minY ) minY = d[ 1 ];
			if ( d[ 1 ] > maxY ) maxY = d[ 1 ];
		}

		northWestCorner[ 0 ] = minX;
		northWestCorner[ 1 ] = minY;
		southEastCorner[ 0 ] = maxX;
		southEastCorner[ 1 ] = maxY;
	}

	public String getName ( ) { return name; }
	public int getNumberOfCities ( ) { return numberOfCities; };

	public double[ ] getCity ( int n )
	{
		double[ ] result = new double[ 2 ];

		if ( n >= 0 && n < numberOfCities )
		{
			result[ 0 ] = city[ n ][ 0 ];
			result[ 1 ] = city[ n ][ 1 ];
			return result;
		}

		return null;
	}

	public double[ ] getCityNormalized ( int n )
	{
		double result[ ] = getCity( n );

		if ( result == null )
			return result;

		result[ 0 ] = ( result[ 0 ] - northWestCorner[ 0 ] ) / ( southEastCorner[ 0 ] - northWestCorner[ 0 ] );
		result[ 1 ] = ( result[ 1 ] - northWestCorner[ 1 ] ) / ( southEastCorner[ 1 ] - northWestCorner[ 1 ] );

		return result;
	}

	public int[ ] getCityBitmapCoord ( int n, int w, int h, double b )
	{
		double[ ] city = getCityNormalized( n );
		int[ ] result = new int[ 2 ];

		result[ 0 ] = ( int ) ( ( city[ 0 ] * ( w - ( w * b * 2.0 ) ) ) + ( w * b ) );
		result[ 1 ] = ( int ) ( h - ( ( city[ 1 ] * ( h - ( h * b * 2.0 ) ) ) + ( h * b ) ) );

		return result;
	}

	public int[ ][ ] getAllCityBitmapCoords ( int w, int h, double b )
	{
		int[ ][ ] result = new int[ numberOfCities ][ 2 ];

		for ( int n = 0; n < numberOfCities; n++ )
			result[ n ] = getCityBitmapCoord( n, w, h, b );

		return result;
	}

	@Override
	public String toString ( )
	{
		String result = "";
		int n = 1;

		if ( name != null )
			result += "Name: " + name + "\n";
		if ( numberOfCities != -1 )
			result += "Number Of Cities: " + numberOfCities + "\n";
		if ( city != null )
			for ( double[ ] d : city )
			{
				result += "\t" + n + ":\t" + d[ 0 ] + "\t" + d[ 1 ] + "\n";
				n++;
			}
		if ( northWestCorner != null )
			result += "\n\tMIN: " + northWestCorner[ 0 ] + "\t" + northWestCorner[ 1 ] + "\n";
		if ( southEastCorner != null )
			result += "\n\tMAX: " + southEastCorner[ 0 ] + "\t" + southEastCorner[ 1 ] + "\n";

		return result;
	}
}
