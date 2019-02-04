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
* Save tells the program to save the current game at the end of execution to a give file path or file name. If a file name is provided, the program will attempt to use the user's home directory. If the home directory does not exist, the game's ./bin/res will be used instead.
<br>
`--save <file_path | file_name>`
* Load functions exactly the same as save, but instead will load the file into the application. If the specified load file does not match file_version, loading will fail and the program will terminate
<br>
`--load <file_path | file_name>`
* File Version saved differing amounts of data. File verson 0 will save the current floor the character is on, and no other data. File version 1 will save all floors and all data on each floor
<br>
`--file_version <0 | 1>` 