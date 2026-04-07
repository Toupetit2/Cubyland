#include "script_pch.h"
#include <memory>
#include <vector>
#include <iostream>
#include <string>
#include <imgui.h>
#include "Systems/FunctionRegistrySystem.h"
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

    void DrawHUD() {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoFocusOnAppearing;
        ImGui::Begin("HUD", nullptr, window_flags);
        
        if (showDialogueWindow)
        {
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Un dresseur vous attaque!");
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "Le dressseur envoie un %s !", opponent->getName().c_str());
			ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "%s, GO!", player->mapCubies.front()->getName().c_str());
			if (ImGui::Button("Suivant"))
			{
                showStats = true;
				showDialogueWindow = false;
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
			ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Victoire!");
            if (ImGui::Button("Terminer le combat") && isFightOver)
            {
                showAttackingDialogue = false;
                showStats = false;
            }
		}
		if (lose)
		{
			ImGui::TextColored(ImVec4(1.0f, 0.0f, 0.0f, 1.0f), "Game over! Votre cuby est decede");
            if (ImGui::Button("Terminer le combat") && isFightOver)
            {
                enemyTurn = false;
                showStats = false;
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
    
    Fight(std::unique_ptr<Player> p, std::unique_ptr<NPC> n)
        : player(std::move(p)), opponent(std::move(n))
    {
    }

    std::unique_ptr<Player> player;
    std::unique_ptr<NPC> opponent;

};

extern "C" SCRIPT_API Engine::Scripting::NativeScript* CreateScript() {
    auto player = std::make_unique<Player>();
    auto playerCuby = std::make_unique<NPC>(player->vector1);
    playerCuby->setType(Type::FIRE);
    playerCuby->setName("Roitiflam");
    playerCuby->setLevel(Level::Lv3);
    playerCuby->setATT(30);
    playerCuby->setHP(150);
    player->mapCubies.push_back(std::move(playerCuby));
    auto opponent = std::make_unique<NPC>(player->vector1);
    opponent->getPlayerPosition() = player->vector1;
    opponent->setType(Type::GRASS);
    opponent->setName("Majaspic");
    opponent->setLevel(Level::Lv3);
    opponent->setATT(30);
    opponent->setHP(150);
    return new Fight(std::move(player), std::move(opponent));
}