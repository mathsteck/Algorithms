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
        Node(void);

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

Node::Node(void) {
    next = NULL;
    previous = NULL;
    content = NULL;
}

class List {

    public:
        Node *list_head, *list_end;

        List(void);

        void add_node(Node *new_node) {
            if(list_head == NULL) {
                list_head = new_node;
            }
            else {
                Node *navigation = list_head;

                while(navigation->get_next()!= NULL) {
                    navigation = navigation->get_next();
                }
                //SEGMENTATION FAULT
                navigation->add_next(new_node);
                new_node->add_previous(navigation);
            }
            list_end = new_node;
        }

        void print() {
            Node *navigation = list_head;
            while(navigation != NULL) {
                cout << (int) navigation->get_content() << endl; // FIXME: Need to find a way to auto cast
                navigation = navigation->get_next();
            }
        }
};

List::List(void) {
    list_head = NULL;
    list_end = NULL;
}

int main(void) {

    Node node1, node2, node3;
    List list;

    node1.add_content((int*) 666);
    node2.add_content((int*) 333);
    node3.add_content((int*) 111);

    list.add_node(&node1);
    list.add_node(&node2);
    list.add_node(&node3);
    list.print();
}
