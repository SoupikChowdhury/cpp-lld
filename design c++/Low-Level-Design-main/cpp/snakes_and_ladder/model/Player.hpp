#ifndef PLAYER_HPP
#define PLAYER_HPP

#include <string>

class Player {
private:
    std::string name;
    int position;

public:
    explicit Player(const std::string& name) : name(name), position(0) {}

    std::string getName() const { return name; }
    int getPosition() const { return position; }
    void setPosition(int pos) { position = pos; }
};

#endif // PLAYER_HPP
