#include <vector>
#include <deque>
#include <getopt.h>
#include <string>

using std::vector;
using std::deque;
using std::string;

struct Cell {
	int row;
	int col;
};

class TreasureHunt {

	public:

		void read_data();

		void print_map();

		void print_start();

		void get_options(int argc, char** argv);

		void hunt();

	private:

		bool is_number(const string &);

		bool explore_cell(int row, int col, bool os_land);

		vector<vector<char>> map;

		deque<Cell> path;

		int map_size = 0;

		//char order[4] = {'N', 'E', 'S', 'W'};

};
