#include <iostream>     // std::cout
#include <string>       // std::string
#include <vector>       // std::vector
#include <cctype>       // std::tolower
#include <algorithm>    // std::transform
#include <array>        // std::array
#include <random>       // random numbers
#include <sstream>      // stringstream
#include "room.h"

// Function definitions
std::string get_input();
std::vector<std::string> split_input( std::string );
void init_map( std::vector<Room> &map, int &starting_room, int &current_room );
Room init_room(bool, bool, bool);
void connect_rooms( std::vector<Room> &map );
void generate_warnings( std::vector<Room> &map );
int activate_bat_trap();
bool check_hazard( std::vector<Room> &map, int current_room );
int handle_hazard( std::vector<Room> &map, int &current_room );
std::string parse_input( std::string input );
void describe_room( std::vector<Room> &map, int current_room );
void get_command( std::vector<Room> &map, int &current_room, bool &run_game, bool &game_won );
bool handle_player_death( int &current_room, int starting_room, bool &player_dead, bool &generate_map, bool &run_game, bool &game_won );
bool check_hit(Room room);

// Main code

int main() {
    bool generate_map  = true;
    bool player_dead   = false;
    bool run_game      = true;
    bool game_won      = false;
    int current_room;
    int starting_room  = 1;
    std::vector<Room> map;

    while (run_game) {
        if ( generate_map ) {
            map.clear();
            init_map( map, starting_room, current_room );
            generate_map = false;
        }

        if ( player_dead ) {
            if ( handle_player_death( current_room, starting_room, player_dead, generate_map, run_game, game_won ) ) {
                continue;
            }
            else {
                break;
            }
        }

        if ( game_won ) {
            if ( handle_player_death( current_room, starting_room, player_dead, generate_map, run_game, game_won ) ) {
                continue;
            }
            else {
                break;
            }
        }

        if ( check_hazard( map, current_room ) ) {
            // TODO: Fix this making an infinite loop
            int event_happened = handle_hazard( map, current_room );
            if ( event_happened == 2 ) {
                player_dead = true;

                continue; // Make sure the user can restart
            }
            else if ( event_happened == 1 ) {
                // Restart the loop so we can check for hazards again
                // If we don't and the user enters data wrong, they can unexpectedly die
                continue;
            }
        }

        describe_room( map, current_room );
        get_command( map, current_room, run_game, game_won );
    }

    return 0;
}

std::string get_input() {
    std::string input;
    getline(std::cin, input);

    return input;
}

std::string parse_input( std::string input ) {
    std::vector<std::string> words = split_input( input );
    std::string command = words[0];
    std::transform(command.begin(), command.end(), command.begin(), [](unsigned char c){ return std::tolower(c); }); // Converts to lowercase

    return command;
}

std::vector<std::string> split_input( std::string input ) {
    std::string delimiter = " ";

    std::vector<std::string> words;
    size_t pos = 0;

    while ( (pos = input.find(delimiter)) != std::string::npos) {
        std::string token = input.substr(0, pos);
        words.push_back(token);
        input.erase(0, pos + delimiter.length());
    }
    if ( !input.empty() ) {
        words.push_back(input); // Gets the last word typed in
    }

    return words;
}

void init_map( std::vector<Room> &map, int &starting_room, int &current_room ) {
    // Map consists of 20 rooms
    // Each room has 3 connecting rooms
    // Starting room will vary

    int room_with_wumpus;
    std::random_device rd; // Obtain a random number from hardware
    std::mt19937 eng(rd()); // Seed the generator
    std::uniform_int_distribution<> dist(1, 20); // Define the range
    starting_room    = dist(eng);
    room_with_wumpus = dist(eng);
    current_room     = starting_room;
    std::cerr << "Wumpus is in room: " << room_with_wumpus << std::endl;

    if ( starting_room == room_with_wumpus ) {
        room_with_wumpus = dist(eng);
    }

    Room *fake = new Room;
    map.push_back(*fake);
    for ( int i = 1; i <= 20; i++ ) {
        std::uniform_int_distribution<> distr(0, 2);
        int hazard = distr(eng);

        Room *room = new Room;
        if ( i == room_with_wumpus ) {
            // Don't put the wumpus in more than one room
            *room = init_room(0, 0, 1);
        }
        else {
            if ( starting_room == i ) {
                hazard = 0;
            }
            switch (hazard) {
                case 0:
                    *room = init_room(0, 0, 0);
                    break;
                case 1:
                    *room = init_room(1, 0, 0);
                    break;
                case 2:
                    *room = init_room(0, 1, 0);
                    break;
                default:
                    break;
            }
        }
        room->set_number(i);
        map.push_back(*room);
    }

    connect_rooms( map );
    generate_warnings( map );
}

