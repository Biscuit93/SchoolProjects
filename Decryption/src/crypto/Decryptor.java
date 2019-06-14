/*
Cryptanalysis Program
By Anthony Ferski
5192885 / AF11MB
For COSC4P03 Winter 2018 Assignment 3
Explaination of methodology is given prior to each task method.
*/

package crypto;

import java.util.*;

public class Decryptor
{
	static final String cipher1 = "DYDCFWGKDIOIHPDQDXTFKKGZGXCWFPGCADWCCYKDUDRMHKIVFADOOGRZFCGAFQGTIRYAGDKHDKCYQDPYIKLFMFDRCYYAGGQDXMFXFPYDPGVWDKGOEKDRMYAGZFYYXGKGZGXCWDGCVFRFMGOYICYGFXCGPKGYWXFRCYIYAGGVWDKGCEXYDVFYGTGFWIRYAGOGFYACYFKFRFKVIKGOCWFPGCYFYDIRTDYAGRIEMAWITGKYIOGCYKILFRGRYDKGWXFRGYWEKCEGOZLYAGGVWDKGCCDRDCYGKFMGRYCWKDRPGCCXGDFKFPGCAIVGFZIFKOAGKCYFKCADWPECYIODFRIHYAGCYIXGRWXFRCYAFYPFRCFQGAGKWGIWXGFROKGCYIKGHKGGOIVYIYAGMFXFJL";
	static final String cipher2 = "LGEMWMYIPAGRPPJHEXSWKKLOKXRVOLAVZYQTXICGYKLOKMRVCZBGIXLXITBALVMDKVFRDAGLMXYFZWCAHEXYWQGPYJXJXBSGXIXWPNSBDWJXYKXVOYMMEIGDBWIKFWEIGUBMMVASRXSGGJXYTHCHVQZFARWKVRYGGVLKKZFROTXWSBW";
	static final String cipher3 = "2601 11838 6222 14340 13813 6102 14340 5332 10302 12515 8493 3400 15017 11696 12519 9310 7800 17240 1126 7751 6341 761 14340 4787 411 9822 10581 12403 2281 10274 2498 764 3191 3976 7175 5817 3323 9414 13152 3375 2696 10188 11281 10674 13915 4012 18014 871 14371 10852 37 15025 15514 17186 11838 7236 550 17797 7236 15265 13004 17110 10093 15514 17186 18063 12960 16792 14466 7473 12738 6740 2313 10041 6742 16866 8718 2839 10349 14340 11042 5879 15259 741 1836 3257 16038 13214 6582 9769 14447 3894 7874 8590 14327 2663 3642 11317 2292 8613 10834 13912 14954 5493 16345 6573 550 17360 11126 17578 6702 14446 1072 16147 4301 7128 10324 6406 12226 7874 10535 6573 3471 6911 13742 7009 12285 9770 14340 445 1112 741 2008 14208 11277 16175 8215 7018 2008 10188 6328 9992 6702 5090 13325 12166 9724 14466 6019 14446 2839 10349 1493 13201 15995 7273 11091 4898 9906 3375 14184 9314 1274 14340 11869 16752 3855 4070 10188 16044 6722 10358 9234 5826 11042 5879 9437 4264 14208 11277 16175 15061 7725 15860 1815 15893 14808 14604 9738 14993 12914 14742 12525 3938 13909 14473 11633 6715 10226 14254 1662 1657 5380 15672 13760 4363 4364 17578 6702 15104 14340 15755 11536 12760 13613 906 9963 13061 496 13909 1470 15300 13319 13885 12914 5879 14340 4787";
	static final int publicKeyN = 18209;
	static final int publicKeyB = 3001;

