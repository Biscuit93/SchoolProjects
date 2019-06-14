package tsp;

import javax.swing.*;
import java.awt.*;
import java.awt.event.*;

import static javax.swing.GroupLayout.Alignment.BASELINE;
import static javax.swing.GroupLayout.Alignment.TRAILING;

public class GUI extends JFrame
{
	final private String[ ] ALGORITHMS = new String[ ] { "Random Path", "Nearest Neighbour", "Pairwise Exchange", "Genetic" };

	private String algorithm;
	private double distance;
	private boolean saveResult = false;

	public GUI ( )
	{
		initWindow( );
	}

	private void initWindow ( )
	{
		createView( );

		setTitle( "TSP" );
		setLocationRelativeTo( null );
		setDefaultCloseOperation( EXIT_ON_CLOSE );
		setResizable( false );		
	}

	private void createView ( )
	{
		Container pane = getContentPane( );
		GroupLayout layout = new GroupLayout( pane );
		pane.setLayout( layout );

		MapPanel panelMap = new MapPanel( ALGORITHMS );
		JLabel labelAlgorithm = new JLabel( "Algorithm: " );
		JLabel labelDistance = new JLabel( "Distance: " );
		JTextField textFieldDistance = new JTextField( );
		JComboBox<String> comboBoxAlgorithm = new JComboBox<String>( ALGORITHMS );
		JButton buttonSolve = new JButton( "Solve" );
		buttonSolve.addActionListener( new ActionListener( ) 
		{
			@Override
			public void actionPerformed( ActionEvent e )
			{
				buttonSolve.setEnabled( false );
				algorithm = comboBoxAlgorithm.getSelectedItem( ).toString( );
				panelMap.setAlgorithm( algorithm );
				distance = panelMap.computeTSP( algorithm, saveResult );
				panelMap.repaint( );
				textFieldDistance.setText( Double.toString( distance ) );
				buttonSolve.setEnabled( true );
				buttonSolve.requestFocus( );
			}
		} );
		JCheckBox checkBoxSave = new JCheckBox( "Save result", saveResult );
		checkBoxSave.addActionListener( new ActionListener ( )
		{
			@Override
			public void actionPerformed( ActionEvent e )
			{
				saveResult = checkBoxSave.isSelected( );
			}
		} );

		layout.setAutoCreateGaps( true );
		layout.setAutoCreateContainerGaps( true );

		layout.setHorizontalGroup( layout.createParallelGroup( )
			.addComponent( panelMap )
			.addGroup( layout.createSequentialGroup( )
				.addGroup( layout.createParallelGroup( )
					.addComponent( labelAlgorithm )
					.addComponent( labelDistance )
				)
				.addGroup( layout.createParallelGroup( )
					.addComponent( textFieldDistance )
					.addComponent( comboBoxAlgorithm )
				)
				.addGroup( layout.createParallelGroup( )
					.addComponent( buttonSolve )
					.addComponent( checkBoxSave )
				)
			)
		);

		layout.setVerticalGroup( layout.createSequentialGroup( )
			.addComponent( panelMap )
			.addGroup( layout.createParallelGroup( BASELINE )
				.addComponent( textFieldDistance )
				.addComponent( labelDistance )
				.addComponent( checkBoxSave )
			)
			.addGroup( layout.createParallelGroup( BASELINE )
				.addComponent( labelAlgorithm )
				.addComponent( comboBoxAlgorithm )
				.addComponent( buttonSolve )
			)
		);

		pack( );
	}
}