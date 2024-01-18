#pragma once
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const char FIRST_PREFIX = 'A';
const char SECOND_PREFIX = 'B';

void Split(std::string const& str, char separator, std::vector<std::string>& out);
std::string CreateUniqueStr(const std::vector<std::string>& vec);
