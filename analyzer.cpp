#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "subject.hpp"

std::unordered_map<std::string, std::shared_ptr<Subject>> subj_map;
std::unordered_set<std::string> subj_names;
std::unordered_set<std::string> obj_names;

enum LINE_TYPE
{
    add = 0,
    query,
    comment
};

// static inline void ltrim(std::string& s)
// {
//     s.erase(s.begin(), std::find_if(s.begin(), s.end(),
//                                     [](char ch) { return !std::isspace(ch);
//                                     }));
// }

// static inline void rtrim(std::string& s)
// {
//     s.erase(std::find_if(s.rbegin(), s.rend(),
//                          [](unsigned char ch) { return !std::isspace(ch); })
//                 .base(),
//             s.end());
// }

void execute_query(std::ofstream& fout, const std::string& subj,
                   const std::string& obj, const std::string& priv)
{
    // Start of by assuming access is unathorized,
    bool authorized = false;
    // Subjects that you can take privileges from, that have been queried
    std::unordered_set<std::string> visited = {};
    // Access is only authorized if 1) subject has been initialized previously
    // and 2) subject has r/w privilege
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

bool has_not_appeared(const std::string& potential)
{
    return !subj_names.count(potential) && !obj_names.count(potential);
}

LINE_TYPE find_line_type(std::vector<std::string>& line)
{
    if (line.size() != 4)
    {
        return LINE_TYPE::comment;
    }
    std::string& potential_type = line[0];
    std::string& potential_subj = line[1];
    std::string& potential_obj = line[2];
    std::string& potential_priv = line[3];

    LINE_TYPE line_type;

    if (potential_type == "Query")
    {
        line_type = LINE_TYPE::query;
    }
    else if (potential_type == "Add")
    {
        line_type = LINE_TYPE::add;
    }
    else
    {
        return LINE_TYPE::query;
    }

    if (has_not_appeared(potential_subj) || subj_names.count(potential_subj))
    {
        if (potential_priv == "R" || potential_priv == "W")
        {
            if (has_not_appeared(potential_obj) ||
                obj_names.count(potential_obj))
            {
                subj_names.insert(potential_subj);
                obj_names.insert(potential_obj);
                return line_type;
            }
        }
        else if (potential_priv == "T")
        {
            if (has_not_appeared(potential_obj) ||
                subj_names.count(potential_obj))
            {
                subj_names.insert(potential_subj);
                subj_names.insert(potential_obj);
                return line_type;
            }
        }
    }

    return LINE_TYPE::comment;
}

void process_txt(const std::string& filename, std::ofstream& fout)
{
    std::string line;
    std::ifstream input_file(filename);

    // parse input file, line by line
    while (std::getline(input_file, line))
    {
        std::string line_cp = line;
        size_t idx = line.find('\r');
        if (idx != std::string::npos)
            line = line.substr(0, idx);
        std::vector<std::string> parsed;
        uint32_t count = 0;
        // parsing an individual line of the input file
        while ((idx = line.find(',')) != std::string::npos)
        {
            std::string word = line.substr(0, idx);
            if (count > 0)
            {
                word.erase(0, 1);
            }
            parsed.push_back(word);
            line = line.substr(idx + 1);
            count += 1;
        }
        line.erase(0, 1);
        parsed.push_back(line);
        LINE_TYPE line_type = find_line_type(parsed);
        if (line_type == LINE_TYPE::add)
        {
            execute_add(fout, parsed[1], parsed[2], parsed[3]);
        }
        else if (line_type == LINE_TYPE::query)
        {
            execute_query(fout, parsed[1], parsed[2], parsed[3]);
        }
        else
        {
            fout << line_cp << std::endl;
        }
    }

    input_file.close();
    fout.close();
}

int main(const int argc, const char** argv)
{
    std::ofstream fout(argv[2]);
    process_txt(argv[1], fout);

    return 0;
}
