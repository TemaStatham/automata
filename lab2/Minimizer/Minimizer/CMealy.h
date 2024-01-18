#pragma once
#include "CMoore.h"
#include "utils.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

struct SMealyItem {
    std::string to;
    std::string out;
};

class CMealy {
public:
    CMealy() = default;
    void FromCSVFromStream(std::istream& istream);
    std::string ConvertToCSV();

    void SetStates(std::vector<std::string>& data);
    void SetTransitions(std::vector<std::vector<SMealyItem>>& data);

    std::vector<std::string> GetStates() const;
    std::vector<std::string> GetPaths() const;
    std::vector<std::vector<SMealyItem>> GetTransitions() const;

private:
    void ReadStatesCSV(const std::string& line);
    void ReadTransitionsCSV(const std::string& line);

    std::vector<std::string> m_states;
    std::vector<std::string> m_paths;
    std::vector<std::vector<SMealyItem>> m_transitions;
};
