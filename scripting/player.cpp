#include "script_pch.h"
#include <vector>
#include <memory>
#include "cubylandCore.h"
#include "npc.cpp"
#ifdef _WIN32
#define SCRIPT_API __declspec(dllexport)
#else
#define SCRIPT_API __attribute__((visibility("default")))
#endif

class NPC;

class Player : public Engine::Scripting::NativeScript
{
public:
    std::vector<std::unique_ptr<NPC>> mapCubies;
    Vec3 vector1;
    void getComponent() {};
    void setComponent() {};


    void move() {};
    void update() {};
};

extern "C" SCRIPT_API Engine::Scripting::NativeScript* CreatePlayer() {
    return new Player();
}