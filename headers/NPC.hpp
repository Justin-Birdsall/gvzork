#ifndef NPC_HPP
#define NPC_HPP

#include <string>
#include <vector>
#include <stdexcept>
#include <ostream>

class NPC {
private:
    std::string name;
    std::string description;
    std::vector<std::string> messages;
    int message_index;  // cycles through messages

public:
    // Constructor
    NPC(const std::string& name, const std::string& description);

    // Add a message to the NPC's list of messages
    void add_message(const std::string& msg);

    // Getters
    std::string get_name() const { return name; }
    std::string get_description() const { return description; }

    // Returns the current message and increments the message_index
    std::string get_message();

    // Overloaded operator<< to just return the NPC's name
    friend std::ostream& operator<<(std::ostream& os, const NPC& npc);
};

#endif // NPC_HPP

