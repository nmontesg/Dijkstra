/**********************************************************************************
 * FILE:            kruskal.cpp
 * DESCRIPTION:     This program runs Kruskal's algorithm on a weighted undirected
 *                  graph. It follows the pseudocode described in "Algorithms",
 *                  by Jeff Erickson, page 267.
 * DATE:            05/11/2019
 * AUTHOR:          Nieves Montes Gómez
 **********************************************************************************/

#include <iostream>
#include <vector>
#include <iterator>
#include <utility> 
#include <algorithm>
#include <list>
using namespace std;

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

/*** print_sets() prints the content of each set as the list of nodes. ***/
void print_sets(vector <list <int>>& sets) {
	vector <list <int>>::iterator j;
	list <int>::iterator k;
	int n;
	for (j = sets.begin(); j != sets.end(); ++j) {
		n = distance(sets.begin(), j);
		cout << "nodes in set " << n << ": ";
		for (k = sets[n].begin(); k != sets[n].end(); ++k) {
			cout << *k << " ";
		}
		cout << endl;
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

/*** FIND(u) function: return the index of the sets vector where node u is included. ***/
int find(vector <list <int>>& sets, int u) {
	vector <list <int>>::iterator i;
	list <int>::iterator j;
	int n;
	int index = -1;
	for (i = sets.begin(); i != sets.end(); ++i) {
		n = distance(sets.begin(), i);
		for (j = sets[n].begin(); j != sets[n].end(); ++j) {
			if (*j == u) { index = n; break; }
		}
	}
	if (index == -1) {
		cout << "Node " << u << " not found in any of the sets. Returning index -1." << endl;
	}
	return index;
}

/*** UNION(u, v) function: it adds the set containing the v node to that of the u node.
It deletes the set containing the v node. ***/
void unite(vector <list <int>>& sets, int u, int v) {
	int n = find(sets, u);
	int m = find(sets, v);
	if (n == m) {
		cout << "Nodes " << u << " and " << v << "are already in the same set." << endl;
		return;
	}
	while (!sets[m].empty()) {
		sets[n].push_back(sets[m].front());
		sets[m].pop_front();
	}
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

// sort edges by ascending order of the first element in pair, i.e. the weight, and print
	cout << "sorted edges:" << endl;
	sort(edges.begin(), edges.end());
	print_edges(edges);

// create vector of edges F, where we will dump the selected edges
	vector <pair <float, pair <int, int>>> F;

// for each node v, MAKE_SET(v): each set is a component of vector sets and print the content of each set
	int V = nodes.size();
	int n;
	vector <list <int>> sets(V);
	vector <int>::iterator i;
	for (i = nodes.begin(); i != nodes.end(); ++i) {
		n = distance(nodes.begin(), i);
		sets[n].push_back(*i);
	}
	cout << "individual sets created: " << endl;
	print_sets(sets);

// iterate over all (sorted) edges, and copy into F only those that do not close any cycles
	int u, v;
	vector <pair <float, pair <int, int>>>::iterator it;
	cout << "START KRUSKAL ALGORITHM" << endl << endl;
	for (it = edges.begin(); it != edges.end(); ++it) {
		u = (it->second).first;
		v = (it->second).second;
		cout << "we are scanning edge: " << u << " --" << (it->first) << "-- " << v << endl;
		if (find(sets, u) != find(sets, v)) {
			unite(sets, u, v);
			F.push_back(*it);
			cout << "the edge is safe to be selected" << endl;
			cout << "new sets:" << endl;
			print_sets(sets);
		}
		else {
			cout << "the edge is not safe" << endl << endl;
		}
	}

	cout << endl << "selected edges after running Kruskal algorithm:" << endl;
	print_edges(F);

	float cost = tree_cost(F);
	cout << "Final tree cost: " << cost << endl << endl;


	return 0;
}