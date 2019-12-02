/**********************************************************************************
 * FILE:            prim.cpp
 * DESCRIPTION:     This program runs Prim's algorithm on a weighted undirected
 *                  graph. It follows the pseudocode described in "Algorithms",
 *                  by Jeff Erickson, page 265.
 * DATE:            07/11/2019
 * AUTHOR:          Nieves Montes Gómez
 **********************************************************************************/

#include <iostream>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <iterator>
#include <utility> 
#include <algorithm>
#include <list>
using namespace std;

struct node_prop {
	int id;
	pair <float, pair <int, int>>* node_edge;
	float prior;
};

/*** print_nodes() prints the vector of integer nodes. ***/
void print_nodes(vector <int>& nodes) {
	vector <int>::iterator it;
	for (it = nodes.begin(); it != nodes.end(); ++it) {
		cout << "node " << *it << endl;
	}
	cout << endl;
}

/*** print_edges() prints the vector of edges as node1 --weight-- node2. ***/
void print_edges(vector <pair <float, pair <int, int>>>& edges) {
	vector <pair <float, pair <int, int>>>::iterator it;
	for (it = edges.begin(); it != edges.end(); ++it) {
		cout << (it->second).first << " --" << it->first << "-- " << (it->second).second << endl;
	}
	cout << endl;
}

/*** print_properties() prints the current values of the priority queue ***/
void print_properties(vector <node_prop> P) {
	vector <node_prop>::iterator it;
	for (it = P.begin(); it != P.end(); ++it) {
		cout << "node " << it->id << "; ";
		if ((it->node_edge) == nullptr) { cout << "edge NULL; "; }
		else { cout << "edge " << ((it->node_edge)->second).first << " --" << (it->node_edge)->first << "-- " << ((it->node_edge)->second).second << "; "; }
		cout << "priority " << it->prior << endl;
	}
	cout << endl;
}

/*** add_node() to vector of integer nodes. ***/
void add_node(vector <int>& nodes, int u) {
	nodes.push_back(u);
}

/*** add_edge() to vector of edges, weight included. ***/
void add_edge(vector <pair <float, pair <int, int>>>& edges, int u, int v, float w) {
	edges.push_back(make_pair(w, make_pair(u, v)));
}

/*** find_edge() returns a pointer to the edge between nodes v and s. If there is no such node, it returns a null pointer. ***/
pair <float, pair <int, int>>* find_edge(vector <pair <float, pair <int, int>>>& edges, int v, int s) {
	vector <pair <float, pair <int, int>>>::iterator it;
	for (it = edges.begin(); it != edges.end(); ++it) {
		if ((it->second).first == v && (it->second).second == s) { return &(*it); }
		if ((it->second).first == s && (it->second).second == v) { return &(*it); }
	}
	return nullptr;
}

/*** sorting() is a custom function to help sort variables of type node_prop by descending priority. ***/
bool sorting(node_prop a, node_prop b) { return (a.prior > b.prior); }

/*** find_neighbors() returns a vector of the neighbors of node v. ***/
vector <int> find_neighbors(vector <pair <float, pair <int, int>>>& edges, int v) {
	vector <int> neighbors;
	vector <pair <float, pair <int, int>>>::iterator it;
	for (it = edges.begin(); it != edges.end(); ++it) {
		if ((it->second).first == v) neighbors.push_back((it->second).second);
		if ((it->second).second == v) neighbors.push_back((it->second).first);
	}
	return neighbors;
}

/*** find_priority() returns the priority of node v. ***/
float find_priority(vector <node_prop>& P, int v) {
	vector <node_prop>::iterator it;
	for (it = P.begin(); it != P.end(); ++it) {
		if (it->id == v) { return it->prior; }
	}
	if (it == P.end()) {
		cout << "Node " << v << " is not in the priority vector. find_priority() will return inf." << endl;
		return INFINITY;
	}
}

/*** find_in_queue() returns an integer index such that P[n] corresponds to node v. If v is not in P, it returns -1. ***/
int find_in_queue(vector <node_prop>& P, int v) {
	vector <node_prop>::iterator it;
	for (it = P.begin(); it != P.end(); ++it) {
		if (it->id == v) { return distance(P.begin(), it); }
	}
	if (it == P.end()) { return -1; }
}

