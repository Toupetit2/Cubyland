#pragma once

#include "player.hpp"
#include "npc.hpp"

class Fight
{
public:
    std::unique_ptr<Player>& player;
    std::unique_ptr<NPC>& opponent;
    void fight();



    Fight(std::unique_ptr<Player>& p, std::unique_ptr<NPC>& n);
};