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
void init_map( std::vector<Room> &map);
Room init_room(bool, bool, bool);
void connect_rooms( std::vector<Room> &map );
void generate_warnings( std::vector<Room> &map );

void Room::set_rooms( Room room1, Room room2, Room room3 ) {
    connected_rooms.push_back( room1 );
    connected_rooms.push_back( room2 );
    connected_rooms.push_back( room3 );
}

void Room::set_warnings( std::vector<Room> rooms ) {
    for ( std::vector<Room>::iterator it = rooms.begin(); it != rooms.end(); ++it ) {
        Room current_room = *it;
        if ( current_room.get_wumpus() ) {
            warnings.push_back("You smell a wumpus.");
            warning_present = 1;
        }
        if ( current_room.get_bat() ) {
            warnings.push_back("You hear flapping.");
            warning_present = 1;
        }
        if ( current_room.get_pit() ) {
            warnings.push_back("You feel a breeze.");
            warning_present = 1;
        }
    }
}

void Room::set_wumpus( bool is_here ) {
    wumpus = is_here;
}

void Room::set_bat( bool is_here ) {
    bat = is_here;
}

void Room::set_pit( bool is_here ) {
    pit = is_here;
}

void Room::set_number( int number ) {
    room_number = number;
}

std::vector<Room> Room::get_rooms() {
    return connected_rooms;
}

std::vector<std::string> Room::get_warnings() {
    return warnings;
}

bool Room::get_wumpus() {
    return wumpus;
}

bool Room::get_bat() {
    return bat;
}

bool Room::get_pit() {
    return pit;
}

int Room::get_number() {
    return room_number;
}

bool Room::is_hazardous() {
    return warning_present;
}

int main() {
    // Setup the map and rooms
    std::vector<Room> map;
    init_map( map );

    int starting_room;
    std::random_device rd; // Obtain a random number from hardware
    std::mt19937 eng(rd()); // Seed the generator
    std::uniform_int_distribution<> dist(1, 20); // Define the range
    starting_room    = dist(eng);

    int current_room = starting_room;
    while (1) {
        std::cout << "You are in room " << current_room << std::endl;
        std::cout << "Tunnels lead to " << map.at(current_room).get_rooms().at(0).get_number() << "," << map.at(current_room).get_rooms().at(1).get_number() << "," << map.at(current_room).get_rooms().at(2).get_number() << "." << std::endl;
        if ( map.at(current_room).is_hazardous() ) {
            std::vector<std::string> warnings = map.at(current_room).get_warnings();
            for ( std::vector<std::string>::iterator it = warnings.begin(); it != warnings.end(); ++it ) {
                std::cout << "\t" << *it;
                std::cout << std::endl;
            }
        }
        std::cout << "Move or shoot? ";
        std::string input = get_input();
        std::vector<std::string> words = split_input( input );
        std::string command = words[0];
        std::transform(command.begin(), command.end(), command.begin(), [](unsigned char c){ return std::tolower(c); }); // Converts to lowercase

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
                continue;
            }

            continue;
        }
        else if ( !command.compare("shoot") ) {
            std::cout << "You shoot" << std::endl;
            continue;
        }
        else if ( !command.compare("quit") || !command.compare("exit") ) {
            break;
        }
        else {
            std::cout << "I don't understand" << std::endl;
            continue;
        }

    }

    std::cout << "Goodbye" << std::endl;

    return 0;
}

std::string get_input() {
    std::string input;
    getline(std::cin, input);
    std::cout << "The command you entered was: [" << input << "]" << std::endl;

    return input;
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

    for (std::vector<std::string>::iterator it = words.begin(); it != words.end(); ++it) {
        std::cout << "Word is: [" << *it;
        std::cout << "]" << std::endl;
    }

    return words;
}

void init_map( std::vector<Room> &map ) {
    // Map consists of 20 rooms
    // Each room has 3 connecting rooms
    // Starting room will vary

    int room_with_wumpus;
    std::random_device rd; // Obtain a random number from hardware
    std::mt19937 eng(rd()); // Seed the generator
    std::uniform_int_distribution<> dist(1, 20); // Define the range
    room_with_wumpus = dist(eng);

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
}

void generate_warnings( std::vector<Room> &map ) {
    for ( int i = 1; i <= 20; i++ ) {
        map.at(i).set_warnings( map.at(i).get_rooms() );
    }
}