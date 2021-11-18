#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class subject
{
public:
    // constructor
    subject(std::string name);

    std::string get_name();
    void incr_take(subject* subj);
    void add_priv(std::string& obj, std::string& priv);
    bool is_authorized(std::string& obj, std::string& priv,
                       std::unordered_set<std::string>& visited);

private:
    // name of the subject
    std::string name;
    // map of subjects that this object can take privileges from
    std::unordered_map<std::string, subject*> take_map;
    // capability list
    std::unordered_map<std::string, std::unordered_set<std::string>>
        capability_map;
};