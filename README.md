# Immigration
An SDL2 application that shows a field of Immigration (Conway's Game of Life for two players)

## Usage
The executable takes a number of command line options:
```
    -f field            : file to read starting field from
    -w width -h height  : dimentions for new starting field (ignored if field passed)
    -a                  : autosave - if passed the game will be saved every iteration
```
If the same option is passed multiple times, only the last is counted.

You can load a sample file like this: `./game -f "sample saves/test.txt"`
Options can be chained as follows: `./game -wha 30 20`

## Controls
    - RIGHTCLICK to place a Cell in team B
    - Pressing twice will reset to empty Cell
    - press SPACE to advance the game by one step
    - hold P to continually advance the game
    - press S to export the current state of the game to a savefile (use -a option to do this continually)