#ifndef __KD_PARSER_HPP__
#define __KD_PARSER_HPP__

#include <Godot.hpp>
 
#include <unordered_map>
#include <utility>
#include <cstdlib>

#include "selector.hpp"
#include "action.hpp"
#include "utils.hpp"

namespace godot {

class Shot;

template<typename T>
T parse_argument(String src)
{
    return T();
}

template<>
inline int parse_argument<int>(String src)
{
    return src.to_int();
}

template<>
inline float parse_argument<float>(String src)
{
    return src.to_float();
}

class Parser {
private:
    static Parser* _singleton;

    template<typename T>
    class IFactory {
    public:
        virtual T* parse(PoolStringArray args) = 0;
    };

    template<typename... Args>
    class SelectorFactory : public IFactory<ISelector> {
    private:
        bool(*_function)(Shot*, Args...);

        template<std::size_t... Indices>
        ISelector* parse_arguments(PoolStringArray args, std::index_sequence<Indices...>)
        {
            return new Selector(_function, parse_argument<Args>(args[Indices])...);
        }

    public:
        SelectorFactory(bool(*fn)(Shot*, Args...))
            : _function(fn)
        {}

        ISelector* parse(PoolStringArray args) override
        {
            if (args.size() != sizeof...(Args)) {
                Godot::print_error("Invalid number of parameters passed to selector!", "parse", __FILE__, __LINE__);
                return nullptr;
            }

            return parse_arguments(args, std::index_sequence_for<Args...>{});
        }
    };

    template<typename... Args>
    class ActionFactory : public IFactory<IAction> {
    private:
        void(*_function)(Shot*, Args...);

        template<std::size_t... Indices>
        IAction* parse_arguments(PoolStringArray args, std::index_sequence<Indices...>)
        {
            return new Action(_function, parse_argument<Args>(args[Indices])...);
        }

    public:
        ActionFactory(void(*fn)(Shot*, Args...))
            : _function(fn)
        {}

        IAction* parse(PoolStringArray args) override
        {
            if (args.size() != sizeof...(Args)) {
                Godot::print_error("Invalid number of parameters passed to action!", "parse", __FILE__, __LINE__);
                return nullptr;
            }

            return parse_arguments(args, std::index_sequence_for<Args...>{});
        }
    };

    Map<IFactory<ISelector>*> _selectors;
    Map<IFactory<IAction>*> _actions;

public:
    static Parser* get_singleton();
    static void free_singleton();

    ISelector* parse_selector(String src);
    IAction* parse_action(String src);

    template<typename... Args>
    void register_selector(bool(*fn)(Shot*, Args...), String key)
    {
        _selectors[key] = new SelectorFactory<Args...>(fn);
    }

    template<typename... Args>
    void register_action(void(*fn)(Shot*, Args...), String key)
    {
        _actions[key] = new ActionFactory<Args...>(fn);
    }
};

}

#endif