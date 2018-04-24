#include "ConsensusArray.hpp"

constexpr int ConsensusArray::stateSymbols[];

ConsensusArray::State& ConsensusArray::operator()(int row, int col)
{
    // Take into account periodic boundary conditions.
    row = (row + m_rowCount) % m_rowCount;
    col = (col + m_colCount) % m_colCount;

    // Return 1D index of 1D array corresponding to the 2D index.
    return m_boardData[col + row * m_colCount];
}

const ConsensusArray::State& ConsensusArray::operator()(int row, int col) const
{
    // Take into account periodic boundary conditions we add extra m_rowCount and m_colCount
    // terms here to take into account the fact that the caller may be indexing with -1.
    row = (row + m_rowCount) % m_rowCount;
    col = (col + m_colCount) % m_colCount;

    // Return 1D index of 1D array corresponding to the 2D index.
    return m_boardData[col + row * m_colCount];
}


ConsensusArray::ConsensusArray(
	int rows,
	int cols,
	double prob1,
	double prob2,
	ConsensusArray::State state
	) : m_rowCount{rows},
		m_colCount{cols},
		m_p_1{prob1},
		m_p_2{prob2},
		m_boardData(rows*cols, state)
{

}

ConsensusArray::ConsensusArray(
	std::default_random_engine &generator,
	int rows,
	int cols,
	double prob1,
	double prob2
	) : m_rowCount{rows},
		m_colCount{cols},
		m_p_1{prob1},
		m_p_2{prob2}
{
    //TODO: This can probably be made more efficient.

    // Reserve the correct amount of space in the board data.
    m_boardData.reserve(rows * cols);

    // Create a distribution to sample from the states, we -2 since we are ignoring the immune cells
    // and the parameter is a closed interval.
    static std::uniform_int_distribution<int> distribution(0,static_cast<int>(ConsensusArray::MAXSTATE)-1);
    for(int i = 0; i < rows*cols; ++i)
    {
        m_boardData.push_back(static_cast<ConsensusArray::State>(distribution(generator)));
    }

}

void ConsensusArray::randomise(std::default_random_engine &generator)
{
    // Create a uniform distribution for the states on the board.
    static std::uniform_int_distribution<int> distribution(0,static_cast<int>(ConsensusArray::MAXSTATE)-1);

    for(auto &cell : m_boardData)
    {
        cell = static_cast<ConsensusArray::State>(distribution(generator));
    }

}


int ConsensusArray::getRows() const
{
    return m_rowCount;
}

int ConsensusArray::getCols() const
{
    return m_colCount;
}

int ConsensusArray::getSize() const
{
    return m_colCount * m_rowCount;
}

double ConsensusArray::getp1() const
{
	return m_p_1;
}

double ConsensusArray::getp2() const
{
	return m_p_2;
}


void ConsensusArray::setp1(double prob)
{
	m_p_1 = prob;
}

void ConsensusArray::setp2(double prob)
{
	m_p_2 = prob;
}




ConsensusArray::State ConsensusArray::update(std::default_random_engine& generator)
{
	// Create a uniform distribution for the rows and columns remembering to subtract 1 for the closed limits.
	std::uniform_int_distribution<int> rowDistribution(0,m_rowCount-1);
	std::uniform_int_distribution<int> colDistribution(0,m_colCount-1);

  int row = rowDistribution(generator);
  int col = colDistribution(generator);

  int neighbourRow;
  int neighbourCol;


  // Create distriubtion for randomly selecting neighbour.
  static std::uniform_int_distribution<int> neighbourDistribution(0,3);

  // Generate an index for the neighbour.
  int neighbour = neighbourDistribution(generator);

  // Use four values to select one of 4 possible neighbours.
  switch (neighbour) {
    case 0:
      neighbourRow = row;
      neighbourCol = col + 1;
      break;

    case 1:
      neighbourRow = row + 1;
      neighbourCol = col;
      break;

    case 2:
      neighbourRow = row;
      neighbourCol = col - 1;
      break;

    case 3:
      neighbourRow = row - 1;
      neighbourCol = col;
  }

  // Create a distribution between 0 and 1 for accepting or rejecting an update.
  static std::uniform_real_distribution<double> distribution(0.0,1.0);

  // update the neighbour with a probability determined by the type of update.
  double updateProb = getProbability((*this)(row,col),(*this)(neighbourRow, neighbourCol));

  if(distribution(generator) < updateProb)
  {
    (*this)(neighbourRow, neighbourCol) = (*this)(row,col);
  }
  else
  {

  }
  // Return the updated state, even if it is the same as it was originally.
  return (*this)(row,col);


}

double ConsensusArray::getProbability(ConsensusArray::State state1, ConsensusArray::State state2) const
{
  if((state1==ConsensusArray::Red && state2==ConsensusArray::Green)
      || (state1==ConsensusArray::Green && state2 == ConsensusArray::Blue)
      || (state1==ConsensusArray::Blue && state2 == ConsensusArray::Red))
      {
        return m_p_1;
      }
  else if((state1==ConsensusArray::Green && state2==ConsensusArray::Red)
      || (state1==ConsensusArray::Blue && state2 == ConsensusArray::Green)
      || (state1==ConsensusArray::Red && state2 == ConsensusArray::Blue))
      {
        return m_p_2;
      }
  else
  {
    return 0;
  }
}

int ConsensusArray::stateCount(ConsensusArray::State state) const
{
	double total = 0;
	for(const auto& cellState : m_boardData)
	{
		if(state == cellState)
		{
			total++;
		}
	}

	return total;
}

double ConsensusArray::stateFraction(ConsensusArray::State state) const
{
	double total = 0;
	for(const auto& cellState : m_boardData)
	{
		if(state == cellState)
		{
			total++;
		}
	}

	return total/(m_colCount*m_rowCount);
}



std::ostream& operator<<(std::ostream& out, const ConsensusArray &board)
{


    int maxRows = board.getRows();
    int maxCols = board.getCols();

    for(int row = 0; row < maxRows; ++row)
    {
        for(int col = 0; col < maxCols; ++ col)
        {
            out << ConsensusArray::stateSymbols[board(row,col)] << ' ';
        }

        out << '\n';
    }

    return out;
}
