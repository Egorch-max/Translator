#pragma once
#include <iostream>

using namespace std;

enum type {NUMBER, OPERATOR, OPENING_BRACKET, CLOSING_BRACKET};

class Term 
{
public:
	type token;

	type GetType() 
	{
		return token;
	}
	virtual void print() = 0;
};

class Number : public Term 
{
public:
	double value;

	Number(double num)
	{
		token = type::NUMBER;

		value = num;
	}

	double getValue() 
	{
		return value;
	}

	void print() 
	{
		cout << value << " ";
	}
};

class Operator : public Term 
{
public:
	char value;

	Operator(char oper) 
	{
		token = type::OPERATOR;

		value = oper;
	}
	int Priority() 
	{
		if (value == '*' || value == '/')
		{
			return 1;
		}

		if (value == '+' || value == '-')
		{
			return 0;
		}

		//else throw "unidentified operation";
	}

	char getOp() const
	{
		return value;
	}

	void print()
	{
		cout << value << " ";
	}
};

class Bracket : public Term 
{
public:
	char value;

	Bracket(char br) 
	{
		if (br == '(')
		{
			token = type::OPENING_BRACKET;
		}

		else
		{
			token = type::CLOSING_BRACKET;
		}

		value = br;
	}
	char get_op() const
	{
		return value;
	}

	void print() 
	{
		cout << value << " ";
	}
};