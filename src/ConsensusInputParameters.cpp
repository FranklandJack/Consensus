#include "ConsensusInputParameters.hpp"

std::ostream& operator<<(std::ostream& out, const ConsensusInputParameters& params)
{
	int outputColumnWidth = 30;
    out << "Input-Parameters..." << '\n';
    out << std::setw(outputColumnWidth) << std::setfill(' ') << std::left << "Rows: " << std::right << params.rowCount << '\n';
    out << std::setw(outputColumnWidth) << std::setfill(' ') << std::left << "Columns: " << std::right << params.colCount << '\n';
    out << std::setw(outputColumnWidth) << std::setfill(' ') << std::left << "p_1: " << std::right << params.p_1 << '\n';
    out << std::setw(outputColumnWidth) << std::setfill(' ') << std::left << "p_2: " << std::right << params.p_2 << '\n';
    out << std::setw(outputColumnWidth) << std::setfill(' ') << std::left << "Sweeps: " << std::right << params.sweeps << '\n';
		out << std::setw(outputColumnWidth) << std::setfill(' ') << std::left << "Output-Directory: " << std::right << params.outputDirectory << '\n';
    return out;
}
