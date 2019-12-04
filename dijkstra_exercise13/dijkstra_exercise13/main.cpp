/**********************************************************************************
 * FILE:            dijkstra_ex13.cpp
 * DESCRIPTION:     This program runs Dijkstra's algorithm on a weighted directed
 *                  graph. It solves exercise 25 in "Algorithms",
 *                  by Jeff Erickson, page 306.
 * DATE:            11/11/2019
 * AUTHOR:          Nieves Montes Gómez
 **********************************************************************************/

#include <iostream>
#include <string>
#include <queue>
#include <algorithm>
#include <vector>
#include <utility>

using namespace std;

struct node {
	string name;				
	bool visited = false;				
	double saf = 0.f;				
	struct node* pred = NULL;		
	vector <pair <struct node*, double>> adjList;	
};							

/*** Functions to sort a queue of nodes according to descending safety function ***/
bool priority(node* a, node* b) { return (a->saf > b->saf); }
void sortQueue(queue <struct node*> &Q) {
	vector <struct node*> aux;
	while (!Q.empty()) {
		aux.push_back(Q.front());
		Q.pop();
	}
	sort(aux.begin(), aux.end(), priority);
	for (int i = 0; i < aux.size(); i++) { Q.push(aux[i]); }
}

int main(int argc, char** argv) {

// build nodes
	struct node* VA = new node;		VA->name = "VA";		VA->saf = 1.f;		VA->visited = true;
	struct node* TN = new node;		TN->name = "TN";		
	struct node* NV = new node;		NV->name = "NV";		
	struct node* AZ = new node;		AZ->name = "AZ";		

// build directed edges
	(VA->adjList).push_back(make_pair(AZ, 0.5));
	(VA->adjList).push_back(make_pair(TN, 0.7));
	(VA->adjList).push_back(make_pair(NV, 0.2));
	(TN->adjList).push_back(make_pair(NV, 0.1));
	(TN->adjList).push_back(make_pair(AZ, 0.9));
	(NV->adjList).push_back(make_pair(AZ, 0.5));
	
// dump all nodes into a queue
	queue <struct node*> Q;
	Q.push(TN); Q.push(NV); Q.push(AZ); Q.push(VA);
	sortQueue(Q);

// empty queue to dump visited nodes
	queue <struct node*> S;
	
/*** DIJKSTRA ALGORITHM ***/
	node* currNode = NULL;
	node* succ = NULL;
	vector <pair<struct node*, double>>::iterator it;
	double p;
	while (!Q.empty()) {
		currNode = Q.front();       // extract node of highest saf. function
		currNode->visited = true;   // set it to "visited"
		Q.pop();                    // delete it from Q
		S.push(currNode);           // put it in S
		if (currNode->saf == 0) {cout << "Sorry, you have been abducted" << endl; break;}
		if (currNode == AZ) {cout << "Congratulations, you have reached area 51, AZ." << endl; break;}
		// check all adjacent nodes, but only if they are not in Q
		for (it = (currNode->adjList).begin(); it != (currNode->adjList).end(); ++it) {
			succ = it->first;
			p = it->second;
			if ((succ->saf < currNode->saf * p) && (succ->visited == false)) succ->saf = currNode->saf * p;
		}
		sortQueue(Q);				// arrange nodes that are left in descending saf.function
	}

// print the result
	if (currNode == AZ) {
		cout << "This has been your trip:\n";
		while (!S.empty()) {
			cout << "Position: " << S.front()->name << endl;
			cout << "\tSafety function: " << S.front()->saf << endl;
			S.pop();
		}
	}

	else if (Q.empty()) {
		cout << "Error: Dijkstra algorithm did not find an exit." << endl;
		exit(EXIT_FAILURE);
	}

// free memory
	delete VA;	delete TN;	delete NV;	delete AZ;

	return 0;
}
