#include "List.h"




// Default constructor
List::List(): head(new OperationNode("", "", "", nullptr)) // create a head node for the list
{}

// Destructor
List::~List() {

	OperationNode* temp = head->next;// points to node after head

	while (temp != nullptr) {

		delete head;
		head = temp;
		temp = temp->next;
	}
	delete head;
}

int List::getSize() {

	return size;
}

std::pair<std::string, std::string> List::getCommand(std::string s, std::vector<bool>&b)
{
	OperationNode* ptr = head->next;
	int counter = 0;
	std::pair<std::string, std::string> make_pair("","");
	
	while (ptr != nullptr)
	{
		if (ptr->input_lhs == s && b[counter] == false) {
			b[counter] = true; // set to used

			make_pair.first = ptr->input_rhs;
			make_pair.second = ptr->operation;
			return make_pair;
		}
		ptr = ptr->next;
		counter++;
	}

	return make_pair;
}


bool List::allRegisterCalculated(std::string s, std::vector<bool>&b)
{
	OperationNode* ptr = head->next;
	int howManyReg = 0;
	int howManyCalculated = 0;

	while (ptr != nullptr)
	{
		if (ptr->input_lhs == s || ptr->input_rhs == s) {

			howManyReg++;
		}
		ptr = ptr->next;
	}

	for (int i = 0; i < b.size(); i++) {
		if (b[i] == true) {
			howManyCalculated++;
		}
	}

	if (howManyCalculated == howManyReg) {
		return true;
	}

	return false;
}


//Add as first element in the list
List& List::add_first(std::string n, std::string oper, std::string m)
{
	size++;
	head->next = new OperationNode(n, oper, m, head->next);
	return *this;
}


std::ostream& operator<< (std::ostream& os, const List& L)
{
	List::OperationNode* ptr = L.head->next;

	if (ptr == nullptr)
	{
		os << "The list is empty!!" << "\n";
		return os;
	}

	while (ptr != nullptr)
	{
		os <<" " << ptr->input_lhs << " " << ptr->operation << " " << ptr->input_rhs <<" ";
		ptr = ptr->next;
	}

	return os;
}
