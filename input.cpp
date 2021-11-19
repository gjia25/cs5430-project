#include "input.hpp"

#include <memory>

#include "subject.hpp"

extern std::unordered_map<std::string, std::shared_ptr<Subject>> subj_map;

void execute_query(std::ofstream& fout, const std::string& subj,
                   const std::string& obj, const std::string& priv)
{
    bool authorized = false;
    std::unordered_set<std::string> visited = {};
    if (subj_map.count(subj) &&
        subj_map.at(subj)->is_authorized(obj, priv, visited))
    {
        authorized = true;
    }
    fout << "Query, " << subj << ", " << obj << ", " << priv << " "
         << (authorized ? "YES" : "NO") << std::endl;
}

void execute_add(std::ofstream& fout, const std::string& subj,
                 const std::string& obj, const std::string& priv)
{
    // if subj has never been seen before, we initialize a new subject
    if (!subj_map.count(subj))
    {
        auto new_subj = new Subject(subj);
        subj_map[subj] =
            std::shared_ptr<Subject>(static_cast<Subject*>(new_subj));
    }
    // if not a take command, we add read/write privilege for a specific obj
    if (priv != "T")
    {
        subj_map.at(subj)->add_priv(obj, priv);
    }
    // if take command,
    else
    {
        // if subj to take has never been seen before, we initialize a new
        // subject
        if (!subj_map.count(obj))
        {
            Subject* taken_subj = new Subject(obj);
            subj_map[obj] =
                std::shared_ptr<Subject>(static_cast<Subject*>(taken_subj));
        }
        // add the taken_subj to the list of subjs whose privileges can be taken
        // by subj
        subj_map.at(subj)->incr_take(subj_map[obj]);
    }
    fout << "Add, " << subj << ", " << obj << ", " << priv << std::endl;
}

Command::Command(const std::string& type, const std::string subj,
                 const std::string obj, const std::string priv)
    : type(type == "Query" ? COMMAND_TYPE::query : COMMAND_TYPE::add),
      subj(subj),
      obj(obj),
      priv(priv)
{
}

void Command::execute(std::ofstream& fout)
{
    switch (type)
    {
    case COMMAND_TYPE::add:
        execute_add(fout, subj, obj, priv);
        break;
    case COMMAND_TYPE::query:
        execute_query(fout, subj, obj, priv);
        break;
    }
}

Comment::Comment(const std::string body) : body(body) {}

void Comment::execute(std::ofstream& fout) { fout << body << std::endl; }