/*** tree_cost() computes the cost of the final spanning tree. ***/
float tree_cost(vector <pair <float, pair <int, int>>>& edges) {
	float cost = 0.f;
	vector <pair <float, pair <int, int>>>::iterator it;
	for (it = edges.begin(); it != edges.end(); ++it) { cost += it->first; }
	return cost;
}

int main(int argc, char** argv) {

// create vector of nodes, add some and print
	vector <int> nodes;
	add_node(nodes, 1);
	add_node(nodes, 2);
	add_node(nodes, 3);
	add_node(nodes, 4);
	add_node(nodes, 5);
	add_node(nodes, 6);
	add_node(nodes, 7);
	cout << "these are the graph's nodes:" << endl;
	print_nodes(nodes);

// create vector of edges, add some and print
	vector <pair <float, pair <int, int>>> edges;
	add_edge(edges, 1, 2, 26);
	add_edge(edges, 1, 3, 4);
	add_edge(edges, 1, 4, 14);
	add_edge(edges, 3, 4, 12);
	add_edge(edges, 2, 4, 30);
	add_edge(edges, 3, 6, 18);
	add_edge(edges, 2, 5, 16);
	add_edge(edges, 4, 6, 2);
	add_edge(edges, 4, 5, 3);
	add_edge(edges, 5, 6, 10);
	add_edge(edges, 6, 7, 8);
	add_edge(edges, 5, 7, 5);
	cout << "edges as input:" << endl;
	print_edges(edges);

// select source node
	int s = atoi(argv[1]);
	if (s > nodes.size()) { cout << "Invalid source." << endl; return 1; }
// remove source from nodes vector (erase-remove idiom)
	nodes.erase(remove(nodes.begin(), nodes.end(), s));
	cout << "nodes after deleting the source:" << endl;
	print_nodes(nodes);

// Initialize priority queue
	vector <node_prop> P;
	vector <int>::iterator i;
	pair <float, pair <int, int>>* the_edge;
	float prior;
	for (i = nodes.begin(); i != nodes.end(); ++i) {
		the_edge = find_edge(edges, *i, s);
		if (the_edge == nullptr) { prior = INFINITY; }
		else { prior = the_edge->first; }
		P.push_back({ *i, the_edge, prior });
	}
	cout << "initial priority queue:" << endl;
	print_properties(P);

	sort(P.begin(), P.end(), sorting);
	cout << "initial priority queue after sorting:" << endl;
	print_properties(P);

// Prim Loop
	vector <int> T_nodes;
	T_nodes.push_back(s);
	vector <pair <float, pair <int, int>>> T_edges;
	node_prop currNode;
	vector <int> neighbors;
	int n_neighbor;
	pair <float, pair <int, int>>* edge_to_curr;
	float prior_neighbor, w;

	cout << "START PRIM ALGORITHM" << endl << endl;
	while (!P.empty()) {
		currNode = P.back();
		P.pop_back();
		cout << "selected node: " << currNode.id << endl;
		T_nodes.push_back(currNode.id);
		if (currNode.node_edge == nullptr) { T_edges.push_back(pair <float, pair <int, int>>()); }
		else T_edges.push_back(*currNode.node_edge);
		neighbors = find_neighbors(edges, currNode.id);
		cout << "current node's neighbors:" << endl;
		print_nodes(neighbors);
		for (i = neighbors.begin(); i != neighbors.end(); ++i) {
			n_neighbor = find_in_queue(P, *i);
			if (n_neighbor == -1) {
				cout << "neighbor " << *i << " is no longer in the priority queue." << endl;
				continue;
			}
			edge_to_curr = find_edge(edges, currNode.id, *i);
			prior_neighbor = P[n_neighbor].prior;
			w = edge_to_curr->first;
			if (prior_neighbor > w) {
				P[n_neighbor].node_edge = edge_to_curr;
				P[n_neighbor].prior = w;
				cout << "neighbor " << *i << " has updated its priority from " << prior_neighbor << " to " << w << "." << endl;
			}
			else cout << "neighbor " << *i << " has not updated its priority." << endl;
		}
		sort(P.begin(), P.end(), sorting);
		cout << endl << "updated priority queue:" << endl;
		print_properties(P);
	}

	cout << endl << "selected nodes after running Prim algorithm (all nodes):" << endl;
	print_nodes(T_nodes);

	cout << endl << "selected edges after running Prim algorithm:" << endl;
	print_edges(T_edges);

	float cost = tree_cost(T_edges);
	cout << "Final tree cost: " << cost << endl << endl;

	return 0;
}
