# CprE 327 Escape from Darkness #

Escape from Darkness is a dungeon game which the user navigates through a series of floors battling monsters, collecting gear, and growing stronger. The goal is not just to survive, but thrive.

## Building ##
Escape from Darkness uses a standard build procedure. Typically all that is necessary to build Escape from Darkness is `make`, but the following commands are a more robust way of building:

    % make directories
    % make resources
    % make build

## Usage
You are Sir Nick, a legendary hero represented by the '@' symbol. Monsters are represented by numbers and characters. And items are represented by punctuation and symbols.
<br>
To run Escape from Darkness navigate to the ./bin directory and execute the binary file. This file is named based off the current version of the application, but will always contain "coms327_dykhuizen_" as the start of the executable's name.
<br>
The following is an example run...
<br>
`./coms327_dykhuizen_0.1.0 --load mygame.rlg327 --save mygame.rlg327 --file_version 1`

## Flags
* The save flag allows the game to save the current dungeon to a specified file upon termination of the game instance.<br>If only a file name is provided, the program will attempt to use the user's home directory and hidden folder .rlg327 to save the file. If the home directory does not exist, or cannot be opened, the game will then attempt to save the file to ./bin/res.<br>`--save <file_path | file_name>`
* The load flag enables the game to load a dungeon rather than generating a fresh dungeon instance.<br>The file ordering is exactly the same as the save flag.<br>`--load <file_path | file_name>`
* The file version saves differing amounts of data.<br>=> File version 0: Save the current floor the character is on<br>=> File version 1: Save all floors<br>`--file_version <0 | 1>` 
* The expanded print flag will add extra padding and include column and row numbers when printing out dungeon information.<br>`--expanded_print`
* The number of monsters flag specifies the number of monsters per floor to generate. If the flag is not present, a random number is generated. Please note that the use of this flag with `--load` will have no affect.<br>`--nummon <number>`
* Whether the application uses ncurses or not can be toggled using the `--no_ncurses` flag. By default ncurses will be enabled.<br>`--no_ncurses`

## Monsters
* Intelligence: Intelligent monsters understand the dungeon layout and move on the shortest path (as per path finding) toward the PC. Unintelligent monsters move in a straight line toward the PC (which may trap them in a corner or force them to tunnel through the hardest rocks). Intelligent monsters also can remember the last known position of a previously seen PC.
* Telepathy: Telepathic monsters always know where the PC is. Non-telepathic monsters only know the position of the PC if they have line of sight (smart ones can remember the position where they last saw the PC when line of sight is broken). Telepathic monsters will always move toward the PC.Non-telepathic monsters will only move toward a visible PC or a remembered last position.
* Tunneling Ability: Tunneling monsters can tunnel through rock. Non-tunneling monsters cannot. These two classes will use different distance maps. Tunneling monsters when attempting to move through rock will subtract 85 from its hardness, down to a minimum of zero. When the hardness reaches zero, the rock becomes corridor and the monster immediately moves. Reducing the hardness
of rock forces a tunneling distance map recalculation. Converting rock to corridor forces tunneling and non-tunneling distance map recalculations.
* Erratic Behavior: Erratic monsters have; a 50% chance of moving as per their other characteristics, a 45% chance to move to a neighboring cell (per their tunneler abilities), and a 5% chance to teleport across the map (per tunneling abilities)

| ID | Binary | Erratic | Tunneler | Telepathic | Intelligent |
|----|--------|---------|----------|------------|-------------|
|  0 |  0000  | FALSE   | FALSE    | FALSE      | FALSE       |
|  1 |  0001  | FALSE   | FALSE    | FALSE      | TRUE        |
|  2 |  0010  | FALSE   | FALSE    | TRUE       | FALSE       |
|  3 |  0011  | FALSE   | FALSE    | TRUE       | TRUE        |
|  4 |  0100  | FALSE   | TRUE     | FALSE      | FALSE       |
|  5 |  0101  | FALSE   | TRUE     | FALSE      | TRUE        |
|  6 |  0110  | FALSE   | TRUE     | TRUE       | FALSE       |
|  7 |  0111  | FALSE   | TRUE     | TRUE       | TRUE        |
|  8 |  1000  | TRUE    | FALSE    | FALSE      | FALSE       |
|  9 |  1001  | TRUE    | FALSE    | FALSE      | TRUE        |
| 10 |  1010  | TRUE    | FALSE    | TRUE       | FALSE       |
| 11 |  1011  | TRUE    | FALSE    | TRUE       | TRUE        |
| 12 |  1100  | TRUE    | TRUE     | FALSE      | FALSE       |
| 13 |  1101  | TRUE    | TRUE     | FALSE      | TRUE        |
| 14 |  1110  | TRUE    | TRUE     | TRUE       | FALSE       |
| 15 |  1111  | TRUE    | TRUE     | TRUE       | TRUE        |

## Items
|     Type     | Symbol |
|--------------|--------|
| Weapon       |   Pipe |
| Offhand      |      ) |
| Ranged       |      } |
| Armor        |      [ |
| Helmet       |      ] |
| Cloak        |      ( |
| Gloves       |      { |
| Boots        |      \ |
| Ring         |      = |
| Amulet       |      " |
| Light        |      _ |
| Scroll       |      ` |
| Books        |      ? |
| Flask        |      ! |
| Gold         |      $ |
| Ammunition   |      / |
| Food         |      , |
| Wand         |      - |
| Container    |      % |
| Stack        |      & |


## Key Bindings
|  Primary Key  | Secondary Key |      Action        |
|---------------|---------------|--------------------|
|             7 |             y | Move Up Left       |
|             8 |             k | Move Up            |
|             9 |             u | Move Up Right      |
|             6 |             l | Move Right         | 
|             3 |             n | Move Bot Right     |
|             2 |             j | Move Down          |
|             1 |             b | Move Down Left     |
|             4 |             h | Move Left          |
|             > |               | Go Down Stairs     |
|             < |               | Go Up Stairs       |
|             5 |             . | No Action          |
|             m |               | Monster Menu       |
|             w |               | Wear Item Menu     |
|             t |               | Take Off Item Menu |
|             d |               | Drop Item Menu     |
|             x |               | Destroy Item Menu  |
|             i |               | List Inventory     |
|             e |               | List Equipment     |
|             I |               | Inspect Item       |
|             L |               | Inspect Monster    |
|            UP |               | Scroll             |
|          DOWN |               | Scroll             |
|           ESC |               | Quit Menu          |
|             Q |               | Quit Game          |
