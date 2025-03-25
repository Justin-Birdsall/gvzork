#include "../headers/Game.hpp"
#include "../headers/ascii.hpp" 
#include <iostream>
#include <sstream>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <functional>
#include <algorithm> // for std::remove_if

std::map<std::string, std::reference_wrapper<Location>> neighbors;

Game::Game()
    : straw_hat_ship("Thousand Sunny",
         "The Straw Hats’ famous ship, with a lion depicted on the bow."),
      baratie("Baratie",
         "A floating restaurant ship owned by Zeff. Delicious seafood is served here."),
      marine_base("Marine Base",
         "A heavily guarded marine outpost. Navy soldiers are walking around."),
      shell_town("Shell Town",
         "A quiet town under marine watch, rumored to have a big swordsman locked up."),
      syrup_village("Syrup Village",
         "A peaceful village with a slope leading to a mansion. Home to Usopp."),
      cocoyashi("Cocoyashi Village",
         "A tangerine orchard lines the coast. It’s Nami’s hometown."),
      jaya("Jaya",
         "A lawless island full of pirates, bars, and shady deals."),
      one_piece("One Piece",
         "A fabled location that Gold D. Roger said he left all his treasure."),
      current_location(std::ref(straw_hat_ship)),  
      current_weight(0.0f), Luffy_calorie_need(5000), in_progress(true)
{   
    commands = setup_commands();
    create_world();

}

std::map<std::string, Game::Command> Game::setup_commands() {
    std::map<std::string, Command> cmd_map;
    cmd_map["help"]      = std::bind(&Game::show_help, this, std::placeholders::_1);
    cmd_map["?"]         = std::bind(&Game::show_help, this, std::placeholders::_1);
    cmd_map["talk"]      = std::bind(&Game::talk, this, std::placeholders::_1);
    cmd_map["meet"]      = std::bind(&Game::meet, this, std::placeholders::_1);
    cmd_map["take"]      = std::bind(&Game::take_item, this, std::placeholders::_1);
    cmd_map["give"]      = std::bind(&Game::give_item, this, std::placeholders::_1);
    cmd_map["go"]        = std::bind(&Game::go, this, std::placeholders::_1);
    cmd_map["look"]      = std::bind(&Game::look, this, std::placeholders::_1);
    cmd_map["items"]     = std::bind(&Game::show_items, this, std::placeholders::_1);
    cmd_map["quit"]      = std::bind(&Game::quit, this, std::placeholders::_1);

    // Creating 2 custom commands to have "moves and Power ups or like eating in items"
    cmd_map["eat"]       = std::bind(&Game::eat, this, std::placeholders::_1);
    cmd_map["shambles"]  = std::bind(&Game::special_move, this, std::placeholders::_1);

    return cmd_map;
}
void Game::create_world() {
    // Add items to locations
    straw_hat_ship.add_item(Item("Anchor", 
       "An anchor from the Thousand Sunny. Way too heavy for practical carrying.", 0, 20.0f));
    straw_hat_ship.add_item(Item("Devil Fruit", 
       "A mysterious fruit that grants special powers.", 1000, 5.0f));

    baratie.add_item(Item("Bowl of Seafood Ramen", 
       "A hearty meal made aboard the Baratie.", 200, 1.0f));
    baratie.add_item(Item("Barrel of Rum", 
       "Strong pirate rum.", 300, 10.0f));

    marine_base.add_item(Item("Marine Sword", 
       "A standard issue Navy sword.", 0, 7.0f));

    shell_town.add_item(Item("Onigiri", 
       "Rice balls – a common East Blue snack!", 100, 0.3f));

    cocoyashi.add_item(Item("Tangerine", 
       "A fresh tangerine from Nami’s orchard.", 50, 0.5f));

    jaya.add_item(Item("Log Pose", 
       "A compass-like orb that points towards specific locations.", 0, 0.1f));
    jaya.add_item(Item("Stealth Mantle", 
       "A cloak rumored to hide the wearer from marines.", 0, 2.0f));

    // Add NPCs
    straw_hat_ship.add_npc(NPC("Luffy", "The Straw Hats Captain who wants to be Pirate King.",luffy_art));
    marine_base.add_npc(NPC("Zoro", "A swordsman who wields three swords."));
    baratie.add_npc(NPC("Sanji", "A chivalrous cook with a fiery kick."));
    cocoyashi.add_npc(NPC("Nami", "A skilled navigator for the Straw Hats."));
    one_piece.add_npc(NPC("Gold D. Roger", "A mythical pirate who started the new pirate age."));

    // Connecting locations (ensuring references remain valid)
    straw_hat_ship.add_location("east", baratie);
    baratie.add_location("west", straw_hat_ship);

    baratie.add_location("north", marine_base);
    marine_base.add_location("south", baratie);

    marine_base.add_location("east", shell_town);
    shell_town.add_location("west", marine_base);

    shell_town.add_location("south", syrup_village);
    syrup_village.add_location("north", shell_town);

    syrup_village.add_location("east", cocoyashi);
    cocoyashi.add_location("west", syrup_village);

    jaya.add_location("north", baratie);
    cocoyashi.add_location("south", one_piece);

    // Ensure locations are properly stored
    world_locations.push_back(std::ref(straw_hat_ship));
    world_locations.push_back(std::ref(baratie));
    world_locations.push_back(std::ref(marine_base));
    world_locations.push_back(std::ref(shell_town));
    world_locations.push_back(std::ref(syrup_village));
    world_locations.push_back(std::ref(cocoyashi));
    world_locations.push_back(std::ref(jaya));
    world_locations.push_back(std::ref(one_piece));
}
Location Game::random_location() {
    // Choose a random location from our vector
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, (int)world_locations.size()-2);
    int index = dist(gen);
    return world_locations[index];
}

