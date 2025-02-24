#ifndef GAME_HPP
#define GAME_HPP

#include <map>
#include <vector>
#include <string>
#include <functional>
#include <random>
#include "Location.hpp"

class Game {
private:
    // Command is: a function that returns void and takes std::vector<std::string> as parameter.
    using Command = std::function<void(std::vector<std::string>)>;

    // Data
    std::map<std::string, Command> commands;        // mapping user command to a function
    std::vector<Item> player_inventory;             // items the player currently carries
    float current_weight;                           // current carried weight
    std::vector<Location> world_locations;          // all locations in our world
    Location current_location;                      // player's current location
    int elf_calorie_need;                           // how many calories the elf still needs
    bool in_progress;                               // is the game still going?

public:
    // Constructor
    Game();

    // Setup
    std::map<std::string, Command> setup_commands();
    void create_world();
    Location random_location();

    // The main game loop
    void play();

    // Command methods
    void show_help(std::vector<std::string> args);
    void talk(std::vector<std::string> args);
    void meet(std::vector<std::string> args);
    void take_item(std::vector<std::string> args);
    void give_item(std::vector<std::string> args);
    void go(std::vector<std::string> args);
    void show_items(std::vector<std::string> args);
    void look(std::vector<std::string> args);
    void quit(std::vector<std::string> args);

    // Additional custom commands
    void eat(std::vector<std::string> args);     // sample custom command
    void special_move(std::vector<std::string> args); // Another custom command
};

#endif // GAME_HPP

