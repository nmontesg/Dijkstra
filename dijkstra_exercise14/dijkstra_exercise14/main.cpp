/**********************************************************************************
 * FILE:            dijkstra_ex14.cpp
 * DESCRIPTION:     This program runs Dijkstra's algorithm on a weighted directed
 *                  graph. It solves exercise 26 in "Algorithms",
 *                  by Jeff Erickson, page 307.
 * DATE:            11/11/2019
 * AUTHOR:          Nieves Montes Gómez
 **********************************************************************************/

#include <iostream>
#include <string>
#include <queue>
#include <algorithm>
#include <vector>
#include <stdlib.h>
#include <crtdbg.h>

using namespace std;

struct node {
	string name;
	bool visited = false;
	double p = 0.f; // parameter of the problem
	double risk = 1.f;
	vector <pair<struct node*, double>> adjList;
};

/*** add_edge() builds an undirected weighted edge between nodes a and b ***/
void add_edge(node* node_a, node* node_b, float prob) {
	(node_a->adjList).push_back(make_pair(node_b, prob));
	(node_b->adjList).push_back(make_pair(node_a, prob));
}

/*** Functions to sort a queue of nodes according to ascening risk function ***/
bool priority(node* a, node* b) { return (a->risk < b->risk); }
void sortQueue(queue <struct node*>& Q) {
	vector <struct node*> aux;
	while (!Q.empty()) {
		aux.push_back(Q.front());
		Q.pop();
	}
	sort(aux.begin(), aux.end(), priority);
	for (int i = 0; i < aux.size(); i++) { Q.push(aux[i]); }
}

int main(int argc, char** argv) {

	_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);

// build the nodes
	struct node* a = new node;		a->name = "a";		a->p = 0.3;		a->risk = a->p;			a->visited = true;
	struct node* b = new node;		b->name = "b";		b->p = 0.4;
	struct node* c = new node;		c->name = "c";		c->p = 0.6;
	struct node* d = new node;		d->name = "d";		d->p = 0.4;
	struct node* e = new node;		e->name = "e";		e->p = 0.4;
	struct node* f = new node;		f->name = "f";		f->p = 0.7;
	struct node* g = new node;		g->name = "g";		g->p = 0.5;
	struct node* h = new node;		h->name = "h";		h->p = 0.6;
	struct node* i = new node;		i->name = "i";		i->p = 0.4;
	struct node* j = new node;		j->name = "j";		j->p = 0.4;
	struct node* k = new node;		k->name = "k";		k->p = 0.6;
	
// build the edges
	add_edge(a, b, 0.3);	add_edge(a, j, 0.5);	add_edge(b, j, 0.4);	add_edge(b, c, 0.2);
	add_edge(c, j, 0.2);	add_edge(j, k, 0.4);	add_edge(j, i, 0.3);	add_edge(c, k, 0.4);
	add_edge(c, d, 0.3);	add_edge(i, k, 0.3);	add_edge(k, d, 0.5);	add_edge(i, h, 0.4);
	add_edge(k, h, 0.4);	add_edge(k, e, 0.5);	add_edge(d, e, 0.2);	add_edge(h, e, 0.3);
	add_edge(h, g, 0.3);	add_edge(e, g, 0.6);	add_edge(h, f, 0.6);	add_edge(e, f, 0.4);
	add_edge(f, g, 0.2);

// dump all nodes into a queue
	queue <struct node*> Q;
	Q.push(a); Q.push(b); Q.push(c); Q.push(d); Q.push(e); Q.push(f);
	Q.push(g); Q.push(h); Q.push(i); Q.push(j); Q.push(k);
	sortQueue(Q);

// empty queue to dump visited nodes
	queue <struct node*> S;

/*** DIJKSTRA ALGORITHM ***/
	node* currNode = NULL;
	node* succ = NULL;
	vector <pair<struct node*, double>>::iterator it;
	double aux;
	double new_risk;
	while (!Q.empty()) {
		currNode = Q.front();       // extract node of lowest risk function
		currNode->visited = true;   // set it to "visited"
		Q.pop();                    // delete it from Q
		S.push(currNode);           // put it in S
		if (currNode->risk >= 1) { cout << "Sorry, you are dead" << endl; break; }
		if (currNode == g) { cout << "Congratulations, you have made it out alive." << endl; break; }
		// check all adjacent nodes, but only if they are not in Q
		for (it = (currNode->adjList).begin(); it < (currNode->adjList).end(); it++) {
			succ = it->first;
			aux = it->second + succ->p - it->second * succ->p;
			new_risk = currNode->risk + aux - currNode->risk * aux;
			if ((succ->risk > new_risk) && (succ->visited == false)) succ->risk = new_risk;
		}
		sortQueue(Q);   // arrange nodes that are left in descending saf.function
	}

// print the result
	if (currNode == g) {
		cout << "This has been your trip:\n";
		while (!S.empty()) {
			cout << "Position: " << S.front()->name << endl;
			cout << "\tRisk function: " << S.front()->risk << endl;
			S.pop();
		}
	}

	else if (Q.empty()) {
		cout << "Error: Dijkstra algorithm did not find an exit." << endl;
		exit(EXIT_FAILURE);
	}

// free memory
	delete a;	delete b;	delete c;	delete d;	delete e;	delete f;	delete g;
	delete h;	delete i;	delete j;	delete k;

	return 0;
}
