#include <iostream>
#include <cctype>

class RecursiveDescentParser
{
public:
	RecursiveDescentParser(const std::string& input) : input_(input) , pos_(0)
	{
	}

	bool parse()
	{
		return parseE() && pos_ == input_.size();
	}

private:
	bool parseE()
	{
		if (parseT())
		{
			return parseEPrime();
		}
		return false;
	}

	bool parseEPrime()
	{
		if (input_[pos_] == '+')
		{
			pos_++;
			if (parseT() && parseEPrime())
			{
				return true;
			}
			return false;
		}
		return true;
	}

	bool parseT()
	{
		if (parseF())
		{
			return parseTPrime();
		}
		return false;
	}

	bool parseTPrime()
	{
		if (input_[pos_] == '*')
		{
			pos_++;
			if (parseF() && parseTPrime())
			{
				return true;
			}
			return false;
		}
		return true;
	}

	bool parseF()
	{
		if (input_[pos_] == '(')
		{
			pos_++;
			if (parseE() && input_[pos_] == ')')
			{
				pos_++;
				return true;
			}
			return false;
		}
		else if (input_[pos_] == '-')
		{
			pos_++;
			return parseF();
		}
		else if (input_[pos_] == 'a' || input_[pos_] == 'b' || input_[pos_] == '5')
		{
			pos_++;
			return true;
		}
		return false;
	}

private:
	const std::string& input_;
	std::size_t pos_;
};

int main()
{
	std::string input;
	std::cout << "Enter an expression: ";
	std::cin >> input;

	RecursiveDescentParser parser(input);

	if (parser.parse())
	{
		std::cout << "Parsing successful.\n";
	}
	else
	{
		std::cout << "Parsing failed.\n";
	}

	return 0;
}
