#include "npc.hpp"


    int NPC::getATT() { return ATT; }
    void NPC::setATT(int newATT) { ATT = newATT; }

    int NPC::getHP() { return HP; }
    void NPC::setHP(int newHP) { HP = newHP; }

    int NPC::getXP() { return XP; }
    void NPC::setXP(int newXP) { XP = newXP; }

    bool NPC::isCuby() { return isCubyVar; }
    void NPC::setIsCuby(bool newStatus) { isCubyVar = newStatus; }

    std::string NPC::getName() { return name; }
    void NPC::setName(std::string newName) { name = newName; }

    Type NPC::getType() { return type; }
    void NPC::setType(Type newType) { type = newType; }

    Level NPC::getLevel() { return level; }
    void NPC::setLevel(Level newLevel) { level = newLevel; }


    std::string NPC::getTypeString() {
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
    std::string NPC::getLevelString()
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
    Vec3& NPC::getPlayerPosition() { return playerPos; }

    Efficiency NPC::handleEfficiency(Type t1, Type t2)
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
    bool NPC::attack(std::unique_ptr<NPC>& target)
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
    NPC::NPC(Vec3& playerPos) : playerPos(playerPos) {}

extern "C" SCRIPT_API Engine::Scripting::NativeScript* CreateScript() {
    return new NPC(Vec3());
}