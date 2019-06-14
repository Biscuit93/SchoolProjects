package simulation;

import java.awt.*;

public class Packet extends Entity
{
	public static final Color PACKET_COLOR = Color.BLACK;

	public int packetNumber;
	public Data data;
	boolean sentByServer;

	public Packet ( int x, int y, int sp, int si, int pn, Data d, boolean s )
	{
		xPos = x;
		yPos = y;
		xSize = si;
		ySize = si;
		xSpeed = sp;
		packetNumber = pn;
		data = d;
		sentByServer = s;
	}

	public void update ( )
	{
		super.update( );

		data.xPos = xPos;
		data.yPos = yPos;
	}

	public void draw ( Graphics g )
	{
		g.setColor( PACKET_COLOR );
		g.drawRect( xPos - 2, yPos - 2, xSize + 3, ySize + 3 );
		data.draw( g );
	}
}