void Game::play() {
    std::cout << "Welcome to Grand Line Zork (One Piece Edition)!\n";
    std::cout << "Your mission is to collect 500 calories worth of edible items\n"
                 "and bring them to the elf in the Elf Woods behind campus.\n"
                 "Be careful about carrying over 30 pounds of items, though!\n";
//while the game is in progress read inputs and parse the tokens. 
    while (in_progress) {
        // Prompt user
        std::cout << "\n> ";
        std::string user_input;
        std::getline(std::cin, user_input);
        if (user_input.empty()) {
            continue;
        }

        // Tokenize user input by spaces
        std::stringstream ss(user_input);
        std::vector<std::string> tokens;
        std::string token;
        while (ss >> token) {
            tokens.push_back(token);
        }
        if (tokens.empty()) {
            continue;
        }

        // The first token is the command, the rest is arguments
        std::string command = tokens[0];
	// remove the command from the tokens so that way we can parse the rest of the text
        tokens.erase(tokens.begin());  

        // We call the function from the map to find the token
        auto it = commands.find(command);
        if (it != commands.end()) {
            Command cmd_func = it->second;
            cmd_func(tokens);
        } else {
            std::cout << "I don't understand that command.\n";
        }
    }

    // Once the loop ends, check if the elf was satisfied
    if (Luffy_calorie_need <= 0) {
        std::cout << "\n** Congratulations! You have fed the Luffy enough calories to save GVSU! **\n";
    } else {
        std::cout << "\n** You have quit or failed. Luffy is sad you couldn't stay and eat more meat! **\n";
    }
}
//Help 
void Game::show_help(std::vector<std::string> args) {
    std::cout << "Available commands:\n";
    for (auto const& pair : commands) {
        std::cout << " - " << pair.first << "\n";
    }
    // Show current time
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::cout << "Current time: " << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S") 
              << "\n";
}

void Game::talk(std::vector<std::string> args) {
    if (args.empty()) {
        std::cout << "Talk to whom?\n";
        return;
    }
    std::string npc_name;
    for (auto &t : args) {
        npc_name += t + " ";
    }
    npc_name = npc_name.substr(0, npc_name.size()-1); // remove trailing space

    // Find the NPC in the current location
    auto &npcs = current_location.get().get_npcs();
    for (auto &npc : npcs) {
        if (npc.get_name() == npc_name) {
            std::cout << npc.get_message() << "\n";
            return;
        }
    }
    std::cout << "No one named \"" << npc_name << "\" is here.\n";
}

void Game::meet(std::vector<std::string> args) {
    if (args.empty()) {
        std::cout << "Meet who?\n";
        return;
    }
    std::string npc_name;
    for (auto &t : args) {
        npc_name += t + " ";
    }
    npc_name = npc_name.substr(0, npc_name.size()-1);

    auto &npcs = current_location.get().get_npcs();
    for (auto &npc : npcs) {
        if (npc.get_name() == npc_name) {
	    std::cout << npc.get_ascii_art(); 
            std::cout << npc.get_description() << "\n";
            return;
        }
    }
    std::cout << "No one named \"" << npc_name << "\" is here.\n";
}

