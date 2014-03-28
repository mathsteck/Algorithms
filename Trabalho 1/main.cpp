/*
  Authors:
  Matheus Steck Cardoso - mathsteck@gmail.com
  Rafael dos Santos França Rodrigues - ph.sfr@ig.com.br

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
#define TYPE intptr_t                                           // If you running on x64 this need to be intptr_t

using namespace std;

class Node {

    int id;
    void *content;                                              // This content can be anything (Pointer to an list or just an integer).
    Node *next, *previous;

    public:
        Node(void) {
            id = 0;
            next = NULL;
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

    Node *first, *last;

    public:
        List(void) {
            first = NULL;
            last = NULL;
        }

        void add(Node *new_node) {
            if(first == NULL) {
                first = new_node;                                               // Create a new head's list
                last = new_node;
            }
            else {
                new_node->add_previous(last);
                last->add_next(new_node);                                       // Add the new node in the end of the list
                last = new_node;
            }
        }

        void remove_first(void) {
            if (first != NULL) {
                Node *trash = first;
                first = first->get_next();
                delete(trash);
            }
        }

        Node* search(int id) {                                                  // The id (position in the list) of the Node
            int i;
            Node *navigation = first;

            for(i = 0; navigation->get_next()!= NULL && i < id; i++) {          // Navigate in the list
                navigation = navigation->get_next();
            }

            if(i < id)                                                          // If reaches the end of the list
                return NULL;

            return navigation;                                                  // Return a pointer to the searched Node

        }

        Node* get_head(void) {
            return first;
        }

        Node *get_tail(void) {
            return last;
        }

        void print() {
            Node *navigation = first;
            while(navigation != NULL) {
                cout << (TYPE) navigation->get_content() << " -> ";              // FIXME: Need to find a way to auto cast the content
                navigation = navigation->get_next();
            }
            cout << endl;
        }
};

class Graph {

    int number_of_vertex;
    List vertex_list;                                                           // Stores a list of pointers to all adjacent lists of this graph

    public:
        Graph(void) {
            number_of_vertex = 0;
        }
                                                                                // Resume: Vertex list -> Adjacent list -> head_of_adjacent
        void add_vertex(Node* head_of_adjacent) {
            Node *link_adjacent = new Node;                                     // Create a new link to store a pointer to an adjacent list
            List *adjacent_list = new List;                                     // Create an adjacent list

            adjacent_list->add(head_of_adjacent);                          // Add the head of the adjacent list
            link_adjacent->add_content((List*) adjacent_list);                  // Link the vertex list with the adjacent list
            vertex_list.add(link_adjacent);                                // Add the pointer to an adjacent list into the vertex list

            head_of_adjacent->set_id(number_of_vertex);
            number_of_vertex++;
        }

        void connect_vertex(int vertex_id1, int vertex_id2) {
            Node *link_adjacent1 = (Node*) vertex_list.search(vertex_id1);      // Get the link to the adjacent list of the first vertex
            Node *link_adjacent2 = (Node*) vertex_list.search(vertex_id2);      // Get the link to the adjacent list of the second vertex

            List *adjacent_list1 = (List*) link_adjacent1->get_content();       // Get the adjacent list of the first vertex
            List *adjacent_list2 = (List*) link_adjacent2->get_content();       // Get the adjacent list of the second

            Node *vertex_copy1 = new Node;
            Node *vertex_copy2 = new Node;

            *vertex_copy1 = *adjacent_list1->get_head();                        // Copy the content of the original first vertex
            *vertex_copy2 = *adjacent_list2->get_head();                        // Copy the content of the original second vertex
            vertex_copy1->add_next(NULL);
            vertex_copy2->add_next(NULL);


            if(adjacent_list1 != NULL && adjacent_list2 != NULL) {
                adjacent_list1->add((Node*) vertex_copy2);                 // Add the vertex2 into the adjacent list from vertex1
                adjacent_list2->add((Node*) vertex_copy1);                 // Add the vertex1 into the adjacent list from vertex2
            }
        }

        void print() {
            vertex_list.print();
        }

        void print_adjacent(int id) {
            Node *node = (Node*) vertex_list.search(id);
            List *adjacent_list = (List*) node->get_content();

            adjacent_list->print();
        }

        int get_n_vertex(void) {
            return number_of_vertex;
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
    cout << file_path << endl;
    input_file.open(file_path.c_str());

    input_file >> n_vertex;
    input_file >> n_edges;

    cout << n_vertex << endl << n_edges << endl;

    for(TYPE i = 1; i <= n_vertex; i++) {                   // Create a vertex list
        Node *node = new Node;
        node->add_content((TYPE*) i);
        g.add_vertex(node);
    }

    while(true) {                                           // Connect all the vertex from file
        input_file >> vertex_id1 >> vertex_id2;

        if(input_file.eof())
            break;

        cout << vertex_id1 << " " << vertex_id2 << endl;
        g.connect_vertex(vertex_id1 - 1, vertex_id2 - 1);
    }

    cout << "\nAdjacent List\n";
    for(int i = 1; i <= g.get_n_vertex(); i++)
        g.print_adjacent(i-1);

    input_file.close();
}