	static final int lettersInEnglish = 26;
	static final String etaoin = "ETAOINSHRDLUCMFWYPVBGKJQXZ";
	static final String[ ] digraphs = { "TH", "HE", "AN", "IN", "ER", "ON", "RE", "ED", "ND", "HA", "AT", "EN", "ES", "OF", "NT", "EA", "TI", "TO", "IO", "LE", "IS", "OU", "AR", "AS", "DE", "RT", "VE" };
	static final String[ ] trigraphs = { "THE", "AND", "THA", "ENT", "ION", "TIO", "FOR", "NDE", "HAS", "NCE", "TIS", "OFT", "MEN" };
	static final String[ ] doubles = { "SS", "EE", "TT", "FF", "LL", "MM", "OO" };
	
	//Selects the decryption task to be taken
	public Decryptor ( )
	{
		switch ( promptOption( "Select task ( 1, 2, 3 ) :", 3 ) )
		{
			case 1: 
				decrypt1( );
				break;
			case 2:
				decrypt2( );
				break;
			case 3:
				decrypt3( );
				break;
			default:
				System.out.println( "Error" );
				break;
		}
	}

	/*
	Methodology for Part 1:

	The solution for this part found by using a genetic algorithm.
	Each individual of the population holds a decryption key as its 
	chromosome. The fitness of each individual is evaluated by how well
	the decrypted ciphertext matches english letter frequency as well as 
	frequencies of digraphs, trigraphs, and double letters. All initial 
	individuals start with a chromosome that is based on the frequency 
	analysis of the ciphertext. At each generation, most of the population 
	is removed and the new individuals' chromosomes are based on the elite 
	that remained.

	This alone does not get the correct key, but reveals enough of the
	plaintext to perform plaintext attacks. Along with the frequencies of
	digraphs, trigraphs and doubles, the individuals are also scored using
	cribs, or known plaintexts. After each thousand generations, the user
	can add cribs to help direct the population toward the correct solution.
	For most attempts, the phrase DEATHSTAR is often revealed within the
	first trial, so cribs related to Star Wars help a lot here; for example:
	REBEL, GALAXY, GALACTIC, EMPIRE, SPACESHIP, PRINCESSLEIA. Also, at the
	end of the ciphertext, the phrase RESTOREFREEDOMTOTHEGALAXY can be 
	guessed rather easily using cribs.

	For every subsequent trial, the population is evolved and reevaluated 
	using the new cribs. The solution is often found within 10 trials with
	effective use of cribs.
	*/

	public void decrypt1 ( )
	{
		int trial = 1;

		GeneticDecryptor g = new GeneticDecryptor( cipher1 );
		while ( true )
		{
			System.out.println( "Trial " + trial );
			g.runTrial( );

			if ( !promptYesNo( "Try again?") )
				break;

			if ( promptYesNo( "Add cribs?") )
				g.addCribs( );

			trial++;
		}
	}

	public class GeneticDecryptor
	{
		private ArrayList<Individual> population;
		private double mutationRate;
		private int generations, populationSize, elitePopulation,
		            frequencyScore, digraphScore, trigraphScore, doubleScore, cribScore;
		private String target, base;
		private ArrayList<String> cribs;

		public GeneticDecryptor ( String t )
		{
			cribs = new ArrayList<String>( );
			target = t;
			mutationRate = 0.25;
			generations = 1000;
			populationSize = 100;
			elitePopulation = 10;

			frequencyScore = 10;
			digraphScore = 20;
			trigraphScore = 40;
			doubleScore = 30;
			cribScore = 100;

			createBaseGuess( );
			initPopulation( );
		}

		public void runTrial( )
		{
			evolvePopulation( );
			System.out.println( decrypt( ) );
		}

		//This will create the base chromosome that the first
		//individuals use. It is based on the character frequency
		//analysis of the ciphertext.
		private void createBaseGuess ( )
		{

			int[ ] letterCount = new int[ lettersInEnglish ];
			boolean[ ] used = new boolean[ lettersInEnglish ];
			char[ ] baseChars = new char[ lettersInEnglish ];
			int max, curr;
			String temp = "";

			for ( char c : target.toCharArray( ) )
				letterCount[ charToInt( c ) ]++;

			for ( int i = 0; i < lettersInEnglish; i++ )
			{
				max = -1;
				curr = -1;
				for ( int j = 0; j < lettersInEnglish; j++ )
					if ( !used[ j ] && letterCount[ j ] > max )
					{
						max = letterCount[ j ];
						curr = j;
					}
				baseChars[ curr ] = etaoin.charAt( i );
				used[ curr ] = true;
			}

			for ( char c : baseChars )
				temp += c;

			base = temp;
		}

