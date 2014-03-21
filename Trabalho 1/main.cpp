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

using namespace std;

class Node {

    void *content;
    Node *next, *previous;

    public:
        Node(void) {
            next = NULL;
            previous = NULL;
            content = NULL;
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

        void* get_content(void) {
            return content;
        }

        Node* get_next() {
            return next;
        }
};

class List {

    Node *list_head, *list_end;

    public:
        List(void) {
            list_head = NULL;
            list_end = NULL;
        }

        void add_node(Node *new_node) {
            if(list_head == NULL) {
                list_head = new_node;
            }
            else {
                Node *navigation = list_head;

                while(navigation->get_next()!= NULL)
                    navigation = navigation->get_next();

                navigation->add_next(new_node);
                new_node->add_previous(navigation);
            }
            list_end = new_node;
        }

        Node* search(int id) {
            int i;
            Node *navigation = list_head;

            for(i = 0; navigation->get_next()!= NULL && i < id; i++) {
                navigation = navigation->get_next();
            }

            if(i < id)                                                  // If reach the end of the list before the id value
                return NULL;

            return navigation;

        }

        void print() {
            Node *navigation = list_head;
            while(navigation != NULL) {
                Node* node = (Node*) navigation->get_content();
                cout << (int) node->get_content()  << endl; // FIXME: Need to find a way to auto cast
                navigation = navigation->get_next();
            }
        }
};

class Graph {

    List adjacent_list; // FIXME mudar pra vertex_list

    public:
        void add_vertex(void* value) {
            Node *adjacent_node = new Node;                 // Create a node to store a vertex list
            Node *vertex_head = new Node;                   // Create a vertex
            List *vertex_list = new List;                   // Create a vertex list

            vertex_head->add_content(value);                // Add a value to the vertex
            vertex_list->add_node(vertex_head);             // Add this vertex to the vertex list
            adjacent_node->add_content((List*)vertex_list); // Add the vertex list to the adjacent node
            adjacent_list.add_node(adjacent_node);          // Add the adjacent node to the adjacent list (with the vertex list inside)
                                                            // Resume: Adjacent_list->adjacent_node(content)->vertex_list(content)->vertex_head
        }

        // FIXME eu não sei se o problema está na hora de conectar os vértices na lista
        // ou na hora de imprimir a lista de adjacência
        void connect_vertex(int vertex_id, Node *vertex) {
            Node *adjacent_node = (Node*) adjacent_list.search(vertex_id);
            if(adjacent_node == NULL)   {
                cout << "Vertex " << vertex_id << " doesnt exists!" << endl;
            }
            else {
                List *vertex_list = (List*) adjacent_node->get_content();

                if(vertex_list != NULL) {
                    vertex_list->add_node((Node*) vertex);
                }
            }
        }

        void print() {
            adjacent_list.print();
        }

        void print(int id) {
            Node *node = (Node*) adjacent_list.search(id);
            List *vertex_list = (List*) node->get_content();

            cout << (int) node->get_content() << " -> " << endl;

            vertex_list->print();
        }
};

int main(void) {

    Node node1, node2, node3;
    Graph g;

    node1.add_content((int*) 666);
    node2.add_content((int*) 333);
    node3.add_content((int*) 111);

    g.add_vertex(&node1);
    g.add_vertex(&node2);
    g.add_vertex(&node3);

    g.connect_vertex(0, &node2);
    g.connect_vertex(0, &node2);
    cout << "\nAdress\n";
    g.print();
    cout << "\nAdjacent\n";
    g.print(0);
}
