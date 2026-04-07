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


// def mesh
Graphics::Mesh CreateCube()
{
    std::vector<float> vertices = {
        // FRONT (+Z)
        -0.5f,-0.5f, 0.5f,   0,0,1,   0,0,
         0.5f,-0.5f, 0.5f,   0,0,1,   1,0,
         0.5f, 0.5f, 0.5f,   0,0,1,   1,1,
        -0.5f, 0.5f, 0.5f,   0,0,1,   0,1,

        // BACK (-Z)
         0.5f,-0.5f,-0.5f,   0,0,-1,  0,0,
        -0.5f,-0.5f,-0.5f,   0,0,-1,  1,0,
        -0.5f, 0.5f,-0.5f,   0,0,-1,  1,1,
         0.5f, 0.5f,-0.5f,   0,0,-1,  0,1,

         // LEFT (-X)
         -0.5f,-0.5f,-0.5f,  -1,0,0,   0,0,
         -0.5f,-0.5f, 0.5f,  -1,0,0,   1,0,
         -0.5f, 0.5f, 0.5f,  -1,0,0,   1,1,
         -0.5f, 0.5f,-0.5f,  -1,0,0,   0,1,

         // RIGHT (+X)
          0.5f,-0.5f, 0.5f,   1,0,0,   0,0,
          0.5f,-0.5f,-0.5f,   1,0,0,   1,0,
          0.5f, 0.5f,-0.5f,   1,0,0,   1,1,
          0.5f, 0.5f, 0.5f,   1,0,0,   0,1,

          // TOP (+Y)
          -0.5f, 0.5f, 0.5f,   0,1,0,   0,0,
           0.5f, 0.5f, 0.5f,   0,1,0,   1,0,
           0.5f, 0.5f,-0.5f,   0,1,0,   1,1,
          -0.5f, 0.5f,-0.5f,   0,1,0,   0,1,

          // BOTTOM (-Y)
          -0.5f,-0.5f,-0.5f,   0,-1,0,  0,0,
           0.5f,-0.5f,-0.5f,   0,-1,0,  1,0,
           0.5f,-0.5f, 0.5f,   0,-1,0,  1,1,
          -0.5f,-0.5f, 0.5f,   0,-1,0,  0,1,
    };

    std::vector<uint32_t> indices = {
        0,1,2,  2,3,0,       // front
        4,5,6,  6,7,4,       // back
        8,9,10, 10,11,8,     // left
        12,13,14, 14,15,12,  // right
        16,17,18, 18,19,16,  // top
        20,21,22, 22,23,20   // bottom
    };

    Graphics::Mesh mesh;
    mesh.vertex_data = std::move(vertices);
    mesh.indices = std::move(indices);
    mesh.vertexCount = 24;

    mesh.ComputeBoundsAndCentroid();
    mesh.UploadBuffers();
    mesh.BuildVAO();

    return mesh;
}


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

        // Def mat
        Graphics::Material matRed;
        matRed.name = "Red Material";
        matRed.albedoMap = std::make_shared<Graphics::Texture2D>(1, 1, new uint8_t[3]{ 255, 0, 0});
        matRed.normalMap = std::make_shared<Graphics::Texture2D>(1, 1, new uint8_t[3]{ 128, 128, 255});
        matRed.metallicRoughnessMap = std::make_shared<Graphics::Texture2D>(1, 1, new uint8_t[3]{ 0, 128, 255});

		Graphics::Material matGreen;
		matGreen.name = "Green Material";
		matGreen.albedoMap = std::make_shared<Graphics::Texture2D>(1, 1, new uint8_t[3]{ 0, 255, 0 });
		matGreen.normalMap = std::make_shared<Graphics::Texture2D>(1, 1, new uint8_t[3]{ 128, 128, 255 });
		matGreen.metallicRoughnessMap = std::make_shared<Graphics::Texture2D>(1, 1, new uint8_t[3]{ 0, 128, 255 });

		Graphics::Material matBlue;
		matBlue.name = "Blue Material";
		matBlue.albedoMap = std::make_shared<Graphics::Texture2D>(1, 1, new uint8_t[3]{ 0, 0, 255 });
		matBlue.normalMap = std::make_shared<Graphics::Texture2D>(1, 1, new uint8_t[3]{ 128, 128, 255 });
		matBlue.metallicRoughnessMap = std::make_shared<Graphics::Texture2D>(1, 1, new uint8_t[3]{ 0, 128, 255 });

        //a
        auto sharedCube = std::make_shared<Graphics::Mesh>(CreateCube());
        sharedCube->upload();

        auto redMat = std::make_shared<Graphics::Material>(matRed);
        auto greenMat = std::make_shared<Graphics::Material>(matGreen);
        auto blueMat = std::make_shared<Graphics::Material>(matBlue);

        //b
        for (int x = 0; x < 50; x++)
        {
            for (int y = 0; y < 50; y++)
            {
                ECS::Entity e = registry->CreateEntity();

                registry->SetEntityName(e, "Map");

                //c
                registry->AddComponent(e, Components::Transform{});
                registry->AddComponent(e, Components::MeshRenderer{});

                auto& mr = registry->GetComponent<Components::MeshRenderer>(e);
                auto& tr = registry->GetComponent<Components::Transform>(e);

                mr.filePath = "Assets/Default/Cube_Mesh.pa";
                mr.meshes.push_back(sharedCube); //d

                switch (MapGeneration::getType(x, y))
                {
                case Fire:  mr.materials.push_back(redMat); break;
                case Grass: mr.materials.push_back(greenMat); break;
                case Water: mr.materials.push_back(blueMat); break;
                }

                mr.SetActive(true);

                tr.Position = glm::vec3(x, 0.0f, y);
            }
        }
        




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
