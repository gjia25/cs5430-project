#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "subject.hpp"

std::unordered_map<std::string, subject*> subj_map;

std::vector<std::vector<std::string>> read_txt(const std::string& filename)
{
    std::string line;
    std::vector<std::vector<std::string>> the_txt;
    std::ifstream input_file(filename);

    while (std::getline(input_file, line))
    {
        size_t idx = line.find('\r');
        if (idx != std::string::npos)
            line = line.substr(0, idx);

        std::vector<std::string> parsed;
        while ((idx = line.find(',')) != std::string::npos)
        {
            parsed.push_back(line.substr(0, idx));
            line = line.substr(idx + 1);
        }
        parsed.push_back(line);
        the_txt.push_back(parsed);
    }

    input_file.close();
    return the_txt;
}

void execute_query(std::ofstream& fout, std::string& subj, std::string& obj,
                   std::string& priv)
{
    std::string authorized;
    std::unordered_set<std::string> visited = {};
    if (subj_map.count(subj) &&
        subj_map.at(subj)->is_authorized(obj, priv, visited))
    {
        authorized = "YES";
    }
    else
    {
        authorized = "NO";
    }
    std::cout << "Query, " << subj << ", " << obj << ", " << priv << " "
              << authorized << std::endl;
    fout << "Query, " << subj << ", " << obj << ", " << priv << " "
              << authorized << std::endl;
}

void execute_add(std::ofstream& fout, std::string& subj, std::string& obj,
                 std::string& priv)
{
    if (!subj_map.count(subj))
    {
        subject* new_subj = new subject(subj);
        subj_map[subj] = new_subj;
    }
    if (priv != "T") {
      subj_map.at(subj)->add_priv(obj, priv);
    } else {
      if (!subj_map.count(obj))
      {
          subject* new_subj_2 = new subject(obj);
          subj_map[obj] = new_subj_2;
      }
      subj_map.at(subj)->incr_take(subj_map[obj]);
    }
    std::cout << "Add, " << subj << ", " << obj << ", " << priv << std::endl;
    fout << "Add, " << subj << ", " << obj << ", " << priv << std::endl;
}

int main(const int argc, const char** argv)
{
    std::vector<std::vector<std::string>> input = read_txt(argv[1]);
    std::ofstream fout(argv[2]);

    std::cout << "output file " << argv[2] << std::endl;

    for (std::vector<std::string> line : input)
    {
        if (line[0] == "Query")
        {
            execute_query(fout, line[1], line[2], line[3]);
        }
        else if (line[0] == "Add")
        {
            execute_add(fout, line[1], line[2], line[3]);
        }
    }

    return 0;
}
