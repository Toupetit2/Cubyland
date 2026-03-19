/*
 * template_V3.cpp
 * A comprehensive "kitchen sink" template for the Photon Engine.
 * Updated for modern ECS Architecture.
 */

#include "pch.h" 
#include <json.hpp> 

#include "Noise2d.h"

 // Depending on your pch, you may need to ensure these namespaces are accessible
using json = nlohmann::json;

using namespace Engine;

class TemplateV3 : public Engine::Scripting::NativeScript {
public:
    // --- Public Configurable Variables ---
    

private:
    // --- System Pointers ---
    // Systems are safe to cache as they persist for the engine's lifetime
    Systems::FunctionRegisterySystem* funcRegistry = nullptr;
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
        funcRegistry = engine->GetSystem<Systems::FunctionRegisterySystem>();
        inputSystem = engine->GetSystem<Systems::InputSystem>();

        if (!funcRegistry && terminal) {
            terminal->error("TemplateV3: FunctionRegisterySystem not found!");
        }
        
        terminal->print("test123");
		terminal->print(Noise2d::PrintNoise()); 
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