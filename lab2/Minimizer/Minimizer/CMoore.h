#pragma once
#include "CMealy.h"
#include "utils.h"
#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

class CMoore {
public:
    CMoore() = default;
    void FromCSVFromStream(std::istream& istream);
    std::string ConvertToCSV();

    void SetStates(std::vector<std::string>& data);
    void SetOutput(std::vector<std::string>& data);
    void SetTransitions(std::vector<std::vector<std::string>>& data);

    std::vector<std::string> GetStates() const;
    std::vector<std::string> GetPaths() const;
    std::vector<std::string> GetOutput() const;
    std::vector<std::vector<std::string>> GetTransitions() const;

private:
    void ReadOutputsCSV(const std::string& line);
    void ReadStatesCSV(const std::string& line);
    void ReadTransitionsCSV(const std::string& line);

    std::vector<std::string> m_states;
    std::vector<std::string> m_paths;
    std::vector<std::string> m_output;
    std::vector<std::vector<std::string>> m_transitions;
};
