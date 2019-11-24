/**********************************************************************************
 * FILE:            dijkstra_ex12.cpp
 * DESCRIPTION:     This program runs Dijkstra's algorithm on a weighted directed
 *                  graph. It solves exercise 21 in "Algorithms",
 *                  by Jeff Erickson, page 304.
 * DATE:            14/11/2019
 * AUTHOR:          Nieves Montes Gómez
 **********************************************************************************/

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <algorithm>
#include <utility>

using namespace std;

typedef struct node {
	char name[4];       // name of the node
	bool visited;       // has the node been visited
	float time;			// time to get to airport node in hours
	struct node* pred;  // predecessor node
} node;

/*** buildNode() allocates memory to a nodes ***/
node* buildNode(char airport[4]) {
	node* newNode = (node*)malloc(sizeof(struct node));
	strcpy_s(newNode->name, airport);
	newNode->visited = false;
	newNode->time = INFINITY;
	newNode->pred = NULL;
	return newNode;
}

/*** Functions to sort a queue of nodes according to ascending time to get there ***/
bool priority(node* a, node* b) { return (a->time < b->time); }
queue <struct node*> sortQueue(queue <struct node*>& Q) {
	vector <struct node*> aux;
	while (!Q.empty()) {
		aux.push_back(Q.front());
		Q.pop();
	}
	sort(aux.begin(), aux.end(), priority);
	for (unsigned int i = 0; i < aux.size(); i++) { Q.push(aux[i]); }
	return Q;
}

void print_flight(node* currNode, vector <pair < pair <node*, node*>, pair <float, float> >>& flights) {
	node* pred = currNode->pred;
	vector <pair < pair <node*, node*>, pair <float, float> >>::iterator it;
	for (it = flights.begin(); it < flights.end(); it++) {
		if (((it->first).first == pred) && ((it->first).second == currNode)) {
			cout << "from " << pred->name << " to " << currNode->name;
			cout << " ; departure " << (it->second).first << " ; arrival " << (it->second).second << endl;
			break;
		}
	}

}

int main(int argc, char** argv) {

	/*** build the directed graph ***/
	node* BCN = buildNode((char*)"BCN"); // Barcelona (ROOT)
	BCN->time = 0.f;                     // set BCN node as root
	node* MSW = buildNode((char*)"MSW"); // Moscow
	node* DUB = buildNode((char*)"DUB"); // Dubai
	node* HKG = buildNode((char*)"HKG"); // Hong Kong
	node* PER = buildNode((char*)"PER"); // Perth

// data of flights is stored in a vector with structure <<origin,destination>,<departure,arrival>>
	vector <pair < pair <node*, node*>, pair <float, float> >> flights;
	flights.push_back(make_pair(make_pair(BCN, MSW), make_pair(0, 4.67)));
	flights.push_back(make_pair(make_pair(BCN, DUB), make_pair(2.08, 8.75)));
	flights.push_back(make_pair(make_pair(MSW, DUB), make_pair(6.33, 14)));
	flights.push_back(make_pair(make_pair(MSW, HKG), make_pair(8.17, 22.67)));
	flights.push_back(make_pair(make_pair(DUB, HKG), make_pair(15.83, 27.08)));
	flights.push_back(make_pair(make_pair(DUB, PER), make_pair(15.33, 29.92)));
	flights.push_back(make_pair(make_pair(HKG, PER), make_pair(30.75, 42.17)));
	vector <pair < pair <node*, node*>, pair <float, float> >>::iterator it;

	// dump all nodes into a queue and sort
	queue <struct node*> Q;
	Q.push(BCN); Q.push(MSW); Q.push(DUB); Q.push(HKG); Q.push(PER);
	Q = sortQueue(Q);

	// empty queue to dump visited nodes
	queue <struct node*> S;
	node* currNode = NULL;
	node* possNextNode = NULL;

	// now variable stores current time
	float now = 0;	// today at 11:25
// weight variable is not fixed for every edge, because it depends on the current time
	float weight;

	/*** DIJKSTRA ALGORITHM ***/
	cout << "... Dijkstra algorithm in progres..." << endl << endl;
	while (!Q.empty()) {
		currNode = Q.front();       // extract node of lowest time to get there
		currNode->visited = true;   // set it to "visited"
		Q.pop();                    // delete it from Q
		S.push(currNode);           // put it in S
		now = currNode->time;
		cout << "we are in " << currNode->name << endl;
		if (currNode == PER) { cout << endl << "Congratulations, you have reached your destination." << endl; break; }
		// iterate over the list of flights to find flights departing from current node
		for (it = flights.begin(); it < flights.end(); it++) {
			if ((it->first).first == currNode) {
				possNextNode = (it->first).second;
				cout << "possible next node: " << possNextNode->name << endl;
				weight = (it->second).second - now;
				if ((possNextNode->time > currNode->time + weight) && (possNextNode->visited == false) && ((it->second).first > now)) {
					possNextNode->time = currNode->time + weight;
					possNextNode->pred = currNode;
				}
			}
		}
		cout << endl;
		Q = sortQueue(Q);   // arrange nodes that are left in descending time function
	}
	cout << "... Dijkstra algorithm finished..." << endl << endl;

	// Reconstruct the trip
	node* pred = NULL;
	if (currNode == PER) {
		cout << endl << "This has been your trip:\n";
		while (currNode != BCN) {
			print_flight(currNode, flights);
			currNode = currNode->pred;
		}
	}

	// free allocated memory for the nodes
	free(BCN);
	free(MSW);
	free(DUB);
	free(HKG);
	free(PER);

	return 0;
}