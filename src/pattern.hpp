#ifndef __KD_PATTERN_HPP__
#define __KD_PATTERN_HPP__

#include <Godot.hpp>
#include <Node2D.hpp>
#include <Reference.hpp>

#include "utils.hpp"
#include "danmaku.hpp"
#include "shot.hpp"
#include "selector.hpp"
#include "action.hpp"

namespace godot {

class Pattern : public Node2D {
    GODOT_CLASS(Pattern, Node2D)

private:
    Danmaku* _danmaku;

    Shot** _shots;
    int _shots_size;
    int _active_count;

    Ref<Reference> delegate;

    Shot** buffer(int count);
    bool prepare(const String& sprite, int& sprite_id, float& radius, float& angle, int count, bool aim, Shot**& buf);

    ISelector* make_selector(String source);
    IAction* make_action(String source);

public:
    static void _register_methods();
    void _init() {};

    Pattern();
    ~Pattern();

    void _enter_tree();
    void _exit_tree();
    void _physics_process(float delta);
    void _draw();

    Danmaku* get_danmaku() { return _danmaku; }
    
    void fire(String sprite, float speed, float angle, bool aim);
    void fire_layered(String sprite, int layers, float min_speed, float max_speed, float angle, bool aim);
    void fire_circle(String sprite, int count, float speed, float angle, bool aim);
    void fire_layered_circle(String sprite, int count, int layers, float min_speed, float max_speed, float angle, bool aim);
    void fire_fan(String sprite, int count, float speed, float angle, float theta, bool aim);
    void fire_layered_fan(String sprite, int count, int layers, float min_speed, float max_speed, float angle, float theta, bool aim);
    void fire_custom(String sprite, int count, String name);

    Array select(String selector);
    void apply(String action);
};

};

#endif