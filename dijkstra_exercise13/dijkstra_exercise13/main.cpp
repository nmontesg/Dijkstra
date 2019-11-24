/**********************************************************************************
 * FILE:            dijkstra_ex13.cpp
 * DESCRIPTION:     This program runs Dijkstra's algorithm on a weighted directed
 *                  graph. It solves exercise 25 in "Algorithms",
 *                  by Jeff Erickson, page 306.
 * DATE:            11/11/2019
 * AUTHOR:          Nieves Montes Gómez
 **********************************************************************************/

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <queue>
#include <algorithm>

using namespace std;

typedef struct node {
	char name[3];       // name of the node
	int n_succ;         // number of successors
	bool visited;       // has the node been visited
	float saf;          // safety function of node
	struct node** adj;  // adjacency list
	float* probs;       // probabilities of edges to adjacent nodes
} node;

/*** buildNode() allocates memory to all the nodes ***/
node* buildNode(char state[3], int succ) {
	node* newNode = NULL;
	newNode = (node*)malloc(sizeof(struct node));
	strcpy_s(newNode->name, state);
	newNode->n_succ = succ;
	newNode->visited = false;
	newNode->saf = 0.f;
	newNode->adj = (struct node**)malloc((succ + 1) * sizeof(struct node*));
	newNode->probs = (float*)malloc(succ * sizeof(float));
	return newNode;
}

/*** freeNode() frees the allocated memory for a node ***/
void freeNode(node* anode) {
	free(anode->adj);
	free(anode->probs);
	free(anode);
}

/*** Functions to sort a queue of nodes according to descending
safety function ***/
bool priority(node* a, node* b) { return (a->saf > b->saf); }
queue <struct node*> sortQueue(queue <struct node*> Q) {
	vector <struct node*> aux;
	while (!Q.empty()) {
		aux.push_back(Q.front());
		Q.pop();
	}
	sort(aux.begin(), aux.end(), priority);
	for (int i = 0; i < aux.size(); i++) { Q.push(aux[i]); }
	return Q;
}

int main(int argc, char** argv) {

	/*** build the directed graph ***/
	node* VA = buildNode((char*)"VA", 3); // Langley, VA (ROOT)
	VA->saf = 1.f;                        // set VA node as root
	node* TN = buildNode((char*)"TN", 2); // Memphis, TN
	node* NV = buildNode((char*)"NV", 1); // Las Vegas, NV
	node* AZ = buildNode((char*)"AZ", 0); // Area 51, AZ

// adjacency list is arranged alphabetically
	*(VA->adj + 0) = AZ;    *(VA->probs + 0) = 0.5;
	*(VA->adj + 1) = NV;    *(VA->probs + 1) = 0.2;
	*(VA->adj + 2) = TN;    *(VA->probs + 2) = 0.7;
	*(VA->adj + 3) = NULL;
	*(TN->adj + 0) = AZ;    *(TN->probs + 0) = 0.9;
	*(TN->adj + 1) = NV;    *(TN->probs + 1) = 0.1;
	*(TN->adj + 2) = NULL;
	*(NV->adj + 0) = AZ;    *(NV->probs + 0) = 0.5;
	*(NV->adj + 1) = NULL;
	*(AZ->adj + 0) = NULL;

	// dump all nodes into a queue
	queue <struct node*> Q;
	Q.push(TN); Q.push(NV); Q.push(AZ); Q.push(VA);
	Q = sortQueue(Q);

	// empty queue to dump visited nodes
	queue <struct node*> S;
	node* currNode = NULL;
	node* possNextNode = NULL;

	/*** DIJKSTRA ALGORITHM ***/
	int j;
	while (!Q.empty()) {
		currNode = Q.front();       // extract node of highest saf. function
		currNode->visited = true;   // set it to "visited"
		Q.pop();                    // delete it from Q
		S.push(currNode);           // put it in S
		if (currNode->saf == 0) { cout << "Sorry, you have been abducted" << endl; break; }
		if (currNode == AZ) { cout << "Congratulations, you have reached area 51, AZ." << endl; break; }
		j = 0;  // check all adjacent nodes, but only if they are not in Q
		while (*(currNode->adj + j) != NULL) {
			possNextNode = *(currNode->adj + j);
			if ((possNextNode->saf < currNode->saf * *(currNode->probs + j)) && (possNextNode->visited == false)) {
				possNextNode->saf = currNode->saf * *(currNode->probs + j);
			}
			j++;
		}
		Q = sortQueue(Q);   // arrange nodes that are left in descending saf.function
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

	// free allocated memory for the nodes
	freeNode(VA);   freeNode(TN);   freeNode(NV);   freeNode(AZ);

	return 0;
}