		private void initPopulation ( )
		{
			population = new ArrayList<Individual>( );
			for ( int i = 0; i < populationSize; i++ )
				population.add( new Individual( ) );
			Collections.sort( population );
		}

		//Evolves the population using simple dropout and
		//cloning / mutation of the elite population
		private void evolvePopulation ( )
		{
			for ( int g = 1; g <= generations; g++ )
			{
				for ( int i = 0; i < populationSize - elitePopulation; i++ )
					population.remove( 0 );

				for ( int i = 0; i < populationSize - elitePopulation; i++ )
					population.add( new Individual ( population.get( ( int ) ( Math.random( ) * elitePopulation ) ) ) );

				Collections.sort( population );
			}
		}

		public String decrypt ( )
		{
			Individual best = population.get( populationSize - 1 );
			String key = best.getChromosome( );
			double score = best.getFitnessScore( );
			System.out.println( "Key: " + key + "\nScore: " + score );
			return decryptSubstitutionCipher( target, key );
		}

		private void printFitnessOfPopulation ( )
		{
			for ( Individual i : population )
				System.out.println( i.getFitnessScore( ) );
			System.out.println( );
		}

		private void printBestFitness ( )
		{
			System.out.println( population.get( populationSize - 1 ).getFitnessScore( ) );
		}

		//This will prompt to add cribs to augment the fitness scoring
		//of the individuals in the population
		public void addCribs ( )
		{
			String input;
			String[ ] temp;
			Scanner scan = new Scanner( System.in );

			while ( true )
			{
				System.out.println( "Enter cribs seperated by spaces now.");
				input = scan.nextLine( );
				temp = input.toUpperCase( ).split( " " );

				System.out.println( );	
				for ( String s : temp )
					System.out.println( "\t" + s );
				System.out.println( );

				if ( promptYesNo( "Add these cribs?" ) )
					break;
			}

			for ( String s : temp )
				cribs.add( s );
		}

		//This is the method that actually decrypts the cipher
		public String decryptSubstitutionCipher ( String cipher, String key )
		{
			String result = "";

			for ( char c : cipher.toCharArray( ) )
				result += key.charAt( charToInt( c ) );

			return result; 
		}

		public class Individual implements Comparable<Individual>
		{
			private double fitnessScore;
			private String chromosome;

			public Individual ( )
			{
				chromosome = base;
				mutate( );
				evaluateFitness( );
			}

			public Individual ( Individual i )
			{
				chromosome = i.getChromosome( );
				mutate( );
				evaluateFitness( );
			}

			//Mutate the chromosome through gene swapping
			//i.e. swap letters around in the key
			private void mutate ( )
			{
				char[ ] genes = chromosome.toCharArray( );
				char temp;
				int index;
				String newChromosome = "";

				for ( int i = 0; i < genes.length; i++ )
					if ( Math.random( ) < mutationRate )
					{
						index = ( int ) ( Math.random( ) * genes.length );
						temp = genes[ i ];
						genes[ i ] = genes[ index ];
						genes[ index ] = temp;
					}

				for ( char c : genes )
					newChromosome += c;

				chromosome = newChromosome;
			}

