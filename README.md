# EECS 281: Project 1 - Treasure Hunt

## Introduction

Description

### Sample map

```
# Spec Map #1
M
5
$o...
oo...
#..o.
..oo.
....@
```

### Valid Map Symbols

* .: Open water
* o: Land
* #: Impassable (land or water)
* @: Starting location
* $: Treasure location

## Routing Scheme

Discussion

### Algorithm

1. a
2. b

## Input File

### Map Format

### List Format

## Solution Output

Discussion

### Option: Verbose

### Option: Stats

### Option: Output Map or List

#### Treasure Map

#### Coordinate List

## The Command Line Interface (CLI)

Discussion

### Supported Options

* --captain <"queue"|"stack">, -c <"queue"|"stack">: The route-finding container
used while searching in the water. Default: stack
* --first-mate <"queue"|"stack">, -f <"queue"|"stack">: The route-finding
container used while searching on land. Default: queue
* --help, -h: Print a useful help message and exit.
* --output <M|L>, -o <M|L>: Display a treasure map or list of locations along
the route.
* --search-pattern <order>, -p <order>: The order of exploration around the
current location. Default: nesw
* --stats, -s: Display statistics after search is complete.
* --verbose, -v: Print verbose output while searching.

## Error Handling and Assumptions

Discussion

### Possible Errors




## Libraries and Restrictions
