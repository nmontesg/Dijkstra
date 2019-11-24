/**********************************************************************************
 * FILE:            dijkstra_ex14.cpp
 * DESCRIPTION:     This program runs Dijkstra's algorithm on a weighted directed
 *                  graph. It solves exercise 26 in "Algorithms",
 *                  by Jeff Erickson, page 307.
 * DATE:            11/11/2019
 * AUTHOR:          Nieves Montes Gómez
 **********************************************************************************/

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <algorithm>
#include <vector>
#include <cstring>

using namespace std;

typedef struct node {
	char name[2];	// name of the node
	int n_succ;         // number of successors
	bool visited;       // has the node been visited
	float p_node;		// probability of meeting a vampire at the node
	float risk;         // risk function of node
	vector <pair<struct node*, float>> adj;  // adjacency list: adjacent node, probability of node
} node;

/*** buildNode() allocates memory to all the nodes ***/
node* buildNode(char* id, int succ, float p_v) {
	node* newNode = NULL;
	newNode = (node*)malloc(sizeof(struct node));
	strcpy_s(newNode->name, id);
	newNode->n_succ = succ;
	newNode->visited = false;
	newNode->p_node = p_v;
	newNode->risk = INFINITY;
	return newNode;
}

/*** add_edge() builds an undirected weighted edge between nodes a and b ***/
void add_edge(node* node_a, node* node_b, float prob) {
	(node_a->adj).push_back(make_pair(node_b, prob));
	(node_b->adj).push_back(make_pair(node_a, prob));
}

/*** Functions to sort a queue of nodes according to ascening risk function ***/
bool priority(node* a, node* b) { return (a->risk < b->risk); }
queue <struct node*> sortQueue(queue <struct node*> Q) {
	vector <struct node*> aux;
	while (!Q.empty()) {
		aux.push_back(Q.front());
		Q.pop();
	}
	sort(aux.begin(), aux.end(), priority);
	for (unsigned int i = 0; i < aux.size(); i++) { Q.push(aux[i]); }
	return Q;
}

int main(int argc, char** argv) {

	/*** build the graph ***/
	node* a = buildNode((char*)"a", 2, (float)0.3);
	a->risk = a->p_node;
	node* b = buildNode((char*)"b", 3, (float)0.4);
	node* c = buildNode((char*)"c", 4, (float)0.6);
	node* d = buildNode((char*)"d", 3, (float)0.4);
	node* e = buildNode((char*)"e", 5, (float)0.4);
	node* f = buildNode((char*)"f", 3, (float)0.7);
	node* g = buildNode((char*)"g", 3, (float)0.5);
	node* h = buildNode((char*)"h", 5, (float)0.6);
	node* i = buildNode((char*)"i", 3, (float)0.4);
	node* j = buildNode((char*)"j", 4, (float)0.4);
	node* k = buildNode((char*)"k", 6, (float)0.6);

	// build edges
	add_edge(a, b, (float)0.3);	add_edge(a, j, (float)0.5);	add_edge(b, j, (float)0.4);	add_edge(b, c, (float)0.2);
	add_edge(c, j, (float)0.2);	add_edge(j, k, (float)0.4);	add_edge(j, i, (float)0.3);	add_edge(c, k, (float)0.4);
	add_edge(c, d, (float)0.3);	add_edge(i, k, (float)0.3);	add_edge(k, d, (float)0.5);	add_edge(i, h, (float)0.4);
	add_edge(k, h, (float)0.4);	add_edge(k, e, (float)0.5);	add_edge(d, e, (float)0.2);	add_edge(h, e, (float)0.3);
	add_edge(h, g, (float)0.3);	add_edge(e, g, (float)0.6);	add_edge(h, f, (float)0.6);	add_edge(e, f, (float)0.4);
	add_edge(f, g, (float)0.2);

	// dump all nodes into a queue
	queue <struct node*> Q;
	Q.push(a); Q.push(b); Q.push(c); Q.push(d); Q.push(e); Q.push(f);
	Q.push(g); Q.push(h); Q.push(i); Q.push(j); Q.push(k);
	Q = sortQueue(Q);

	// empty queue to dump visited nodes
	queue <struct node*> S;
	node* currNode = NULL;
	node* possNextNode = NULL;

	// iterator over adjacency list
	vector <pair<struct node*, float>>::iterator it;
	float aux;

	/*** DIJKSTRA ALGORITHM ***/
	while (!Q.empty()) {
		currNode = Q.front();       // extract node of lowest risk function
		currNode->visited = true;   // set it to "visited"
		Q.pop();                    // delete it from Q
		S.push(currNode);           // put it in S
		if (currNode->risk >= 1) { cout << "Sorry, you are dead" << endl; break; }
		if ((currNode == e) || (currNode == g)) { cout << "Congratulations, you have made it out alive." << endl; break; }
		// check all adjacent nodes, but only if they are not in Q
		for (it = (currNode->adj).begin(); it < (currNode->adj).end(); it++) {
			possNextNode = it->first;
			aux = it->second + possNextNode->p_node - it->second * possNextNode->p_node;
			if ((possNextNode->risk > currNode->risk + aux - currNode->risk * aux) && (possNextNode->visited == false)) {
				possNextNode->risk = currNode->risk + aux - currNode->risk * aux;
			}
		}
		Q = sortQueue(Q);   // arrange nodes that are left in descending saf.function
	}

	// print the result
	if ((currNode == e) || (currNode == g)) {
		cout << "This has been your trip:\n";
		while (!S.empty()) {
			cout << "Position: " << S.front()->name << endl;
			cout << "\tRisk function: " << S.front()->risk << endl;
			S.pop();
		}
	}

	// free allocated memory for the nodes
	free(a); free(b); free(c); free(d); free(e); free(f);
	free(g); free(h); free(i); free(j); free(k);

	return 0;
}