Room init_room( bool bat, bool pit, bool wumpus ) {
    Room room;
    room.set_bat(bat);
    room.set_pit(pit);
    room.set_wumpus(wumpus);

    return room;
}

void connect_rooms( std::vector<Room> &map ) {
    for ( int i = 1; i <= 20; i++ ) {
        switch (i) {
            case 1:
                map.at(i).set_rooms( map.at(2), map.at(5), map.at(10) );
                break;
            case 2:
                map.at(i).set_rooms( map.at(1), map.at(3), map.at(4) );
                break;
            case 3:
                map.at(i).set_rooms( map.at(2), map.at(6), map.at(7) );
                break;
            case 4:
                map.at(i).set_rooms( map.at(2), map.at(8), map.at(9) );
                break;
            case 5:
                map.at(i).set_rooms( map.at(1), map.at(6), map.at(19) );
                break;
            case 6:
                map.at(i).set_rooms( map.at(3), map.at(5), map.at(13) );
                break;
            case 7:
                map.at(i).set_rooms( map.at(3), map.at(8), map.at(11) );
                break;
            case 8:
                map.at(i).set_rooms( map.at(4), map.at(7), map.at(12) );
                break;
            case 9:
                map.at(i).set_rooms( map.at(4), map.at(10), map.at(14) );
                break;
            case 10:
                map.at(i).set_rooms( map.at(1), map.at(9), map.at(20) );
                break;
            case 11:
                map.at(i).set_rooms( map.at(7), map.at(13), map.at(15) );
                break;
            case 12:
                map.at(i).set_rooms( map.at(8), map.at(14), map.at(15) );
                break;
            case 13:
                map.at(i).set_rooms( map.at(6), map.at(11), map.at(17) );
                break;
            case 14:
                map.at(i).set_rooms( map.at(9), map.at(12), map.at(18) );
                break;
            case 15:
                map.at(i).set_rooms( map.at(11), map.at(12), map.at(16) );
                break;
            case 16:
                map.at(i).set_rooms( map.at(15), map.at(17), map.at(18) );
                break;
            case 17:
                map.at(i).set_rooms( map.at(13), map.at(16), map.at(19) );
                break;
            case 18:
                map.at(i).set_rooms( map.at(14), map.at(16), map.at(20) );
                break;
            case 19:
                map.at(i).set_rooms( map.at(5), map.at(17), map.at(20) );
                break;
            case 20:
                map.at(i).set_rooms( map.at(10), map.at(18), map.at(19) );
                break;

            default:
                break;
        }
    }
    for ( int i = 1; i <= 20; i++ ) {
        map.at(i).set_connected_numbers();
    }
}

void generate_warnings( std::vector<Room> &map ) {
    for ( int i = 1; i <= 20; i++ ) {
        map.at(i).set_warnings( map.at(i).get_rooms() );
    }
}

int activate_bat_trap() {

    std::cout << "Bats carried you away!" << std::endl;

    std::random_device rd; // Obtain a random number from hardware
    std::mt19937 eng(rd()); // Seed the generator
    std::uniform_int_distribution<> dist(1, 20); // Define the range

    return dist(eng);
}

bool check_hazard( std::vector<Room> &map, int current_room ) {
    return map.at(current_room).is_hazardous();
}

int handle_hazard( std::vector<Room> &map, int &current_room ) {
    // Returns 2 if the player dies
    // Returns 1 if the player has something happen, but lives
    // Returns 0 if nothing happens

    if ( map.at(current_room).get_wumpus() ) {
        // Kill the player
        std::cout << "You have been eaten by a wumpus!" << std::endl;
        std::cout << "You are dead!" << std::endl;
        return 2;
    }
    if ( map.at(current_room).get_bat() ) {
        // Carry the player away
        current_room = activate_bat_trap();
        return 1;
    }
    if ( map.at(current_room).get_pit() ) {
        // Kill the player
        std::cout << "You fell down a pit!" << std::endl;
        std::cout << "You are dead!" << std::endl;
        return 2;
    }

    return 0; // Default
}

void describe_room( std::vector<Room> &map, int current_room ) {
    std::cout << "You are in room " << current_room << std::endl;
    std::cout << "Tunnels lead to " << map.at(current_room).get_rooms().at(0).get_number() << "," << map.at(current_room).get_rooms().at(1).get_number() << "," << map.at(current_room).get_rooms().at(2).get_number() << "." << std::endl;
    if ( map.at(current_room).is_hazardous() ) {
        std::vector<std::string> warnings = map.at(current_room).get_warnings();
        for ( std::vector<std::string>::iterator it = warnings.begin(); it != warnings.end(); ++it ) {
            std::cout << "\t" << *it;
            std::cout << std::endl;
        }
    }
}

