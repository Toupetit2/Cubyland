#pragma once

#include <memory>
#include <iostream>
#include <string>
#include "cubylandCore.h"

class NPC
{
private:
    int ATT = 0;
    int HP = 0;
    int XP = 0;
    bool isCubyVar = false;
    std::string name = "NAME";
    Type type = FIRE;
    Vec3 position;
    Vec3& playerPos;
    Level level = Level::Lv1;

public:
    int getATT();
    void setATT(int newATT);

    int getHP();
    void setHP(int newHP);

    int getXP();
    void setXP(int newXP);

    bool isCuby();
    void setIsCuby(bool newStatus);

    std::string getName();
    void setName(std::string newName);
    Type getType();
    void setType(Type newType);

    Level getLevel();
    void setLevel(Level newLevel);


    std::string getTypeString();
    std::string getLevelString();
    Vec3& getPlayerPosition();

    Efficiency handleEfficiency(Type t1, Type t2);
    bool attack(std::unique_ptr<NPC>& target);
    NPC(Vec3& playerPos);
};
