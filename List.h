#pragma once
#include <vector>
#include <utility>
#include <iostream>
#include <iomanip>
#include <ostream>


class List
{
public:
	// Default constructor
	List();

	//Destructor
	~List();

	//Add as first element in the list
	List& add_first(std::string n, std::string oper, std::string m);

	// Copy and assignment constructors disabled
	List(const List& source) = delete;
	List& operator=(const List& source) = delete;

	int getSize();
	std::pair<std::string, std::string> getCommand(std::string s, std::vector<bool>&b);
	bool allRegisterCalculated(std::string s, std::vector<bool>&b);

	friend std::ostream& operator<<(std::ostream& os, const List& L);

private:
	// Using a node to store the operations commands: "a add 10" 
	// Save it to use later for lazy evaluation

	int size = 0;

	class OperationNode
	{
	public:

		// Default constructor
		OperationNode();

		// Constructor
		OperationNode(std::string n, std::string oper, std::string m, OperationNode* ptr) : input_lhs(n), operation(oper), input_rhs(m), next(ptr)
		{ }

		/*
		Example 1	a add 10, a=input_lhs, add=operation, 10=input_rhs
		Example 2	b add a, b=input_lhs, add=operation, a=input_rhs
		*/
		std::string input_lhs; // name or integer value
		std::string operation; // add, subtract, multiply
		std::string input_rhs; // name or integer value, if value --> converted to integer later using e.g std::stoi()
		OperationNode* next; // point to next node

		friend std::ostream& operator<<(std::ostream& os, const List& L);
	};

	OperationNode* head;
};