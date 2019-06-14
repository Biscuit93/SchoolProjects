package tsp;

import javax.swing.*;

public class Main
{
	public Main ( )
	{
		createGUI( );
	}

	private void createGUI ( )
	{
		SwingUtilities.invokeLater( new Runnable( )
		{
			@Override
			public void run( )
			{
				new GUI( ).setVisible( true );;
			}
		} ) ;
	}

	public static void main ( String[ ] args )
	{
		Main m = new Main( );
	}
}