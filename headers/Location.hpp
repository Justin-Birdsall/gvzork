#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include <ostream>
#include "NPC.hpp"
#include "Item.hpp"
class Location {
private:
    std::string name;
    std::string description;
    bool visited;

    // Value-based map storing entire Location objects
    std::map<std::string, Location> neighbors;

    std::vector<NPC> npcs;
    std::vector<Item> items;

public:
    // Constructor
    Location();
    Location(const std::string& name, const std::string& description);

    // Accessors
    bool get_visited() const;
    void set_visited();

    std::map<std::string, Location>& get_locations();

    // Value-based add_location
    void add_location(const std::string &direction, const Location &loc);

    // NPC handling
    void add_npc(const NPC& npc);
    std::vector<NPC>& get_npcs();

    // Item handling
    void add_item(const Item& item);
    std::vector<Item>& get_items();

    // Overload stream operator
    friend std::ostream& operator<<(std::ostream& os, const Location& loc);
};
#endif // LOCATION_HPP

