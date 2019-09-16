#include <string>       // std::string
#include <vector>       // std::vector
#include "room.h"       // Class declarations

// Class function definitions

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

void Room::set_connected_numbers() {
    connected_room_numbers[0] = connected_rooms.at(0).get_number();
    connected_room_numbers[1] = connected_rooms.at(1).get_number();
    connected_room_numbers[2] = connected_rooms.at(2).get_number();
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

int* Room::get_room_numbers() {
    return connected_room_numbers;
}