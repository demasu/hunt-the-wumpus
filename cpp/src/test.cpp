#include <iostream>
#include <string>
#include <vector>
#include <random>
#include "room.h"

// Function prototypes
Room init_room(bool, bool, bool);

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
        }
        if ( current_room.get_bat() ) {
            warnings.push_back("You hear flapping.");
        }
        if ( current_room.get_pit() ) {
            warnings.push_back("You feel a breeze.");
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

int main() {
    Room room;
    room.set_bat(0);
    room.set_pit(0);
    room.set_wumpus(0);

    std::random_device rd; // Obtain a random number from hardware
    std::mt19937 eng(rd()); // Seed the generator
    std::uniform_int_distribution<> dist(1, 20); // Define the range
    int room_with_wumpus = dist(eng);
    std::cerr << "Wumpus is in room: [" << room_with_wumpus << "]" << std::endl;

    std::vector<Room> rooms;
    for ( int i = 1; i < 4; i++ ) {
        std::uniform_int_distribution<> distr(0, 3);
        int hazard = distr(eng);

        if ( hazard == 3 && i != room_with_wumpus ) {
            // Don't put the wumpus in more than one room
            i--;
            continue;
        }

        Room room;        
        switch (hazard) {
            case 0:
                room = init_room(0, 0, 0);
                break;
            case 1:
                room = init_room(1, 0, 0);
                break;
            case 2:
                room = init_room(0, 1, 0);
                break;
            case 3:
                room = init_room(0, 0, 1);
                break;
            default:
                break;
        }
        rooms.push_back(room);
    }

    Room room1, room2, room3;
    room3 = rooms.back();
    rooms.pop_back();
    room2 = rooms.back();
    rooms.pop_back();
    room1 = rooms.back();
    rooms.pop_back();
    room.set_rooms( room1, room2, room3 );

    room.set_warnings( room.get_rooms() );

    std::vector<std::string> warnings = room.get_warnings();
    for ( std::vector<std::string>::iterator it = warnings.begin(); it != warnings.end(); ++it ) {
        std::cout << *it;
        std::cout << std::endl;
    }

    return 0;
}

Room init_room( bool bat, bool pit, bool wumpus ) {
    Room room;
    room.set_bat(bat);
    room.set_pit(pit);
    room.set_wumpus(wumpus);

    return room;
}