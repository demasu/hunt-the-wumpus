#include <iostream>     // std::cout
#include <string>       // std::string
#include <vector>       // std::vector
#include <cctype>       // std::tolower
#include <algorithm>    // std::transform

// Function definitions
std::string get_input();
std::vector<std::string> split_input( std::string );

int main() {
    //std::cout << "Enter in a command: " << std::endl;
    //std::string input = get_input();

    //std::vector<std::string> words = split_input( input );
    //std::string command = words[0];
    //std::transform(command.begin(), command.end(), command.begin(), [](unsigned char c){ return std::tolower(c); }); // Converts to lowercase
    //std::cout << "Command entered is: [" << command << "]" << std::endl;

    while (1) {
        std::cout << "Enter in a command: " << std::endl;
        std::string input = get_input();
        std::vector<std::string> words = split_input( input );
        std::string command = words[0];
        std::transform(command.begin(), command.end(), command.begin(), [](unsigned char c){ return std::tolower(c); }); // Converts to lowercase

        if ( command.compare("move") ) {
            std::cout << "You move" << std::endl;
            continue;
        }
        else if ( command.compare("shoot") ) {
            std::cout << "You shoot" << std::endl;
            continue;
        }
        else if ( command.compare("quit") || command.compare("exit") ) {
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
    if ( !words.empty() ) {
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