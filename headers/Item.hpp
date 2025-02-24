#ifndef ITEM_HPP
#define ITEM_HPP

#include <string>
#include <stdexcept>
#include <ostream>

class Item {
private:
    std::string name;
    std::string description;
    int calories;       // 0 <= calories <= 1000
    float weight;       // 0 <= weight <= 500

public:
    // Constructor
    Item(const std::string& name, const std::string& description, int calories, float weight);

    // Getters (optional if needed)
    std::string get_name() const { return name; }
    std::string get_description() const { return description; }
    int get_calories() const { return calories; }
    float get_weight() const { return weight; }

    // Overloaded operator<< for printing
    friend std::ostream& operator<<(std::ostream& os, const Item& item);
};

#endif // ITEM_HPP

