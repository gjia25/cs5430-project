#include <algorithm>
#include <fstream>
#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "input.hpp"
#include "subject.hpp"

std::unordered_map<std::string, std::shared_ptr<Subject>> subj_map;
std::unordered_set<std::string> subj_names;
std::unordered_set<std::string> obj_names;

static inline void ltrim(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(),
                                    [](char ch) { return !std::isspace(ch); }));
}

bool has_not_appeared(const std::string& potential)
{
    return !subj_names.count(potential) && !obj_names.count(potential);
}

bool is_command(std::vector<std::string>& line)
{
    if (line.size() != 4)
    {
        return false;
    }
    std::string& potential_type = line[0];
    std::string& potential_subj = line[1];
    std::string& potential_obj = line[2];
    std::string& potential_priv = line[3];

    if (potential_type == "Query" || potential_type == "Add")
    {
        if (has_not_appeared(potential_subj) ||
            subj_names.count(potential_subj))
        {
            if (potential_priv == "R" || potential_priv == "W")
            {
                if (has_not_appeared(potential_obj) ||
                    obj_names.count(potential_obj))
                {
                    subj_names.insert(potential_subj);
                    obj_names.insert(potential_obj);
                    return true;
                }
            }
            else if (potential_priv == "T")
            {
                if (has_not_appeared(potential_obj) ||
                    subj_names.count(potential_obj))
                {
                    subj_names.insert(potential_subj);
                    subj_names.insert(potential_obj);
                    return true;
                }
            }
        }
    }
    return false;
}

std::vector<std::shared_ptr<Line>> read_txt(const std::string& filename)
{
    std::string line;
    std::vector<std::shared_ptr<Line>> the_txt;
    std::ifstream input_file(filename);

    // parse input file, line by line
    while (std::getline(input_file, line))
    {
        std::string line_cp = line;
        size_t idx = line.find('\r');
        if (idx != std::string::npos)
            line = line.substr(0, idx);
        std::vector<std::string> parsed;
        // parsing an individual line of the input file
        while ((idx = line.find(',')) != std::string::npos)
        {
            std::string word = line.substr(0, idx);
            ltrim(word);
            parsed.push_back(word);
            line = line.substr(idx + 1);
        }
        parsed.push_back(line);
        if (is_command(parsed))
        {
            auto new_command =
                new Command(parsed[0], parsed[1], parsed[2], parsed[3]);
            the_txt.emplace_back(
                std::shared_ptr<Line>(static_cast<Line*>(new_command)));
        }
        else
        {
            auto new_comment = new Comment(line_cp);
            the_txt.emplace_back(
                std::shared_ptr<Line>(static_cast<Line*>(new_comment)));
        }
    }

    input_file.close();
    return the_txt;
}

int main(const int argc, const char** argv)
{
    std::vector<std::shared_ptr<Line>> input = read_txt(argv[1]);
    std::ofstream fout(argv[2]);

    for (auto& line : input)
    {
        line->execute(fout);
    }

    return 0;
}
