#include "../headers/Game.hpp"
#include <iostream>
#include <sstream>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <algorithm> // for std::remove_if

Game::Game() 
    : current_weight(0.0f), elf_calorie_need(500), in_progress(true)
{
    commands = setup_commands();
    create_world();

    // Start the player in a random location
    current_location = random_location();
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
	//Creating some locations based on the One Piece Lore for Zork
    Location straw_hat_ship("Thousand Sunny",
       "The Straw Hats’ famous ship, with a lion depicted on the bow.");
    Location baratie("Baratie",
       "A floating restaurant ship  owned by Zeff. Delicious seafood is served here.");
    Location marine_base("Marine Base",
       "A heavily guarded marine outpost. Navy soldiers are walking around.");
    Location shell_town("Shell Town",
       "A quiet town under marine watch, rumored to have a big swordsman locked up.");
    Location syrup_village("Syrup Village",
       "A peaceful village with a slope leading to a mansion. Home to Usopp.");
    Location cocoyashi("Cocoyashi Village",
       "A tangerine orchard lines the coast. It’s Nami’s hometown.");
    Location jaya("Jaya",
       "A lawless island full of pirates, bars, and shady deals.");
    Location one_piece("One Piece",
       "A fabled location that Gold D. Roger said he left all his treasure.");

    // 10 items for rubrik 
    Item devil_fruit("Devil Fruit", 
       "A mysterious fruit that grants special powers to the eater \n"
       "beware that eating it bestows a curse on the user making it impossible to swim",
       1000, 5.0f);
    Item meat_on_bone("Meat on the Bone",
       "Luffy’s favorite meal – restores energy quickly.",
       500, 3.0f);
    Item tangerine("Tangerine",
       "A fresh tangerine from Nami’s orchard.",
       50, 0.5f);
    Item marine_sword("Marine Sword", 
       "A standard issue Navy sword with no special powers.",
       0, 7.0f);
    Item onigiri("Onigiri", 
       "Rice balls – a common East Blue snack!",
       100, 0.3f);
    Item log_pose("Log Pose", 
       "A compass like orb that Points towards specific location.",
       0, 0.1f);
    Item rum("Barrel of Rum", 
       "Strong pirate rum. Not exactly food, but it's consumable with enough courage.",
       300, 10.0f);
    Item anchor("Heavy Anchor", 
       "An anchor from the Thousand Sunny. Way too heavy for practical carrying. \n"
       "but whose to say it may be useful later...",
       0, 20.0f);
    Item stealth_mantle("Stealth Mantle",
       "A cloak rumored to hide the wearer from marines.",
       0, 2.0f);
    Item ramen("Bowl of Seafood Ramen",
       "A hearty meal made aboard the Baratie, curated by the chef Zeff.",
       200, 1.0f);

    // Add items to locations
    straw_hat_ship.add_item(anchor);
    straw_hat_ship.add_item(devil_fruit);

    baratie.add_item(ramen);
    baratie.add_item(rum);

    marine_base.add_item(marine_sword);

    shell_town.add_item(onigiri);

    cocoyashi.add_item(tangerine);

    jaya.add_item(log_pose);
    jaya.add_item(stealth_mantle);

    // NPCs (at least 5)
    NPC luffy("Luffy", "The Straw Hats Captain who wants to be Pirate King.");
    luffy.add_message("I'm Luffy! I’m gonna be the Pirate King!");
    luffy.add_message("*Picking Nose* You got any meat?");
    luffy.add_message("Let's set sail!");

    NPC zoro("Zoro", "A swordsman who wields three swords.");
    zoro.add_message("I will become the greatest swordsman in the world!");
    zoro.add_message("Do you know where I am, I keep losing my sense of direction...");
    zoro.add_message("Don’t get in my way, or I’ll cut you down.");

    NPC nami("Nami", "A skilled navigator for the Straw Hats.");
    nami.add_message("I only steal from pirates I don’t like!");
    nami.add_message("If you need help navigating, let me know");

    NPC sanji("Sanji", "A chivalrous cook with a fiery kick.");
    sanji.add_message("Anyone hungry? I can whip up a meal!");
    sanji.add_message("Don’t ever waste food in my kitchen!");

    NPC goldroger("Gold D. Rogger", "A mythical pirate who started the new pirate age \n.");
    goldroger.add_message("I HAVE LEFT ALL MY TREASURE IN ONE PLACE");

    // Place NPCs
    straw_hat_ship.add_npc(luffy);
    marine_base.add_npc(zoro);
    baratie.add_npc(sanji);
    cocoyashi.add_npc(nami);
    one_piece.add_npc(goldroger);

    // Connecting some of the locations for gameplay
    // one piece should probably be a one-way entrance)
    straw_hat_ship.add_location("east",baratie);
    baratie.add_location("west", straw_hat_ship);

    baratie.add_location("north", marine_base);
    marine_base.add_location("south",baratie);

    marine_base.add_location("east", shell_town);
    shell_town.add_location("west", marine_base);

    shell_town.add_location("south", syrup_village);
    syrup_village.add_location("north", shell_town);

    syrup_village.add_location("east", cocoyashi);
    cocoyashi.add_location("west", syrup_village);

    jaya.add_location("north", baratie);
    // (One-way from jaya -> baratie if we don't link baratie back to jaya)

    // Make the one piece  "hidden" - only accessible from cocoyashi
    cocoyashi.add_location("south", one_piece);
    // finding the one piece should symbolize the end of the game no  linking back to cocoyashi
    // Push the locations into a world vector
    world_locations.push_back(straw_hat_ship);
    world_locations.push_back(baratie);
    world_locations.push_back(marine_base);
    world_locations.push_back(shell_town);
    world_locations.push_back(syrup_village);
    world_locations.push_back(cocoyashi);
    world_locations.push_back(jaya);
    world_locations.push_back(one_piece);
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
    if (elf_calorie_need <= 0) {
        std::cout << "\n** Congratulations! You have fed the elf enough calories to save GVSU! **\n";
    } else {
        std::cout << "\n** You have quit or failed. The elf did not get enough calories. **\n";
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
    auto &npcs = current_location.get_npcs();
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

    auto &npcs = current_location.get_npcs();
    for (auto &npc : npcs) {
        if (npc.get_name() == npc_name) {
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

    auto &items = current_location.get_items();
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
            current_location.add_item(*it);
            current_weight -= it->get_weight();

                if (it->get_calories() > 0) {
                    // edible => reduce elf_calorie_need
                    elf_calorie_need -= it->get_calories();
                    std::cout << "The elf happily accepts your offering of " 
                              << it->get_calories() << " calories.\n";
                } else {
                    // 0 calorie => teleport player randomly
                    std::cout << "The elf is displeased with your 0-calorie offering and teleports you away!\n";
                    current_location = random_location();
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
    current_location.set_visited();

    // Check weight restriction
    if (current_weight > 30.0f) {
        std::cout << "You're over 30 lbs. You cannot move! Maybe drop something first.\n";
        return;
    }

    auto &neighbors = current_location.get_locations();
    auto it = neighbors.find(direction);
    if (it == neighbors.end()) {
        std::cout << "You can't go that way.\n";
        return;
    }
    current_location = it->second; // Move to neighbor
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
    current_location = random_location();
    std::cout << current_location << "\n";
}

