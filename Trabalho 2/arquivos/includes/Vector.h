#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <stdlib.h>
#include <stdio.h>
#include <PrimaryIndex.h>

// Classe auxiliar Vector

template <class T>

class Vector {

    private:
        T* vector;
        int counter;

    public:
        Vector() {
            this->vector = NULL;
            this->counter = 0;
        }

        virtual ~Vector() {
            if (this->vector != NULL)
                free(this->vector);
        }

        // adiciona um novo elemento ao vetor
        void add(T elem) {
            this->vector = (T *) realloc(this->vector, sizeof(T) * (this->counter + 1));
            this->vector[this->counter++] = elem;
        }

        void swap(int i, int j) {
            T aux = this->vector[i];
            this->vector[i] = this->vector[j];
            this->vector[j] = aux;
        }

        // remove um elemento
        void remove(int pos) {
            if (pos >= 0 && pos < this->counter) {
                for (int i = pos; i < this->counter-1; i++)
                    this->vector[i] = this->vector[i+1];
                this->vector = (T *) realloc (this->vector, sizeof(T) * (--this->counter));
            }
        }

        // retorna um elemento
        T get(int pos) {
            if (pos >= 0 && pos <= this->counter)
                return this->vector[pos];

            return NULL;
        }

        // retorna tamanho do vetor
        int size() {
            return this->counter;
        }
};

#endif
