#ifndef ROOM_H
#define ROOM_H

class Room {
    private:
        std::vector<Room> connected_rooms;
        std::vector<std::string> warnings;
        bool wumpus;
        bool bat;
        bool pit;
        int room_number;
        bool warning_present;
        int connected_room_numbers[3];
    public:
        void set_rooms( Room room1, Room room2, Room room3 );
        void set_warnings( std::vector<Room> rooms );
        void set_wumpus( bool );
        void set_bat( bool );
        void set_pit( bool );
        void set_number( int );
        void set_connected_numbers();
        std::vector<Room> get_rooms();
        std::vector<std::string> get_warnings();
        bool get_wumpus();
        bool get_bat();
        bool get_pit();
        int get_number();
        bool is_hazardous();
        int* get_room_numbers();
};

#endif