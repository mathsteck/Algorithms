/*
  Authors:
  Matheus Steck Cardoso - mathsteck@gmail.com

    The MIT License (MIT)

    Copyright (c) 2014 Matheus Steck Cardoso and Rafael

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/

#include <iostream>
#include <typeinfo>
#include <stdlib.h>
#include <fstream>

#define TYPE int // If you running on x64 this need to be intptr_t
#define INFINITE 666666

using namespace std;

class Node {

    int id;
    void *content; // This content can be anything (Pointer to an list or just an integer).
    Node *next, *previous;

    public:
        Node(Node* copy_node) {
            content = NULL;
            if(copy_node != NULL) {
                id = copy_node->get_id();
                content = copy_node->get_content();
            }
            next = NULL;
            previous = NULL;
        }

        Node(void) {
            id = 0;
            next = NULL;
            previous = NULL;
            content = NULL;
        }

        Node(void *new_content) {
            content = new_content;
        }

        void add_next(Node *node) {
            next = node;
        }

        void add_previous(Node *node) {
            previous = node;
        }

        void add_content(void *new_content) {
            content = new_content;
        }

        void set_id(int new_id) {
            id = new_id;
        }

        int get_id(void) {
            return id;
        }

        void* get_content(void) {
            return content;
        }

        Node* get_next() {
            return next;
        }

        Node* get_previous() {
            return previous;
        }
};

class List {

    int n_elements;
    Node *first, *last;

    public:
        List(void) {
            n_elements = 0;
            first = NULL;
            last = NULL;
        }

        void add(Node *new_node) {
            if(first == NULL) {
                first = new_node; // Create a new head's list
                last = new_node;
            }
            else {
                new_node->add_previous(last);
                last->add_next(new_node); // Add the new node in the end of the list
                last = new_node;
            }
            n_elements++;
        }

        void remove_first(void) {
            if (first != NULL) {
                Node *trash = first;
                first = first->get_next();
                delete(trash);
                n_elements--;
            }
        }

        Node* search(int id) { // The id (position in the list) of the Node
            int i;
            Node *navigation = first;

            for(i = 0; navigation->get_next() != NULL && i < id; i++) { // Navigate in the list
                navigation = navigation->get_next();
            }

            if(i < id)  // If reaches the end of the list
                return NULL;

            return navigation; // Return a pointer to the searched Node
        }

        Node* unordered_search(int id) {
            Node *navigation = first;

            while(navigation->get_next() != NULL && navigation->get_id() != id)
                navigation = navigation->get_next();

            if(navigation->get_id() != id)
                return NULL;
            return navigation;
        }

        Node* get_first(void) {
            return first;
        }

        Node *get_last(void) {
            return last;
        }

        int get_n_elements() {
            return n_elements;
        }

        int is_empty() {
            if(first == NULL && last == NULL)
                return true;
            return false;
        }

        void insert_content(int id, void *new_content) {
            Node *vertex = unordered_search(id);

            if(vertex != NULL)
                vertex->add_content(new_content);
        }

        void* get_content(int id) {
            Node *vertex = unordered_search(id);
            return vertex->get_content();
        }

        void print() {
            Node *navigation = first;
            while(navigation != NULL) {
                cout << "id: " << navigation->get_id() << " "  << (TYPE) navigation->get_content() << " -> ";
                navigation = navigation->get_next();
            }
            cout << endl;
        }
};

typedef struct edge {
    int v1, v2, value;
}Edge;

class Graph {

    int n_vertex, n_edges;
    List vertex_list; // Stores a list of pointers to all adjacent lists of this graph
    List edge_list;

    public:
        Graph(void) {
            n_vertex = 0;
            n_edges = 0;
        }
                                                   // Resume: Vertex list -> Adjacent list -> head_of_adjacent
        void add_vertex(Node* head_of_adjacent) {
            Node *link_adjacent = new Node;  // Create a new link to store a pointer to an adjacent list
            List *adjacent_list = new List;  // Create an adjacent list

            adjacent_list->add(head_of_adjacent);  // Add the head of the adjacent list
            link_adjacent->add_content((List*) adjacent_list); // Link the vertex list with the adjacent list
            vertex_list.add(link_adjacent); // Add the pointer to an adjacent list into the vertex list

            head_of_adjacent->set_id(n_vertex + 1);
            n_vertex++;
        }

        void connect_vertex(int vertex_id1, int vertex_id2) {
            Node *link_adjacent1 = (Node*) vertex_list.search(vertex_id1); // Get the link to the adjacent list of the first vertex
            Node *link_adjacent2 = (Node*) vertex_list.search(vertex_id2); // Get the link to the adjacent list of the second vertex

            List *adjacent_list1 = (List*) link_adjacent1->get_content(); // Get the adjacent list of the first vertex
            List *adjacent_list2 = (List*) link_adjacent2->get_content(); // Get the adjacent list of the second

            Node *vertex_copy1 = new Node;
            Node *vertex_copy2 = new Node;

            *vertex_copy1 = *adjacent_list1->get_first(); // Copy the content of the original first vertex
            *vertex_copy2 = *adjacent_list2->get_first(); // Copy the content of the original second vertex
            vertex_copy1->add_next(NULL);
            vertex_copy2->add_next(NULL);


            if(adjacent_list1 != NULL && adjacent_list2 != NULL) {
                adjacent_list1->add((Node*) vertex_copy2); // Add the vertex2 into the adjacent list from vertex1
                adjacent_list2->add((Node*) vertex_copy1); // Add the vertex1 into the adjacent list from vertex2
            }

            Edge *edge = new Edge;

            edge->v1 = vertex_copy1->get_id();
            edge->v2 = vertex_copy2->get_id();
            edge->value = 0;

            Node *node_edge = new Node();

            node_edge->add_content((Edge*) edge);

            edge_list.add(node_edge);
            n_edges++;
        }

        void print() {
            vertex_list.print();
        }

        void print_edges() {
            for(int i = 0; i < edge_list.get_n_elements(); i++) {
                Node *node = edge_list.search(i);
                Edge *e = (Edge*) node->get_content();

                cout << node->get_id() << ") ";

                if((int) e->v1 < (int) e->v2)
                    cout << (int) e->v1 << " " << (int) e->v2 << " " << (int) e->value << endl;
                else
                    cout << (int) e->v2 << " " << (int) e->v1 << " " << (int) e->value << endl;
            }
        }
        void print_adjacent(int id) {
            Node *node = (Node*) vertex_list.search(id);
            List *adjacent_list = (List*) node->get_content();

            adjacent_list->print();
        }

        int get_n_vertex(void) {
            return n_vertex;
        }

        Node* BFS(int begin_id, int id, int *path) {
            List queue, visited, distance;

            for(int i = 0; i < n_vertex; i++) { // Fill the distance list with INFINITE
                Node *dist = new Node();
                dist->set_id(i + 1);
                dist->add_content((int*) INFINITE);
                distance.add(dist);
            }

            Node *node = vertex_list.search(begin_id - 1);  // Get the origin vertex
            List *adjacent = (List*) node->get_content();

            Node *vertex = new Node(adjacent->get_first()); // Create a copy of it (damn pointers)
            Node *vertex2 = new Node(adjacent->get_first());

            distance.insert_content(begin_id, 0);   // Add the distance 0 to the origin
            visited.add(vertex2);   // Add the vertex to the visited list
            queue.add(vertex);      // Queue the vertex

            while(!queue.is_empty()) {
                Node *t = new Node(queue.get_first());
                queue.remove_first();   // Remove the first vertex from the queue

                if(t->get_id() == id) { // t is the vertex that we are looking for (but we still cant find the droids)
                    int max = 0, best_id = 0;

                    for(int i = 0; i < n_vertex; i++) { // Search for the greatest distance
                        int dist = (int) distance.get_content(i + 1);

                        if(dist > max && dist < INFINITE) {
                            max = dist;
                            best_id = i + 1;
                        }
                    }
                    Node *link = (Node*) vertex_list.search(begin_id-1); // Find the origin vertex
                    List *adj = (List*) link->get_content();
                    Node *vertex = adj->get_first();

                    if(max > (int) vertex->get_content()) // Just add the istance if it is greater
                        vertex->add_content((int*) max);
                    return t;
                }

                Node *node = new Node(vertex_list.search(t->get_id()-1));
                List *adjacent = (List*) node->get_content();

                for(int j = 1; j < adjacent->get_n_elements(); j++) { // Get all adjacent vertex
                    Node *v = new Node(adjacent->search(j));
                    Node *v2 = new Node(adjacent->search(j));
                    Node *aux = visited.unordered_search(v->get_id());

                    if(INFINITE == (int) distance.get_content(v->get_id())) {   // If the distance < INFINITE then add the new distance in the list
                        int dist = (int) distance.get_content(t->get_id());
                        distance.insert_content(v->get_id(), (int*) (dist + 1));
                    }
                    if(aux == NULL) { // If is not visited
                        path[v->get_id()-1] = t->get_id();
                        visited.add(v); // Mark as visited
                        queue.add(v2);  // Queue this vertex
                    }
                }
            }
            return NULL;    // If not found
        }

        int best_placement(void) {
            int min = INFINITE, id = 0;

            for(int i = 0; i < n_vertex + 1; i++)
                for(int j = 0; j < n_vertex + 1; j++)
                    BFS(j, i, new int[n_vertex]);  // Do a BFS to all pair of vertex in this graph

            for(int i = 1; i < n_vertex; i++) {  // Search for the vertex with lowest eccentricity value
                Node *link = (Node*) vertex_list.search(i);
                List *adj = (List*) link->get_content();
                Node *head = adj->get_first();
                int dist = (int) head->get_content();

                if(min > dist) {
                    min = dist; // Get the distance
                    id = i+1;   // Get the id
                }
            }
            return id;  // Return the id of the vertex with the lowest eccentricity value
        }

        void read_shortest_path(int *output, int *path, int begin, int end) {
            int current = end - 1, i;

            for(i = 0; path[current] != -1; i++) {
                output[i] = current + 1;
                current = path[current] - 1;
            }
            output[i] = begin;
        }

        void clear(int *p, int size) {
            for (int i = 0; i < size; i++)
                p[i] = -1;
        }
        void worst_edges(int n_edges) {
            int path[n_vertex], out[n_vertex];

            clear(path, n_vertex);
            clear(out, n_vertex);

            for(int i = 1; i < n_vertex + 1; i++) {
                for(int j = 1; j < n_vertex + 1 && j != i; j++) {
                    BFS(j, i, path);  // Do a BFS to all pair of vertex in this graph

                    cout << "Searching: " << j << " " << i << endl;
                    read_shortest_path(out, path, j, i);

                    for (int k = 0; k < n_vertex; k++)
                        cout << path[k] << " ";
                    cout << " // ";
                    for (int k = 0; k < n_vertex; k++)
                        cout << out[k] << " ";
                    cout << endl;
                    for(int k = 0; k + 1 < n_edges && out[k + 1] != -1; k++){
                        for(int l = 0; l < n_edges+1; l++) {
                            Node *aux = edge_list.search(l);
                            Edge *e = (Edge*) aux->get_content();

                                cout << e->v1 << " " << e->v2 << endl;
                            if((e->v1 == out[k] && e->v2 == out[k+1]) || (e->v2 == out[k] && e->v1 == out[k+1])) {
                                e->value ++;
                                break;
                            }
                        }
                    }
                    clear(path, n_vertex);
                    clear(out, n_vertex);
                }
            }
        }
};

int main(void) {

    int n_vertex = 0, n_edges = 0;
    int vertex_id1 = 0, vertex_id2 = 0;

    ifstream input_file;
    string file_path;

    Graph g;

    /* inputs */
    cin >> file_path;
    input_file.open(file_path.c_str());

    input_file >> n_vertex;
    input_file >> n_edges;


    for(TYPE i = 1; i <= n_vertex; i++) { // Create a vertex list
        Node *node = new Node;
        g.add_vertex(node);
    }

    while(true) {  // Connect all pair of vertex from a file
        input_file >> vertex_id1 >> vertex_id2;

        if(input_file.eof())
            break;

        g.connect_vertex(vertex_id1 - 1, vertex_id2 - 1);
    }
    input_file.close();

    //cout << g.best_placement() << endl;  // Search for the vertex with the lowest eccentricity value

    /* I know that I need to delete all allocated memory but because of lack of time
     * it was not possible. Especially because the code becomes a confuse mess of pointers
     * and the deadline was too short to create a proper code.
     * */

    g.print_edges();
    g.worst_edges(n_edges);
    cout << endl;
    g.print_edges();
}
