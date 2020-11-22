#include "hitbox.hpp"

using namespace godot;

void Hitbox::_enter_tree() {
    Node* parent = get_parent();
    while ((bool)parent->call("is_danmaku") != true) {
        parent = parent->get_parent();
        ERR_FAIL_COND(parent == nullptr);
    }
    danmaku = Object::cast_to<Danmaku>(parent);
    danmaku->register_hitbox(this);
}

void Hitbox::_exit_tree() {
    danmaku->remove_hitbox();
}

Danmaku* Hitbox::get_danmaku() {
    return danmaku;
}

Shot* Hitbox::get_colliding_shot() {
    return colliding_shot;
}

Shot* Hitbox::get_grazing_shot() {
    return grazing_shot;
}

float Hitbox::get_collision_radius() {
    return collision_radius;
}

float Hitbox::get_graze_radius() {
    return graze_radius;
}

void Hitbox::hit(Shot* p_shot) {
    if (invulnerable)
        return;
    colliding_shot = p_shot;
    emit_signal("hit");
}

void Hitbox::graze(Shot* p_shot) {
    if (invulnerable)
        return;
    grazing_shot = p_shot;
    emit_signal("graze");
}

void Hitbox::_register_methods() {
    register_property<Hitbox, float>("collision_radius", &Hitbox::collision_radius, 2);
    register_property<Hitbox, float>("graze_radius", &Hitbox::graze_radius, 16);
    register_property<Hitbox, bool>("invulnerable", &Hitbox::invulnerable, false);

    register_method("_enter_tree", &Hitbox::_enter_tree);
    register_method("_exit_tree", &Hitbox::_exit_tree);

    register_method("get_danmaku", &Hitbox::get_danmaku);
    register_method("get_colliding_shot", &Hitbox::get_colliding_shot);
    register_method("get_grazing_shot", &Hitbox::get_grazing_shot);

    register_signal<Hitbox>("hit", Dictionary());
    register_signal<Hitbox>("graze", Dictionary());
}

void Hitbox::_init() {
    collision_radius = 2;
    graze_radius = 16;
    invulnerable = false;
    colliding_shot = nullptr;
    grazing_shot = nullptr;
    danmaku = nullptr;
}