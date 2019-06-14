/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * K Means Test on Iris Dataset                                              * 
 * Written by Anthony Ferski                                                 *
 * 5192885 / AF11MB                                                          *
 * For COSC4P76 Winter 2018 Assignment #2                                    *
 * Explanation at bottom of source file                                      *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

import java.util.ArrayList;
import java.io.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class KMeans
{
	static final String TITLE = "K-Means on Iris Dataset";
	static final String FILE1 = "setosa.csv";
	static final String FILE2 = "versicolor.csv";
	static final String FILE3 = "virginicana.csv";
	static final int NUM_CLUSTERS = 3;

	static final int WINDOW_WIDTH = 1200;
	static final int WINDOW_HEIGHT = 600;
	static final int GRAPH_SIZE = 600;
	static final int GRAPH_GAP = 20;
	static final int POINT_SIZE = 6;
	static final double GRAPH_BUFFER = 0.05;

	private DataPool pool;
	private Centroid[ ] centroids;
	private int dimension;

	//the algorithm in the constructor
	//written as clearly as possible
	public KMeans ( )
	{
		init( );
		loadData( );
		placeCentroids( );
		while ( centroidsAreMoving( ) )
		{
			groupData( );
			adjustAllCentroids( );
		}
		printData( );
		showResultsWindow( );
	}

	//set up the pool and centroids
	private void init ( )
	{
		pool = new DataPool( );
		centroids = new Centroid[ NUM_CLUSTERS ];
		dimension = -1;
	}

	//load in the data
	private void loadData ( )
	{
		pool.addData( FILE1 );
		pool.addData( FILE2 );
		pool.addData( FILE3 );
		pool.findExtremes( );
	}

	//place the centroids initially on some random points of the dataset
	private void placeCentroids ( )
	{
		for ( int i = 0; i < NUM_CLUSTERS; i++ )
			centroids[ i ] = new Centroid( i + 1, 
			                 pool.getRandomVector( ).getData( ) );
	}

	//associate each datapoint with its closest centroid
	private void groupData ( )
	{
		double min, dist;
		int closest;

		for ( DataVector dv : pool.getVectors( ) )
		{
			min = Double.MAX_VALUE;
			closest = 0;
			for ( Centroid c : centroids )
			{
				dist = euclideanDistance( c.getVector( ), 
				                          dv.getData( ) );
				if ( dist < min )
				{
					min = dist;
					closest = c.getID( );
				}
			}
			dv.setCluster( closest );
		}
	}

	//calculate the distance between two n-dimensional points
	public double euclideanDistance ( double[ ] v1, double[ ] v2 )
	{
		double result = 0;

		for ( int i = 0; i < v1.length; i++ )
			result += ( v1[ i ] - v2[ i ] ) * ( v1[ i ] - v2[ i ] );

		return Math.sqrt( result );
	}

	//adjust the centroids to their group mean
	public void adjustAllCentroids ( )
	{
		for ( Centroid c : centroids )
			c.adjustCentroid( pool );
	}

	//checks if any of the centroids have changed positions
	//for ending the algorithm iterations
	public boolean centroidsAreMoving ( )
	{
		for ( Centroid c : centroids )
			if ( c.hasMoved( ) )
				return true;
		return false;
	}

	//add the components of two arrays together
	public double[ ] addVectors ( double[ ] v1, double[ ] v2 )
	{
		double[ ] result = new double[ v1.length ];

		for ( int i = 0; i < result.length; i++ )
			result[ i ] = v1[ i ] + v2[ i ];

		return result;
	}

	//return the average vector from a total sum vector divided by n
	public double[ ] averageVector ( double[ ] v, int n )
	{
		double[ ] result = new double[ v.length ];

		for ( int i = 0; i < v.length; i++ )
			result[ i ] = v[ i ] / n;

		return result;
	}

	//print all the data that is loaded along with types and groups
	public void printData ( )
	{
		String result = "";
		for ( DataVector dv : pool.getVectors( ) )
			result += dv.toString( );

		System.out.println( result );
	}

	//shows the graph at the end
	public void showResultsWindow ( ) 
	{
		SwingUtilities.invokeLater( new Runnable( ) 
		{
			@Override
			public void run ( )
			{
				new GraphWindow( );
			}
		} );
	}

	//the centroids that determine the clusters
	private class Centroid
	{
		private double[ ] vector;
		private int id;
		private boolean moved;

		private Centroid ( int i, double[ ] v )
		{
			id = i;
			vector = addVectors( new double[ v.length ], v );
			moved = true;
		}

		public int getID ( ) { return id; }
		public double[ ] getVector ( ) { return vector; }
		public boolean hasMoved ( ) { return moved; }

		public String toString ( )
		{
			String result = "Centroid " + id + ":";

			for ( double d : vector )
				result += "\t" + d;

			return result + "\n";
		}

		//adjust a single centroid to the mean of its associated data
		public void adjustCentroid ( DataPool dp )
		{
			double[ ] lastVector = addVectors( 
			                       new double[ vector.length ], 
			                       vector );
			double[ ] newPosition = new double[ vector.length ];
			int count = 0;

			for ( DataVector dv : dp.getVectors( ) )
				if ( dv.getCluster( ) == id )
				{
					newPosition = addVectors( newPosition, 
					              dv.getData( ) );
					count++;
				}

			newPosition = averageVector( newPosition, count );
			moved = false;

			for ( int i = 0; i < newPosition.length; i++ )
				if ( lastVector[ i ] != newPosition[ i ] )
					moved = true;

			vector = addVectors( new double[ vector.length ], 
			                     newPosition );
		}
	}

	//the entirety of the data
	private class DataPool
	{
		private ArrayList<DataVector> data;
		private double[ ] minVector, maxVector;

		public DataPool ( )
		{
			data = new ArrayList<DataVector>( );
		}

		//load a subset of the data
		public void addData ( String fileName )
		{
			String line;
			String[ ] temp = fileName.split( "\\.");
			String type = temp[ 0 ];
			double[ ] vector;

			try
			{
				FileReader fr = new FileReader( fileName );
				BufferedReader br = new BufferedReader( fr );

				while ( ( line = br.readLine( ) ) != null )
				{
					temp = line.split( "," );
					vector = new double[ temp.length ];

					if ( dimension == -1 )
						dimension = temp.length;
					
					for ( int i = 0; i < temp.length; i++ )
						vector[ i ] = Double.parseDouble( temp[ i ] );

					data.add( new DataVector( type, vector ) );
				}

			} catch ( IOException e ) {
				System.out.println( "Something went wrong" );
			}
		}

		//find the min and max boundaries for graphing purposes
		public void findExtremes ( )
		{
			double min, max, temp;

			minVector = new double[ dimension ];
			maxVector = new double[ dimension ];

			for ( int i = 0; i < dimension; i++ )
			{
				min = Double.MAX_VALUE;
				max = Double.MIN_VALUE;
				for ( DataVector dv : data )
				{
					temp = dv.getData( )[ i ];
					if ( temp > max )
						max = temp;
					if ( temp < min )
						min = temp;
				}
				minVector[ i ] = min;
				maxVector[ i ] = max;
			}
		}

		public ArrayList<DataVector> getVectors ( ) { return data; }
		public double[ ] getMinVector ( ) { return minVector; }
		public double[ ] getMaxVector ( ) { return maxVector; }

		//return a random vector for choosing initial centroid positions
		public DataVector getRandomVector ( )
		{
			return data.get( ( int ) ( Math.random( ) 
			       * data.size( ) ) );
		}

		public String toString ( )
		{
			String result = "Data pool consists of the following:\n";

			for ( DataVector dv : data )
				result += "\t" + dv.toString( );

			return result + "\n";
		}
	}

	//a single data vector
	private class DataVector
	{
		private int cluster;
		private String type;
		private double[ ] data;

		public DataVector ( String t, double[ ] d )
		{
			type = t;
			data = new double[ d.length ];
			for ( int i = 0; i < d.length; i++ )
				data[ i ] = d[ i ];
			cluster = -1;
		}

		public String getType ( ) { return type; }
		public int getCluster ( ) { return cluster; }
		public void setCluster ( int c ) { cluster = c; }
		public double[ ] getData ( ) { return data; }
		public void getData ( double[ ] d ) { data = d; }

		public String toString ( )
		{
			String result = type + " (" + cluster + "):\t";

			for ( double d : data )
				result += d + "\t";

			return result + "\n";
		}
	}

	//the window that shows the result graph
	private class GraphWindow extends JFrame
	{
		private DrawCanvas canvas;

		public GraphWindow ( )
		{
			canvas = new DrawCanvas( );
			canvas.setPreferredSize( new Dimension( WINDOW_WIDTH, 
			                         WINDOW_HEIGHT ) );
			Container c = getContentPane( );
			c.add( canvas );
			setDefaultCloseOperation( EXIT_ON_CLOSE );
			pack( );
			setTitle( TITLE );
			setVisible( true );
		}

		private class DrawCanvas extends JPanel
		{
			@Override
			public void paintComponent ( Graphics g )
			{
				super.paintComponent( g );
				setBackground( Color.WHITE );
				drawGraph( g, GRAPH_GAP, GRAPH_GAP, GRAPH_SIZE 
				           - ( 2 * GRAPH_GAP ), GRAPH_SIZE 
				           - ( 2 * GRAPH_GAP ), true );
				drawGraph( g, GRAPH_GAP + WINDOW_WIDTH / 2, 
				           GRAPH_GAP, GRAPH_SIZE 
				           - ( 2 * GRAPH_GAP ), GRAPH_SIZE 
				           - ( 2 * GRAPH_GAP ), false );
			}

			private void drawGraph ( Graphics g, int x, int y, int l, int h, boolean petals )
			{
				int pointX, pointY;
				int offset = 0;
				if ( !petals )
					offset += 2;

				g.setColor( Color.BLACK );
				g.drawRect( x, y, l, h );

				for ( DataVector dv : pool.getVectors( ) )
				{
					pointX = getVectorCoord( normalizeValue( dv.getData( )[ 0 + offset ], pool.getMinVector( )[ 0 + offset ], pool.getMaxVector( )[ 0 + offset ] ), l, GRAPH_BUFFER ) + x;
					pointY = h - ( getVectorCoord( normalizeValue( dv.getData( )[ 1 + offset ], pool.getMinVector( )[ 1 + offset ], pool.getMaxVector( )[ 1 + offset ] ), h, GRAPH_BUFFER ) ) + y;

					switch ( dv.getCluster( ) )
					{
						case 1:
							g.setColor( Color.RED );
							break;
						case 2:
							g.setColor( Color.GREEN );
							break;
						case 3:
							g.setColor( Color.BLUE );
							break;
						default:
							g.setColor( Color.BLACK );
							break;
					}

					g.fillOval( pointX - POINT_SIZE, pointY - POINT_SIZE, POINT_SIZE * 2, POINT_SIZE * 2 );

					if ( dv.type.equalsIgnoreCase( "setosa" ) )
						g.setColor( Color.MAGENTA );
					else if ( dv.type.equalsIgnoreCase( "versicolor" ) )
						g.setColor( Color.YELLOW );
					else if ( dv.type.equalsIgnoreCase( "virginicana" ) )
						g.setColor( Color.CYAN );
					else
						g.setColor( Color.BLACK );

					g.fillOval( pointX - ( POINT_SIZE / 2 ), pointY - ( POINT_SIZE / 2 ), POINT_SIZE, POINT_SIZE);
				}

				for ( Centroid c : centroids )
				{
					pointX = getVectorCoord( normalizeValue( c.getVector( )[ 0 + offset ], pool.getMinVector( )[ 0 + offset ], pool.getMaxVector( )[ 0 + offset ] ), l, GRAPH_BUFFER ) + x;
					pointY = h - ( getVectorCoord( normalizeValue( c.getVector( )[ 1 + offset ], pool.getMinVector( )[ 1 + offset ], pool.getMaxVector( )[ 1 + offset ] ), h, GRAPH_BUFFER ) ) + y;


					switch ( c.getID( ) )
					{
						case 1:
							g.setColor( Color.RED );
							break;
						case 2:
							g.setColor( Color.GREEN );
							break;
						case 3:
							g.setColor( Color.BLUE );
							break;
						default:
							g.setColor( Color.BLACK );
							break;
					}

					drawCross( g, pointX, pointY, POINT_SIZE );
				}

			}

			private void drawCross ( Graphics g, int x, int y, int s )
			{
				g.fillRect( x - ( s * 4 ), y - ( s / 2 ), 
				          ( s * 8 ), s );
				g.fillRect( x - ( s / 2 ), y - ( s * 4 ), 
				            s, ( s * 8 ) );
			}

			private int getVectorCoord ( double d, int s, double b )
			{
				return ( int ) ( d * ( s - ( 2 * b * s ) ) 
				       + ( b * s ) );
			}

			private double normalizeValue ( double d, double min, double max )
			{
				return ( ( d - min ) / ( max - min ) );
			}
		}
	}

	public static void main ( String[ ] args )
	{
		KMeans km = new KMeans( );
	}
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * This is my implementation of k-means clustering. There are many like it,  *
 * but this one is mine. It is as you expect: centroids move around the      *
 * data space, and the data vectors are assigned a grouping by the closest   *
 * centroid. Once the centroids stop moving, the algorithm halts. Then a     *
 * pair of graphs appear. The inner circle colour is the actual species of   *
 * flower and the outer circle colour is the group the algorithm assigned it *
 * to. Sometimes the algorithm fails to halt for some reason, but this is    *
 * rare. Magenta -> Setosa, Yellow -> Versicolor, Cyan -> Virginicana.       *
 * Left graph -> Petals, Right graph -> Sepals ( I think... )                *
 *                                                                           *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
