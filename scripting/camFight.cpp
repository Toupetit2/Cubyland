#include "script_pch.h"
#include "Systems/AnimatorSystem.h"
#include "Systems/RenderSystem.h" 

#ifdef _WIN32
#define SCRIPT_API __declspec(dllexport)
#else
#define SCRIPT_API __attribute__((visibility("default")))
#endif

class camFight : public Engine::Scripting::NativeScript {
public:
    float distance = 1.5f;
    float sensitivity = 0.1f;
    float targetHeightOffset = 0.1f;

    float yaw = 0.0f;
    float pitch = 20.0f;

    bool firstFrame = true;

    Engine::ECS::Entity targetEntity = Engine::ECS::NULL_ENTITY;
    bool isMouseCaptured = false;
    bool animationsInitialized = false;

    // --- Animations ---
    std::string idleAnim = "Assets\\Mixamo\\Idle_anim_mixamorig_Hips.pa";
    std::string forwardAnim = "Assets\\Mixamo\\Jogging_anim_mixamo_Hips.pa";
    std::string backwardAnim = "Assets\\Mixamo\\Jog Backward_anim_mixamorig_Hips.pa";

    void OnInit() override {
        Inspect("Distance", &distance);
        Inspect("Sensitivity", &sensitivity);
        Inspect("Height Offset", &targetHeightOffset);
    }

    void OnCreate() override {
        // Reset des valeurs logiques
        firstFrame = true;
        yaw = 0.0f;
        pitch = 20.0f;
        isMouseCaptured = false;
        animationsInitialized = false;

        // --- ACTIVATION DE LA CAMÉRA "Cam" ---
        auto renderSys = engine->GetSystem<Engine::Systems::RenderSystem>();
        if (renderSys) {
            // On définit l'entité actuelle (Cam) comme caméra principale
            renderSys->SetMainCamera(entityID);

            // Log de confirmation pour le debug
            std::cout << "[camFight] Camera '" << registry->GetEntityName(entityID)
                << "' définie comme MainCamera." << std::endl;
        }

        FindTarget();
    }

    void FindTarget() {
        for (auto e : registry->View<Engine::Components::Transform>()) {
            // On cherche le joueur par son nom dans la scène
            if (registry->GetEntityName(e) == "Player_cuby") {
                targetEntity = e;
                break;
            }
        }
    }

    void InitAnimations() {
        auto animSys = engine->GetSystem<Engine::Systems::AnimatorSystem>();
        if (animSys && registry->HasComponent<Engine::Components::Animator>(targetEntity)) {
            animSys->PlayBlendAnimation(targetEntity, idleAnim, true);
            animSys->PlayBlendAnimation(targetEntity, forwardAnim, true);
            animSys->PlayBlendAnimation(targetEntity, backwardAnim, true);
            animationsInitialized = true;
        }
    }

    void OnUpdate(float dt) override {
        // Force le reset de la vision au tout premier frame de rendu de la scène
        if (firstFrame) {
            yaw = 0.0f;
            pitch = 20.0f;
            firstFrame = false;
        }

        if (targetEntity == Engine::ECS::NULL_ENTITY) {
            FindTarget();
            if (targetEntity == Engine::ECS::NULL_ENTITY) return;
        }

        if (!animationsInitialized) InitAnimations();

        // Rotation via souris
        if (InputSysteminstance->GetMouseButtonState(1)) {
            if (!isMouseCaptured) {
                InputSysteminstance->SetMouseCapture(true);
                isMouseCaptured = true;
            }
            glm::vec2 look = InputSysteminstance->lookInput;
            yaw -= look.x * sensitivity;
            pitch += look.y * sensitivity;
            pitch = glm::clamp(pitch, -89.0f, 89.0f);
        }
        else {
            if (isMouseCaptured) {
                InputSysteminstance->SetMouseCapture(false);
                isMouseCaptured = false;
            }
        }

        // Application de la transformation
        if (registry->HasComponent<Engine::Components::Transform>(entityID) &&
            registry->HasComponent<Engine::Components::Transform>(targetEntity)) {

            auto& cameraTransform = registry->GetComponent<Engine::Components::Transform>(entityID);
            auto& targetTransform = registry->GetComponent<Engine::Components::Transform>(targetEntity);

            cameraTransform.Rotation.x = pitch;
            cameraTransform.Rotation.y = yaw;

            glm::mat4 rotationMatrix = glm::mat4(1.0f);
            rotationMatrix = glm::rotate(rotationMatrix, glm::radians(yaw), glm::vec3(0, 1, 0));
            rotationMatrix = glm::rotate(rotationMatrix, glm::radians(pitch), glm::vec3(1, 0, 0));
            cameraTransform.Forward = glm::normalize(glm::vec3(rotationMatrix * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f)));

            // Suivi du joueur (position uniquement)
            glm::vec3 targetFocusPos = targetTransform.Position + glm::vec3(0.0f, targetHeightOffset, 0.0f);
            cameraTransform.Position = targetFocusPos - (cameraTransform.Forward * distance);
        }
    }

    void OnDestroy() override {
        if (isMouseCaptured) {
            InputSysteminstance->SetMouseCapture(false);
        }
    }
};

extern "C" SCRIPT_API Engine::Scripting::NativeScript* CreateScript() {
    return new camFight();
}