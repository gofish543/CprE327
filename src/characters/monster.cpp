#include "monster.h"

Monster::Monster(Floor* floor, u_char x, u_char y, std::string* name, std::string* description, u_short color, u_char speed, u_short abilities, u_int hitPoints, Dice* attackDamage, u_char symbol, u_char rarity)
        : Character(floor, x, y, symbol, speed, false, true) {
    this->name.assign(*name);
    this->description.assign(*description);
    this->color = color;
    this->abilities = abilities;
    this->hitPoints = hitPoints;
    this->attackDamage = attackDamage;
    this->rarity = rarity;

    this->playerLastSpottedX = 0;
    this->playerLastSpottedY = 0;
}

Monster::~Monster() {
    delete (this->attackDamage);
}

int Monster::NextEventTick(Event* event) {
    auto monster = (Monster*) event->character;
    if (monster->isAlive()) {
        return monster->getFloor()->getDungeon()->getEventManager()->getTick() + (CHARACTER_SPEED_NUMERATOR / monster->getSpeed());
    } else {
        return -1;
    }
}

int Monster::HandleEvent(Event* event) {
    auto monster = (Monster*) event->character;

    if (monster->isAlive()) {
        Floor* floor = monster->getFloor();

        void (* monsterMovement[])(Monster*, u_char*, u_char*) = {
                Monster::Move0, Monster::Move1,
                Monster::Move2, Monster::Move3,
                Monster::Move4, Monster::Move5,
                Monster::Move6, Monster::Move7,
                Monster::Move8
        };

        u_char x = monster->getX();
        u_char y = monster->getY();

        monsterMovement[std::min(monster->getMovementValue(), u_char(8))](monster, &x, &y);

        // Now a few things could happen
        // 0) The monster just moved on themselves
        // 1) The monster just fell on the player
        // 2) The monster just fell on another monster
        // 3) The monster just fell on open land
        if (x == monster->getX() && y == monster->getY()) { // 0) The monster just moved onto themselves
            return 0;
        } else if (floor->getCharacterAt(x, y) != null) {// 1-2) The monster moved onto someone else
            if (floor->getCharacterAt(x, y)->isPlayer()) { // 1) The monster fell on a player
                auto player = (Player*) floor->getCharacterAt(x, y);

                monster->battlePlayer(player);

                if (!monster->isAlive()) { // The player survived the battle
                    // Where our glorious monster was now become a corridor if they are a tunneler and were on rock
                    if (monster->isTunneler() && floor->getTerrainAt(monster->getX(), monster->getY())->isRock()) {
                        delete (floor->getTerrainAt(monster->getX(), monster->getY()));
                        floor->setTerrainAt(new Corridor(floor, monster->getX(), monster->getY()), monster->getX(), monster->getY());
                    }
                }

                return 0;
            } else if (floor->getCharacterAt(x, y)->isMonster()) { // The monster fell on another monster
                // Ignore
                return 0;
            } else {
                // Unknown case encountered, exit
                return 0;
            }
        }

        // Regardless, if made down to here, move the monster there
        monster->moveTo(x, y);
    }

    return 0;
}

u_int Monster::AliveCount(Dungeon* dungeon) {
    u_char index;
    u_char monsterIndex;
    u_int monstersAlive = 0;
    for (index = 0; index < dungeon->getFloorCount(); index++) {
        for (monsterIndex = 0; monsterIndex < dungeon->getFloor(index)->getMonsterCount(); monsterIndex++) {
            if (dungeon->getFloor(index)->getMonster(monsterIndex)->isAlive()) {
                monstersAlive++;
            }
        }
    }

    return monstersAlive;
}

int Monster::DijkstraCompare(const void* A, const void* B) {
    auto monsterA = (MonsterCost*) A;
    auto monsterB = (MonsterCost*) B;

    return monsterA->getCost() - monsterB->getCost();
}