			//This will evaluate the fitness based on frequencies
			//of common occuring patterns and plaintext cribs
			private void evaluateFitness ( )
			{
				fitnessScore = 0;
				String text = decryptSubstitutionCipher( target, chromosome );

				int[ ] letterCount = new int[ lettersInEnglish ];

				for ( char c : text.toCharArray( ) )
					letterCount[ charToInt( c ) ]++;

				for ( int i = 0; i < lettersInEnglish; i++ )
					fitnessScore += letterCount[ charToInt( etaoin.charAt( i ) ) ] * ( frequencyScore / ( i + 1 ) );

				for ( int i = 0; i < digraphs.length; i++)
					fitnessScore += matchCount( text, digraphs[ i ] ) * ( digraphScore / ( i + 1 ) );

				for ( int i = 0; i < trigraphs.length; i++)
					fitnessScore += matchCount( text, trigraphs[ i ] ) * ( trigraphScore / ( i + 1 ) );

				for ( int i = 0; i < doubles.length; i++)
					fitnessScore += matchCount( text, doubles[ i ] ) * ( doubleScore / ( i + 1 ) );

				for ( String s : cribs )
					fitnessScore += matchCount( text, s ) * cribScore * s.length( );
			}

			private int matchCount ( String str, String target )
			{
				int lastIndex = 0, result = 0;

				while( lastIndex != -1 )
				{
					lastIndex = str.indexOf( target, lastIndex );
					if ( lastIndex != -1 )
					{
						result++;
						lastIndex += target.length( );
					}

				}

				return result;
			}

			public double getFitnessScore ( )
			{
				return fitnessScore;
			}

			public String getChromosome ( )
			{
				return chromosome;
			}

			public int compareTo ( Individual other )
			{
				return ( int ) ( fitnessScore - other.getFitnessScore( ) );
			}
		}
	}

	/*
	Methodology for Part 2:

	First the potential lengths of keys are found using a simple form of
	coincidence calculation. The incidence of characters matching other
	characters on the ciphertext is found, then for each key length, 
	a coincidence index is calculated through averaging the occurence of
	coincidences that are n spaces apart. Any index that is twice the
	average of all the coincidences is taken as a potential key size.

	After this, for each of these key sizes, we perform rudimentary
	frequency analysis to determine each letter of the key. The frequency
	analysis is done seperately on portions of the ciphertext corresponding
	to each character in the key.

	Again, this alone does not find the correct key, but reveals enough of
	the plaintext to guess the correct key. This is either a weakness of how
	I evaluate the character frequency or a consequence of the short 
	ciphertext length. As it happens, the shortest key found is off by
	one character, which makes guessing the correct key rather simple.
	*/

	public void decrypt2 ( )
	{
		VigenereDecryptor v = new VigenereDecryptor( cipher2 );
	}

	public class VigenereDecryptor
	{
		private String target;
		private ArrayList<String> guesses;

		public VigenereDecryptor ( String t )
		{
			target = t;
			int[ ] possibleKeySize = findKeySize( );

			System.out.println( "Best guesses:" );
			for ( int ks : possibleKeySize )
			{
				String key = findKey( ks );
				System.out.println( "Key: " + key + "\n" + decryptVigenere( target, key ) );
				System.out.println( );
			}
			if ( promptYesNo( "Guess key?" ) )
			{
				guesses = new ArrayList<String>( );
				addKeyGuesses( );
				for ( String g : guesses )
				{
					System.out.println( "Key: " + g + "\n" + decryptVigenere( target, g ) );
					System.out.println( );
				}
			}
		}

		//This finds the potential key sizes using coincidence index.
		private int[ ] findKeySize ( )
		{
			int[ ] keys;
			int[ ] coincidences = new int[ target.length( ) ];
			double[ ] coincidenceIndex = new double[ target.length( ) ];

			coincidences[ 0 ] = 0;
			coincidences[ target.length( ) - 1 ] = 0;
			for ( int s = 1; s < target.length( ) - 2; s++ )
				for ( int l = 1; l < target.length( ) - 2 - s; l++ )
					if ( target.charAt( s + l ) == target.charAt( l ))
						coincidences[ s ] += 1.0;

			int step, temp, count;
			for ( int i = 1; i < target.length( ); i++ )
			{
				step = i;
				temp = step;
				count = 0;
				while ( temp < target.length( ) )
				{
					coincidenceIndex[ i ] += coincidences[ temp ];
					temp += step;
					count++;
				}
				coincidenceIndex[ i ] /= count;
			}

			double average = 0;
			for ( double d : coincidenceIndex )
				average += d;
			average /= coincidenceIndex.length;

			count = 0;

			for ( double d : coincidenceIndex )
				if ( d > ( average * 2.0 ) )
					count++;

			keys = new int[ count ];

			step = 0;
			for ( int i = 0; i < coincidenceIndex.length; i++ )
				if ( coincidenceIndex[ i ] > ( average * 2.0 ) )
				{
					keys[ step ] = i;
					step++;
				}

			return keys;
		}

