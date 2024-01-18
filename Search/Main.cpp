#include "Node.h" // node class	
#include <iostream> 
#include <queue>
#include <stack>

using namespace std; // for cout, endl, vector
using char_node_t = node<char>; // alias for node with char data

vector<char_node_t*> getDFS(char_node_t* root, char target)
{
	// check for valid root node // if root is null then return empty vector
	if (!root) return vector<char_node_t*>();

	// create nodes stack and push root onto stack
	vector<char_node_t*> node_stack; // 
	root->visited = true; // 
	node_stack.push_back(root); // push root onto stack

	while (!node_stack.empty())
	{
		// get current node from the top of stack
		char_node_t* current = node_stack.back(); //<get node on top of stack>
		// check if the current node data is target value
		if (current->data == target) // <if current node data is target value>
		{
			cout << "found: " << current->data << endl;
			break;
		}
		else
		{	// if current node data is not target value then print visited and continue
			cout << "visited: " << current->data << endl;
		}

		// if target not found then go down tree (child of current node)
		// push unvisited child node onto stack
		bool popNode = true; // pop node if there are no unvisited children

		for (auto& child : current->children) // <get child in children>
		{
			// if child node is unvisited then mark visited and push onto stack
			if (!child->visited)
			{
				child->visited = true; //<mark child visited>
				node_stack.push_back(child); //<push child onto node_stack>
				popNode = false; // don't pop current node off of stack
				break;
			}
		}
		// if no unvisited children of the current node then pop the stack to go back up the tree
		if (popNode)
		{	// if no unvisited children then pop node off of stack
			node_stack.pop_back(); //<pop node off of node_stack>
		}
	}

	// convert stack nodes to vector of nodes (path)
	// stack nodes are in reverse order with the target at the top
	// add stack nodes to front of path vector
	vector<char_node_t*> path;
	while (!node_stack.empty())
	{
		// add top node and then pop node off of stack
		path.insert(path.begin(), node_stack.back()); //<begin() of path>, <top of node_stack>
		node_stack.pop_back(); //<pop node_stack>
	}

	return path;
}

vector<char_node_t*> getBFS(char_node_t* root, char target)
{
	// check for valid root node
	if (!root) return vector<char_node_t*>();

	// create nodes queue and queue root onto stack
	queue<char_node_t*> node_queue; // <create queue of char_node_t*>
	root->visited = true;//<mark root visited>
	node_queue.push(root);	//<push root onto queue>

	// variable to hold target node
	char_node_t* targetNode = nullptr;

	while (!node_queue.empty())
	{
		// get current node from the front of the queue
		char_node_t* current = node_queue.front(); //<get node on top of stack>

		// check if the current node data is target value
		if (current->data == target) //< current node data is equal to target >
		{
			cout << "found: " << current->data << endl;
			targetNode = current;
			break;
		}
		else
		{	// if current node data is not target value then print visited and continue
			cout << "visit: " << current->data << endl;
		}

		// if target not found then push all the children of the current node into the queue
		for (auto& child : current->children) //<get child in children>
		{
			if (!child->visited) //<child not visited>
			{
				child->visited = true;//<set child visited to true>
				child->parent = current; //<set child parent to current>
				node_queue.push(child); //<push child onto node_queue>
			}
		}

		// pop the front of the queue
		node_queue.pop(); //<pop node_queue>
	}


	// convert nodes to vector of nodes (path)
	// nodes are in reverse order with the parent used to move through the nodes
	  // add nodes to front of path vector

	// construct path using parent pointers
	vector<char_node_t*> path; // <create path vector> // reconstruct path from target node to root
	//char_node_t* node = !node_queue.empty() ? node_queue.front() : nullptr; //<get front of node_queue, if node_queue is empty then set node to nullptr>
	while (targetNode != nullptr)
	{
		// add node and then set node to node parent
		path.insert(path.begin(), targetNode); //<begin() of path>, <top of node_stack>
		targetNode = targetNode->parent; // <node parent>;
	}

	return path;
}

int main()
{
	//      A
	//    B   C
	//  D   E   F
	// create nodes with char data

	char_node_t* nodeA = new char_node_t{ 'A' };
	char_node_t* nodeB = new char_node_t{ 'B' };
	char_node_t* nodeC = new char_node_t{ 'C' };
	char_node_t* nodeD = new char_node_t{ 'D' };
	char_node_t* nodeE = new char_node_t{ 'E' };
	char_node_t* nodeF = new char_node_t{ 'F' };

	// create tree from nodes (children)
	nodeA->children.push_back(nodeB); // A->B (A is parent, B is child)
	nodeA->children.push_back(nodeC); // A->C
	nodeB->children.push_back(nodeD); // B->D
	nodeB->children.push_back(nodeE); // B->E
	nodeC->children.push_back(nodeE); // C->E
	nodeC->children.push_back(nodeF); // C->F

	// get search path to 'F' from node 'A'
	//auto path = getDFS(nodeA, 'F');
	auto path = getBFS(nodeA, 'F');

	// display path result
	cout << "path: ";
	for (auto node : path) //<iterate through all the path nodes> // iterates over each node in the path vector 
	{
		cout << "->" << node->data; //<print node data>
	}
	cout << endl;


}


