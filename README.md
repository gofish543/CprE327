# CprE 327 Escape from Darkness #

Escape from Darkness is a dungeon game which the user navigates through a series of floors battling monsters, collecting gear, and growing stronger. The goal is not just to survive, but thrive.

## Building ##
Escape from Darkness uses a standard build procedure. Typically all that is necessary to build Escape from Darkness is `make`, but the following commands are a more robust way of building:

    % make directories
    % make resources
    % make build

## Usage
To run Escape from Darkness navigate to the ./bin directory and execute the binary file. This file is named based off the current version of the application, but will always contain "coms327_dykhuizen_" as the start of the executable's name.
<br>
The following is an example run...
<br>
`./coms327_dykhuizen_0.1.0 --load mygame.rlg327 --save mygame.rlg327 --file_version 1`

## Flags
* The save flag allows the game to save the current dungeon to a specified file upon termination of the game instance.<br><br>If only a file name is provided, the program will attempt to use the user's home directory and hidden folder .rlg327 to save the file. If the home directory does not exist, or cannot be opened, the game will then attempt to save the file to ./bin/res.<br>`--save <file_path | file_name>`
* The load flag enables the game to load a dungeon rather than generating a fresh dungeon instance.<br><br>The file ordering is exactly the same as the save flag.<br>`--load <file_path | file_name>`
* File Version saved differing amounts of data.<br>=> File version 0: Save the current floor the character is on<br>=> File version 1: Save all floors<br>`--file_version <0 | 1>` 