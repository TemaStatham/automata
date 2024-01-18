#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm> 
#include <map>

const char DELIMITER = ';';
const char OUTPUT_STATE_NAME = 'Q';

const std::string mealyInFile = "mealyInputFile.csv";
const std::string mooreInFile = "mooreInputFile.csv";

using Machine = std::vector<std::vector<std::string>>;
//using Mask = std::map<int, std::vector<std::string>>;

std::vector<std::string> ParseStringInCSVFile(const std::string& line)
{
	/*std::stringstream stream(line);
	std::string stateName;
	std::vector<std::string> vec;

	while (getline(stream, stateName, DELIMITER))
	{
		vec.push_back(stateName);
	}

	return vec;*/
	stringstream stream(line);
	string stateName;
	vector<string> vec;

	while (getline(stream , stateName , DELIMITER))
	{
		vec.push_back(stateName);
	}

	if (stateName == EMPTY_STRING)
	{
		vec.push_back(stateName);
	}

	return vec;
}

Machine CSVFileReader(const std::string& fileName)
{
	std::ifstream workFile(fileName);
	std::string line;

	Machine outputMachine;

	while (getline(workFile , line))
	{
		outputMachine.push_back(ParseStringInCSVFile(line));
	}

	workFile.close();
	return outputMachine;
}

void WriteToOutputFile(const Machine& machine , std::ostream& outputFile)
{
	for (int i = 0; i < machine.size(); i++)
	{
		for (int j = 0; j < machine[i].size(); j++)
		{
			outputFile << machine[i][j] << DELIMITER;
		}
		outputFile << std::endl;
	}
	outputFile << std::endl;
}

void ParseStatesAndOutsElement(const std::string& input , std::string& state , std::string& outSignal)
{
	std::stringstream ss(input);
	std::getline(ss , state , '/');
	std::getline(ss , outSignal);
}

std::vector<std::string> GetStatesAndOutsForMoore(const Machine& machine)
{
	std::vector<std::string> statesAndOuts;

	for (int i = 1; i < machine.size(); i++)
	{
		for (int j = 1; j < machine[i].size(); j++)
		{
			statesAndOuts.push_back(machine[i][j]);
		}
	}

	return statesAndOuts;
}

std::vector<std::string> GetNewStatesNames(std::vector<std::string> states)
{
	std::vector<std::string> newStates;

	for (int i = 0; i < states.size(); i++)
	{
		newStates.push_back(OUTPUT_STATE_NAME + std::to_string(i));
	}

	return newStates;
}

Machine ParseStatesAndOutsToTittle(std::vector<std::string> statesAndOuts)
{
	std::vector<std::string> outSignals;
	std::vector<std::string> states;

	for (auto& el : statesAndOuts)
	{
		std::string state;
		std::string outSignal;

		ParseStatesAndOutsElement(el , state , outSignal);

		outSignals.push_back(outSignal);
		states.push_back(state);
	}

	Machine mooreTittle;

	mooreTittle.push_back(outSignals);
	mooreTittle.push_back(states);

	mooreTittle.push_back(GetNewStatesNames(states));

	return mooreTittle;
}

Machine GetTittleForMoore(const Machine& machine)
{
	return ParseStatesAndOutsToTittle(GetStatesAndOutsForMoore(machine));
}

Machine GetTransitionTable(const Machine& mealy , const Machine& tittle)
{
	Machine transitionTable(mealy.size() - 1 , std::vector<std::string>(tittle[0].size()));

	for (int a = 0; a < mealy[0].size(); a++)
	{
		for (int z = 0; z < tittle[1].size(); z++)
		{
			if (mealy[0][a] != tittle[1][z])
			{
				continue;
			}

			for (int b = 1; b < mealy.size(); b++)
			{
				std::string state;
				std::string outSignal;

				ParseStatesAndOutsElement(mealy[b][a] , state , outSignal);

				for (int i = 0; i < tittle[1].size(); i++)
				{
					if (tittle[1][i] == state && tittle[0][i] == outSignal)
					{
						transitionTable[b - 1][z] = tittle[2][i];
					}
				}
			}

		}
	}

	return transitionTable;
}

