#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

enum COMMAND_TYPE
{
    add = 0,
    query
};

class Line
{
public:
    virtual void execute(std::ofstream& fout) = 0;
};

class Command : public Line
{
public:
    Command(const std::string& type, const std::string subj,
            const std::string obj, const std::string priv);
    virtual void execute(std::ofstream& fout);

private:
    const COMMAND_TYPE type;
    const std::string subj;
    const std::string obj;
    const std::string priv;
};

class Comment : public Line
{
public:
    Comment(const std::string body);
    virtual void execute(std::ofstream& fout);

private:
    const std::string body;
};