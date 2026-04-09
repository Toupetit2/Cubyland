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


class Cuby2 : public Engine::Scripting::NativeScript {
public:

	Engine::ECS::Entity targetPlayer = Engine::ECS::NULL_ENTITY;
	Engine::ECS::Entity targetSelf = Engine::ECS::NULL_ENTITY;

	int attaque = 0;
	int hp = 0;
	std::string name = "robert2";
	std::string type = "water";
	int lv = 2;


	void OnInit() override {

	}

	void OnCreate() override {
		FindTarget();
		FindSelf();
	}

	void FindTarget() {
		for (auto e : registry->View<Engine::Components::Transform>()) {
			if (registry->GetEntityName(e) == "player") {
				targetPlayer = e;
				std::cout << "player trouver \n";
				break;
			}
		}
	}

	void FindSelf() {
		for (auto e : registry->View<Engine::Components::Transform>()) {
			if (registry->GetEntityName(e) == "cuby2") {
				targetSelf = e;
				std::cout << "cuby2 trouver \n";
				break;
			}
		}
	}



	void OnUpdate(float dt) override {

		auto& playerTransform = registry->GetComponent<Engine::Components::Transform>(targetPlayer);
		auto& selfTransform = registry->GetComponent<Engine::Components::Transform>(targetSelf);


		float distance = glm::distance(playerTransform.Position, selfTransform.Position);

		if (distance <= 0.3) {
			
			UpdateCubyData(type, lv);
			engine->GetSystem<Engine::Systems::SceneSerializerSystem>()->RequestSceneDeserialization("Assets/Scenes/LV_Fight2.pscene");
		}



	}


	void UpdateCubyData(const std::string& type, int enemyLevel)
	{
		std::string filePath = "scripting/cubyData.json";
		json data;

		std::ifstream inputFile(filePath);
		if (inputFile.is_open())
		{
			try {
				inputFile >> data;
				inputFile.close();
			}
			catch (json::parse_error& e) {
				// Si le fichier est corrompu ou vide, on initialise des valeurs de secours
				std::cerr << "Erreur de lecture JSON, initialisation par defaut." << std::endl;
				data["playerLevel"] = 1;
				data["playerXP"] = 0;
			}
		}
		else
		{
			//Securite si le fichier n'existe pas, on le cree avec des valeurs par defaut (probablement useless)
			data["playerLevel"] = 1;
			data["playerXP"] = 0;
		}

		data["type"] = type;
		data["enemyLevel"] = enemyLevel;

		std::ofstream outputFile(filePath);
		if (outputFile.is_open())
		{
			outputFile << data.dump(2);
			outputFile.close();
			std::cout << "Fichier cubyData.json mis a jour (donnees joueur preservees) !" << std::endl;
		}
		else
		{
			std::cerr << "Erreur lors de l'ouverture du fichier pour l'ecriture !" << std::endl;
		}
	}

};



extern "C" SCRIPT_API Engine::Scripting::NativeScript* CreateScript() {
	return new Cuby2();
}