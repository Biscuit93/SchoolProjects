package tsp;

import javax.swing.*;
import java.awt.*;
import java.util.*;

public class MapPanel extends JPanel
{
	final private int MAP_LENGTH = 500;
	final private int MAP_HEIGHT = MAP_LENGTH;
	final private int MAP_GRID_SIZE = MAP_LENGTH / 20;
	final private int MAP_CITY_SIZE = 6;
	final private int MAP_POINT_SIZE = MAP_CITY_SIZE / 2;
	final private double MAP_BUFFER = 0.05;
	final private String DEFAULT_FILE = "berlin52.tsp";

	private Map map;
	private Recorder recorder;
	private Solution solution;
	private String[ ] algorithms;

	public MapPanel ( String[ ] algos )
	{
		setPreferredSize( new Dimension( MAP_LENGTH + 1, MAP_HEIGHT + 1 ) );
		map = new Map( DEFAULT_FILE );
		recorder = new Recorder( );
		algorithms = algos;
	}

	private void drawMap ( Graphics g )
	{
		Graphics2D g2d = ( Graphics2D ) g;

		drawMapBackground( g );
		if ( map != null )
			drawMapCities( g );
		if ( solution != null )
			drawSolutionPath( g );
	}

	private void drawMapBackground ( Graphics g )
	{
		g.setColor( Color.WHITE );
		g.fillRect( 0, 0, MAP_LENGTH, MAP_HEIGHT );
		g.setColor( Color.LIGHT_GRAY );
		for ( int i = MAP_GRID_SIZE; i < MAP_LENGTH; i += MAP_GRID_SIZE )
		{
			g.drawLine( i, 0, i, MAP_HEIGHT );
			g.drawLine( 0, i, MAP_LENGTH, i );
		}
		g.setColor( Color.BLACK );
		g.drawRect( 0, 0, MAP_LENGTH - 1, MAP_HEIGHT - 1 );
	}

	private void drawMapCities ( Graphics g )
	{
		g.setColor( Color.RED );
		for ( int[ ] city : map.getAllCityBitmapCoords( MAP_LENGTH, MAP_HEIGHT, MAP_BUFFER ) )
			g.drawRect( city[ 0 ] - ( MAP_CITY_SIZE / 2 ), city[ 1 ]  - ( MAP_CITY_SIZE / 2 ), MAP_CITY_SIZE, MAP_CITY_SIZE );
	}

	private void drawSolutionPath ( Graphics g )
	{
		int[ ] cityOrder = solution.getSolution( );
		int[ ][ ] city = map.getAllCityBitmapCoords( MAP_LENGTH, MAP_HEIGHT, MAP_BUFFER );

		g.setColor( Color.BLUE );
		for ( int i = 0; i < cityOrder.length; i++ )
		{
			g.fillRect( city[ cityOrder[ i ] ][ 0 ] - ( MAP_POINT_SIZE / 2 ), city[ cityOrder[ i ] ][ 1 ]  - ( MAP_POINT_SIZE / 2 ), MAP_POINT_SIZE, MAP_POINT_SIZE );
			g.drawLine( city[ cityOrder[ i ] ][ 0 ], city[ cityOrder[ i ] ][ 1 ], city[ cityOrder[ ( i + 1 ) % cityOrder.length ] ][ 0 ], city[ cityOrder[ ( i + 1 ) % cityOrder.length ] ][ 1 ] );
		}
	}

	public void setAlgorithm ( String algo )
	{
		switch ( Arrays.asList( algorithms ).indexOf( algo ) )
		{
			case 0:
				solution = new RandomPath( );
				break;
			case 1:
				solution = new NearestNeighbour( );
				break;
			case 2:
				solution = new PairwiseExchange( );
				break;
			case 3:
				solution = new Genetic( );
				break;
			default:
				solution = new RandomPath( );
				break;
		}
		
	}

	public double computeTSP ( String algo, boolean save )
	{
		double dist;
		String entry;

		solution.solve( map );
		dist = solution.getDistance( );

		if ( save )
		{
			entry = algo + "," + dist + ",\n";
			recorder.record( entry );
		}

		return dist;
	}

	@Override
	public void paintComponent ( Graphics g )
	{
		super.paintComponent( g );
		drawMap( g );
	}
}