		//This finds a potential key for all potential key sizes found
		//in the earlier step. These are found through frequency
		//analysis.
		private String findKey ( int size )
		{
			char[ ] key = new char[ size ];
			int[ ] letterCount;
			double score, best;
			String result = "";

			for ( int i = 0; i < size; i++ )
			{
				best = 0.0;
				for ( int l = 0; l < lettersInEnglish; l++ )
				{
					score = 0.0;
					letterCount = new int[ lettersInEnglish ];
					for ( int s = i; s < target.length( ); s += size )
						letterCount[ ( charToInt( target.charAt( s ) ) - l + lettersInEnglish ) % lettersInEnglish  ]++;
					for ( int lc = 0; lc < lettersInEnglish; lc++ )
						score += letterCount[ charToInt( etaoin.charAt( lc ) ) ] / ( lc + 1 );
					if ( score > best )
					{
						best = score;
						key[ i ] = intToChar( l );
					}
				}
			}

			for ( char c : key )
				result += c;

			return result;
		}

		//This decrypts the ciphertext.
		private String decryptVigenere ( String cipher, String key )
		{
			String result = "";

			int step1 = 0, step2 = 0;
			while ( step1 < cipher.length( ) )
			{
				result += intToChar( ( charToInt( cipher.charAt( step1 ) ) - charToInt( key.charAt( step2 ) ) + lettersInEnglish ) % lettersInEnglish );
				step1++;
				step2++;
				if ( step2 == key.length( ) )
					step2 = 0;
			}

			return result;
		}

		//This allows the user to add guesses for when the above
		//fails to find the correct key by one character place.
		public void addKeyGuesses ( )
		{
			String input;
			String[ ] temp;
			Scanner scan = new Scanner( System.in );

			while ( true )
			{
				System.out.println( "Enter guesses seperated by spaces now.");
				input = scan.nextLine( );
				temp = input.toUpperCase( ).split( " " );

				System.out.println( );	
				for ( String s : temp )
					System.out.println( "\t" + s );
				System.out.println( );

				if ( promptYesNo( "Add these keys?" ) )
					break;
			}

			for ( String s : temp )
				guesses.add( s );
		}
	}

	/*
	Methodology for Part 3:

	Because the RSA public key is small, it can easily be factored into its
	prime component using brute force. After this, it is trivial to
	recreate the private key. This private key can then be used to reveal
	the plaintext.

	We generate primes using the Sieve of Eratosthenes, then we use trial
	and error to find the two primes that multiply to N. After this, phi of
	N can be found through subtracting 1 from each factor and multiplying
	together. A can be found through brute force to find the value that 
	multiplies with B to have a remainder of 1 when divided by phi of N.
	After this, we apply A and N to decrypt the ciphertext.
	*/

	public void decrypt3 ( )
	{
		RSADecryptor rsa = new RSADecryptor( cipher3 );
	}

	public class RSADecryptor
	{
		private String target;

		public RSADecryptor ( String t )
		{
			target = t;
			int[ ][ ] pq = bruteForceN( publicKeyN );
			int privateKeyP = pq[ 0 ][ 0 ];
			int privateKeyQ = pq[ 0 ][ 1 ];
			int phiN = ( privateKeyP - 1 ) * ( privateKeyQ - 1 );
			int privateA = bruteForceA( publicKeyB, phiN );
			System.out.println( "N: " + publicKeyN + "\tP: " + privateKeyP + "\tQ: " + privateKeyQ + "\tA: " + privateA );
			System.out.println( decryptRSA( target, privateA, publicKeyN ) );
		}

