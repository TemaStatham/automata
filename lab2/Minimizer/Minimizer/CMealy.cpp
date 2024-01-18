#include "./CMealy.h"

void CMealy::FromCSVFromStream(std::istream& istream) {
    bool hasFirstLine = false;
    std::string line;
    while (std::getline(istream, line)) {
        if (!hasFirstLine) {
            ReadStatesCSV(line);
            hasFirstLine = true;
        }
        else {
            ReadTransitionsCSV(line);
        }
    }
}

std::string CMealy::ConvertToCSV() {
    std::string csv;
    for (const auto& st : m_states) {
        csv.append(";" + st);
    }
    csv.append("\n");
    for (int i = 0; i < m_paths.size(); ++i) {
        csv.append(m_paths[i]);
        for (auto& j : m_transitions[i]) {
            csv.append(";" + j.to + "/" + j.out);
        }
        csv.append("\n");
    }
    return csv;
}

void CMealy::ReadStatesCSV(const std::string& line) {
    std::vector<std::string> states;
    Split(line, ';', states);
    for (const auto& state : states) {
        if (!state.empty()) {
            m_states.push_back(state);
        }
    }
}

void CMealy::ReadTransitionsCSV(const std::string& line) {
    std::vector<std::string> transitions;
    Split(line, ';', transitions);
    auto path = transitions[0];
    m_paths.push_back(path);
    std::vector<SMealyItem> transitionLine;
    for (int i = 1; i < transitions.size(); ++i) {
        auto transition = transitions[i];
        std::vector<std::string> items;
        Split(transition, '/', items);
        transitionLine.push_back(SMealyItem{ items[0], items[1] });
    }
    m_transitions.push_back(transitionLine);
}

void CMealy::SetStates(std::vector<std::string>& data) {
    m_states = data;
}

void CMealy::SetTransitions(std::vector<std::vector<SMealyItem>>& data) {
    m_transitions = data;
}

std::vector<std::string> CMealy::GetStates() const {
    return m_states;
}

std::vector<std::string> CMealy::GetPaths() const {
    return m_paths;
}

std::vector<std::vector<SMealyItem>> CMealy::GetTransitions() const {
    return m_transitions;
}
