#pragma once
#include "CMealy.h"

class CMinimizer {
public:
    CMinimizer() = default;
    static void MealyMinimizer(CMealy& mealy);
    static void MooreMinimizer(CMoore& moore);

private:
    static std::vector<std::string> SelectState(std::vector<std::vector<SMealyItem>> data, size_t rowCount, size_t columnCount);
    static std::vector<std::string> SelectState(const std::vector<std::vector<std::string>>& transitions, size_t rowCount, size_t columnCount);
    static std::vector<std::string> SelectUniqueState(const std::vector<std::string>& data);
    static std::vector<std::string> SelectUniqueState(const std::vector<std::string>& data, const std::vector<std::string>& oldUnique);
    static std::vector<std::vector<std::string>> CreateNewTransitions(const CMealy& mealy, const std::vector<std::string>& data);
    static std::vector<std::vector<std::string>> CreateNewTransitions(const CMoore& moore, const std::vector<std::string>& data);
};
