#include "escape_from_darkness.h"

int main(int argc, char* argv[]) {
    auto dungeon = new Dungeon(argc, argv);

    set_global_ncurses(dungeon->getSettings()->doNCursesPrint());

//    output(dungeon, output_print_current_floor);
//    std::cout << Monster::MonstersAlive(dungeon) << std::endl;
//    debug_terminate();
    while (dungeon->getPlayer()->getIsAlive() && Monster::MonstersAlive(dungeon) && !dungeon->getPlayer()->getRequestTerminate() && dungeon->getEventManager()->getQueue()->size > 0) {
        if (game_tick(dungeon)) {
            print_error(dungeon->getWindow(), dungeon->getSettings()->doNCursesPrint(), "Game tick error encountered, exiting while loop\n");
            break;
        }
    }
    output(dungeon, output_print_endgame);

    delete (dungeon);

    return 0;
}

int game_tick(Dungeon* dungeon) {
    if (dungeon->getEventManager()->peekNext()->getType() == event_type_player) {
        output(dungeon, output_print_current_floor);
    }

    dungeon->getEventManager()->handleNext();

    dungeon->getPlayer()->incrementDaysSurvived();

    if (dungeon->getPlayer()->getTakingStaircase() != null) {
        dungeon->getPlayer()->getTakingStaircase()->take();
        dungeon->getPlayer()->setTakingStaircase(null);
    }

    return 0;
}