void ReverseMachine(Machine& machine)
{
	for (auto& el : machine)
	{
		std::reverse(el.begin() , el.end());
	}
}

Machine ConvertMealyToMoore(const Machine& mealy)
{
	Machine tittle = GetTittleForMoore(mealy);

	Machine transitionTable = GetTransitionTable(mealy , tittle);

	for (auto& el : tittle)
	{
		el.push_back("");
	}

	for (int i = 0; i < transitionTable.size(); i++)
	{
		transitionTable[i].push_back("X" + std::to_string(i + 1));
	}

	ReverseMachine(tittle);
	ReverseMachine(transitionTable);

	Machine moore;

	moore.push_back(tittle[0]);
	moore.push_back(tittle[2]);

	for (int i = 0; i < transitionTable.size(); i++)
	{
		moore.push_back(transitionTable[i]);
	}

	return moore;
}

void VisualizeMooreMachine(const Machine& moore)
{
	std::ofstream dotFile("mealy.dot");
	dotFile << "digraph G {\n";

	for (size_t i = 2; i < moore.size(); i++)
	{
		std::string fromNode = moore[i][0];

		for (size_t j = 1; j < moore[i].size(); j++)
		{
			std::stringstream ss(moore[i][j]);
			std::string signal;
			std::getline(ss , signal , '/');
			std::string toNode = moore[1][j];

			if (moore[i][j] == "-")
			{
				continue;
			}
			dotFile << "\t" << toNode << " -> " << moore[i][j] << " [label=\"" << fromNode << "\"];\n";
		}
	}

	dotFile << "}";

	dotFile.close();

	std::string command = "dot -Tpng mealy.dot -o mealy.png";
	system(command.c_str());
}

Machine ConvertMooreToMealy(const Machine& moore)
{
	Machine copyOfMoore = moore;

	for (int i = 2; i < copyOfMoore.size(); i++)
	{
		for (int j = 1; j < copyOfMoore[i].size(); j++)
		{
			for (int k = 0; k < copyOfMoore[1].size(); k++)
			{
				if (copyOfMoore[1][k] == copyOfMoore[i][j])
				{
					copyOfMoore[i][j] += "/" + copyOfMoore[0][k];
				}
			}
		}
	}

	Machine mealy;
	for (int i = 1; i < copyOfMoore.size(); i++)
	{
		mealy.push_back(copyOfMoore[i]);
	}

	return mealy;
}

void VisualizeMealyMachine(const Machine& mealy)
{
	std::ofstream dotFile("moore.dot");

	dotFile << "digraph G {\n";

	for (size_t i = 1; i < mealy.size(); i++)
	{
		std::string fromNode = mealy[i][0];

		for (size_t j = 1; j < mealy[i].size(); j++)
		{
			std::stringstream ss(mealy[i][j]);
			std::string signal;
			std::getline(ss , signal , '/');
			std::string toNode = mealy[i][j];
			std::string delimiter = "/";
			std::string state = mealy[0][j];
			size_t pos = toNode.find(delimiter);
			std::string pointer = toNode.substr(pos + 1);
			std::string end = toNode.substr(0 , pos);

			if (end == "-")
			{
				continue;
			}
			dotFile << "\t" << state << " -> " << end << " [label=\"" << fromNode + "/" + pointer << "\"];\n";
		}
	}

	dotFile << "}";

	dotFile.close();

	std::string command = "dot -Tpng moore.dot -o moore.png";
	system(command.c_str());
}

int main()
{
	std::string command;

	std::cin >> command;

	Machine outMachine;

	if (command == "mealy2moore")
	{
		Machine machine = CSVFileReader(mealyInFile);
		WriteToOutputFile(machine , std::cout);

		outMachine = ConvertMealyToMoore(machine);

		VisualizeMooreMachine(outMachine);

	}

	if (command == "moore2mealy")
	{
		Machine machine = CSVFileReader(mooreInFile);
		WriteToOutputFile(machine , std::cout);

		outMachine = ConvertMooreToMealy(machine);

		VisualizeMealyMachine(outMachine);

	}

	WriteToOutputFile(outMachine , std::cout);

	std::ofstream outputFile("output.csv");
	WriteToOutputFile(outMachine , outputFile);

	return 0;
}
