# EECS 281: Project 1 - Treasure Hunt

## Introduction

It's a Pirate's Life for me, for me, it's a Pirate's Life for me!

It's time to set sail in search for buried treasure. The only problem? There's
no map! But a wily captain and hardy first mate have teamed up for a treasure
hunt. They've conscripted you to write a program to lead their search for riches
in and around a chain of islands. The Captain will pilot the ship over the
water, and the First Mate will lead the search party on land.

### A Chain of Islands

The chain of islands will be specified in a text file, and can be represented
as a 2D-grid size N x N, with N >= 2. The text file will either be a map or a
list of grid coordinates and terrains. The following ascii characters will be
used to describe terrain.

* .: Open water
* o: Land
* #: Impassable (land or water)
* @: Starting location (always water)
* $: Treasure location

The grid is a complete representation of all relevant features in the chain but
it does not include a path from the start to the treasure. Using the algorithm
described below and input from the command line and a file, you will try to
find and describe the path, if one exists. In either case, the details of the
hunt must be displayed, or you will be forced to walk the plank!

### Example Map

```
# Spec Map #1
# Two small islands with treasure in the NW corner and start in the SE corner.
M
5
$o...
oo...
#..o.
..oo.
....@
```

## Routing Scheme

The Captain and the First Mate don't always see eye-to-eye, and will sometimes
favor different methods of exploration. Most days, the Captain will navigate
"by the seat of their pants", while the First Mate, in charge of discipline on
the ship is usually more methodical.

The Captain will travel from island to island with their own log and future
plans, moving only over water locations ('.'). As soon as land is discovered
while sailing, a search party is put ashore before any other discovery is
attempted. Once put ashore to lead a search party, the First Mate will search
all available land with a separate log and plans before returning to the ship,
moving only over land locations ('o'). Any water (inland or open ocean)
encountered by the search party is completely ignored.  Neither the Captain nor
the First Mate will ever accidentally overlook treasure ('$'), and both are
unable to move over impassable terrain ('#').

All discovery happens in adjacent squares using cardinal directions only: North,
South, East, or West. New locations can be discovered by looking one square in
each of these directions, but never diagonally or at a greater distance. Each
square can only be discovered once; get these pirates stuck in an infinite loop,
and you will most surely be keelhauled.

To satisfy the moods of your "employers", you develop two routing schemes, one
that uses a stack and one that uses a queue to store newly discovered locations.
The Captain and First Mate can choose from either of these container types.
Stack-based routing tending to continue along a recent heading, eventually
covering the search area, while queue-based routing covers the search area in
ever-increasing radii.

### The Search Order

When discovering new locations use the Search Order to determine the order of
investigation. By default the search order is North, East, South, West. Any
expedition can choose another search order at the command line.

### The Hunt Algorithm

The treasure hunt will be one large hunt (over water) with zero or more smaller
hunts (over land). The Captain always begins the hunt from the start location
(`@`), which is a water location. The First Mate begins a search party hunt from
a land location that was discovered by the Captain. Use the following hunt
algorithm with the container (stack or queue) and location that suits the hunter
in charge.

1. Add the starting location to the container. 
  - For the Captain, this is '@', a water location, on the map
  - For the First Mate, this is wherever the search party was put ashore
2. If the container is empty, part of the hunt has ended. If not, set the
"current location" to the "next" available location in the container (front for
queue, back for stack) and remove it from the container. 
  - For the Captain, an empty container means the large hunt has ended in
    failure; jump to Step #6
  - For the First Mate, an empty container means a small hunt has ended and the
    search party has failed and must return to the ship; jump to Step #3 in the
    Captain's hunt
3. From the current location, add any adjacent locations that are not impassable
and have not already been discovered. Discover new locations in the order of the
Search Pattern.
  - The Captain will only add water locations. If the Captain discovers land,
    the First Mate will be immediately put ashore, before examining other
    adjacent locations, to start a search party at that location using a
    separate container; jump to Step #1. 
  - The First Mate will only add land locations. If the First Mate encounters
    water it will be ignored regardless of whether it has already been
    discovered or not.
4. If any location added to a container is the treasure ('$'), end the hunt
right away, because the existence of a path has been found; jump to Step #6
5. Repeat from Step #2
6. Report the outcome of the hunt (see Output Format)

## Command Line Interfaces (CLI)

The solution you generate will run from the command line, in the spirit of the
Unix tradition.

### Silence is Golden

Most programs run with as little output as possible. Often, programs that are
able to successfully execute will print nothing at all. Programs that generate
output will usually display the smallest possible output that registers success
or failure. By default, this solution will generate a single line of output.

### Modifying Behavior

