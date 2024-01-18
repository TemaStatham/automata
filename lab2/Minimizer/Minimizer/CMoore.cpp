#include "CMoore.h"

void CMoore::FromCSVFromStream(std::istream& istream) {
    int index = 0;
    std::string line;
    while (std::getline(istream, line)) {
        if (index == 0) {
            ReadOutputsCSV(line);
            index++;
        }
        else if (index == 1) {
            ReadStatesCSV(line);
            index++;
        }
        else {
            ReadTransitionsCSV(line);
        }
    }
}

std::string CMoore::ConvertToCSV() {
    std::string csv;
    for (const auto& out : m_output) {
        csv.append(";" + out);
    }
    csv.append("\n");
    for (const auto& st : m_states) {
        csv.append(";" + st);
    }
    csv.append("\n");
    for (int i = 0; i < m_paths.size(); ++i) {
        csv.append(m_paths[i]);
        for (auto& j : m_transitions[i]) {
            csv.append(";" + j);
        }
        csv.append("\n");
    }
    return csv;
}

void CMoore::ReadOutputsCSV(const std::string& line) {
    std::vector<std::string> outputs;
    Split(line, ';', outputs);
    if (line[line.size() - 1] == ';') {
        outputs.emplace_back("");
    }
    for (int i = 1; i < outputs.size(); ++i) {
        m_output.push_back(outputs[i]);
    }
}

void CMoore::ReadStatesCSV(const std::string& line) {
    std::vector<std::string> states;
    Split(line, ';', states);
    if (line[line.size() - 1] == ';') {
        states.emplace_back("");
    }
    for (int i = 1; i < states.size(); ++i) {
        m_states.push_back(states[i]);
    }
}

void CMoore::ReadTransitionsCSV(const std::string& line) {
    std::vector<std::string> transitions;
    Split(line, ';', transitions);
    if (line[line.size() - 1] == ';') {
        transitions.emplace_back("");
    }
    auto path = transitions[0];
    m_paths.push_back(path);
    std::vector<std::string> transitionLine;
    for (int i = 1; i < transitions.size(); ++i) {
        transitionLine.push_back(transitions[i]);
    }
    m_transitions.push_back(transitionLine);
}

void CMoore::SetStates(std::vector<std::string>& data) {
    m_states = data;
}

void CMoore::SetOutput(std::vector<std::string>& data) {
    m_output = data;
}

void CMoore::SetTransitions(std::vector<std::vector<std::string>>& data) {
    m_transitions = data;
}

std::vector<std::string> CMoore::GetStates() const {
    return m_states;
}

std::vector<std::string> CMoore::GetPaths() const {
    return m_paths;
}

std::vector<std::string> CMoore::GetOutput() const {
    return m_output;
}

std::vector<std::vector<std::string>> CMoore::GetTransitions() const {
    return m_transitions;
}
