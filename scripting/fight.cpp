#include "player.hpp"
#include "npc.hpp"
#include "fight.hpp"

//cpp

#include "player.cpp"
#include "npc.cpp"


    
    void Fight::fight()
    {
		if (!player || !opponent)
		{
			std::cout << "Error: Player or opponent is null." << std::endl;
			return;
		}
        int runAwayCounter = 0;
        bool isRuningAway = false;

        bool isFightOver = false;
        std::cout << "Un dresseur vous attaque!" << std::endl
            << "Le dressseur envoie un " << opponent->getName() << " !" << std::endl;
        std::cin.get();
        std::cout << (*player->mapCubies.begin())->getName() << ", GO!" << std::endl;
        std::cin.get();

        while (isFightOver == false)
        {
            if (player->mapCubies.empty())
            {
				std::cout << "Game over! Votre equipe est decedee" << std::endl;
				break;
            }
            
                auto& playerCuby = *player->mapCubies.begin();
            
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
                isFightOver = playerCuby->attack(*opponent);
                if (isFightOver == true)
                {
                    std::cout << "Victoire!" << std::endl;
                    break;
                }
                if (isFightOver == false)
                {
                    if (player->mapCubies.size() > 0)
                    {
                        isFightOver = opponent->attack(*playerCuby);
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
    };



    Fight::Fight(std::unique_ptr<Player>& p, std::unique_ptr<NPC>& n) : player(p.get()), opponent(n.get())
    {

    }