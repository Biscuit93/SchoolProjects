/******************************************************************************
 * Packet Transfer Simulation                                                 *
 * For COSC 4P14 Assignment 2                                                 *
 * Written by Anthony Ferski ( 5192885 )                                      *
 * 11 / 28 / 17                                                               *
 ******************************************************************************/

package simulation;

import java.util.*;
import java.awt.*;
import java.awt.event.*;
import javax.swing.*;

public class PacketTransferSim extends JFrame
{
	public static final int CANVAS_WIDTH = 1000;
	public static final int CANVAS_HEIGHT = 600;
	public static final int DELAY_TIME = 16;
	public static final Color CANVAS_BACKGROUND = Color.WHITE;

	public static final int DATA_WIDTH = 16;
	public static final int PACKET_SIZE = 8;
	public static final int PACKET_MIN_SPEED = 1;
	public static final int PACKET_MAX_SPEED = 5;
	public static final int NODE_SIZE = DATA_WIDTH * ( PACKET_SIZE + 1 ) 
	                                    + 2;
	public static final int NODE_YPOS = ( CANVAS_HEIGHT / 2 ) 
	                                    - ( NODE_SIZE / 2 );
	public static final int SENDER_XPOS = ( CANVAS_WIDTH / 6 ) 
	                                      - ( NODE_SIZE / 2 );
	public static final int RECEIVER_XPOS = ( 5 * ( CANVAS_WIDTH / 6 ) )
	                                        - ( NODE_SIZE / 2 );	                                      

	public static final int[ ] SPRITE = { 0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,
	                                      0,0,0,0,0,0,2,2,2,2,2,0,0,1,1,1,
	                                      0,0,0,0,0,2,2,2,2,2,2,2,2,2,1,1,
	                                      0,0,0,0,0,3,3,3,1,1,3,1,0,3,3,3,
	                                      0,0,0,0,3,1,3,1,1,1,3,1,1,1,3,3,
	                                      0,0,0,0,3,1,3,3,1,1,1,3,1,1,1,3,
	                                      0,0,0,0,3,3,1,1,1,1,3,3,3,3,3,0,
	                                      0,0,0,0,0,0,1,1,1,1,1,1,1,3,0,0,
	                                      0,0,3,3,3,3,3,2,3,3,3,2,3,0,0,0,
	                                      0,3,3,3,3,3,3,3,2,3,3,3,2,0,0,3,
	                                      1,1,3,3,3,3,3,3,2,2,2,2,2,0,0,3,
	                                      1,1,1,0,0,2,3,2,2,2,1,2,1,2,3,3,
	                                      0,1,0,3,0,2,2,2,2,2,2,2,2,2,3,3,
	                                      0,0,3,3,3,2,2,2,2,2,2,2,2,2,3,3,
	                                      0,3,3,3,2,2,2,2,2,2,2,0,0,0,0,0,
	                                      0,3,0,0,2,2,2,2,0,0,0,0,0,0,0,0 };
	private DrawCanvas canvas;
	private Node sender;
	private Node receiver;
	private ArrayList <Packet> packets;
	private Pong pong;

	public PacketTransferSim ( )
	{
		setupWindow( );
		setupSimulation( );
	}

	private void setupWindow ( )
	{
		canvas = new DrawCanvas( );
		canvas.setPreferredSize( new Dimension( CANVAS_WIDTH, 
		                         CANVAS_HEIGHT ) );

		Container cp = getContentPane( );
		cp.setLayout( new BorderLayout( ) );
		cp.add( canvas, BorderLayout.CENTER );

		setTitle( "PacketTransferSim" );
		setDefaultCloseOperation( JFrame.EXIT_ON_CLOSE );
		pack( );
		setVisible( true );
		requestFocus( );
	}

	private void setupSimulation ( )
	{
		sender = new Node( SENDER_XPOS, NODE_YPOS, NODE_SIZE,
		                   NODE_SIZE, DATA_WIDTH, PACKET_SIZE, true);
		receiver = new Node( RECEIVER_XPOS, NODE_YPOS, NODE_SIZE,
			             NODE_SIZE, DATA_WIDTH, PACKET_SIZE, false );
		sender.loadData( SPRITE );
		pong = new Pong( CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2 );
		packets = new ArrayList( );

		Thread updateThread = new Thread( )
		{
			@Override
			public void run ( )
			{
				while ( true )
				{
					update( );
					repaint( );
					try {
						Thread.sleep( DELAY_TIME );
					} catch ( Exception e ) { }
				}
			}
		};
		updateThread.start( );
	}

	private void update( )
	{
		sender.update( packets );
		receiver.update( packets );
		pong.update( packets );
		for ( Packet p : packets )
			p.update( );
	}

	private class DrawCanvas extends JPanel 
	{
		@Override
		public void paintComponent( Graphics g ) 
		{
			super.paintComponent( g );
			setBackground( CANVAS_BACKGROUND );
			sender.draw( g );
			receiver.draw( g );
			pong.draw( g ); 
			for ( Packet p : packets )
				p.draw( g );
		}
	}

	public static void main ( String[ ] args )
	{
		SwingUtilities.invokeLater( new Runnable( ) 
		{
			@Override
			public void run ( ) 
			{
				new PacketTransferSim( );
			}
		} );
	}
}