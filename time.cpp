#include <fstream>
#include <iostream>
#include <string>

int main(const int argc, const char** argv)
{
    std::ofstream fout(argv[1]);
    uint32_t num = std::stoi(argv[2]);
    std::string type = argv[3];

    if (type == "Add")
    {
        for (uint32_t i = 0; i < num; i++)
        {
            std::string priv = i % 2 ? "R" : "W";
            fout << "Add, "
                 << "s" << i << ", "
                 << "o" << i << ", " << priv << std::endl;
            fout << "Add, "
                 << "s" << i << ", "
                 << "s" << i + 1 << ", T" << std::endl;
        }
    }
    else
    {
        for (uint32_t i = 0; i < num; i++)
        {
            fout << "Add, "
                 << "s" << i << ", "
                 << "o" << i << ", R" << std::endl;
            if (i == num - 1)
            {
                fout << "Add, "
                     << "s" << i << ", "
                     << "s0, T" << std::endl;
            }
            else
            {
                fout << "Add, "
                     << "s" << i << ", "
                     << "s" << i + 1 << ", T" << std::endl;
            }
        }
        for (uint32_t i = 0; i < num; i++)
        {
            fout << "Query, "
                 << "s0, "
                 << "o" << i << ", R" << std::endl;
        }
    }

    return 0;
}