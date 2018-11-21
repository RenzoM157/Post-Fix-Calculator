#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "ArgumentManager.h"
using namespace std;


struct Node {
	string info;
	Node* link;
};


class StackNode
{
private:
	Node* topPointer;
public:
	StackNode()
	{
		topPointer = nullptr;
	}

	~StackNode()
	{
		reset();
	}

	bool isFull() const
	{
		return false;
	}

	bool isEmpty() const
	{
		return topPointer == nullptr;
	}


	void reset() {
		// empty the linked list by deleting element by element
		Node *current = topPointer;
		Node *link;
		while (current != NULL) {
			link = current->link;
			delete current;
			current = link;
		}
		topPointer = NULL;
	}


	string top() const // returns the top value in the stack
	{
		if (isEmpty())
		{
			cout << "Error: Stack is empty!";
		}

		return topPointer->info;
	}

	string pop() {
		if (isEmpty())
		{
			cout << "There is nothing to delete, stack is empty!";
			return 0;
		}
		string value = topPointer->info;
		Node *node = topPointer;
		topPointer = topPointer->link;
		delete node;
		return value;
	}


	void push(string x) // inserting a node at the beginning of the stack   
	{
		Node* node = new Node;
		node->info = x;
		node->link = topPointer;
		topPointer = node;
	}
};


bool Operand(string op) // this determines if the character inside of the equation is a letter or a number
{
	try {
		stod(op);
	}
	catch (const exception& e) {
		return false;
	}
	return true;
}

bool Operator(string op) // this determines if the character inside of the equation is an operator
{
	return (op == "*" || op == "/" || op == "+" || op == "-");
}

int rightAssoc(string op) // this function determines if it is associativity
{
	return false;
}


bool parenthesesPairing(string open, string close) // this function checks to see if they parentheses pairs
{

	if (open == "(" && close == ")")
	{
		return true;
	}

	else if (open == "{" && close == "}")
	{
		return true;
	}
	else if (open == "[" && close == "]")
	{
		return true;
	}

	else
	{
		return false;
	}
}

bool checkParentheses(string equation) // this function checks for balanced parentheses
{
	StackNode stack;
	string eq;

	for (int i = 0; i<equation.length(); i++)

	{
		if (equation[i] == ',' || equation[i] == ' ') // checks to see if there exists a delimitter, if so then ignore
		{
			continue;
		}

		if (equation[i] == '(' || equation[i] == '{' || equation[i] == '['){
			eq += equation[i];
			stack.push(eq);
			eq.clear();
		}
		else if (equation[i] == ')' || equation[i] == '}' || equation[i] == ']')

		{
			eq += equation[i];
			if (stack.isEmpty() || !(parenthesesPairing(stack.top(), eq)))

				return false;

			else

				stack.pop();

			eq.clear();
		}

	}

	return stack.isEmpty();

}

bool checkOperators(string equation) // determines if there is a missing operator between two operands
{
	string eq1, eq2;

	for (int i = 0; i < equation.length() - 1; i++)
	{
		if (i > 0 && equation[i] == ' ')
		{
			eq1.clear(); eq2.clear();
			eq1 += equation[i - 1];
			eq2 += equation[i + 1];

			if ((Operand(eq1)) && (Operand(eq2)))
			{
				return false;
			}
		}
	}

	return true;
}

int operatorImportance(string oI) // determines the operator's value which shows the difference between each operator's value
{
	int value = 0;

	if (oI == "/")
	{
		value = 4;
	}

	if (oI == "*")
	{
		value = 3;
	}

	if (oI == "-")
	{
		value = 2;
	}
	
	if (oI == "+")
	{
		value = 1;
	}

	return value;
}

string operatorOverwrite(string equation) // this functions checks to see if there are two operators, if so erase the last one
{
	string eq1, eq2;
	for (int i = 0; i < equation.length() - 2; i++) {

		eq1.clear();  eq2.clear();
		eq1 += equation[i];
		eq2 += equation[i + 2];
		while (Operator(eq1) && Operator(eq2)) {

			equation = equation.erase(i, 2);

			eq1.clear(); eq2.clear();
			eq1 += equation[i];
			eq2 += equation[i + 2];

		}
	}

	return equation;
}


string clearCE(string eq) // this function finds the keyword CE and C and clears 
{
	int startIndex = 0, counter = 0;
	for (int i = 0; i < eq.length() - 1; i++)
	{
		if (eq[i] == 'C'  && eq[i + 1] == 'E')
		{
			startIndex = i + 2;
		}
	}

	eq = eq.substr(startIndex);

	for (int i = 0; i < eq.length() - 1; i++) {
		if (eq[i] == 'C')
		{
			if (i == eq.length() - 1 || eq[i + 1])
			{
				int j = 0;
				while (eq[i - 2 - j] != ' ')
				{
					j++;
				}
				eq.erase(i - 2 - j, j + 3);

				i -= 3;

				
			}
			
		}
		else if (eq[i] != ' ')
		{
			counter = i;
		}
	}

	return eq;
}

