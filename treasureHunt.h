#include <vector>
#include <deque>
#include <getopt.h>
#include <string>

using std::vector;
using std::deque;
using std::string;

struct Cell {
	unsigned row;
	unsigned col;
};

class TreasureHunt {

	public:

		void read_data();

		void print_map();

		void get_options(int argc, char** argv);

		void hunt();

	private:

		bool is_number(const string &);

		vector<vector<char>> map;

		deque<Cell> path;


		unsigned map_size = 0;

		//char order[4] = {'N', 'E', 'S', 'W'};

};
