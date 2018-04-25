#include "ConsensusResults.hpp"

std::ostream& operator<<(std::ostream &out, const ConsensusResults &results)
{
	int outputColumnWidth = 30;
	out << "Results..." << '\n';
  out << std::setw(outputColumnWidth) << std::setfill(' ') << std::left << "Absorbing-State: " <<
   	std::right << results.absorbingState << '\n';

	return out;
}
