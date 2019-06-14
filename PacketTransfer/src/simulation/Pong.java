package simulation;

import java.awt.*;
import java.util.*;

public class Pong 
{
	public static final int FIELD_WIDTH = 350;
	public static final int FIELD_LENGTH = 500;
	public static final int PADDLE_SPACE = 32;
	public static final int PADDLE_WIDTH = 32;
	public static final int BALL_SIZE = 3;
	public static final Color GAME_COLOR = Color.BLACK;

	private int fieldX, fieldY;
	private Paddle paddle1, paddle2;
	private Ball ball;

	public Pong ( int x, int y )
	{
		fieldX = x - ( FIELD_WIDTH / 2 );
		fieldY = y - ( FIELD_LENGTH / 2 );
		paddle1 = new Paddle( FIELD_WIDTH / 2 - ( PADDLE_WIDTH / 2 ), 
			              PADDLE_SPACE );
		paddle2 = new Paddle( FIELD_WIDTH / 2 - ( PADDLE_WIDTH / 2 ), 
			              FIELD_LENGTH - PADDLE_SPACE );
		ball = new Ball( );
	}

	public void update ( ArrayList<Packet> pl)
	{
		paddle1.update( );
		paddle2.update( );
		ball.update( pl );
	}

	public void draw ( Graphics g )
	{
		g.setColor( GAME_COLOR );
		drawField( g );
		paddle1.draw( g );
		paddle2.draw( g );
		ball.draw( g );
	}

	public void drawField ( Graphics g )
	{
		g.drawRect( fieldX, fieldY, FIELD_WIDTH, FIELD_LENGTH );
		for ( int i = fieldX; i < fieldX + FIELD_WIDTH; i += 4 )
			g.fillRect( i, fieldY + ( FIELD_LENGTH / 2 ), 1, 1 );
	}

	private class Paddle extends Entity
	{
		private Paddle ( int x, int y )
		{
			xPos = x;
			yPos = y;
		}

		public void update ( )
		{
			super.update( );
			
			if ( ball.xPos > xPos + ( PADDLE_WIDTH / 2 ) )
				xPos += 3;
			else if ( ball.xPos < xPos + ( PADDLE_WIDTH / 2 ))
				xPos -= 3;
			
			if ( xPos < 0 )
				xPos = 0;
			if ( xPos > FIELD_WIDTH - PADDLE_WIDTH )
				xPos = FIELD_WIDTH - PADDLE_WIDTH; 	
		}
		public void draw ( Graphics g )
		{
			g.fillRect( xPos + fieldX, yPos + fieldY, 
			            PADDLE_WIDTH, 2 );
		}
	}

	private class Ball extends Entity
	{
		private Ball ( )
		{
			xPos = FIELD_WIDTH / 2;
			yPos = FIELD_LENGTH / 2;
			xSpeed = 2;
			ySpeed = 4;
		}

		public void update ( ArrayList<Packet> pl )
		{
			super.update( );
			if ( ( xSpeed > 0 && xPos >= FIELD_WIDTH ) 
			     || ( xSpeed < 0 && xPos < 0 ) )
				xSpeed = -xSpeed;
			if ( ( ySpeed > 0 && yPos >= paddle2.yPos ) 
			     || ( ySpeed < 0 && yPos < paddle1.yPos ) )
				ySpeed = -ySpeed;

			for ( int i = 0; i < pl.size( ); i++ )
			{
				Packet p = pl.get( i );
				if ( ( p.xPos - 2 - fieldX < xPos ) &&
				     ( p.xPos + 2 + p.xSize - fieldX >= xPos ) &&
				     ( p.yPos - 2 - fieldY < yPos ) &&
				     ( p.yPos + 2 + p.ySize - fieldY >= yPos ) )
				{
					System.out.println( "[ !! ] Packet #" + p.packetNumber + " has been dropped.");
					pl.remove( i );
					ySpeed = -ySpeed;
				}
					
			}
		}

		public void draw ( Graphics g )
		{
			g.fillRect( xPos + fieldX - ( BALL_SIZE / 2 ), 
				    yPos + fieldY - ( BALL_SIZE / 2 ), 
				    BALL_SIZE, BALL_SIZE );
		}
	}
}