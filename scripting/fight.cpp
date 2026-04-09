#include "script_pch.h"
#include <memory>
#include <vector>
#include <iostream>
#include <string>
#include <imgui.h>
#include "json.hpp"
#include "Systems/FunctionRegistrySystem.h"
#include <fstream>
#ifdef _WIN32
#define SCRIPT_API __declspec(dllexport)
#else
#define SCRIPT_API __attribute__((visibility("default")))
#endif
using json = nlohmann::json;

enum Type { FIRE, WATER, GRASS };
enum Level { Lv1, Lv2, Lv3 };
enum Efficiency { NOT, EFFICIENT, VERY };

class NPC : public Engine::Scripting::NativeScript
{
private:
    int ATT = 0;
    int HP = 0;
    int XP = 0;
    bool isCubyVar = false;
    std::string name = "NAME";
    Type type = FIRE;
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
            return "Feu";
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
            return "1";
            break;
        }
    }

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
            return Efficiency::EFFICIENT;
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
       // std::cout << name << " attaque!" << std::endl;
        // std::cin.get();
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
          //  std::cout << efficiencyMessage << std::endl;
            // std::cin.get();
        }
        //std::cout << name << " inflige " << damages << " degats a " << target->name << " !" << std::endl;
        target->HP -= damages;
        if (target->getHP() <= 0)
        {
           // std::cout << target->getName() << " est mort!" << std::endl;
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
    void  getComponent() {}
    void  setComponent() {}


    void  move() {}
    void  update() {}


};



class Fight : public Engine::Scripting::NativeScript
{
public:
	bool showStats = false;
    bool showDialogueWindow = true;
    bool showAttackingDialogue = false;
	bool showFleeingDialogue = false;
	bool enemyTurn = false;
    int runAwayCounter = 2;
    bool isRuningAway = false;
    bool isFightOver = false;
	bool win = false;
	bool lose = false;
	bool ranAway = false;
    bool showSuivant = true;
	bool showSuivantSelectionCuby = false;

    void UpdateCubyData(int enemyLv, int playerLv, int xp, const std::string& type) {
        json data;
        data["enemyLevel"] = enemyLv;
        data["playerLevel"] = playerLv;
        data["playerXP"] = xp;
        data["type"] = type;

        std::string path = "scripting/cubyData.json";

        std::ofstream file(path);
        if (file.is_open()) {
            file << std::setw(4) << data << std::endl;
            file.close();
		}
        else {
            std::cerr << "Erreur lors de l'ouverture du fichier " << path << " pour l'écriture!" << std::endl;
        }
    }

