package tsp;

import java.io.*;

public class Recorder
{
	final private String PATH = "data/out/";
	final private String FILE = "results.csv";

	public Recorder ( )
	{

	}

	public void record ( String entry )
	{
		FileWriter fw = null;
		BufferedWriter bw = null;
		PrintWriter out = null;

		try
		{
			fw = new FileWriter( PATH + FILE, true );
			bw = new BufferedWriter( fw );
			out = new PrintWriter( bw );
			out.write( entry );
		} catch ( IOException e ) {
			e.printStackTrace( );
		} finally {
			try
			{
				if ( out != null )
					out.close( );
				if ( bw != null )
					bw.close( );
				if ( fw != null )
					fw.close( );

			} catch ( IOException e ) { 
				e.printStackTrace( );
			}
		}
	}
}