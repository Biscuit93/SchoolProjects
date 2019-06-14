package tsp;

import java.util.*;

public class Genetic extends Solution
{
	final private int GENERATIONS = 100;
	final private int POPULATION_SIZE = 1000;
	final private int ELITE_SIZE = 5;
	final private int SELECT_ROUNDS = 5;
	final private double MUTATION_RATE = 0.10;

	private ArrayList<Individual> population;

	@Override
	public void solve ( Map m )
	{
		super.setMap( m );

		initializePopulation( );
		evolvePopulation( );
		
		super.setSolution( getBestPath( ) );
		super.setDistance( );
	}

	private void initializePopulation ( )
	{
		population = new ArrayList<Individual>( );
		for ( int i = 0; i < POPULATION_SIZE; i++ )
			population.add( new Individual( super.getMap( ) ) );
		Collections.sort( population );
	}

	private void evolvePopulation ( )
	{
		for ( int g = 1; g <= GENERATIONS; g++ )
		{
			for ( int i = 0; i < POPULATION_SIZE - ELITE_SIZE; i++ )
				population.add( new Individual ( super.getMap( ), tournamentSelect( ), tournamentSelect( ) ) );

			Collections.sort( population );

			for ( int i = 0; i < POPULATION_SIZE - ELITE_SIZE; i++ )
				population.remove( 0 );
		}
	}

	private Individual eliteSelect ( )
	{
		Individual best = null;
		double score = Double.MAX_VALUE;

		for ( Individual i : population )
			if ( i.getFitnessScore( ) < score )
			{
				best = i;
				score = i.getFitnessScore( );
			}

		return best;
	}

	private Individual tournamentSelect ( ) 
	{
		Individual best = null;
		int index;

		for ( int i = 0; i < SELECT_ROUNDS; i++ ) 
		{
			index = ( int ) ( POPULATION_SIZE * Math.random( ) );

			if ( best == null ) 
				best = population.get( index );

			if ( population.get( index ).getFitnessScore( ) < best.getFitnessScore( ) )
				best = population.get( index );
		}

		return best;
	}

	private int[ ] crossover ( int[ ] chro1, int[ ] chro2 )
	{
		int numberOfCities = super.getMap( ).getNumberOfCities( );
		int[ ] result = new int[ numberOfCities ];
		boolean[ ] bitMask = new boolean[ numberOfCities ];
		int[ ] leftOver;

		int numberLeftOver = numberOfCities;
		for ( int i = 0; i < numberOfCities; i++ ) 
		{
			if ( Math.random( ) < 0.50 ) 
			{
				bitMask[ i ] = true;
				numberLeftOver--;
			}
			else
				bitMask[ i ] = false;
		}

		leftOver = new int[ numberLeftOver ];
		numberLeftOver = 0;
		for ( int i = 0; i < numberOfCities; i++ ) 
		{
			if ( bitMask[ i ] )
				result[ i ] = chro1[ i ];
			else 
			{
				leftOver[ numberLeftOver ] = chro1[ i ];
				numberLeftOver++;
			}
		}

		int temp;
		numberLeftOver = 0;
		for ( int i = 0; i < numberOfCities; i++ ) 
			if ( !bitMask[ i ] ) 
				for ( int j = 0; j < numberOfCities; j++ )
				{
					temp = chro2[ j ];
					for ( int k = 0; k < leftOver.length; k++ ) 
						if ( temp == leftOver[k]) {

							result[ i ] = temp;
							leftOver[ k ] = -1;
							break;
						}

					if ( result[ i ] != 0 )
						break;
				}

		return result;
	}

	private int[ ] getBestPath ( )
	{
		return population.get( POPULATION_SIZE - 1 ).getChromosome( );
	}

	private class Individual extends Solution implements Comparable<Individual>
	{
		private double fitnessScore;
		private int[ ] chromosome;

		public Individual ( Map m )
		{
			super.setMap( m );
			chromosome = super.shuffle( super.createOrder( super.getMap( ).getNumberOfCities( ) ) );
			evaluateFitness( );
		}

		public Individual ( Map m, Individual i )
		{
			super.setMap( m );
			chromosome = i.getChromosome( );
			mutate( );
			evaluateFitness( );
		}

		public Individual ( Map m, Individual i1, Individual i2 )
		{
			super.setMap( m );
			chromosome = crossover( i1.getChromosome( ), i2.getChromosome( ) );
			mutate( );
			evaluateFitness( );
		}

		private void evaluateFitness ( )
		{
			fitnessScore = super.calculateDistance( chromosome );
		}

		private void mutate ( )
		{
			if ( Math.random( ) > MUTATION_RATE )
				return;

			int lower, upper, range;
			int[ ] subtour;

			range = ( int ) ( MUTATION_RATE * super.getMap( ).getNumberOfCities( ) );
			lower = ( int ) ( Math.random( ) * ( super.getMap( ).getNumberOfCities( ) - range ) );
			upper = lower + range;

			subtour = new int[ upper - lower ];

			for ( int i = 0; i < subtour.length; i++ )
				subtour[ i ] = chromosome[ lower + i ];

			subtour = super.shuffle( subtour );

			for ( int i = 0; i < subtour.length; i++ ) 
				chromosome[ lower + i ] = subtour[ i ];
		}

		public int compareTo ( Individual other )
		{
			return ( int ) ( other.getFitnessScore( ) - fitnessScore );
		}

		public double getFitnessScore ( ) { return fitnessScore; }
		public int[ ] getChromosome ( ) { return chromosome; }

		@Override
		public void solve ( Map m )
		{

		}
	}
}