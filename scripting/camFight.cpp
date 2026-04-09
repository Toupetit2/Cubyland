#include "script_pch.h"
#include "Systems/AnimatorSystem.h" // Include AnimatorSystem to control weights
#include <iostream>

#ifdef _WIN32
#define SCRIPT_API __declspec(dllexport)
#else
#define SCRIPT_API __attribute__((visibility("default")))
#endif

class camFight : public Engine::Scripting::NativeScript {
public : 

    Engine::ECS::Entity targetEntity = Engine::ECS::NULL_ENTITY;

    float distance = 0.2f;
    float targetHeightOffset = 0.1f;

    void OnInit() override {
        
        
    }
    

    void OnCreate() override {
        FindTarget();
    }


    void FindTarget() {
        for (auto e : registry->View<Engine::Components::Transform>()) {
            if (registry->GetEntityName(e) == "camObj") {
                targetEntity = e;
                std::cout << "nique ta mere 1\n";
                break;
            }
        }
    }

    void OnUpdate(float dt) override {    

        //std::cout << "nique ta mere 2\n";
        auto& camTransform = registry->GetComponent<Engine::Components::Transform>(entityID);
        auto& targetTransform = registry->GetComponent<Engine::Components::Transform>(targetEntity);
        
        glm::mat4 rotationMatrix = glm::mat4(1.0f);
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(camTransform.Rotation.y), glm::vec3(0, 1, 0));
        rotationMatrix = glm::rotate(rotationMatrix, glm::radians(camTransform.Rotation.x), glm::vec3(1, 0, 0));
        
        camTransform.Forward = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));

        glm::vec3 flatForward = glm::normalize(glm::vec3(camTransform.Forward.x, 0.0f, camTransform.Forward.z));


        glm::vec3 targetFocusPos = targetTransform.Position + glm::vec3(0.0f, targetHeightOffset, 0.0f);
        camTransform.Position = targetFocusPos - (camTransform.Forward * distance);
    }

};

extern "C" SCRIPT_API Engine::Scripting::NativeScript* CreateScript() {
    return new camFight();
}