#include "CMinimizer.h"

void CMinimizer::MealyMinimizer(CMealy& mealy) {
    auto transitions = mealy.GetTransitions();
    auto statesOld = mealy.GetStates();
    auto states = SelectState(transitions, mealy.GetPaths().size(), statesOld.size());
    auto uniques = SelectUniqueState(states);

    std::string equivalentString;
    auto uniqueString = CreateUniqueStr(uniques);
    while (equivalentString != uniqueString) {
        equivalentString = uniqueString;
        auto newTransitions = CreateNewTransitions(mealy, uniques);
        auto newStates = SelectState(newTransitions, mealy.GetPaths().size(), mealy.GetStates().size());
        uniques = SelectUniqueState(newStates, uniques);
        uniqueString = CreateUniqueStr(uniques);
    }

    std::vector<std::string> newState;
    std::vector<int> indexVec;
    for (int i = 0; i < uniques.size(); ++i) {
        auto unique = uniques[i];
        if (std::find(newState.begin(), newState.end(), unique) == newState.end()) {
            newState.push_back(unique);
            indexVec.push_back(i);
        }
    }

    std::vector<std::vector<SMealyItem>> newTransitions;
    for (int i = 0; i < mealy.GetPaths().size(); ++i) {
        std::vector<SMealyItem> newTransitionLine;
        for (const auto indexCurrent : indexVec) {
            auto item = transitions[i][indexCurrent];
            auto it = std::find(statesOld.begin(), statesOld.end(), item.to);
            if (it != statesOld.end()) {
                auto index = it - statesOld.begin();
                newTransitionLine.push_back({ uniques[index], item.out });
            }
        }
        newTransitions.push_back(newTransitionLine);
    }

    mealy.SetStates(newState);
    mealy.SetTransitions(newTransitions);
}

void CMinimizer::MooreMinimizer(CMoore& moore) {
    auto transitions = moore.GetTransitions();
    auto statesOld = moore.GetStates();
    auto uniques = SelectUniqueState(moore.GetOutput());

    std::string equivalentString;
    auto uniqueString = CreateUniqueStr(uniques);
    while (equivalentString != uniqueString) {
        equivalentString = uniqueString;
        auto newTransitions = CreateNewTransitions(moore, uniques);
        auto newStates = SelectState(newTransitions, moore.GetPaths().size(), moore.GetStates().size());
        uniques = SelectUniqueState(newStates, uniques);
        uniqueString = CreateUniqueStr(uniques);
    }

    std::vector<std::string> newState;
    std::vector<int> indexVec;
    for (int i = 0; i < uniques.size(); ++i) {
        auto unique = uniques[i];
        if (std::find(newState.begin(), newState.end(), unique) == newState.end()) {
            newState.push_back(unique);
            indexVec.push_back(i);
        }
    }

    std::vector<std::vector<std::string>> newTransitions;
    for (int i = 0; i < moore.GetPaths().size(); ++i) {
        std::vector<std::string> newTransitionLine;
        for (const auto indexCurrent : indexVec) {
            auto item = transitions[i][indexCurrent];
            auto it = std::find(statesOld.begin(), statesOld.end(), item);
            if (it != statesOld.end()) {
                auto index = it - statesOld.begin();
                newTransitionLine.push_back(uniques[index]);
            }
            else {
                newTransitionLine.emplace_back("");
            }
        }
        newTransitions.push_back(newTransitionLine);
    }

    auto output = moore.GetOutput();
    std::vector<std::string> newOutput;
    for (const auto& state : newState) {
        auto it = std::find(uniques.begin(), uniques.end(), state);
        if (it != uniques.end()) {
            auto index = std::distance(uniques.begin(), it);
            newOutput.push_back(output[index]);
        }
    }

    moore.SetStates(newState);
    moore.SetOutput(newOutput);
    moore.SetTransitions(newTransitions);
}

std::vector<std::string> CMinimizer::SelectState(std::vector<std::vector<SMealyItem>> state, size_t rowCount, size_t columnCount) {
    std::vector<std::string> result;

    for (int i = 0; i < columnCount; ++i) {
        std::string columnState;
        for (int j = 0; j < rowCount; ++j) {
            columnState += state[j][i].out;
        }
        result.push_back(columnState);
    }

    return result;
}

std::vector<std::string> CMinimizer::SelectUniqueState(const std::vector<std::string>& data) {
    std::vector<std::string> result;
    std::string newPrefix(1, data[0][0] == FIRST_PREFIX ? SECOND_PREFIX : FIRST_PREFIX);

    std::vector<std::string> uniques;
    std::vector<std::string> newResStates;
    for (const auto& item : data) {
        auto it = std::find(uniques.begin(), uniques.end(), item);
        if (it == uniques.end()) {
            uniques.push_back(item);
            auto str = newPrefix + std::to_string(uniques.size());
            newResStates.push_back(str);
            result.push_back(str);
        }
        else {
            auto index = it - uniques.begin();
            result.push_back(newResStates[index]);
        }
    }

    return result;
}

std::vector<std::vector<std::string>> CMinimizer::CreateNewTransitions(const CMealy& mealy, const std::vector<std::string>& data) {
    auto transitions = mealy.GetTransitions();
    auto states = mealy.GetStates();

    std::vector<std::vector<std::string>> result;
    for (auto& transition : transitions) {
        std::vector<std::string> line;
        for (auto& j : transition) {
            auto it = std::find(states.begin(), states.end(), j.to);
            if (it != states.end()) {
                auto index = it - states.begin();
                line.push_back(data[index]);
            }
        }
        result.push_back(line);
    }
    return result;
}

std::vector<std::string> CMinimizer::SelectState(const std::vector<std::vector<std::string>>& transitions, size_t rowCount, size_t columnCount) {
    std::vector<std::string> result;

    for (int i = 0; i < columnCount; ++i) {
        std::string columnState;
        for (int j = 0; j < rowCount; ++j) {
            columnState += transitions[j][i];
        }

        result.push_back(columnState);
    }

    return result;
}

std::vector<std::string> CMinimizer::SelectUniqueState(const std::vector<std::string>& data, const std::vector<std::string>& oldUnique) {
    std::vector<std::string> result;
    std::string newPrefix(1, oldUnique[0][0] == FIRST_PREFIX ? SECOND_PREFIX : FIRST_PREFIX);

    std::vector<std::string> newResStates;
    std::vector<std::string> newStrData;
    for (int i = 0; i < data.size(); ++i) {
        auto newStr = oldUnique[i] + data[i];
        auto it = std::find(newStrData.begin(), newStrData.end(), newStr);
        if (it == newStrData.end()) {
            newStrData.push_back(newStr);
            auto str = newPrefix + std::to_string(newStrData.size());
            newResStates.push_back(str);
            result.push_back(str);
        }
        else {
            auto index = it - newStrData.begin();
            result.push_back(newResStates[index]);
        }
    }

    return result;
}

std::vector<std::vector<std::string>> CMinimizer::CreateNewTransitions(const CMoore& moore, const std::vector<std::string>& data) {
    auto transitions = moore.GetTransitions();
    auto states = moore.GetStates();

    std::vector<std::vector<std::string>> result;
    for (auto& transition : transitions) {
        std::vector<std::string> line;
        for (auto& j : transition) {
            auto it = std::find(states.begin(), states.end(), j);
            if (it != states.end()) {
                auto index = it - states.begin();
                line.push_back(data[index]);
            }
            else {
                line.emplace_back("");
            }
        }
        result.push_back(line);
    }
    return result;
}
