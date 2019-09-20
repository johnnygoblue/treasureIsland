
// IDENTIFIER = 40FB54C86566B9DDEAB902CC80E8CE85C1C62AAD

#include <iostream>
#include <vector>
#include <deque>
#include <getopt.h>
#include <string.h>

using std::vector;
using std::deque;
using std::string;
using std::cout;
using std::cerr;
using std::endl;

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
		// Print cell in row,col format in single line
		inline void output() {
			cout << row << "," << col << "\n";
		}
		// Get the previous cell
		Cell prev(char ch) {
			Cell ret = Cell(-1, -1);
			switch(ch) {
				case 'N': case 'n':
					ret = south();
					break;
				case 'E': case 'e':
					ret = west();
					break;
				case 'S': case 's':
					ret = north();
					break;
				case 'W': case 'w':
					ret = east();
					break;
				default:
					cerr << "Unrecognized dir '" << ch << "' in " <<
						__func__ << endl;
			} // switch
			return ret;
		}
		int row;
		int col;
};

class TreasureHunt {

	public:

		TreasureHunt() {}

		void read_data();

		void print_map();

		void print_sea();

		void print_land();

		void print_start();

		void get_options(int argc, char** argv);

		void hunt();

	private:

		inline char get_cell(Cell c);

		char set_curr_path(char prev, char curr);

		bool is_number(const string &);

		inline bool is_land(Cell c);

		inline bool cell_equal(Cell c1, Cell c2);

		inline bool is_in_bound(Cell c);

		bool is_valid_cell(Cell c, bool on_land);

		inline bool is_ashore(Cell c, char dir);

		inline bool is_treasure(Cell c, char dir);

		bool add_cell(Cell c, bool on_land, char dir);

		inline void set_cell(Cell c, const char ch);

		void reset_cell(Cell c);

		void captain_do();

		void first_mate_do();

		void print_cell(Cell c);

		void print_current_cell(Cell c, bool on_land);

		void calculate_path_length();

		void clean_up();

		void print_hunt_stats();

		void print_path();

		vector<vector<char>> map;

		deque<Cell> sea;

		deque<Cell> land;

		Cell treasure = Cell(-1, -1);

		Cell start = Cell(-1, -1);

		int map_size = 0;

		char order[4] = {'N', 'E', 'S', 'W'};

		unsigned short path_length;

		unsigned short water_loc = 0;

		unsigned short land_loc = 0;

		unsigned short ashore = 0;

		char capt_mode = 's';

		char mate_mode = 'q';

		char show_path = 'f'; // false by default

		bool print_verbose = false;

		bool print_stats = false;

		bool treasure_found = false;

};
