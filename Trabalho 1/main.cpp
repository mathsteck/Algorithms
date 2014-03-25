/*
  Authors:
  Matheus Steck Cardoso - matheus.steck.cardoso@gmail.com
  Rafael    - @gmail.com

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
#define TYPE intptr_t                                           // If you running on x64 this need to be intptr_t

using namespace std;

class Node {

    int id;
    void *content;                                              // This content can be anything (Pointer to an list or just an integer).
    Node *next;                                                 // Points to the next node

    public:
        Node(void) {
            id = 0;
            next = NULL;
            content = NULL;
        }

        void add_next(Node *node) {
            next = node;
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
};

class List {

    Node *list_head;

    public:
        List(void) {
            list_head = NULL;
        }

        void add_node(Node *new_node) {
            if(list_head == NULL) {
                list_head = new_node;                                           // Create a new head's list
            }
            else {
                Node *navigation = list_head;

                while(navigation->get_next()!= NULL)                            // Find the end of the list. FIXME: Store the end of the list is faster
                    navigation = navigation->get_next();

                navigation->add_next(new_node);                                 // Add the new node in the end of the list
            }
        }

        Node* search(int id) {                                                  // The id (position in the list) of the Node
            int i;
            Node *navigation = list_head;

            for(i = 0; navigation->get_next()!= NULL && i < id; i++) {          // Navigate in the list
                navigation = navigation->get_next();
            }

            if(i < id)                                                          // If reaches the end of the list
                return NULL;

            return navigation;                                                  // Return a pointer to the searched Node

        }

        void print() {
            Node *navigation = list_head;
            while(navigation != NULL) {
                cout << navigation->get_id() << ") " << (TYPE) navigation->get_content() << " -> ";              // FIXME: Need to find a way to auto cast the content
                navigation = navigation->get_next();
            }
            cout << endl;
        }
};

class Graph {

    int number_of_vertex;
    List vertex_list;                                                       // Stores a list of pointers to all adjacent lists of this graph

    void connect_vertex(int vertex_id, Node *vertex) {
        Node *link_adjacent = (Node*) vertex_list.search(vertex_id);        // Get the link to the adjacent list
        Node *vertex_copy = new Node;

        *vertex_copy = *vertex;                                             // Copy the content of the original vertex
        vertex_copy->add_next(NULL);

        if(link_adjacent == NULL)   {
            cout << "Vertex " << vertex_id << " doesnt exists!" << endl;
        }
        else {
            List *adjacent_list = (List*) link_adjacent->get_content();     // Get the adjacent list

            if(adjacent_list != NULL)
                adjacent_list->add_node((Node*) vertex_copy);               // Add the vertex into the adjacent list
        }
    }

    public:
        Graph(void) {
            number_of_vertex = 0;
        }
                                                                                // Resume: Vertex list -> Adjacent list -> head_of_adjacent
        void add_vertex(Node* head_of_adjacent) {
            Node *link_adjacent = new Node;                                     // Create a new link to store a pointer to an adjacent list
            List *adjacent_list = new List;                                     // Create an adjacent list

            adjacent_list->add_node(head_of_adjacent);                          // Add the head of the adjacent list
            link_adjacent->add_content((List*) adjacent_list);                  // Link the vertex list with the adjacent list
            vertex_list.add_node(link_adjacent);                                // Add the pointer to an adjacent list into the vertex list

            head_of_adjacent->set_id(number_of_vertex);
            number_of_vertex++;
        }

        void connect_vertex(Node *vertex1, Node *vertex2) {
            connect_vertex(vertex2->get_id(), vertex1);
            connect_vertex(vertex1->get_id(), vertex2);
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

    Node node1, node2, node3, node4;
    Graph g;

    node1.add_content((TYPE*) 666);
    node2.add_content((TYPE*) 333);
    node3.add_content((TYPE*) 111);

    g.add_vertex(&node1);
    g.add_vertex(&node2);
    g.add_vertex(&node3);

    g.connect_vertex(&node1, &node3);
    g.connect_vertex(&node2, &node3);

    cout << "\nAdjacent List\n";
    for(int i = 0; i < g.get_n_vertex(); i++)
        g.print_adjacent(i);
}