		//This decrypts the entire ciphertext
		private String decryptRSA ( String cipher, int a, int n )
		{
			String result = "";
			String[ ] temp = cipher.split( " " );
			int[ ] numbers = new int[ temp.length ];
			for ( int i = 0; i < temp.length; i++ )
				numbers[ i ] = Integer.parseInt( temp[ i ] );

			for ( int i : numbers )
				result += decodeNumber( RSAStep( i, a, n ) );

			return result;
		}

		//Performs RSA decryption on one number
		private int RSAStep ( int y, int a, int n )
		{
			if ( n == 1 ) return 0;
			int c = 1;
			for ( int i = 0; i < a; i++ )
				c = ( c * y ) % n;
			return c;
		}

		//Decodes a number into a character triplet
		private String decodeNumber ( int n )
		{
			String result = "";
			int temp;

			result += intToChar( n / 676 );
			result += intToChar( ( n / 26 ) % 26 );
			result += intToChar( n % 26 );

			return result;
		}

		//This brute forces N though prime multiplication
		private int[ ][ ] bruteForceN ( int key )
		{
			int[ ] primes = generatePrimes( key );
			int[ ][ ] result;
			String pairs = "";
			int count = 0;

			for ( int p = 0; p < primes.length - 1; p++ )
			{
				for ( int q = primes.length - 1; q > p; q-- )
				{
					if ( p * q == publicKeyN )
					{
						pairs += p + " " + q + " ";
						count++;
					}

					if ( p * q < publicKeyN )
						break;
				}
			}

			String[ ] temp = pairs.split( " " );
			result = new int[ count ][ 2 ];

			for ( int i = 0; i < count; i++ )
			{
				result[ i ][ 0 ] = Integer.parseInt( temp[ i * 2 ] );
				result[ i ][ 1 ] = Integer.parseInt( temp[ i * 2  + 1] );
			}
			return result;
		}

		//Similar to above but with modulo division
		private int bruteForceA ( int b, int phi )
		{
			int a = 0;
			while ( true )
			{
				if ( ( a * b ) % phi == 1 )
					break;
				a++;
			}

			return a;
		}

		//Sift the Two's and Sift the Three's,
		//The Sieve of Eratosthenes.
		//When the multiples sublime,
		//The numbers that remain are Prime.
		private int[ ] generatePrimes ( int max )
		{
			boolean[ ] notPrime = new boolean[ max ];
			int[ ] primes;

			notPrime[ 0 ] = true;
			notPrime[ 1 ] = true;

			int count = 0, temp, step, i = 2;
			while ( i < max )
			{
				if ( notPrime[ i ] == false )
				{
					count++;
					step = i;
					temp = i + step;
					while ( temp < max )
					{
						notPrime[ temp ] = true;
						temp += step;
					}
				}
				i++;
			}

			i = 0;
			temp = 0;
			primes = new int[ count ];
			while ( i < max )
			{
				if ( !notPrime[ i ] )
				{
					primes[ temp ] = i;
					temp++;
				}
				i++;
			}

			return primes;
		}
	}

	//A - Z -> 0 - 25
	private int charToInt ( char c )
	{
		return ( int ) ( c - 65 );
	}

	//0 - 25 -> A - Z
	private char intToChar ( int i )
	{
		return ( char ) ( i + 65 );
	}

	private boolean promptYesNo ( String question )
	{
		String answer = "";
		Scanner scan = new Scanner( System.in );

		System.out.println( question + " ( yes / no )" );

		while ( !answer.equalsIgnoreCase( "yes" ) && !answer.equalsIgnoreCase( "no" ))
			answer = scan.nextLine( );

		if ( answer.equalsIgnoreCase( "yes" ) )
			return true;
		else
			return false;

	}

	private int promptOption ( String question, int upper )
	{
		int answer = 0;
		Scanner scan = new Scanner( System.in );

		System.out.println( question );
		while ( answer <= 0 || answer > upper )
			answer = scan.nextInt( );

		return answer;
	}

	public static void main ( String[ ] args )
	{
		Decryptor d = new Decryptor( );
	}
}
