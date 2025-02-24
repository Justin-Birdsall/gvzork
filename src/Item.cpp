#include "../headers/Item.hpp"

Item::Item(const std::string& name, const std::string& description, int calories, float weight) 
    : name(name), description(description), calories(calories), weight(weight)
{
    // Validate name
    if (name.empty()) {
        throw std::invalid_argument("Item name cannot be blank.");
    }
    // Validate description
    if (description.empty()) {
        throw std::invalid_argument("Item description cannot be blank.");
    }
    // Validate calories
    if (calories < 0 || calories > 1000) {
        throw std::invalid_argument("Item calories must be between 0 and 1000.");
    }
    // Validate weight
    if (weight < 0.0f || weight > 500.0f) {
        throw std::invalid_argument("Item weight must be between 0 and 500.");
    }
}

std::ostream& operator<<(std::ostream& os, const Item& item) {
    os << item.name << " (" << item.calories << " calories) "
       << "- " << item.weight << " lb"
       << " - " << item.description;
    return os;
}

