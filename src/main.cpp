#include "ConsensusArray.hpp"
#include "getTimeStamp.hpp"
#include "makeDirectory.hpp"
#include "ConsensusInputParameters.hpp"
#include "DataArray.hpp"
#include "ConsensusResults.hpp"
#include "Timer.hpp"
#include "Susceptibility.hpp"
#include <random>
#include <iostream>
#include <algorithm>
#include <thread>
#include <chrono>
#include <boost/program_options.hpp>
#include <fstream>
#include <iomanip>
#include <string>

int main(int argc, char const *argv[])
{
/*************************************************************************************************************************
************************************************* Preparations **********************************************************
*************************************************************************************************************************/

    // Start the clock so execution time can be calculated.
    Timer timer;

    // Seed the pseudo random number generator using the system clock.
    unsigned int seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());

    // Create a generator that can be fed to any distribution to produce pseudo random numbers according to that distribution.
    std::default_random_engine generator(seed);

    // Input parameters.
    int rowCount;
    int colCount;
    double p_1;
    double p_2;
    int totalSweeps;
    int measurementInterval;
    std::string outputName;

    // Set up optional command line arguments.
    boost::program_options::options_description desc("Options for Consensus simulation");

    // Add all optional command line arguments.
    desc.add_options()

        ("column-count,c", boost::program_options::value<int>(&rowCount)->default_value(50), "The number of rows in the lattice.")
        ("row-count,r", boost::program_options::value<int>(&colCount)->default_value(50), "The number of columns in the lattice.")
        ("p_1,p", boost::program_options::value<double>(&p_1)->default_value(1), "Value of p_1 in simulation.")
        ("p_2,p", boost::program_options::value<double>(&p_2)->default_value(1), "Value of p_2 in simulation.")
        ("sweeps,s", boost::program_options::value<int>(&totalSweeps)->default_value(10000), "The number of sweeps in the simulation.")
        ("output,o",boost::program_options::value<std::string>(&outputName)->default_value(getTimeStamp()), "Name of output directory to save output files into.")
        ("animate,a","Animate the program by printing the current state of the lattice to an output file during simulation")
        ("help,h", "Produce help message");

    // Make arguments available to program.
    boost::program_options::variables_map vm;
    boost::program_options::store(boost::program_options::parse_command_line(argc,argv,desc), vm);
    boost::program_options::notify(vm);

    // If the user asks for help display it then exit.
    if(vm.count("help"))
    {
        std::cout << desc << '\n';
        return 1;
    }

    // Create an output directory from either the default time stamp or the user defined string.
    makeDirectory(outputName);

    // Create an output file for the lattice so it can be animated.
    std::fstream latticeOutput(outputName+"/Lattice.dat", std::ios::out);

    // Create an output file for the order parameter which in this case is the fraction of infected states.
    std::fstream fractionsOutput(outputName+"/Fractions.dat", std::ios::out);

    // Create an output file for the input parameters.
    std::fstream inputParametersOutput(outputName+"/Input.txt", std::ios::out);

    // Create an output file for the results.
    std::fstream resultsOutput(outputName+"/Results.txt", std::ios::out);

    // Create a Consensus lattice that will be used in the simulation.
    ConsensusArray lattice(generator,rowCount, colCount, p_1, p_2);

    // Print the initial lattice to an output file.
    latticeOutput << lattice;

    // Create an object to hold the input parameters.
    ConsensusInputParameters inputParameters
    {
      rowCount,
      colCount,
      p_1,
      p_2,
      totalSweeps,
      outputName
    };

    // Print the input parameters to the command line and to the output file.
    std::cout << inputParameters << '\n';
    inputParametersOutput << inputParameters << '\n';

/*************************************************************************************************************************
************************************************* Main Loop *************************************************************
*************************************************************************************************************************/


   for(int sweep = 0; sweep < totalSweeps; ++sweep )
   {
      // Update the lattice by performing row*col sweeps.
      for(int i = 0; i < lattice.getSize(); ++i)
      {
        lattice.update(generator);
      }

      // If we are on a measurement sweep then do any measurement/output.
      if((0 == sweep%10))
      {
        // Calculate the fraction of each type.
        double redFrac = lattice.stateFraction(ConsensusArray::Red);
        double greenFrac = lattice.stateFraction(ConsensusArray::Green);
        double blueFrac = lattice.stateFraction(ConsensusArray::Blue);

        // Output the fraction of infected states and the current sweep.
        fractionsOutput << sweep << ' ' <<  redFrac << ' ' << greenFrac << ' ' << blueFrac << '\n';


      }
      if(vm.count("animate"))
      {
        // Move to the top of the file.
      latticeOutput.seekg(0,std::ios::beg);

      // Output the current state of the lattice.
      latticeOutput << lattice << std::flush;
      }
   }


/*************************************************************************************************************************
******************************************** Output/Clean Up *************************************************************
**************************************************************************************************************************/


   // Report how long the program took to execute.
   std::cout << std::setw(30) << std::setfill(' ') << std::left << "Time take to execute(s) =    " <<
   std::right << timer.elapsed() << '\n';

   return 0;
}
