#include "Scripting/NativeScripting.h"
#include "ECS/Registry.h" // MUST be included for the EXPORT macro to work

struct HealthComponent {
    float currentHealth = 100.0f;
    float maxHealth = 100.0f;
    bool isAlive = true;
};