    void DrawHUD() {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing;
        ImGui::Begin("HUD", nullptr, window_flags);

        if (showDialogueWindow)
        {
            std::string label1 = "Type Feu: " + cubyFire.getName();
			std::string label2 = "Type Eau: " + cubyWater.getName();
			std::string label3 = "Type Plante: " + cubyGrass.getName();
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Un dresseur vous attaque!");
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Le dressseur envoie un %s !", opponent->getName().c_str());
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Choisissez votre cuby!");
			if (ImGui::Button(label1.c_str()))
			{
                player->mapCubies.front() = std::make_unique<NPC>(cubyFire);
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s, GO!", player->mapCubies.front()->getName().c_str());
				showSuivantSelectionCuby = true;
			}
            if (ImGui::Button(label2.c_str()))
            {
				player->mapCubies.front() = std::make_unique<NPC>(cubyWater);
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s, GO!", player->mapCubies.front()->getName().c_str());
				showSuivantSelectionCuby = true;
            }
			if (ImGui::Button(label3.c_str()))
			{
				player->mapCubies.front() = std::make_unique<NPC>(cubyGrass);
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s, GO!", player->mapCubies.front()->getName().c_str());
				showSuivantSelectionCuby = true;
			}
			
        }
		if (showSuivantSelectionCuby)
		{
			if (ImGui::Button("Commencer le combat"))
			{
				showDialogueWindow = false;
				showSuivantSelectionCuby = false;
				showStats = true;
			}
		}
		if (showStats)
        {
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Nom de votre cuby: %s", player->mapCubies.front()->getName());
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Niveau de votre cuby: %s", player->mapCubies.front()->getLevelString());
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Type de votre cuby: %s", player->mapCubies.front()->getTypeString());
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "PV de votre cuby: %d", player->mapCubies.front()->getHP());
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Attaque de votre cuby: %d", player->mapCubies.front()->getATT());
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Nom Ennemi: %s", opponent->getName());
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Niveau Ennemi: %s", opponent->getLevelString());
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Type Ennemi: %s", opponent->getTypeString());
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "PV Ennemi: %d", opponent->getHP());
            ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Attaque Ennemi: %d", opponent->getATT());
            if (!(player->mapCubies.front()->getWantsToRunAway()))
            {
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Choisissez une action");
            }
            if (!player->mapCubies.front()->getWantsToRunAway())
            {
                if (ImGui::Button("Attaquer"))
                {
                    showStats = false;
                    player->mapCubies.front()->attack(opponent);
                    showAttackingDialogue = true;

                }
                if (ImGui::Button("Fuir"))
                {
					showStats = false;
					enemyTurn = true;
                    opponent->attack(player->mapCubies.front());
                    player->mapCubies.front()->toggleWantsToRunAway();
                }
            }
            else if (runAwayCounter > 0)
            {
                if (ImGui::Button("Vous essayez de fuir..."))
                {
                    opponent->attack(player->mapCubies.front());
                    enemyTurn = true;
					showStats = false;
                    runAwayCounter--;
                }
				
            }
			else if (runAwayCounter <= 0)
            {
                ranAway = true;
                isFightOver = true;
            }
        }
        if (showAttackingDialogue)
        {
			int damages = player->mapCubies.front()->getATT();
            switch (handleEfficiency(player->mapCubies.front()->getType(), opponent->getType()))
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
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s attaque!", player->mapCubies.front()->getName().c_str());
            switch (player->mapCubies.front()->handleEfficiency(player->mapCubies.front()->getType(), opponent->getType()))
            {
            case Efficiency::NOT:
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Ce n'est pas tres efficace...");
                break;
            case Efficiency::VERY:
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "C'est super efficace!");
                break;
            default:
                break;
            }
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s inflige %d degats a %s!", player->mapCubies.front()->getName().c_str(), damages, opponent->getName().c_str());
			if (opponent->getHP() <= 0)
			{
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s est mort!", opponent->getName().c_str());
				isFightOver = true;
				win = true;
				showSuivant = false;
			}
			if (showSuivant)
            {
                if (ImGui::Button("Suivant") && !isFightOver)
                {
                    showAttackingDialogue = false;
                    if (!isFightOver)
                    {
                        enemyTurn = true;
                        opponent->attack(player->mapCubies.front());
                    }
                    else
                    {
                        showStats = false;
                    }
                }
            }
           
           
        }
        if (enemyTurn)
        {
			int damages = opponent->getATT();
            switch (handleEfficiency(opponent->getType(), player->mapCubies.front()->getType()))
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
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s attaque!", opponent->getName().c_str());
            switch (opponent->handleEfficiency(opponent->getType(), player->mapCubies.front()->getType()))
            {
            case Efficiency::NOT:
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Ce n'est pas tres efficace...");
                break;
            case Efficiency::VERY:
                ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "C'est super efficace!");
                break;
            default:
                break;
            }
            ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s inflige %d degats a %s!", opponent->getName().c_str(), damages, player->mapCubies.front()->getName().c_str());
			if (player->mapCubies.front()->getHP() <= 0)
			{
				ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s est mort!", player->mapCubies.front()->getName().c_str());
				isFightOver = true;
				lose = true;
				showSuivant = false;
			}
			if (showSuivant)
            {
                if (ImGui::Button("Suivant") && !isFightOver)
                {
                    enemyTurn = false;
                    {
                        if (!isFightOver)
                        {
                            showStats = true;
                        }
                        else
                        {
                            showStats = false;
                        }
                    }
                }
            }
          
        }
        if(win)
		{
            if (player->mapCubies.front()->getLevel() < Level::Lv3)
            {
                ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Vous gagnez 50 XP!");

                if (player->mapCubies.front()->getXP() + 50 >= 100)
                {
                    ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Votre cuby a gagne un niveau!");
                }
            }

            if (ImGui::Button("Terminer le combat") && isFightOver)
            {
                showAttackingDialogue = false;
                showStats = false;

                if (player->mapCubies.front()->getLevel() < Level::Lv3)
                {
                    player->mapCubies.front()->setXP(player->mapCubies.front()->getXP() + 50);

                    if (player->mapCubies.front()->getXP() >= 100)
                    {
                        int nextLevel = static_cast<int>(player->mapCubies.front()->getLevel()) + 1;
                        player->mapCubies.front()->setLevel(static_cast<Level>(std::min(nextLevel, static_cast<int>(Level::Lv3))));
                        player->mapCubies.front()->setXP(0);
                    }
                }

				UpdateCubyData(std::stoi(opponent->getLevelString()), std::stoi(player->mapCubies.front()->getLevelString()), player->mapCubies.front()->getXP(), player->mapCubies.front()->getTypeString());
                engine->GetSystem<Engine::Systems::SceneSerializerSystem>()->RequestSceneDeserialization("Assets/Scenes/LV_World.pscene");
            }
		}
		if (lose)
		{
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Game over! Votre cuby est decede");
            if (ImGui::Button("Terminer le combat") && isFightOver)
            {
                enemyTurn = false;
                showStats = false;
                UpdateCubyData(std::stoi(opponent->getLevelString()), std::stoi(player->mapCubies.front()->getLevelString()), player->mapCubies.front()->getXP(), player->mapCubies.front()->getTypeString());
                engine->GetSystem<Engine::Systems::SceneSerializerSystem>()->RequestSceneDeserialization("Assets/Scenes/LV_World.pscene");
            }
		}
		if (ranAway)
		{
			showStats = false;
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Vous avez fuit comme une poule mouillee, gros noob");
			if (ImGui::Button("Terminer le combat") && isFightOver)
			{
				showAttackingDialogue = false;
				showStats = false;
                UpdateCubyData(std::stoi(opponent->getLevelString()), std::stoi(player->mapCubies.front()->getLevelString()), player->mapCubies.front()->getXP(), player->mapCubies.front()->getTypeString());
                engine->GetSystem<Engine::Systems::SceneSerializerSystem>()->RequestSceneDeserialization("Assets/Scenes/LV_World.pscene");
			}
		}
        ImGui::End();
    }
    bool execute = true;
    void OnInit() override { 
        if (Engine::Systems::ImGuiSystem* ImGuiSystem = engine->GetSystem<Engine::Systems::ImGuiSystem>()) {
            ImGuiSystem->RegisterUICallback("DrawPlayerHUD", [this]() {
                this->DrawHUD();
                });
        }

    };
    void OnDestroy() override {
        if (Engine::Systems::ImGuiSystem* ImGuiSystem = engine->GetSystem<Engine::Systems::ImGuiSystem>()) {
            ImGuiSystem->UnregisterUICallback("DrawPlayerHUD");
        }

    }
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
            return Efficiency::EFFICIENT;
            break;
        }
    }
    
    std::unique_ptr<Player> player;
    std::unique_ptr<NPC> opponent;
    NPC cubyFire;
    NPC cubyWater;
    NPC cubyGrass;

    Fight(std::unique_ptr<Player> p, std::unique_ptr<NPC> n, NPC cf, NPC cw, NPC cg)
		: player(std::move(p)), opponent(std::move(n)), cubyFire(cf), cubyWater(cw), cubyGrass(cg)
    {
    }

    


};

