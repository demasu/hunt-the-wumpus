#include <iostream>     // std::cout
#include <string>       // std::string
#include <vector>       // std::vector
#include <cctype>       // std::tolower
#include <algorithm>    // std::transform
#include <array>        // std::array
#include "room.h"

// Function definitions
std::string get_input();
std::vector<std::string> split_input( std::string );
void init_map( std::array<Room, 20> &map);

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
    std::cerr << "# In the main function" << std::endl;

    // Setup the map and rooms
    std::array<Room, 20> map;
    init_map( map );

    std::cerr << "# Starting infinite loop" << std::endl;
    while (1) {
        std::cerr << "# Inside the loop" << std::endl;
        std::cerr << "# Getting input" << std::endl;
        std::cout << "Enter in a command: ";
        std::cerr << "# Calling get_input" << std::endl;
        std::string input = get_input();
        std::cerr << "# Calling split_input" << std::endl;
        std::vector<std::string> words = split_input( input );
        std::cerr << "# Getting the first word returned" << std::endl;
        std::string command = words[0];
        std::cerr << "# Converting it to lowercase" << std::endl;
        std::transform(command.begin(), command.end(), command.begin(), [](unsigned char c){ return std::tolower(c); }); // Converts to lowercase

        std::cerr << "# Checking our decision tree" << std::endl;
        if ( !command.compare("move") ) {
            std::cerr << "# Command was move" << std::endl;
            std::cout << "You move" << std::endl;
            continue;
        }
        else if ( !command.compare("shoot") ) {
            std::cerr << "# Command was shoot" << std::endl;
            std::cout << "You shoot" << std::endl;
            continue;
        }
        else if ( !command.compare("quit") || !command.compare("exit") ) {
            std::cerr << "# Command was quit or exit" << std::endl;
            break;
        }
        else {
            std::cerr << "# Command was something else" << std::endl;
            std::cout << "I don't understand" << std::endl;
            continue;
        }

    }
    std::cerr << "# Done with the loop" << std::endl;

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
    std::cerr << "# In the split_command function." << std::endl;
    std::string delimiter = " ";

    std::vector<std::string> words;
    size_t pos = 0;

    std::cerr << "# Iterating over the command." << std::endl;
    while ( (pos = input.find(delimiter)) != std::string::npos) {
        std::cerr << "# Command is: [" << input << "]" << std::endl;
        std::string token = input.substr(0, pos);
        std::cerr << "# Token is: [" << token << "]" << std::endl;
        words.push_back(token);
        input.erase(0, pos + delimiter.length());
    }
    std::cerr << "# Done iterating." << std::endl;
    std::cerr << "# Command is now: [" << input << "]" << std::endl;
    if ( !input.empty() ) {
        std::cerr << "# Adding last word to the list." << std::endl;
        words.push_back(input); // Gets the last word typed in
    }

    std::cerr << "# Iterating over the words vector." << std::endl;
    for (std::vector<std::string>::iterator it = words.begin(); it != words.end(); ++it) {
        std::cerr << "# In the for loop." << std::endl;
        std::cout << "Word is: [" << *it;
        std::cout << "]" << std::endl;
    }
    std::cerr << "# Done iterating." << std::endl;

    return words;
}

void init_map( std::array<Room, 20> &map ) {
    std::cerr << "# In the init_map function" << std::endl;
}