package simulation;

import java.awt.*;

public class Data extends Entity
{
	public int color;

	public Data ( int x, int y, int s, int c )
	{
		xPos = x;
		yPos = y;
		xSize = s;
		ySize = s;
		color = c;
	}

	public void update ( )
	{

	}

	public void draw ( Graphics g )
	{
		setDataColor( g, color );
		g.fillRect( xPos, yPos, xSize, ySize );
	}

	private void setDataColor ( Graphics g, int c )
	{
		switch ( c )
		{
			case -1 :
				g.setColor( Color.WHITE );
			break;
			case 0 :
				g.setColor( Color.LIGHT_GRAY );
			break;
			case 1 :
				g.setColor( Color.YELLOW );
			break;
			case 2 :
				g.setColor( Color.RED );
			break;
			case 3 :
				g.setColor( Color.BLUE );
			break;
			default :
				g.setColor( Color.BLACK );
			break;
		}
	}
}