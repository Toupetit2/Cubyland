#include "script_pch.h"
#include <memory>
#include <iostream>
#include <string>
#include "cubylandCore.h"

#ifdef _WIN32
    #define SCRIPT_API __declspec(dllexport)
#else
    #define SCRIPT_API __attribute__((visibility("default")))
#endif

class NPC : public Engine::Scripting::NativeScript
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
    int getATT() { return ATT; }
    void setATT(int newATT) { ATT = newATT; }

    int getHP() { return HP; }
    void setHP(int newHP) { HP = newHP; }

    int getXP() { return XP; }
    void setXP(int newXP) { XP = newXP; }

    bool isCuby() { return isCubyVar; }
    void setIsCuby(bool newStatus) { isCubyVar = newStatus; }

    std::string getName() { return name; }
    void setName(std::string newName) { name = newName; }

    Type getType() { return type; }
    void setType(Type newType) { type = newType; }

    Level getLevel() { return level; }
    void setLevel(Level newLevel) { level = newLevel; }


    std::string getTypeString() {
        switch (type)
        {
        case Type::FIRE:
            return "Feu";
            break;
        case Type::WATER:
            return "Eau";
            break;
        case Type::GRASS:
            return "Plante";
            break;
        default:
            break;
        }
    }
    std::string getLevelString()
    {
        switch (level)
        {
        case Level::Lv1:
            return "1";
            break;
        case Level::Lv2:
            return "2";
            break;
        case Level::Lv3:
            return "3";
            break;
        default:
            break;
        }
    }
    Vec3& getPlayerPosition() { return playerPos; }

    Efficiency handleEfficiency(Type t1, Type t2)
    {
        switch (t1)
        {
        case FIRE:
            if (t2 == Type::FIRE || t2 == Type::WATER)
                return Efficiency::NOT;
            else if (t2 == Type::GRASS)
                return Efficiency::VERY;
            break;
        case WATER:
            if (t2 == Type::GRASS || t2 == Type::WATER)
                return Efficiency::NOT;
            else if (t2 == Type::FIRE)
                return Efficiency::VERY;
            break;
        case GRASS:
            if (t2 == Type::GRASS || t2 == Type::FIRE)
                return Efficiency::NOT;
            else if (t2 == Type::WATER)
                return Efficiency::VERY;
            break;
        default:
            break;
        }
    }
    bool attack(std::unique_ptr<NPC>& target)
    {
        int damages = ATT;
        std::string efficiencyMessage;
        bool showMessage = false;
        switch (handleEfficiency(this->type, target->type))
        {
        case VERY:
            damages *= 2;
            break;
        case NOT:
            damages /= 2;
            break;
        case EFFICIENT:
            break;
        default:
            break;
        }
        std::cout << name << " attaque!" << std::endl;
        std::cin.get();
        if (handleEfficiency(this->type, target->type) == NOT)
        {
            efficiencyMessage = "Ce n'est pas tres efficace...";
            showMessage = true;
        }
        else if (handleEfficiency(this->type, target->type) == VERY)
        {
            efficiencyMessage = "C'est super efficace!";
            showMessage = true;
        }
        if (showMessage)
        {
            std::cout << efficiencyMessage << std::endl;
            std::cin.get();
        }
        std::cout << name << " inflige " << damages << " degats a " << target->name << " !" << std::endl;
        target->HP -= damages;
        if (target->getHP() <= 0)
        {
            std::cout << target->getName() << " est mort!" << std::endl;
            return true;
        }
        return false;
    }
    NPC(Vec3& playerPos) : playerPos(playerPos) {}

};

extern "C" SCRIPT_API Engine::Scripting::NativeScript* CreateNPC() {
    return new NPC(Vec3());
}