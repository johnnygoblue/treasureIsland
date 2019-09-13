#include <vector>
#include <deque>
#include <getopt.h>

using std::vector;
using std::deque;

struct Cell {
	unsigned row;
	unsigned col;
};

class TreasureHunt {

	public:

		void read_data();

		void get_options(int argc, char** argv);

		void hunt();

	private:

		vector<vector<char>> map;

		deque<Cell> path;

		//char order[4] = {'N', 'E', 'S', 'W'};

};