int mostPrecedence(string firstOperand, string secondOperand) // this functions determines the precendence of the operators
{
	int firstOpValue = operatorImportance(firstOperand);
	int secondOpValue = operatorImportance(secondOperand);

	if (firstOpValue == secondOpValue) // if both operators are the same then.... 
	{
		if (rightAssoc(firstOperand))
		{
			return true; //the operator is left associative....
		}
		else
		{
			return false; // if not, then operator is right associative
		}
	}

	return firstOpValue > secondOpValue;
}



string infixToPostFix(string equation) // function reads the string that is in txt file and convert it from infix to postfix
{
	StackNode stack;
	string result = ""; // result of the conversion from infix to postfix
	string eq;
	for (int i = 0; i < equation.length(); i++)
	{
		eq.clear();
		eq += equation[i];
		if (equation[i] == ',' || equation[i] == ' ') // checks to see if there exists a delimitter, if so then ignore
		{
			continue;
		}

		else if (equation[i] == '.')
		{
			
			result = result + equation[i];

			if (equation[i + 1] == ' ')
			{
				result = result + " ";
			}

		}
		
		else if (Operand(eq)) // if it is an operand
		{

			result = result + equation[i];

			if (equation[i + 1] == ' ')
			{
				result = result + " ";
			}
		

		}

		else if (Operator(eq))
		{ //If it is an operator, then pop two contents from stack and perform action
			while (!(stack.isEmpty()) && stack.top() != "(" && mostPrecedence(stack.top(), eq))
			{
				result = result + stack.top();
				result = result + " ";
				stack.pop();
			}
			stack.push(eq); // pushes back the result of the action into stack
		}

		else if (equation[i] == '(') // if it is an open parentheses push it inside of stack
		{
			stack.push(eq);
		}

		else if (equation[i] == ')') // if it is a closed parentheses pop out of stack and perform action
		{
			while (!(stack.isEmpty()) && stack.top() != "(")
			{
				result = result + stack.top();
				result = result + " ";
				stack.pop();
			}

			stack.pop();
		}

	}



	while (!(stack.isEmpty())) // puts stack in an empty state
	{
		result = result + " ";
		result = result + stack.top();
		stack.pop();
	}



	return result;

}


string replaceBraces(string eq) // this functions replaces the [ brackets with ( so the clearCE function can work properly
{
	for (auto& c : eq)
	{
		if (c == '[') c = '(';
		else if (c == ']') c = ')';
	}

	return eq;
}

double postEvaluation(double firstOp, double secondOp, string operatorVar) // performs operation of postfix equation.
{

	if (operatorVar == "/")
	{
		if (secondOp == 0)
		{
			cout << "Error: Cannot divide by 0";
		}
		else
		{
			return (firstOp / secondOp);
		}
	}

	if (operatorVar == "*")
	{
		return (firstOp * secondOp);
	}

	if (operatorVar == "-")
	{
		return (firstOp - secondOp);
	}

	if (operatorVar == "+")
	{
		return (firstOp + secondOp);
	}
	
}


double evaluation(string equation) // calculates the total sum of the equation of the postfix
{
	StackNode stack;
	double op1, op2;
	double total;
	string eq, result;

	for (int i = 0; i < equation.length(); i++)
	{
		eq.clear();
		eq += equation[i];

		if (equation[i] == ',' || equation[i] == ' ') // checks to see if there exists a delimitter, if so then ignore
		{
			if (!result.empty())
			{
				stack.push(result);
			}
			eq.clear();
			result.clear();
		}
		else if (Operator(eq)) // once it reaches an operator do the following...
		{
			op2 = stod(stack.top()); // get the value of the top of stack
			stack.pop(); // pop op2
			op1 = stod(stack.top()); // get the value of the top of the stack
			stack.pop(); // pop op1
			total = postEvaluation(op1, op2, eq); // calculate op1 & op2
			stack.push(to_string(total)); // pushes the new value back into the stack
			eq.clear();
		}
		else 
		{
			result += equation[i];
			eq.clear();
		}

	}

	return stod(stack.top());
}


int main(int argc, char* argv[])
{
	StackNode obj;
	ArgumentManager am(argc, argv);
	string infilename = am.get("A");
	string outfilename = am.get("C");
	string var;
	ifstream readFile;
	readFile.open(infilename.c_str());


	if (!(readFile.is_open()))
	{
		cout << "Error: File not found";
		exit(1);
	}

	
	getline(readFile, var);

	
	var = (clearCE(var));

	if (checkParentheses(var) && checkOperators(var))
	{
		ofstream writting(outfilename.c_str());
		var = operatorOverwrite(var);
		string conversion = infixToPostFix(replaceBraces(var)); 
		double total = evaluation(conversion);
		writting << conversion << endl;
		writting << fixed << setprecision(2) << total << endl;
		writting.close();
		readFile.close();
	}
	else
	{
		ofstream writting(outfilename.c_str());
		writting.close();
	}


	//system("pause");
	return 0;
}



