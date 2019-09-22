
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
		Cell(const short n, const short m) : row(n), col(m) {}
		Cell north() {
			return Cell(row-1, col);
		};
		Cell east() {
			return Cell(row, col+1);
		};
		Cell south() {
			return Cell(row+1, col);
		};
		Cell west() {
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
		short row;
		short col;
};

class TreasureHunt {

	public:

		TreasureHunt() {}

		void read_data();

		void print_map();

		void get_options(int argc, char** argv);

		void hunt();

	private:

		char get_cell(Cell c) {
			return map[static_cast<size_t>(c.row)][static_cast<size_t>(c.col)];
		};

		char set_curr_path(char prev, char curr);

		bool is_number(const string &);

		inline bool is_land(Cell c);

		bool cell_equal(Cell c1, Cell c2) {
			return (c1.row == c2.row) && (c1.col == c2.col);
		};

		bool is_in_bound(Cell c);

		bool is_valid_cell(Cell c, bool on_land);

		bool is_ashore(Cell c, char dir);

		bool is_treasure(Cell c, char dir);

		bool add_cell(Cell c, bool on_land, char dir);

		void set_cell(Cell c, const char ch) {
			map[static_cast<size_t>(c.row)][static_cast<size_t>(c.col)] = ch;
		};

		void reset_cell(Cell c);

		void captain_do();

		void first_mate_do();

		void print_cell(Cell c);

		void calculate_path_length();

		void clean_up();

		void print_hunt_stats();

		void print_path();

		vector<vector<char>> map;

		deque<Cell> sea;

		deque<Cell> land;

		Cell treasure = Cell(-1, -1);

		Cell start = Cell(-1, -1);

		unsigned short map_size = 0;

		char order[4] = {'N', 'E', 'S', 'W'};

		unsigned short path_length = 0;

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
