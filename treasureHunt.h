#include <vector>
#include <deque>
#include <getopt.h>
#include <string>

using std::vector;
using std::deque;
using std::string;

class Cell {
	public:
		Cell(const int n, const int m) : row(n), col(m) {}
		inline Cell north() {
			return Cell(row-1, col);
		};
		inline Cell east() {
			return Cell(row, col+1);
		};
		inline Cell south() {
			return Cell(row+1, col);
		};
		inline Cell west() {
			return Cell(row, col-1);
		};
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

		inline bool is_land(Cell c);

		inline bool is_in_bound(Cell c);

		bool is_valid_cell(Cell c, bool on_land);

		inline bool is_ashore(Cell c, char dir);

		inline bool is_treasure(Cell c);

		bool add_cell(Cell c, bool on_land);

		inline void set_cell(Cell c, const char ch);

		void captain_do();

		void first_mate_do();

		vector<vector<char>> map;

		deque<Cell> sea;

		deque<Cell> land;

		Cell treasure = Cell(-1, -1);

		int map_size = 0;

		char order[4] = {'N', 'E', 'S', 'W'};

		char capt_mode = 's';

		char mate_mode = 'q';

};
