#include "subject.hpp"

Subject::Subject(std::string name) : name(name) {}

std::string Subject::get_name() { return name; }

void Subject::incr_take(std::shared_ptr<Subject> subj)
{
    std::string new_name = subj->get_name();
    if (take_map.count(new_name))
    {
        return;
    }
    take_map[new_name] = subj;
}

void Subject::add_priv(const std::string& obj, const std::string& priv)
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

bool Subject::is_authorized(const std::string& obj, const std::string& priv,
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