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
		// Création de l'objet JSON
		json data;
		data["type"] = type;             // ex: "water"
		data["enemyLevel"] = enemyLevel; // ex: 2
		data["playerLevel"] = 1;         // ex: 5*
		data["playerXP"] = 0;           // ex: 1500



		// Ouverture et écriture dans le fichier json
		std::ofstream file("scripting/cubyData.json");
		if (file.is_open())
		{
			// dump(2) permet de formater le JSON avec une indentation de 2 espaces
			file << data.dump(2);
			file.close();
			std::cout << "Fichier cubyData.json mis a jour avec succes !" << std::endl;
		}
		else
		{
			std::cerr << "Erreur lors de l'ouverture du fichier cubyData.json pour l'ecriture !" << std::endl;
		}
	}

};



extern "C" SCRIPT_API Engine::Scripting::NativeScript* CreateScript() {
	return new Cuby2();
}