#include "script_pch.h"
#include <memory>
#include <vector>
#include <iostream>
#include <string>
#ifdef _WIN32
#define SCRIPT_API __declspec(dllexport)
#else
#define SCRIPT_API __attribute__((visibility("default")))
#endif
enum Type { FIRE, WATER, GRASS };
enum Level { Lv1, Lv2, Lv3 };
enum Efficiency { NOT, EFFICIENT, VERY };

class Vec3 {
public:
    int x;
    int y;
    int z;
};

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
    bool wantsToRunAway = false;
    int runAwayCounter = 2;
public:
    
    int getATT() { return ATT; }
    void setATT(int newATT) { ATT = newATT; }

    int getHP() { return HP; }
    void setHP(int newHP) { HP = newHP; }

    int getXP() { return XP; }
    void setXP(int newXP) { XP = newXP; }

    bool isCuby() { return isCubyVar; }
    void setIsCuby(bool newStatus) { isCubyVar = newStatus; }

    std::string NPC::getName() { return name; }
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

    bool runningAway()
    {
        runAwayCounter--;
        if (runAwayCounter == 0)
        {
            runAwayCounter = 2;
            return true;
        }
        else
            return false;
    }

    NPC(Vec3& playerPos) : playerPos(playerPos) {}

    bool getWantsToRunAway()
    {
        return wantsToRunAway;
    }
    void toggleWantsToRunAway()
    {
        if (wantsToRunAway)
            wantsToRunAway = false;
        else
            wantsToRunAway = true;
    }


};

class Player : public Engine::Scripting::NativeScript
{
public:
    std::vector<std::unique_ptr<NPC>> mapCubies;
    Vec3 vector1;
    void  getComponent() {}
    void  setComponent() {}


    void  move() {}
    void  update() {}


};



class Fight : public Engine::Scripting::NativeScript
{
public:
    void OnInit() override { };
    void OnCreate() override {};
    void OnUpdate(float dt) override {};
    void fight()
    {
        int runAwayCounter = 0;
        bool isRuningAway = false;

        bool isFightOver = false;
        std::cout << "Un dresseur vous attaque!" << std::endl
            << "Le dressseur envoie un " << opponent->getName() << " !" << std::endl;
        std::cin.get();
        std::cout << player->mapCubies.begin()->get()->getName() << ", GO!" << std::endl;
        std::cin.get();

        while (isFightOver == false)
        {
            auto& playerCuby = *player->mapCubies.begin();
            if (!(player->mapCubies.empty()))
            {
                std::cout << "Votre cuby: \n" <<
                    "Nom: " << playerCuby->getName() << "\n"
                    "Niveau: " << playerCuby->getLevelString() << "\n"
                    "Type: " << playerCuby->getTypeString() << "\n"
                    "HP: " << playerCuby->getHP() << "\n"
                    "ATT: " << playerCuby->getATT() << "\n" << std::endl;
                std::cout << "--------------------------------------------" << std::endl;
                std::cout << "Cuby ennemi: \n" <<
                    "Nom: " << opponent->getName() << "\n"
                    "Niveau: " << opponent->getLevelString() << "\n"
                    "Type: " << opponent->getTypeString() << "\n"
                    "HP: " << opponent->getHP() << "\n"
                    "ATT: " << opponent->getATT() << "\n" << std::endl;
                if (!playerCuby->getWantsToRunAway())
                {
                    printControls(playerCuby, opponent);
                }
                else
                {
                    if (playerCuby->runningAway())
                        isFightOver = true;
                }

                if (playerCuby->getHP() <= 0 || opponent->getHP() <= 0)
                {
                    isFightOver = true;
                }
                if (isFightOver == false)
                {
                    if (player->mapCubies.size() > 0)
                    {
                        isFightOver = opponent->attack(playerCuby);
                    }
                }
                if (isFightOver == true)
                {
                    if (!(player->mapCubies.empty()))
                    {
                        if (playerCuby->getHP() < 0)
                        {
                            player->mapCubies.erase(player->mapCubies.begin());
                            std::cout << "Game over! Votre cuby est decede" << std::endl;
                            std::cout << "Taille de votre equipe: " << player->mapCubies.size();
                            return;
                        }
                        else if (playerCuby->getWantsToRunAway() == false)
                        {
                            std::cout << "Victoire!" << std::endl;
                            return;
                        }
                        else
                        {
                            std::cout << "Vous avez fuit comme une poule mouillee, gros noob" << std::endl;
                        }
                    }
                }
            }
        }
    }
    void printControls(std::unique_ptr<NPC>& attacker, std::unique_ptr<NPC>& defender)
    {
        bool completed = false;
        char command = '0';

        while (!completed)
        {
            std::cout << "A: attaquer \n E: Fuite" << std::endl;
            std::cin >> command;
            if (command != 'A' && command != 'a' && command != 'E' && command != 'e')
            {
                std::cout << "Erreur: commande non valide." << std::endl;
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }
            else if (command == 'A' || command == 'a')
            {
                completed = true;
                attacker->attack(defender);
                break;
            }
            else if (command == 'E' || command == 'e')
            {
                completed = true;
                attacker->toggleWantsToRunAway();
                break;
            }
        }
    }
    Fight(std::unique_ptr<Player>& p, std::unique_ptr<NPC>& n) : player(p), opponent(n)
    {
    }

    std::unique_ptr<Player>& player;
    std::unique_ptr<NPC>& opponent;
    
};



//void Fight::OnInit() override
//{
//}
//
//void Fight::OnCreate() override
//{
//    fight();
//}
//
//void Fight::OnUpdate(float dt) override
//{
//
//}


extern "C" SCRIPT_API Engine::Scripting::NativeScript* CreateScript() {
    auto player = std::make_unique<Player>();
    auto playerCuby = std::make_unique<NPC>(player->vector1);
    playerCuby->setType(Type::FIRE);
    playerCuby->setName("Reshiram");
    playerCuby->setLevel(Level::Lv3);
    playerCuby->setATT(30);
    playerCuby->setHP(150);
    player->mapCubies.push_back(std::move(playerCuby));
    auto opponent = std::make_unique<NPC>(player->vector1);
    opponent->getPlayerPosition() = player->vector1;
    opponent->setType(Type::GRASS);
    opponent->setName("Tournegrain");
    opponent->setLevel(Level::Lv3);
    opponent->setATT(30);
    opponent->setHP(150);
    return new Fight(player, opponent);
}
