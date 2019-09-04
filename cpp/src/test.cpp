#include <iostream>
#include <string>
#include <vector>
#include "room.h"

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

    Room room1, room2, room3;
    room1.set_bat(1);
    room2.set_pit(1);
    room3.set_wumpus(1);

    room.set_rooms( room1, room2, room3 );

    room.set_warnings( room.get_rooms() );

    std::vector<std::string> warnings = room.get_warnings();
    for ( std::vector<std::string>::iterator it = warnings.begin(); it != warnings.end(); ++it ) {
        std::cout << *it;
        std::cout << std::endl;
    }

    return 0;
}