int Monster::RunDijkstraOnFloor(Floor* floor) {
    return Monster::RunDijkstra(floor, MONSTER_DIJKSTRA_TYPE_TUNNELER, floor->tunnelerView) ||
           Monster::RunDijkstra(floor, MONSTER_DIJKSTRA_TYPE_CHEAPEST_PATH, floor->cheapestPathToPlayer) ||
           Monster::RunDijkstra(floor, MONSTER_DIJKSTRA_TYPE_NON_TUNNELER, floor->nonTunnelerView);
}

int Monster::RunDijkstra(Floor* floor, char type, u_char costChart[DUNGEON_FLOOR_HEIGHT][DUNGEON_FLOOR_WIDTH]) {
    switch (type) {
        case MONSTER_DIJKSTRA_TYPE_TUNNELER:
        case MONSTER_DIJKSTRA_TYPE_NON_TUNNELER:
        case MONSTER_DIJKSTRA_TYPE_CHEAPEST_PATH:
            break;
        default:
            throw Exception::DijkstraKeyInvalid();
    }

    MonsterCost* monsterCost[DUNGEON_FLOOR_HEIGHT][DUNGEON_FLOOR_WIDTH];
    heap_t heap;
    u_char width;
    u_char height;

    u_char playerX = floor->getDungeon()->getPlayer()->getX();
    u_char playerY = floor->getDungeon()->getPlayer()->getY();

    heap_init(&heap, Monster::DijkstraCompare, null);

    for (height = 0; height < DUNGEON_FLOOR_HEIGHT; height++) {
        for (width = 0; width < DUNGEON_FLOOR_WIDTH; width++) {
            monsterCost[height][width] = new MonsterCost(floor, width, height, 1);

            // Add in hardness calculation for non zero
            if (type != 0) {
                monsterCost[height][width]->addCost(u_char(floor->getTerrainAt(width, height)->getHardness() / MONSTER_HARDNESS_PER_TURN));
            }

            costChart[height][width] = U_CHAR_MAX;
        }
    }

    monsterCost[playerY][playerX]->setCost(0);
    heap_insert(&heap, monsterCost[playerY][playerX]);

    MonsterCost* item;
    while ((item = (MonsterCost*) heap_remove_min(&heap))) {
        if (!floor->getTerrainAt(item->getX(), item->getY())->isImmutable()) {
            switch (type) {
                case MONSTER_DIJKSTRA_TYPE_TUNNELER: // Type == 1
                    for (height = item->getY() - 1; height <= item->getY() + 1; height++) {
                        for (width = item->getX() - 1; width <= item->getX() + 1; width++) {
                            if (height != item->getY() || width != item->getX()) {
                                if (costChart[height][width] >= item->getCost() + monsterCost[height][width]->getCost()) {
                                    if (!floor->getTerrainAt(width, height)->isBorder()) {
                                        monsterCost[height][width]->addCost(item->getCost());
                                        costChart[height][width] = monsterCost[height][width]->getCost();

                                        heap_insert(&heap, monsterCost[height][width]);
                                    }
                                }
                            }
                        }
                    }
                    break;
                default:
                case MONSTER_DIJKSTRA_TYPE_CHEAPEST_PATH: // Type == 0
                    for (height = item->getY() - 1; height <= item->getY() + 1; height++) {
                        for (width = item->getX() - 1; width <= item->getX() + 1; width++) {
                            if (height != item->getY() || width != item->getX()) {
                                if (costChart[height][width] >= item->getCost() + monsterCost[height][width]->getCost()) {
                                    monsterCost[height][width]->addCost(item->getCost());
                                    costChart[height][width] = monsterCost[height][width]->getCost();

                                    heap_insert(&heap, monsterCost[height][width]);
                                }
                            }
                        }
                    }
                    break;
                case MONSTER_DIJKSTRA_TYPE_NON_TUNNELER: // Type == -1
                    if (floor->getTerrainAt(item->getX(), item->getY())->isWalkable()) {
                        for (height = item->getY() - 1; height <= item->getY() + 1; height++) {
                            for (width = item->getX() - 1; width <= item->getX() + 1; width++) {
                                if (height != item->getY() || width != item->getX()) {
                                    if (costChart[height][width] >= item->getCost() + monsterCost[height][width]->getCost()) {
                                        if (floor->getTerrainAt(width, height)->isWalkable()) {
                                            monsterCost[height][width]->addCost(item->getCost());
                                            costChart[height][width] = monsterCost[height][width]->getCost();

                                            heap_insert(&heap, monsterCost[height][width]);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    break;
            }
        }
    }
    costChart[playerY][playerX] = U_CHAR_MIN;

    for (height = 0; height < DUNGEON_FLOOR_HEIGHT; height++) {
        for (width = 0; width < DUNGEON_FLOOR_WIDTH; width++) {
            delete (monsterCost[height][width]);
        }
    }

    return 0;
}

int Monster::moveTo(u_char toX, u_char toY) {
    Floor* floor = this->getFloor();
    Terrain* toTerrain = floor->getTerrainAt(toX, toY);

    // Can only move if hardness is low enough
    if (toTerrain->getHardness() < MONSTER_HARDNESS_PER_TURN) {
        // Tunneler monsters leave corridors behind if tunneling and on rock
        if (this->isTunneler() && floor->getTerrainAt(this->getX(), this->getY())->isRock()) {
            delete (floor->getTerrainAt(this->getX(), this->getY()));
            floor->setTerrainAt(new Corridor(floor, this->getX(), this->getY()), this->getX(), this->getY());
        }

        // Can only move if hardness is low enough
        floor->setCharacterAt(null, this->getX(), this->getY());

        this->setX(toX)->setY(toY);

        floor->setCharacterAt(this, this->getX(), this->getY());
    } else {
        toTerrain->setHardness(toTerrain->getHardness() - MONSTER_HARDNESS_PER_TURN);
    }

    return 0;
}

void Monster::battlePlayer(Player* player) {
    // Roll the monster damage
    u_int monsterDamage = this->getAttackDamage()->roll();

    if (player->getHealth() > monsterDamage) {
        // Player lived, subtract health
        player->setHealth(player->getHealth() - monsterDamage);

        std::string message = "%s hit you for %d";
        this->floor->getDungeon()->prependText(&message, this->getName().c_str(), monsterDamage);
    } else {
        // Player died
        player->killCharacter();

        // Remove player corps
        player->getFloor()->setCharacterAt(null, this->getX(), this->getY());

        std::string message = "%s killed you";
        this->floor->getDungeon()->prependText(&message, this->getName().c_str());
    }
}

char* Monster::locationString(char location[19]) {
    Player* player = this->getFloor()->getDungeon()->getPlayer();

    char deltaX = player->getX() - this->getX();
    char deltaY = player->getY() - this->getY();

    char north[] = "NORTH";
    char south[] = "SOUTH";
    char east[] = "EAST";
    char west[] = "WEST";

    char deltaYString[10] = "";
    char deltaXString[9] = "";

    u_char index;
    for (index = 0; index < 19; index++) {
        location[index] = '\0';
    }

    if (deltaY != 0) {
        sprintf(deltaYString, "%2d %5s", abs(deltaY), (deltaY > 0 ? south : north));
    }
    if (deltaX != 0) {
        sprintf(deltaXString, "%2d %4s", abs(deltaX), (deltaX > 0 ? east : west));
    }

    sprintf(location, "%8s | %7s", deltaYString, deltaXString);
    location[18] = '\0';

    return location;
}

bool Monster::isIntelligent() {
    return bool(this->abilities & MONSTER_INTELLIGENT);
}

bool Monster::isTelepathic() {
    return bool(this->abilities & MONSTER_TELEPATHIC);
}

bool Monster::isTunneler() {
    return bool(this->abilities & MONSTER_TUNNELER);
}

bool Monster::isErratic() {
    return bool(this->abilities & MONSTER_ERRATIC);
}

bool Monster::isPass() {
    return bool(this->abilities & MONSTER_PASS);
}

bool Monster::isPickUp() {
    return bool(this->abilities & MONSTER_PICKUP);
}

bool Monster::isDestroy() {
    return bool(this->abilities & MONSTER_DESTROY);
}

bool Monster::isUnique() {
    return bool(this->abilities & MONSTER_UNIQ);
}

bool Monster::isBoss() {
    return bool(this->abilities & MONSTER_BOSS);
}

u_char Monster::getMovementValue() {
    return this->abilities & MONSTER_MOVEMENT_MASK;
}

u_char Monster::getNonErraticMovementValue() {
    return this->abilities & MONSTER_NON_ERRATIC_MOVEMENT_MASK;
}

/** GETTERS **/
std::string Monster::getName() {
    return this->name;
}

std::string Monster::getDescription() {
    return this->description;
}

u_int Monster::getColor() {
    return this->color;
}

u_int Monster::getHitPoints() {
    return this->hitPoints;
}

Dice* Monster::getAttackDamage() {
    return this->attackDamage;
}

u_int Monster::getAbility() {
    return this->abilities;
}

u_char Monster::getRarity() {
    return this->rarity;
}

u_char Monster::getPlayerLastSpottedX() {
    return this->playerLastSpottedX;
}

u_char Monster::getPlayerLastSpottedY() {
    return this->playerLastSpottedY;
}

Dice* Monster::getDamageDice() {
    return this->attackDamage;
}
/** GETTERS **/

/** SETTERS **/
Monster* Monster::setName(std::string* name) {
    this->name = *name;

    return this;
}

Monster* Monster::setDescription(std::string* description) {
    this->description = *description;

    return this;
}

Monster* Monster::setColor(u_short color) {
    this->color = color;

    return this;
}

Monster* Monster::setHitPoints(u_int hitPoints) {
    this->hitPoints = hitPoints;

    return this;
}

Monster* Monster::setAttackDamage(Dice* attackDamage) {
    this->attackDamage = attackDamage;

    return this;
}

Monster* Monster::setAbilities(u_char abilities) {
    this->abilities = abilities;

    return this;
}

Monster* Monster::setRarity(u_char rarity) {
    this->rarity = rarity;

    return this;
}

Monster* Monster::setPlayerLastSpottedX(u_char playerLastSpottedX) {
    this->playerLastSpottedX = playerLastSpottedX;

    return this;
}

Monster* Monster::setPlayerLastSpottedY(u_char playerLastSpottedY) {
    this->playerLastSpottedY = playerLastSpottedY;

    return this;
}
/** SETTERS **/

/**-------------------- MONSTER COST ----------------------**/
MonsterCost::MonsterCost(Floor* floor, u_char x, u_char y, u_char cost) {
    this->floor = floor;
    this->x = x;
    this->y = y;
    this->cost = cost;
}

MonsterCost::~MonsterCost() = default;

MonsterCost* MonsterCost::addCost(u_char cost) {
    this->cost += cost;

    return this;
}

MonsterCost* MonsterCost::removeCost(u_char cost) {
    if (this->cost - cost < 0) {
        this->cost = 0;
    } else {
        this->cost -= cost;
    }

    return this;
}

/** GETTERS **/
Floor* MonsterCost::getFloor() {
    return this->floor;
}

u_char MonsterCost::getX() {
    return this->x;
}

u_char MonsterCost::getY() {
    return this->y;
}

u_char MonsterCost::getCost() {
    return this->cost;
}
/** GETTERS **/

/** SETTERS **/
MonsterCost* MonsterCost::setX(u_char x) {
    this->x = x;

    return this;
}

MonsterCost* MonsterCost::setY(u_char y) {
    this->y = y;

    return this;
}

MonsterCost* MonsterCost::setCost(u_char cost) {
    this->cost = cost;

    return this;
}
/** SETTERS **/
