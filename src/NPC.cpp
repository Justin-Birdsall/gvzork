#include "../headers/NPC.hpp"

NPC::NPC(const std::string& name, const std::string& description)
    : name(name), description(description), message_index(0)
{
    if (name.empty()) {
        throw std::invalid_argument("NPC name cannot be blank.");
    }
    if (description.empty()) {
        throw std::invalid_argument("NPC description cannot be blank.");
    }
}

void NPC::add_message(const std::string& msg) {
    messages.push_back(msg);
}

std::string NPC::get_message() {
    if (messages.empty()) {
        return "NPC has nothing to say.";
    }
    std::string current = messages[message_index];
    // increment message index
    message_index = (message_index + 1) % messages.size();
    return current;
}

std::ostream& operator<<(std::ostream& os, const NPC& npc) {
    os << npc.name;
    return os;
}

