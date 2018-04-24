#ifndef ConsensusInputParameters_hpp
#define ConsensusInputParameters_hpp
#include <iostream>
#include <iomanip>
#include <string>
/**
 *\file
 *\class ConsensusInputParameters
 *\brief Class for easily handling input parameters of Consensus simulation.
 *
 * This class essentially just holds some values and has an operator to easily output
 * them to a stream.
 */
class ConsensusInputParameters
{
public:

	/// Number of rows in lattice.
	int rowCount;
	/// Number of columns in lattice.
	int colCount;
	/// Probability of cell going from susceptible to infected upon contact.
	double p_1;
	/// Probability of cell going from infected to recovered.
	double p_2;
	/// Total number of sweeps in the simulation.
	int sweeps;
	/// Output directory.
	std::string outputDirectory;


    /**
	 *\brief operator<< overload for outputting the results.
	 *\param out std::ostream reference that is the stream being outputted to.
	 *\param params constant ConsensusInputParameters instance to be output.
	 *\return std::ostream reference so the operator can be chained.
	 *
	 * Results will be output in a formatted table for easy viewing in the command line or a file.
	 */
    friend std::ostream& operator<<(std::ostream& out, const ConsensusInputParameters& params);

};
#endif /* ConsensusInputParameters_hpp */
