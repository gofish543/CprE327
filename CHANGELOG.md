# Release Notes for 0.1.x

## [v0.1.1 (2019-01-25)](https://github.com/gofish543/CprE327/releases/tag/v0.1.1)

### Added
- Added Main Escape from Darkness program
- Added dungeon generation
- Added dungeon floor generation. Floor number is randomly generated based off of a minimum and max number of floors
- Added floor room generation. Room number is randomly generated based off of a a minimum and maximum number of rooms per floor
- Added path generation between rooms
- Added printing of floor
- Added generic random function using Linux /dev/urandom for the random seed. If /dev/urandom fails to open, use time() as the seed instead