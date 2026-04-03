#include "player.hpp"


    void Player::getComponent() {}
    void Player::setComponent() {}


    void Player::move() {}
    void Player::update() {}

extern "C" SCRIPT_API Engine::Scripting::NativeScript* CreateScript() {
    return new Player();
}