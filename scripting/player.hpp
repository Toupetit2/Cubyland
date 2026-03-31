#pragma once

#include <vector>
#include <memory>
#include "npc.hpp"


class Player
{
public:


    Vec3 vector1;
    std::vector<std::unique_ptr<NPC>> mapCubies;

    void getComponent();
    void setComponent();


    void move();
    void update();
};

