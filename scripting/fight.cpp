#include "script_pch.h"
#include "player.cpp"
#ifdef _WIN32
#define SCRIPT_API __declspec(dllexport)
#else
#define SCRIPT_API __attribute__((visibility("default")))
#endif

class Fight : public Engine::Scripting::NativeScript
{
public:
    std::unique_ptr<Player>& player;
    std::unique_ptr<NPC>& opponent;
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
                isFightOver = playerCuby->attack(opponent);
                if (isFightOver == true)
                {
                    std::cout << "Victoire!" << std::endl;
                    break;
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
                        player->mapCubies.erase(player->mapCubies.begin());
                        std::cout << "Game over! Votre cuby est decede" << std::endl;
                        std::cout << "Taille de votre equipe: " << player->mapCubies.size();
                        break;
                    }
                }
            }
        }
    }

    Fight(std::unique_ptr<Player>& p, std::unique_ptr<NPC>& n) : player(p), opponent(n)
    {

    }
};
extern "C" SCRIPT_API Engine::Scripting::NativeScript* CreateFight() {
    auto player = std::make_unique<Player>();
    auto playerCuby = std::make_unique<NPC>(player->vector1);
    playerCuby->setType(Type::GRASS);
    playerCuby->setName("Vipeliere");
    playerCuby->setLevel(Level::Lv3);
    playerCuby->setATT(30);
    playerCuby->setHP(150);
    player->mapCubies.push_back(std::move(playerCuby));
    auto opponent = std::make_unique<NPC>(player->vector1);
    opponent->getPlayerPosition() = player->vector1;
    opponent->setType(Type::FIRE);
    opponent->setName("Pyroli");
    opponent->setLevel(Level::Lv3);
    opponent->setATT(30);
    opponent->setHP(150);
    return new Fight(player, opponent);
}
