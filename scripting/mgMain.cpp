#include "script_pch.h" 
#include <json.hpp> 

#include "mgNoise2d.h"


#include "Types.h"

class MapGeneration
{
public:
	static Types getType(float posX, float posY)
	{
		float fireValue = Noise2d::GetNoiseValue(posX, posY);
		float grassValue = Noise2d::GetNoiseValue(posX + 10000, posY); // valeur random pour avoir un autre noise
		float waterValue = Noise2d::GetNoiseValue(posX + 39587, posY);

		if (fireValue > grassValue && fireValue > waterValue)
		{
			return Fire;
		}
		else if (grassValue > waterValue)
		{
			return Grass;
		}
		return Water;
	}

	static std::string drawMap()
	{
		std::string noiseString;

		int width = 80;
		int height = 40;
		float scale = 0.1f;

		for (int y = 0; y < height; y++)
		{
			for (int x = 0; x < width; x++)
			{
				if (getType(x / 6, y / 6) == Fire)
				{
					noiseString += "F";
				}
				else if (getType(x / 6, y / 6) == Water)
				{
					noiseString += "W";
				}
				else
				{
					noiseString += "G";
				}
			}
			noiseString += "\n";
		}
		return noiseString;
	}
private:
};

 // Depending on your pch, you may need to ensure these namespaces are accessible
using json = nlohmann::json;

using namespace Engine;

class TemplateV3 : public Engine::Scripting::NativeScript {
public:
    // --- Public Configurable Variables ---


private:
    // --- System Pointers ---
    // Systems are safe to cache as they persist for the engine's lifetime
    Systems::FunctionRegistrySystem* funcRegistry = nullptr;
    Systems::TerminalSystem* terminal = nullptr;
    Systems::InputSystem* inputSystem = nullptr; // Assuming this matches the new System architecture

public:
    // --- ========================== ---
    // --- SCRIPT LIFECYCLE METHODS   ---
    // --- ========================== ---

    void OnCreate() override {
        
        
        // --- 1. Get Core Systems ---
        // Retrieved via the new engine system manager
        terminal = engine->GetSystem<Systems::TerminalSystem>();
        funcRegistry = engine->GetSystem<Systems::FunctionRegistrySystem>();
        inputSystem = engine->GetSystem<Systems::InputSystem>();

        if (!funcRegistry && terminal) {
            terminal->error("TemplateV3: FunctionRegisterySystem not found!");
        }

        terminal->print("test123");
        terminal->print(Noise2d::PrintNoise());
        terminal->print(MapGeneration::drawMap());

        ECS::Entity e = registry->CreateEntity();
        registry->SetEntityName(e, "EntityTest_1");
        registry->AddComponent(e, Components::Transform{});
        registry->AddComponent(e, Components::MeshRenderer{});
        
        registry->GetComponent<Components::MeshRenderer>(e).filePath = "Assets/Default/Cube_Mesh.pa";
        registry->GetComponent<Components::MeshRenderer>(e).SetActive(true);

        std::cout << registry->GetComponent<Components::MeshRenderer>(e).GetModelCount();

        /*
        std::vector<ECS::Entity> entityVector; 

        for (int x = 0; x < 50; x++)
        {
            for (int y = 0; y < 50; y++)
            {
                entityVector.push_back(registry->CreateEntity());
                registry->SetEntityName(entityVector.back, "Map");
                //registry->AddComponent(e, new Components::Transform);
                //registry->AddComponent(e, new Components::MeshRenderer);
                //registry->AddComponent(e, new Components::RigidBody);
                //changer materiau en fonction de -> MapGeneration::getType(x, y);
                //glm::vec3 position(x, y, 0.0f);
                //registry->GetComponent<Components::Transform>(e).Position = position;
                //
            }
        }
        */




        //glm::vec3 position(1.0f, 2.0f, 3.0f);
        //registry->GetComponent<Components::Transform>(e).Position = position;

        //Components::MeshRenderer& meshRenderer = engine->GetRegistry().GetComponent<Components::MeshRenderer>(entity);
        //meshRenderer.filePath = "test";
        
    }

    void OnUpdate(float deltaTime) override {
        // update
    }

    void OnDestroy() override {
        // destroy
    }

public:
    // --- =================================== ---
    // --- PUBLIC METHODS (for Registry)       ---
    // --- =================================== ---


private:
    // --- ======================== ---
    // --- PRIVATE HELPER METHODS   ---
    // --- ======================== ---

};

extern "C" __declspec(dllexport) Engine::Scripting::NativeScript* CreateScript() {
    return new TemplateV3();
}