extern "C" SCRIPT_API Engine::Scripting::NativeScript* CreateScript() {
    auto player = std::make_unique<Player>();
    auto playerCuby = std::make_unique<NPC>();
    player->mapCubies.push_back(std::make_unique<NPC>());
	auto opponent = std::make_unique<NPC>();
	NPC cubyFire = NPC();
	NPC cubyWater = NPC();
	NPC cubyGrass = NPC();
	cubyFire.setType(Type::FIRE);
	cubyWater.setType(Type::WATER);
	cubyGrass.setType(Type::GRASS);

    std::ifstream file("scripting/cubyData.json");
    if (!file.is_open()) {
		std::cout << "Erreur lors de l'ouverture du fichier!" << std::endl;
        return nullptr;
    }

    json data;
    file >> data;

    std::string type;
    int enemyLevel;
    int playerLevel;
    int playerXP;

    
    type = data.at("type").get<std::string>();
    enemyLevel = data.at("enemyLevel").get<int>();
    playerLevel = data.at("playerLevel").get<int>();
    playerXP = data.at("playerXP").get<int>();

	




	if (type == "fire")
	{
		opponent->setType(Type::FIRE);
	}
	else if (type == "water")
	{
		opponent->setType(Type::WATER);
	}
	else if (type == "grass")
	{
		opponent->setType(Type::GRASS);
	}


	switch (enemyLevel)
	{
	case 1:
		opponent->setLevel(Level::Lv1);
		break;
	case 2:
		opponent->setLevel(Level::Lv2);
		break;
	case 3:
		opponent->setLevel(Level::Lv3);
		break;
	default:
		opponent->setLevel(Level::Lv1);
		break;
	}
	switch (playerLevel)
	{
	case 1:
		player->mapCubies.front()->setLevel(Level::Lv1);
		cubyFire.setLevel(Level::Lv1);
		cubyWater.setLevel(Level::Lv1);
		cubyGrass.setLevel(Level::Lv1);
		break;
	case 2:
		player->mapCubies.front()->setLevel(Level::Lv2);
		cubyFire.setLevel(Level::Lv2);
		cubyWater.setLevel(Level::Lv2);
		cubyGrass.setLevel(Level::Lv2);
		break;
	case 3:
		player->mapCubies.front()->setLevel(Level::Lv3);
		cubyFire.setLevel(Level::Lv3);
		cubyWater.setLevel(Level::Lv3);    
		cubyGrass.setLevel(Level::Lv3);
		break;
	default:
		player->mapCubies.front()->setLevel(Level::Lv1);
        cubyFire.setLevel(Level::Lv1);
        cubyWater.setLevel(Level::Lv1);
        cubyGrass.setLevel(Level::Lv1);
		break;
	}
	
	if (opponent->getLevel() == Level::Lv1)
	{
		opponent->setHP(50);
		opponent->setATT(10);
	}
	else if (opponent->getLevel() == Level::Lv2)
	{
		opponent->setHP(100);
		opponent->setATT(20);
	}
	else if (opponent->getLevel() == Level::Lv3)
	{
		opponent->setHP(150);
		opponent->setATT(30);
	}
	if (player->mapCubies.front()->getLevel() == Level::Lv1)
	{
		cubyFire.setHP(50);
		cubyFire.setATT(10);
		cubyWater.setHP(50);
		cubyWater.setATT(10);
		cubyGrass.setHP(50);
		cubyGrass.setATT(10);
	}
	else if (player->mapCubies.front()->getLevel() == Level::Lv2)
	{
		cubyFire.setHP(100);
		cubyFire.setATT(20);
		cubyWater.setHP(100);
		cubyWater.setATT(20);
		cubyGrass.setHP(100);
		cubyGrass.setATT(20);
	}
	else if (player->mapCubies.front()->getLevel() == Level::Lv3)
	{
		cubyFire.setHP(150);
		cubyFire.setATT(30);
		cubyWater.setHP(150);
		cubyWater.setATT(30);
		cubyGrass.setHP(150);
		cubyGrass.setATT(30);
	}

	//noms en fonction du type et du niveau
	if (playerLevel == 1)
	{
		cubyFire.setName("Salamouche");
		cubyWater.setName("Carapunaise");
		cubyGrass.setName("Bulbchelou");
	}
	else if (playerLevel == 2)
	{
		cubyFire.setName("Reptinpoivre");
		cubyWater.setName("Caratarte");
		cubyGrass.setName("Herbchelou");
	}
	else if (playerLevel == 3)
	{
		cubyFire.setName("Dracoflamme");
		cubyWater.setName("Torchar");
		cubyGrass.setName("Florichelou");
	}
    if (type == "fire")
    {
		if (enemyLevel == 1)
		{
			opponent->setName("Salamouche");
		}
		else if (enemyLevel == 2)
		{
			opponent->setName("Reptinpoivre");
		}
		else if (enemyLevel == 3)
		{
			opponent->setName("Dracoflamme");
		}
    }
	else if (type == "water")
	{
		if (enemyLevel == 1)
		{
			opponent->setName("Carapunaise");
		}
		else if (enemyLevel == 2)
		{
			opponent->setName("Caratarte");
		}
		else if (enemyLevel == 3)
		{
			opponent->setName("Torchar");
		}
	}
	else if (type == "grass")
	{
		if (enemyLevel == 1)
		{
			opponent->setName("Bulbchelou");
		}
		else if (enemyLevel == 2)
		{
			opponent->setName("Herbchelou");
		}
		else if (enemyLevel == 3)
		{
			opponent->setName("Florichelou");
		}
	}
    player->mapCubies.front()->setXP(playerXP);
	cubyFire.setXP(playerXP);
	cubyWater.setXP(playerXP);
	cubyGrass.setXP(playerXP);

    return new Fight(std::move(player), std::move(opponent), cubyFire, cubyWater, cubyGrass);
}