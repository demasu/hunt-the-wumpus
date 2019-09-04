#ifndef ROOM_H
#define ROOM_H

class Room {
    private:
        std::vector<Room> connected_rooms;
        std::vector<std::string> warnings;
        bool wumpus;
        bool bat;
        bool pit;
    public:
        void set_rooms( Room room1, Room room2, Room room3 );
        void set_warnings( std::vector<Room> rooms );
        void set_wumpus( bool );
        void set_bat( bool );
        void set_pit( bool );
        std::vector<Room> get_rooms();
        std::vector<std::string> get_warnings();
        bool get_wumpus();
        bool get_bat();
        bool get_pit();
};

#endif