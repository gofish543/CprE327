# Release Notes for 0.1.x

## Unreleased

### Added
- Added monster menu
- Added monster direction print function
- Added in input class for ncurses vs no ncurses input methods
### Fixed

### Changed
- Changed keys for player movement for assignment

## [v0.1.4 (2019-02-27)](https://github.com/gofish543/CprE327/releases/tag/v0.1.4)

### Added
- Added `--nummon` flag to specify number of monsters
- Added monster speed
- Added monster boolean values
- Added monster classification based on boolean generated values
- Added player speed
- Added player and monster isAlive flag
- Added player statistic data
- Added player movement with keyboard
- Added monster auto-movement with events and game ticks
- Added 3 dungeon text strings for message information
- Implemented monster versus player battle
- Implemented monster versus monster battle
- Added ncurses
- Added empty player keys
- Added custom monster movement based on characteristics
- Added `--no_ncurses flag`
- Added monster vision function
- Added shortest path from anywhere to the PC structure within floor
### Fixed
- Fixed error within make file linking libraries
- Fixed compile warnings on -O2
- Fixed memory leaks within monster generation after refactoring
### Changed
- Changed file version 1 to save monster speed and classification
- Changed file version 1 to load monster speed and classification
- Altered monster movement
- Dead monsters can no longer move
- Moved output of program to output.c with dynamic printf vs printw based on ncurses flag
- Changed monster and player movement
- Entire application refactor
    * Moved terrains to own folder
    * Moved characters to own folder
    * Moved movement to own folder
    * Refactored floor structure
    * Refactored save methods

## [v0.1.3 (2019-02-13)](https://github.com/gofish543/CprE327/releases/tag/v0.1.3)

### Added
- Added in monsters
- Added in monster placement
- Added in dijkstra for tunneling and non tunneling monsters
- Added in game ticks
- Added monsters seeking out enemy players
- Added `--expanded_print` flag to print dungeon with padding, column numbers, and row numbers
- Added tunneler monsters leaving behind corridors
### Fixed
- Fixed double free bug when monster gets placed on another monster
- Fixed fatal error where walking over ground removes the previous floor dot for player character
- Fixed stairUpCount, stairDownCount, and roomCount to u_short from u_char
- Fixed removal of .tmp file if save successful
- Attempted fixing of changelog formatting
- Fixed heap compiler warnings
### Changed
- Changed player movement from walking over dot to changing pointer to character
- Moved print functions to output.c/output.h files
- Minor refactoring of logic

## [v0.1.2 (2019-02-04)](https://github.com/gofish543/CprE327/releases/tag/v0.1.2)

### Added
- Added player placement upon dungeon load
- Added multiple stairs up and down per floor
- Added ability to save first floor of application
- Added save flag `./bin/coms327_dykhuizen_VERSION --save <file_name>`
- Added ability to load first floor of application
- Added load flag `./bin/coms327_dykhuizen_VERSION --load <file_name>`
- Added file version 0, saving only the current floor the user is on
- Added file version 1, saving all the floors, including the floor the user is on
- Added file version default 0, override flag is `--file_version <version>`
- Added error checking for file load and save
- Added in production compilation inside of makefile
### Fixed
- Fixed file size mismatch fault dealing with endianness conversion
- Fixed warnings given by -O2 on makefile
- Allow for variable hardness levels
- Fixed player placement on staircase when loading file
### Changed
- Changed fixed array length fto dynamically sized array for floors
- Changed fixed array length to dynamically sized array for rooms
- Moved settings under the dungeon structure with error checking on passed params

## [v0.1.1 (2019-01-25)](https://github.com/gofish543/CprE327/releases/tag/v0.1.1)

### Added
- Added Main Escape from Darkness program
- Added dungeon generation
- Added dungeon floor generation. Floor number is randomly generated based off of a minimum and max number of floors
- Added floor room generation. Room number is randomly generated based off of a a minimum and maximum number of rooms per floor
- Added path generation between rooms
- Added printing of floor
- Added generic random function using Linux /dev/urandom for the random seed. If /dev/urandom fails to open, use time() as the seed instead