void Game::take_item(std::vector<std::string> args) {
    if (args.empty()) {
        std::cout << "Take what?\n";
        return;
    }
    std::string item_name;
    for (auto &t : args) {
        item_name += t + " ";
    }
    item_name = item_name.substr(0, item_name.size()-1);

    auto &items = current_location.get().get_items();
    for (auto it = items.begin(); it != items.end(); ++it) {
        if (it->get_name() == item_name) {
            float new_weight = current_weight + it->get_weight();
            if (new_weight > 30.0f) {
                std::cout << "You can't carry that much weight!\n";
                return;
            }
            current_weight = new_weight;
            player_inventory.push_back(*it);
            items.erase(it);
            std::cout << "You take the " << item_name << ".\n";
            return;
        }
    }
    std::cout << "There's no item called \"" << item_name << "\" here.\n";
}

void Game::give_item(std::vector<std::string> args) {
    if (args.empty()) {
        std::cout << "Give what?\n";
        return;
    }
    std::string item_name;
    for (auto &t : args) {
        item_name += t + " ";
    }
    item_name = item_name.substr(0, item_name.size()-1);

    // Find the item in player_inventory
    for (auto it = player_inventory.begin(); it != player_inventory.end(); ++it) {
        if (it->get_name() == item_name) {
            // Remove from player inventory, add to location
            current_location.get().add_item(*it);
            current_weight -= it->get_weight();

                if (it->get_calories() > 0) {
                    // edible => reduce Luffy_calorie_need
                    Luffy_calorie_need -= it->get_calories();
                    std::cout << "The elf happily accepts your offering of " 
                              << it->get_calories() << " calories.\n";
                } else {
                    // 0 calorie => teleport player randomly
                    std::cout << "The elf is displeased with your 0-calorie offering and teleports you away!\n";
                }

            player_inventory.erase(it);
            std::cout << "You gave " << item_name << ".\n";
            return;
        }
    }
    std::cout << "You don't have \"" << item_name << "\" in your inventory.\n";
}

void Game::go(std::vector<std::string> args) {
    if (args.empty()) {
        std::cout << "Go where?\n";
        return;
    }
    std::string direction = args[0];  // We'll assume first token is direction
    // Mark the location as visited
    current_location.get().set_visited();

    auto &neighbors = current_location.get().get_locations();
    auto it = neighbors.find(direction);
    if (it == neighbors.end()) {
        std::cout << "You can't go that way.\n";
        return;
    }

    Location &new_Location = it -> second.get(); 
    current_location = std::ref(new_Location);  // Move to neighbor
    std::cout << "You travel " << direction << "...\n";
    std::cout << current_location << "\n"; // auto-LOOK
}

void Game::show_items(std::vector<std::string> args) {
    std::cout << "You are carrying:\n";
    if (player_inventory.empty()) {
        std::cout << " - Nothing.\n";
    } else {
        for (auto &it : player_inventory) {
            std::cout << " - " << it << "\n";
        }
    }
    std::cout << "Current weight: " << current_weight << " lbs\n";
}

void Game::look(std::vector<std::string> args) {
    std::cout << current_location << "\n";
}

void Game::quit(std::vector<std::string> args) {
    std::cout << "You have chosen to quit.\n";
    in_progress = false;
}

// Custom commands
void Game::eat(std::vector<std::string> args) {
    // If no arguments for what to eat ask user again 
    if (args.empty()) {
        std::cout << "Eat what?\n";
        return;
    }
    std::string item_name;
    for (auto &t : args) {
        item_name += t + " ";
    }
    item_name = item_name.substr(0, item_name.size()-1);

    for (auto it = player_inventory.begin(); it != player_inventory.end(); ++it) {
        if (it->get_name() == item_name) {
            if (it->get_calories() > 0) {
                std::cout << "You eat the " << item_name << " and gain some energy!\n";
		//Subtract the item weight from the "bag" / players weight
                current_weight -= it->get_weight();
                if (current_weight < 0) current_weight = 0;  // safety
                player_inventory.erase(it);
                return;
            } else {
                std::cout << "You can't eat that!\n";
                return;
            }
        }
    }
    std::cout << "You don't have \"" << item_name << "\" in your inventory.\n";
}

void Game::special_move(std::vector<std::string> args) {
    // random teleport maybe add to specific island without it being connected if time
    std::cout << "You perform a special move 'Shambles' and teleport!\n";
    std::cout << current_location << "\n";
}