void get_command( std::vector<Room> &map, int &current_room, bool &run_game, bool &game_won ) {

    std::cout << "Move or shoot? ";
    std::string command = parse_input( get_input() );

    if ( !command.compare("move") ) {
        std::cout << "\tTo which room? ";
        std::string room_to_move_to;
        int move_room;

        try {
            room_to_move_to = get_input();
            //move_room = 0;
            std::stringstream mroom(room_to_move_to);
            mroom >> move_room;
            if ( move_room == map.at(current_room).get_rooms().at(0).get_number() ) {
                current_room = move_room;
            }
            else if ( move_room == map.at(current_room).get_rooms().at(1).get_number() ) {
                current_room = move_room;
            }
            else if ( move_room == map.at(current_room).get_rooms().at(2).get_number() ) {
                current_room = move_room;
            }
            else {
                throw "Wrong room";
            }
        }
        catch (...) {
            std::cout << "That room isn't connected to the one you're in." << std::endl;
        }
    }
    else if ( !command.compare("shoot") ) {
        int distance = 0;
        int shoot_room = 0;
        int shoot_direction = -1; // For now, direction is the index the room was found at; 1, 2, or 3

        try {
            std::cout << "Number of rooms (1-5)? ";
            std::string num_rooms = get_input();
            std::stringstream nrange(num_rooms);
            nrange >> distance;

            if ( distance < 1 || distance > 5 ) {
                throw 1;
            }
            else {
                std::cout << "\tShoot towards which room? ";
                std::string room_num = get_input();
                std::stringstream sroom(room_num);
                sroom >> shoot_room;
                bool shoot_room_found = false;
                for ( int i = 0; i < 3; i++ ) {
                    if ( shoot_room == map.at(current_room).get_rooms().at(i).get_number() ) {
                        // Handle shooting detection
                        std::cout << "You shoot towards room " << shoot_room << " and the arrow flies for " << distance << " rooms." << std::endl;
                        shoot_room_found = true;
                        shoot_direction = i;
                        break;
                    }
                }
                if ( !shoot_room_found ) {
                    throw 2;
                }
            }
        }
        catch (int e) {
            if ( e == 1 ) {
                std::cout << "Enter a number from 1 to 5 please." << std::endl;
                return;
            }
            else if ( e == 2 ) {
                std::cout << "Room must be adjacent to shoot into it." << std::endl;
                return;
            }
        }
        // Error checking is done
        // Construct which rooms to search through
        std::vector<Room> search_rooms;

        for ( int i = 0; i < distance; i++ ) {
            if ( i == 0 ) {
                // Always start with the first room
                search_rooms.push_back(map.at(current_room).get_rooms().at(shoot_direction));
                continue;
            }
            int place = i - 1;
            int *rooms = search_rooms.at(place).get_room_numbers();
            search_rooms.push_back(map.at(rooms[shoot_direction]));
        }

        // Search the rooms
        for ( std::vector<Room>::iterator it = search_rooms.begin(); it != search_rooms.end(); ++it ) {
            if ( it->get_wumpus() ) {
                std::cout << "You hit the wumpus!" << std::endl;
                std::cout << "You exit the caves with your prize in hand." << std::endl;
                game_won = true;
            }
        }
        if ( !game_won ) {
            std::cout << "The arrow flies true, but nothing happens. You missed." << std::endl;
        }
    }
    else if ( !command.compare("quit") || !command.compare("exit") ) {
        std::cout << "Are you sure? ";
        command = parse_input( get_input() );

        if ( command.substr(0, 1) == "y" ) {
            std::cout << "Goodbye" << std::endl;
            run_game = false;
        }
    }
    else {
        std::cout << "I don't understand" << std::endl;
    }
    std::cout << std::endl;
}

bool handle_player_death( int &current_room, int starting_room, bool &player_dead, bool &generate_map, bool &run_game, bool &game_won ) {
    std::cout << "Would you like to play again? ";
    std::string restart = parse_input( get_input() );
    if ( restart.substr(0, 1) == "y" ) {
        std::cout << "Would you like to use the same map? ";
        std::string keep_map = parse_input( get_input() );
        if ( keep_map.substr(0, 1) == "y" ) {
            current_room = starting_room;
            player_dead  = false;
            game_won     = false;

            return true;
        }
        else {
            generate_map = true;
            player_dead  = false;
            game_won     = false;

            return true;
        }
    }
    else {
        std::cout << "Goodbye!" << std::endl;
        run_game = false;

        return false;
    }
}