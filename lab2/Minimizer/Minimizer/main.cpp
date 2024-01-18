#include "./CMealy.h"
#include "CMinimizer.h"
#include <fstream>
#include <iostream>

const std::string MEALY = "mealy";
const std::string MOORE = "moore";

enum class MinimizationType {
    Mealy,
    Moore,
};

struct Args {
    MinimizationType type;
    std::string input;
    std::string output;
};

MinimizationType ParseType(const std::string& type) {
    if (type == MEALY) {
        return MinimizationType::Mealy;
    }
    if (type == MOORE) {
        return MinimizationType::Moore;
    }

    throw std::invalid_argument("Invalid type");
}

Args ParseArgs(int argc, char* argv[]) {
    if (argc != 4) {
        throw std::invalid_argument("Invalid argument count");
    }

    return Args{
            ParseType(argv[1]),
            argv[2],
            argv[3],
    };
}

void CheckFileOpen(const std::ifstream& input, const std::ofstream& output) {
    if (!input.is_open()) {
        throw std::ios_base::failure("Error reading the file");
    }

    if (!output.is_open()) {
        throw std::ios_base::failure("Error writing the file");
    }
}

void PrintData(std::ostream& ostream, const std::string& str) {
    ostream << str;
}

void MinimizationMealy(std::istream& input, std::ostream& output) {
    CMealy mealy;
    mealy.FromCSVFromStream(input);
    CMinimizer::MealyMinimizer(mealy);
    PrintData(output, mealy.ConvertToCSV());
}

void MinimizationMoore(std::istream& input, std::ostream& output) {
    CMoore moore;
    moore.FromCSVFromStream(input);
    CMinimizer::MooreMinimizer(moore);
    PrintData(output, moore.ConvertToCSV());
}

void Minimization(const std::string& inputFileName, const std::string& outputFileName, MinimizationType type) {
    std::ifstream input(inputFileName);
    std::ofstream output(outputFileName);
    CheckFileOpen(input, output);

    if (type == MinimizationType::Mealy) {
        MinimizationMealy(input, output);
    }
    if (type == MinimizationType::Moore) {
        MinimizationMoore(input, output);
    }
}

int main(int argc, char* argv[]) {
    try {
        auto args = ParseArgs(argc, argv);
        Minimization(args.input, args.output, args.type);

    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}
