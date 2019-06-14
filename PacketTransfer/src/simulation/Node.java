package simulation;

import java.util.*;
import java.lang.*;
import java.awt.*;

public class Node extends Entity
{
	public static final Color NODE_COLOR = Color.BLACK;
	public static final int RELEASE_INTERVAL = 10;
	public int dataWidth, dataSize, packetSize, 
	           releaseTimer = RELEASE_INTERVAL, nextToSend = 0;
	public Data[ ] data;
	boolean server = false;
	
	public Node ( int x, int y, int w, int h, int dw, int ps, boolean s )
	{
		xPos = x;
		yPos = y;
		xSize = w;
		ySize = h;
		dataWidth = dw;
		dataSize = dataWidth * dataWidth;
		data = new Data[ dataSize ];
		packetSize = ps;
		server = s;
	}

	public void loadData ( int[ ] d )
	{
		for ( int i = 0; i < dataSize; i++ )
			data[ i ] = new Data( xPos + 2 + ( i % dataWidth ) 
			                      * ( packetSize +  1 ), 
				              yPos + 2 + ( i / dataWidth ) 
				              * ( packetSize + 1 ), 
				              packetSize, d[ i ] );
	}

	public void update ( ArrayList<Packet> pl )
	{
		Packet p;
		Data d;
		int sentinel;
		if ( server )
		{
			for ( int i = 0; i < pl.size( ); i++ )
			{
				p = pl.get( i );
				d = p.data;

				if ( !p.sentByServer && p.xPos < xPos + xSize )
				{
					System.out.println( "[ <- ] Server received ACK for packet #" + p.packetNumber + ". No longer sending #" + p.packetNumber + "." );
					pl.remove( i );
					data[ p.packetNumber ].color = -1;
				}
			}
			if ( releaseTimer > 0 )
				releaseTimer--;

			if ( releaseTimer == 0 )
			{
				releaseTimer = RELEASE_INTERVAL;

				d = data[ nextToSend ];

				if ( d.color != -1 )
				{
					System.out.println( "[ -> ] Server is sending packet #" + nextToSend + "." );
					pl.add( new Packet( xPos + xSize - packetSize, 
					            yPos + ( ySize / 2 ) 
					            - ( packetSize / 2 )
					            + randomPacketYPos( ), 
					            randomPacketSpeed( true ), 
					            packetSize, nextToSend, 
					            new Data( d.xPos, d.yPos, 
					            	      d.xSize, 
					            	      d.color), 
					                      true ) );
				}
				
				sentinel = nextToSend;
				while ( true )
				{
					nextToSend++;

					if ( nextToSend == dataSize )
						nextToSend = 0;

					if ( data[ nextToSend ].color != -1 )
						break;

					if ( sentinel == nextToSend )
					{
						releaseTimer = -1;
						System.out.println( "[ !! ] All packets have been acknowledged." );
						break;
					}
				}

				
			}
		}
		else
		{
			for ( int i = 0; i < pl.size( ); i++ )
			{
				p = pl.get( i );
				d = p.data;

				if ( p.sentByServer && p.xPos > xPos )
				{
					System.out.println( "[ -> ] Client received packet #" + p.packetNumber + ".\n[ <- ] Sending ACK for #" + p.packetNumber + " back to server." );
					pl.remove( i );

					data[ p.packetNumber ] = new Data( xPos + 2 + ( p.packetNumber % dataWidth ) * ( packetSize + 1 ), 
				              yPos + 2 + ( p.packetNumber / dataWidth ) * ( packetSize + 1 ),
				              p.xSize, p.data.color );

					pl.add( new Packet( xPos - packetSize, 
					            yPos + ( ySize / 2 ) 
					            - ( packetSize / 2 )
					            + randomPacketYPos( ), 
					            randomPacketSpeed( false ), 
					            packetSize, p.packetNumber, 
					            new Data( d.xPos, d.yPos, 
					            	      d.xSize, 
					            	      d.color), 
					                      false ) );
				}
			}
		}
	}

	public int randomPacketSpeed ( boolean right )
	{
		int result = ( int ) ( Math.random( ) * 4 + 1 );
		if ( right )
			return result;
		else
			return -result;
	}

	public int randomPacketYPos ( )
	{
		return ( int ) ( ( Math.random( ) * 6 ) - 3 ) 
		       * ( ( packetSize + 5 ) * 2 );
	}

	public void draw ( Graphics g )
	{
		g.setColor( NODE_COLOR );
		g.drawRect( xPos, yPos, xSize, ySize );

		for ( int i = 0; i < dataSize; i++ )
			if ( data[ i ] != null )
				data[ i ].draw( g );
	}
}