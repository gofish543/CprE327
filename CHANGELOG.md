# Release Notes for 0.1.x

## Unreleased

### Added
### Fixed
- Fixed stairUpCount, stairDownCount, and roomCount to u_short from u_char
- Fixed removal of .tmp file if save successful
### Changed

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