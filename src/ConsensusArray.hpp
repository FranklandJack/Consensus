#ifndef ConsensusArray_hpp
#define ConsensusArray_hpp

#include <vector> // For holding the data in the array.
#include <random> // For generating random numbers.
#include <iostream> // For outputting board.
#include <utility> // For std::pair.
#include <cmath> // For round.

/**
 * \file
 * \brief Class to model a 2D lattice of cells in the Consensus model that can be Susceptible, Infected
 * or recovered and can move between those states stochastically.
 */
class ConsensusArray
{
public:
    /**
     * \enum State
     * \brief Enumeration type to hold the state of the cell, dead or alive.
     */
    enum State
    {
        Green,
        Red,
        Blue,
        MAXSTATE,
    };

    /// Look-up table for alive/dead cells symbols for printing.
    static constexpr int stateSymbols[MAXSTATE] = {0,1,2};

private:
    /// Member variable that holds number of rows in lattice.
    int m_rowCount;

    /// Member variable that holds number of columns in lattice.
    int m_colCount;

    /// Member variable that holds the actual data in the lattice.
    std::vector<State> m_boardData;

    /// Member variable for the probability of going from susceptible to infected.
    double m_p_1;

    double m_p_2;

public:
    /**
     *\brief operator overload for getting the state at a site.
     *
     * This method is implemented since the states are stored internally as a 1D vector, hence
     * they need to be indexed in a special way in order to get the site that would correspond to
     * the (i,j) site in matrix notation. This function allows the caller to treat the lattice as a
     * 2D matrix without having to worry about the internal implementation.
     *
     *\param row row index of site.
     *\param col column index of site.
     *\return reference to state stored at site so called can use it or set it.
     */
    ConsensusArray::State& operator()(int row, int col);

    /**
     *\brief constant version of non-constant counterpart for use with constant ConsensusArray object.
     *
     * See non-constant version for description.
     *
     *\param row row index of site.
     *\param col column index of site.
     *\return constant reference to state stored at site so called can use it only.
     */
    const ConsensusArray::State& operator()(int row, int col) const;

    /**
     *\brief Constructor that initializes all cells to the state that is its arguments.
     *\param rows number of rows on the board.
     *\param cols number of columns on the board.
     *\param probSI probability of going from susceptible to infected state if cell is in contact with infected cell.
     *\param probIR probability of infected site going from infected to recovered.
     *\param probRS probability of recovered site becoming susceptible again.
     *\param state State instance to initialise all cells to will default to alive.
     *\param immuneFraction floating point instance representing the fraction of the population who are completely immune to the infection.
     */
    ConsensusArray(
    	int rows = 50,
    	int cols = 50,
    	double prob1 = 1.0,
    	double prob2 = 1.0,
    	ConsensusArray::State state = ConsensusArray::Green);

    /**
     *\brief Constructor that randomises lattice to an even mix of states.
     *\param rows number of rows on the board.
     *\param cols number of columns on the board.
     *\param probSI probability of going from susceptible to infected state if cell is in contact with infected cell.
     *\param probIR probability of infected site going from infected to recovered.
     *\param probRS probability of recovered site becoming susceptible again.
     *\param generator std::default_random_engine reference for generating random numbers.
     *\param immuneFraction floating point instance representing the fraction of the population who are completely immune to the infection.
     */
    ConsensusArray(
        std::default_random_engine &generator,
    	int rows = 50,
    	int cols = 50,
    	double prob1 = 1.0,
    	double prob2 = 1.0
    	);

    /**
     *\brief Randomises the cells in the board with equal probability of being in each state.
     *\param std::deafult_random_engine reference for random number generation.
     */
    void randomise(std::default_random_engine &generator);

    /**
     *\brief Getter for the number of rows.
     *\return Integer value representing the number of rows.
     */
    int getRows() const;

    /**
     *\brief Getter for number of columns.
     *\return Integer value representing the number of columns.
     */
    int getCols() const;

    /**
     *\brief Getter for size of lattice #rows * #columns.
     *\return Integer value representing the size of the lattice.
     */
    int getSize() const;

    /**
     *\brief Getter for the probability of going from susceptible to infected upon contact between two cells.
     *\return Floating point value representing the probability of going from susceptible to infected upon contact.
     */
    double getp1() const;

    /**
     *\brief Getter for the probability of going from infected to recovered.
     *\return Floating point value representing the probability of going from infected to recovered.
     */
    double getp2() const;

    /**
     *\brief Setter for the probability of going from susceptible to infected upon contact between two cells.
     *\param prob floating point value representing the probability of going from susceptible to infected.
     */
    void setp1(double prob);

    /**
     *\brief Setter for the probability of going from infected to recovered.
     *\param prob floating point value representing the probability of going from infected to recovered.
     */
    void setp2(double prob);

    /**
     *\brief returns probability of cell going from one state to another.
     *\param state1 current state type.
     *\param state2 proposed state type.
     *\return probability of the update.
     */
     double getProbability(ConsensusArray::State state1, ConsensusArray::State state2) const;

    /**
     *\brief Updates a random cell in the grid.
     *\param std::default_random_engine reference for random number generation.
     *\return the new updated state of the cell.
     */
    ConsensusArray::State update(std::default_random_engine& generator);

    /**
     *\brief calculates the total number of cells in a given state.
     *\param state value representing the state of interest.
     *\return Integer value representing the total number of cells in the state of interest
     */
    int stateCount(ConsensusArray::State state) const;

    /**
     *\brief calculates the total fraction of cells in a given state.
     *\param state value representing the state of interest.
     *\return Floating point value representing the fraction of cells in the state of interest
     */
    double stateFraction(ConsensusArray::State state) const;

    /**
     *\brief streams the board to an output stream in a nicely formatted way
     *\param out std::ostream reference that is being streamed to
     *\param board ConsensusArray reference to be printed
     *\return std::ostream reference to output can be chained.
     */
     friend std::ostream& operator<<(std::ostream& out, const ConsensusArray &board);

};

#endif /* ConsensusArray_hpp */
