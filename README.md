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
To load a previously saved game specify the file name when executing the program.
<br>
The following is an example run `./coms327_dykhuizen_0.1.0 savedGame.efd`