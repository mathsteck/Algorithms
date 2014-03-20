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
#include <stdlib.h>

using namespace std;

#define TYPE int
#define MAX_CHILDREN 8

class Node {
    TYPE value;
    Node **children = NULL, *father = NULL;
    int max_children, number_of_children;

    public:

        Node(int, int);

        /* Setters */

        int set_value(TYPE new_value) {
            if(&new_value != NULL) {
                value = new_value;
                return true;
            }
            return false;
        }

        int set_node(Node *new_node) {
            if(new_node == NULL) {
                cout << "ERROR: Null node's adress\n";
                return false;
            }
            if(number_of_children + 1 < max_children) {
                new_node->father = this;
                children[number_of_children] = new_node;
                number_of_children++;
                return true;
            }
            cout << "Max children's limit reached\n";
            return false;
        }

        /* Getters */

        TYPE get_value(void) {
            return value;
        }

        TYPE get_node_value(int node_number) {
            if(node_number < max_children && node_number >= 0) {
                cout << node_number << " " << max_children << endl;
                return children[node_number]->get_value();
            }
            cout << "Error: Invalid adress\n";
            return -14000;
        }
};

Node::Node(int max_nodes, int new_value) {
    max_children = 0;
    number_of_children = 0;
    value = new_value;

    if(max_nodes > 0) {
        max_children = max_nodes;
        children = new Node * [max_nodes];

        if(children == NULL) {
            cout << "ERROR: Can't allocade node's children.\n";
            exit(EXIT_FAILURE);
        }
    }
    else {
        cout << "Error: invalid max_children value\n";
        exit(EXIT_FAILURE);
    }
}

int main(void) {
    Node node1(2, 666), node2(2, 333);

    node1.set_node(&node2);
    cout << node1.get_value() << "\n";
    cout << node1.get_node_value(0) << "\n";
}