Programs often offer *options* that can change how they work at runtime. These
are specified at the command line in the form of "short options" which are a
single character following a single hyphen (eg. -o), or "long options" which use
full words following a double hyphen (eg. --a-long-option). Multiple short
options can be combined (eg. -al is equivalent to -a -l). Both short and long
options can also accept *arguments*, with some options prohibiting, requiring,
or optionally allowing arguments.

Q: Why both short and long options?

A1: There are only 52 different single letter options, but an infinite number of
    long options can be created.

A2: Two programs can implement the same functionality using different options
    (eg. -R or -r for --recursive), but full words are more easily remembered.

A3: Short options are quick and easy to type at the command line, and when
    programs are used in scripts, long options make scripts more readable.

Like many programs, the solution to this problem will implement both short and
long options. Some options will be implemented with no arguments, and others
will be implemented with required arguments. *An option with a required argument
is still optional and might not be specified at the command line, but if it is
specified, an argument must be provided.*

The complicated task of parsing options and arguments is made easier with a
classic library `<getopt.h>` that provides both `getopt()` for handling short
options and `getopt_long()` for handling short and long options. A helpful
reference can be found at [getopt man page](https://linux.die.net/man/3/getopt).

### Using Standard Input, Standard Output, Standard Error, and Redirection

Most command line shells allow programs to send output to or read input from
files. This is accomplished by using the Input Redirect Operator (`>`) or the
Output Redirection Operator (`<`). The following example runs `program` while
reading input from `input.file` and writing output to `output.file`:

```bash
% program < input.file > output.file
```

Reading input from a file can be thought of as temporarily disconnecting the
keyboard and getting all input from the file specified. The file must exist or
an error will occur. Also, problems will arise if the data in the input file is
not saved in the exact order that it is requested by the program. This technique
is known as input redirection, and replaces keyboard input with "standard
input". Standard input can be accessed in C++ by using `std::cin`.

Writing output to a file can be thought of as temporarily disconnecting the
screen and sending everything that would be printed directly to a file on disk.
The file need not exist before redirection, it will be created as necessary. If
the file already exists, its original contents will be replaced (gone forever)
with the new output. This technique is known as output redirection and sends
text that would go to "standard output" and display on the screen, directly to
the given file. Standard output can be accessed in C++ by using `std::cout`.

Redirection can be done on input, output, both, or neither, as well as on
"standard error", which is accessed with `std::cerr` and redirected with the
Error Redirection Operator (`2>`). A good reference can be found at
[Thoughtbot](https://thoughtbot.com/blog/input-output-redirection-in-the-shell).

Often, a program will read from a file specified at the command line, or if one
is not specified, it will try to read from standard input. Programs that do
this perform identically when invoked with the following two commands:

```bash
% program < input.file   # Input redirection imitating keyboard input
% program input.file     # Data read directly from an opened file
```

To accomplish this in C++, use polymorphism and a few conditionals, and the
program can be written to handle input independently of how it is invoked. This
pseudocode shows the technique:

```c++
#include <fstream>
#include <iostream>
#include <string>


...
    ifstream infile;  // Used to read from a file if a filename is given
    string filename;  // Use getopt_long and argc + argv to get this value if it
                      // is specified at the command line, it's otherwise empty

    if (!filename.empty()) {  // The filename was specified at the command line
        if (!infile.open(filename)) {  // Open the file with a safety check
            cerr << "Unable to open input file: " << filename << endl;
            exit(1);
        }
    }
    istream &in = (infile.is_open()) ? infile : cin;  // Polymorphism & conditional

    // Do the remainder of input using the stream extraction operator and 'in'
    // If a file was specified 'in == infile'
    // If no file was specified 'in == cin'
    in >> file_type >> map_size;

    infile.close();  // Close file after use, it's good housekeeping
...
```

## Input File

The input file representing the grid to be searched will be provided in one of
two ways: a 2D ASCII map, or a list of coordinate/terrain triples. Both files
will have similar front matter:

1. Zero or more lines of comments, each of which...
  - have an octothorpe (#) in column zero
  - can contain any combination of zero or more printable ascii characters
  - end with a newline character
2. A filetype specifier, which is...
  - a single ASCII character
  - either M or L (for map or list files)
  - followed by a newline character
3. A map size value, which...
  - is a positive integer, >= 2
  - represents both the width and the height of the map
  - is followed by a newline character

Any starting location is to be considered a water square.

### Map Format

After the front matter, if the first non-comment byte is 'M', the file is in map
format. There will be N rows of N characters (plus a newline), where N is the
map size value that is on the line immediately following the filetype specifier.
Each of the N characters must be from the Terrain Legend above.

See the Example Map above for a sample of a map format file.

### List Format

After the front matter, if the first non-comment byte is 'L', the file is in
list format. Following the map size integer, there will be two or more lines
(start location and treasure location at least) that are either blank or a
coordinate/terrain triple (CTT). When reading list format files, ignore any
blank lines that occur after the front matter. A CTT is two non-negative
integers followed by a single character to represent the terrain at that
location (refer to the Terrain Legend above). These three values will be
separated by commas followed by a newline character, with no other whitespace
characters on the line.

```
<row> <col> <terrain>
```

List format files may specify a subset of all CTT in a map. If fewer than NxN
CTT are provided, missing locations are assumed to be water (.). No location
will be specified more once in a list format file, so no more than NxN locations
can be included. The order of CTT are unspecified.

An example of one list format file of the example map above follows.

### Example List

```
# Spec Map #1
# Two small islands with treasure in the NW corner and start in the SE corner.
L
5
0,0,$
4,4,@
2,0,#
0,1,o
1,0,o
1,1,o
2,2,.
2,3,o
3,2,o
3,3,o
```

## The Command Line Interface (CLI)

Discussion

### Supported Options

* --help, -h: Print a useful help message and exit
* --captain <"queue"|"stack">, -c <"queue"|"stack">: The route-finding container
used while searching in the water (default: stack)
* --first-mate <"queue"|"stack">, -f <"queue"|"stack">: The route-finding
container used while searching on land (default: queue)
* --search-order <order>, -o <order>: The order of exploration of adjacent
tiles around the current location (default: nesw)
* --verbose, -v: Print verbose output while searching
* --stats, -s: Display statistics after the search is complete
* --show-path <M|L>, -p <M|L>: Display a treasure map or the list of locations
that describe the path

## Output Format

### Search Results

After a treasure hunt, one line is printed describing the success or failure of
the hunt:

```bash
No treasure found after searching 5 locations.
```

or

```bash
Treasure found at 0,0 with path length 8.
```

where the number of searched locations, the treasure location, and the path
length are all calculated based on the input file and command line options
provided.

### Option: Verbose

If the verbose option is requested at the command line (`--verbose` or `-v`),
output additional information while the search is happening. This will always
appear first, if specified, and will consist of a start message, zero or more
ashore messages, one search party result for every trip ashore, and a failure
message if no treasure is found.

```bash
Treasure hunt started at: 4,4
Went ashore at: 3,3
Searching island... party returned with no treasure
Went ashore at: 1,1
Searching island... party found treasure at 0,0
```

or

```bash
Treasure hunt started at: 4,4
Went ashore at: 3,3
Searching island... party returned with no treasure
Treasure hunt failed
```

### Option: Stats

If the stats option is requested at the command line (`--stats` or `-s`), output
a statistical summary after the search has completed. This will appear after
verbose messages (if both are specified), and before the "Search Results."

```bash
--- STATS ---
Starting Location: 4,4
Total Sail: 8
Total Land: 6
Went Ashore: 2
Path Length: 8
Treasure Location: 0,0
--- STATS ---
```

The total lines show the number of tiles searched. If a location is added to a
container but never removed, it is *NOT* counted in its respective total. A line
displaying the number of search parties that went ashore must be included. If a
path to the treasure is found, include a line with the length of the path, and
another line with the location of the treasure. Path length doesn't count the
starting and ending locations, but rather the the steps between them. A hunt
with the start location adjacent to the treasure location will have a path
length of one.

```
# Treasure hunt with path length one
M
2
.$
.@
```

```
# Treasure hunt with path length two
M
3
.$o
.o.
.@.
```

If no path to the treasure is found, omit the last two lines of stats.

```bash
--- STATS ---
Starting Location: 4,4
Total Sail: 8
Total Land: 6
Went Ashore: 2
--- STATS ---
```

### Option: Show Path as Map or List

If the show path option is requested at the command line (`--show-path` or
`-w`), output the path discovered from the start location to the treasure. The
show path option requires an argument which will be a single upper-case
character, an `M` to display a treasure map or an `L` to display the locations
that define the path. This will appear after verbose messages and stats (if
either or both are specified), and before the "Search Results."

#### Treasure Map

A "treasure map" is the original map with a path overlaid from the start
location to the treasure. The path is drawn using vertical lines (`|`),
horizontal lines (`-`), and corners (`+`), with the start location unchanged
(`@`) and the treasure overwritten with an `X`.

```
X+...
o|...
#|.o.
.|oo.
.+--@
```

#### Coordinate List
For more on the path length versus the number of locations, and the off-by-one
error that can result from this type of problem, read
[off-by-one](https://en.wikipedia.org/wiki/Off-by-one_error#Fencepost_error).

## Error Handling and Assumptions
, if any other
characters are read in, the file should be considered unreadable and a prompt
termination with a non-zero exit code should result.

Discussion

### Possible Errors

## Submission to the Autograder

## Grading

### Simulation

### Memory Leaks

### Testing

## Hints and Advice

## Libraries and Restrictions

--------

## Appendix A: An Example

## Appendix B: Tips

## Appendix C: Test Case Legend
