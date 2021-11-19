#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Subject
{
public:
    // constructor
    Subject(std::string name);

    std::string get_name();
    void incr_take(std::shared_ptr<Subject> subj);
    void add_priv(const std::string& obj, const std::string& priv);
    bool is_authorized(const std::string& obj, const std::string& priv,
                       std::unordered_set<std::string>& visited);

private:
    // name of the Subject
    std::string name;
    // map of Subjects that this object can take privileges from
    std::unordered_map<std::string, std::shared_ptr<Subject>> take_map;
    // capability list
    std::unordered_map<std::string, std::unordered_set<std::string>>
        capability_map;
};