Parameters

First Row: Integer Parameters

	1: Data Set
		Default = 0
		0 = L30fft16.out
		1 = L30fft25.out
		2 = L30fft32.out
		3 = L30fft64.out
	
	2: Lattice Width
		Default = 15

	3: Lattice Height
		Default = 15

	4: Wrap Mode
		Default = 3
		0 = no wrap
		1 = horizontal wrap
		2 = vertical wrap
		3 = both horizontal and vertical

	5: Neighbourhood Function
		Default = 0
		0 = Gauss
		1 = Ricker (Mexican Hat)

	6: Radius Cutoff
		Only nodes within radius of BMU updated when on.
		Default = 0
		0 = off
		1 = on

	7: Maximum Epoch
		Default = 1000

Second Row: Floating Point Parameters

	1: Initial Radius
		Default = 7.5

	2: Decay Rate of Radius
		Default = 0.005
		Higher values decay faster.

	3: Minimum Radius
		Default = 0.0

	4: Initial Learning Rate
		Default = 1.0

	5: Decay Rate of Learning Rate
		Higher values decay faster.
		Default = 0.005

	6: Minimum Learning Rate
		Default = 0.0

	7: Testing Set Size Ratio
		Default = 0.15

	8: Update Threshold
		Weights update if the update function yields a value higher than
		this one.
		Default = 0.01

	9: Matching Threshold Distance
		A data vector must be within this distance to be 1-to-1 mapped
		to a lattice node.
		Default = 1000.0

	10: Classification Threshold Distance
		A lattice node must be within this distance to a data vector to
		set to its type.
		Default = 1000.0
