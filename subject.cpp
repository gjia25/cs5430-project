#include "subject.hpp"

subject::subject(std::string name) : name(name) {}

std::string subject::get_name() { return name; }

void subject::incr_take(subject* subj)
{
    std::string new_name = subj->get_name();
    if (take_map.count(new_name))
    {
        return;
    }
    take_map[new_name] = subj;
}

void subject::add_priv(std::string& obj, std::string& priv)
{
    if (capability_map.count(obj))
    {
        capability_map[obj].insert(priv);
    }
    else
    {
        capability_map[obj] = {priv};
    }
}

bool subject::is_authorized(std::string& obj, std::string& priv,
                            std::unordered_set<std::string>& visited)
{
    if (capability_map.count(obj) && capability_map.at(obj).count(priv))
    {
        return true;
    }
    visited.insert(name);
    for (auto& el : take_map)
    {
        if (visited.count(el.second->get_name()))
        {
            continue;
        }
        if (el.second->is_authorized(obj, priv, visited))
        {
            return true;
        }
    }
    return false;
}