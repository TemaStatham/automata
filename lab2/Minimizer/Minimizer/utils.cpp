#include "utils.h"

void Split(std::string const& str, char separator, std::vector<std::string>& out) {
    std::stringstream ss(str);
    std::string s;
    while (std::getline(ss, s, separator)) {
        out.push_back(s);
    }
}

std::string CreateUniqueStr(const std::vector<std::string>& vec) {
    std::string result;
    for (const auto& str : vec) {
        result += str.substr(1, str.size() - 1);
    }
    return result;
}
