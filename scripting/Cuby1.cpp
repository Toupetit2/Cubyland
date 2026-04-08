#include "script_pch.h"
#include <iostream>
#include <string>

#ifdef _WIN32
#define SCRIPT_API __declspec(dllexport)
#else
#define SCRIPT_API __attribute__((visibility("default")))
#endif




class Cubies : public Engine::Scripting::NativeScript {
public : 

	Engine::ECS::Entity targetPlayer = Engine::ECS::NULL_ENTITY;
	Engine::ECS::Entity targetSelf = Engine::ECS::NULL_ENTITY;
	
	int attaque = 0;
	int hp = 0;
	std::string name = "robert";
	std::string type = "feu";
	int lv = 1;


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
			if (registry->GetEntityName(e) == "cuby1") {
				targetSelf = e;
				std::cout << "cuby1 trouver \n";
				break;
			}
		}
	}

	
	
	void OnUpdate(float dt) override {

		auto& playerTransform = registry->GetComponent<Engine::Components::Transform>(targetPlayer);
		auto& selfTransform = registry->GetComponent<Engine::Components::Transform>(targetSelf);
		

		float distance = glm::distance(playerTransform.Position, selfTransform.Position);
		
		if(distance <= 0.3) {
			std::cout << "MERDE \n";
		}

	

	}


};



extern "C" SCRIPT_API Engine::Scripting::NativeScript* CreateScript() {
	return new Cubies();
}
