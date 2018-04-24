#ifndef ConsensusResults_hpp
#define ConsensusResults_hpp
#include <iostream>
#include <iomanip>

/**
 *\file 
 *\class ConsensusResults
 *\brief Class for easily handling results of Consensus simulation.
 *
 * This class essentially just holds some values and has an operator to easily output
 * them to a stream.
 */
class ConsensusResults
{
public:
	/// Order parameter.
	double orderParameter;
	/// Order parameter error.
	double orderParameterError;
	/// Susceptibility.
	double susceptibility;
	/// Susceptibility error.
	double susceptibilityError;

	/** 
	 *\brief operator<< overload for outputting the results.
	 *\param out std::ostream reference that is the stream being outputted to.
	 *\param results constant ConsensusResults instance to be output.
	 *\return std::ostream reference so the operator can be chained.
	 *
	 * Results will be output in a formatted table for easy viewing in the command line or a file.
	 */
	friend std::ostream& operator<<(std::ostream& out, const ConsensusResults &results);
};

#endif /* ConsensusResults_hpp */
