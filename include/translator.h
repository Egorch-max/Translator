#pragma once
//привет
#include <iostream>
#include "stack_queue.h"
#include "term.h"
#include <string>
#include <vector>

using namespace std;

class Translator 
{
private:

	string str;

	vector<Term*> postfix;

	vector<Term*> str_terms;

	bool LexicalAnalysis();

	bool SyntacticAnalysis();

	void ToPostfix();

	double Calculation();

public:
	Translator(string s) : str(s) {}

	~Translator()
	{
		for (size_t i = 0; i < str_terms.size(); i++) 
		{
			delete str_terms[i];
		}
	}
	double solve() 
	{
		if (!LexicalAnalysis()) 
		{
			throw "error in lexical analysis";
		}

		if (!SyntacticAnalysis())
		{
			throw "error in syntactic analysis";
		}

		ToPostfix();

		for (size_t i = 0; i < str.size(); i++)
		{
			cout << str[i];
		}

		cout << "\n";

		for (size_t i = 0; i < str_terms.size(); i++)
		{
			str_terms[i]->print();
		}
		cout << "\n";

		for (size_t i = 0; i < postfix.size(); i++)
		{
			postfix[i]->print();
		}
		cout << "\n";

		double d = Calculation();

		cout << d << "\n";

		return d;
	}
};

bool Translator::LexicalAnalysis()
{
	string tmp_str = str;

	while (!tmp_str.empty()) 
	{

		if (tmp_str[0] == '+' || tmp_str[0] == '-' || tmp_str[0] == '*' || tmp_str[0] == '/') 
		{
			str_terms.push_back(new Operator(tmp_str[0]));

			tmp_str.erase(tmp_str.begin(), tmp_str.begin() + 1);
		}

		else if (tmp_str[0] >= '0' && tmp_str[0] <= '9') 
		{
			string::size_type sz;

			str_terms.push_back(new Number(stod(tmp_str, &sz)));

			tmp_str.erase(tmp_str.begin(), tmp_str.begin() + sz);
		}

		else if (tmp_str[0] == '(' || tmp_str[0] == ')') 
		{
			str_terms.push_back(new Bracket(tmp_str[0]));

			tmp_str.erase(tmp_str.begin(), tmp_str.begin() + 1);
		}

		else return false;
	}
	return true;
}
void Translator::ToPostfix()
{
	Stack<Term*> stack;

	for (size_t i = 0; i < str_terms.size(); i++) 
	{
		switch (str_terms[i]->GetType())
		{
		case type::NUMBER:
		{
			postfix.push_back(str_terms[i]);

			break;
		}

		case type::OPENING_BRACKET:
		{
			stack.push(str_terms[i]);

			break;
		}

		case type::CLOSING_BRACKET:
		{
			while (stack.top()->GetType() != type::OPENING_BRACKET) 
			{
				postfix.push_back(stack.top());

				stack.pop();
			}
			stack.pop();

			break;
		}

		case type::OPERATOR:
		{
			while (!stack.empty() && stack.top()->GetType() == type::OPERATOR && ((Operator*)(stack.top()))->Priority() >= ((Operator*)(str_terms[i]))->Priority()) 
			{
				postfix.push_back(stack.top());
	
				stack.pop();
			}
			stack.push(str_terms[i]);
		}
		break;
		}
	}
	while (!stack.empty()) 
	{
		postfix.push_back(stack.top());

		stack.pop();
	}
}
bool Translator::SyntacticAnalysis()
{
	size_t num_brackets = 0;

	for (size_t i = 0; i < str_terms.size() - 1; i++)
	{
		type term = str_terms[i + 1]->GetType();

		switch (str_terms[i]->GetType())
		{
		case type::OPENING_BRACKET:
		{
			if (term == CLOSING_BRACKET || term == OPERATOR)
			{
				return false;
			}
			else
			{
				num_brackets++;
			}

			break;
		}

		case type::CLOSING_BRACKET:
		{
			if (term == OPENING_BRACKET || term == NUMBER)
			{
				return false;
			}

			else if (--num_brackets < 0)
			{
				return false;
			}

			break;
		}

		case type::NUMBER:
		{
			if (term == OPENING_BRACKET || term == NUMBER)
			{
				return false;
			}

			break;
		}

		case type::OPERATOR:
		{
			if (term == OPERATOR || term == CLOSING_BRACKET)
			{
				return false;
			}

			break;
		}
		}
	}

	if (str_terms[str_terms.size() - 1]->GetType() == OPENING_BRACKET || str_terms[str_terms.size() - 1]->GetType() == OPERATOR)
	{
		return false;
	}

	if (str_terms[str_terms.size() - 1]->GetType() == CLOSING_BRACKET)
	{
		--num_brackets;
	}

	if (num_brackets)
	{
		return false;
	}

	return true;
}

double Translator::Calculation() {

	Stack<double> stack_num;

	for (size_t i = 0; i < postfix.size(); i++) 
	{
		switch (postfix[i]->GetType())
		{

		case type::NUMBER:
		{
			stack_num.push(((Number*)(postfix[i]))->value);

			break;
		}

		case type::OPERATOR:
		{
			double operand_1 = stack_num.top();

			stack_num.pop();

			double operand_2 = stack_num.top();

			stack_num.pop();

			switch (((Operator*)(postfix[i]))->value) 
			{

			case '+':
			{
				stack_num.push(operand_2 + operand_1);

				break;
			}
			case '-':
			{
				stack_num.push(operand_2 - operand_1);

				break;
			}
			case '*':
			{
				stack_num.push(operand_2 * operand_1);

				break;
			}
			case '/':
			{
				if (operand_1 == 0.0)
				{
					throw "division by zero";
				}
				stack_num.push(operand_2 / operand_1);

				break;
			}
			}
			break;
		}
		}
	}
	return stack_num.top();
}