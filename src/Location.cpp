#include "../headers/Location.hpp"
Location::Location()
    : name("Unknown"), description("Placeholder"), visited(false)
{
}

Location::Location(const std::string& name, const std::string& description)
    : name(name), description(description), visited(false)
{
    if (name.empty()) {
        throw std::invalid_argument("Location name cannot be blank.");
    }
    if (description.empty()) {
        throw std::invalid_argument("Location description cannot be blank.");
    }
}

bool Location::get_visited() const {
    return visited;
}

void Location::set_visited() {
    visited = true; 
}

std::map<std::string, Location>& Location::get_locations() {
    return neighbors;
}

void Location::add_location(const std::string &direction, const Location &loc) {
    if (direction.empty()) {
        throw std::invalid_argument("Direction cannot be blank.");
    }
    if (neighbors.find(direction) != neighbors.end()) {
        throw std::invalid_argument("Location already exists for direction: " + direction);
    }
    // Store a copy of loc in neighbors
    neighbors.insert({ direction, loc });
}

void Location::add_npc(const NPC& npc) {
    npcs.push_back(npc);
}

std::vector<NPC>& Location::get_npcs() {
    return npcs;
}

void Location::add_item(const Item& item) {
    items.push_back(item);
}

std::vector<Item>& Location::get_items() {
    return items;
}

std::ostream& operator<<(std::ostream& os, const Location& loc) {
    os << loc.name << " - " << loc.description << "\n";

    // NPCs
    if (!loc.npcs.empty()) {
        os << "You see the following NPCs:\n";
        for (auto &npc : loc.npcs) {
            os << "- " << npc << "\n";
        }
    } else {
        os << "You see no one around.\n";
    }

    // Items
    if (!loc.items.empty()) {
        os << "You see the following Items:\n";
        for (auto &item : loc.items) {
            os << "- " << item << "\n";
        }
    } else {
        os << "There are no items here.\n";
    }

    // Directions
    if (!loc.neighbors.empty()) {
        os << "You can go in the following Directions:\n";
        for (auto &pair : loc.neighbors) {
            const std::string &direction = pair.first;
            const Location &neighborLoc = pair.second;
            if (neighborLoc.get_visited()) {
                os << "- " << direction << " - " << neighborLoc.name << " (Visited)\n";
            } else {
                os << "- " << direction << " - Unknown\n";
            }
        }
    } else {
        os << "There are no obvious exits.\n";
    }

    return os;
}
