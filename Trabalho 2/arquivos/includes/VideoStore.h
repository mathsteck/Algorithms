#ifndef _VIDEO_STORE_H_
#define _VIDEO_STORE_H_

#include <stdlib.h>
#include <stdio.h>

#include "Vector.h"
#include "Sstring.h"
#include "Client.h"
#include "File.h"
#include "PrimaryIndex.h"
#include "SecondaryIndex.h"

#define READFILE 1
#define INSERT 2
#define REMOVE 3
#define SEARCH 4
#define EXIT 5

#define RIGHT_SON(position) (position+1)*2
#define LEFT_SON(position) (position+1)*2-1

class VideoStore {

    private:
        Vector<Client *> *clientList;                        // vetor com objetos de clientes
        Vector<PrimaryIndex *> *clientIndex;                // vetor contendo o indice de clientes
        Vector<SecondaryIndex *> *genreIndex;                // vetor contendo o indice de generos
        Vector<SecondaryIndex *> *occupationIndex;            // vetor contendo o indice de oficios

    public:

        VideoStore() {
            // Inicializa as listas
            clientList = new Vector<Client *>();
            clientIndex = new Vector<PrimaryIndex *>();
            genreIndex = new Vector<SecondaryIndex *>();
            occupationIndex = new Vector<SecondaryIndex *>();
        }

        virtual ~VideoStore() {
            // Delete os elementos das listas
            for (int i = 0; i < clientList->size(); i++) {
                Client *client = clientList->get(i);
                delete client;
            }

            for (int i = 0; i < clientIndex->size(); i++) {
                PrimaryIndex *index = clientIndex->get(i);
                delete index;
            }

            for (int i = 0; i < genreIndex->size(); i++) {
                SecondaryIndex *index = genreIndex->get(i);
                delete index;
            }

            for (int i = 0; i < occupationIndex->size(); i++) {
                SecondaryIndex *index = occupationIndex->get(i);
                delete index;
            }

            // Deleta as listas
            delete clientList;
            delete clientIndex;
            delete genreIndex;
            delete occupationIndex;
        }

        // Menu principal do programa
        void menu() {
            int option;

            do {
                printf("======= MENU =======\n");
                printf("\t1. Ler arquivo de clientes\n");
                printf("\t2. Inserir cliente pelo terminal\n");
                printf("\t3. Remover cliente\n");
                printf("\t4. Buscar cliente\n");
                printf("\t5. Sair\n");

                printf("Digite: ");
                scanf("%d", &option);

                printf("\n\n");

                switch(option) {
                    case READFILE:
                        this->insertClientTxt();
                    break;
                    case INSERT:
                        this->insertClientTerminal(); 
                    break;
                    //case: REMOVE: break;
                    case SEARCH:
                        this->search();
                    break;
                }

            } while (option != EXIT);
        }

    private:

        // Menus secundarios
        void insertClientTxt() {
            int size, last, i, j;
            char *temp, *aux, reg[500], filename[100];
            FILE *arq;

            printf("Digite o nome do arquivo: ");
            scanf("%s", filename);

            arq = fopen(filename, "rb+");

            if (arq != NULL) {
                fseek(arq, 0, SEEK_END);
                size = ftell(arq);
                fseek(arq, 0, SEEK_SET);

                temp = (char *) malloc(sizeof(char)*size);
                fread(temp, sizeof(char), size, arq);

                aux = temp;
                for (i = j = 0; i < size; i++, j++) {
                    reg[j] = temp[i];
                    if (temp[i] == '\n') {
                        reg[j] = '\0';
                        j = -1;
                        Client *client = new Client();
                        client->set(reg, 1);
                        clientList->add(client);
                    }
                }

                fclose(arq);
                free(temp);
            } else {
                printf("\nErro ao abrir arquivo\n\n");
            }

            File *file = new File();
            file->setClient(clientList);
            file->setGenre(clientList);
            file->setClientsGenre(clientList);
            // nessa etapa, nao precisa mais do vetor de clientes pois temos todas as informacoes no dat
            for (int i = 0; i < clientList->size(); i++) {
                Client *client = clientList->get(i);
                delete client;
            }
            delete clientList;

            clientList = new Vector<Client *>();
            // Gera o indice de clientes
            clientIndex = file->createClientIdx();
            this->heapsort(clientIndex, clientIndex->size());
            file->setClientIdx(clientIndex);

            // Gera o indice de ocupação
            occupationIndex = file->createOccupationIdx();
            this->heapsort(occupationIndex, occupationIndex->size());
            file->setOccupationIdx(occupationIndex);

            delete file;

            /* imprimir
            for (int i = 0; i < occupationIndex->size(); i++) {
                SecondaryIndex *pi = occupationIndex->get(i);
                printf("%s %s\n", pi->getCpf()->getString(), pi->getStr()->getString());
            }*/
        }

        // TODO: realizar as mesmas operações do insert TXT!!!
        void insertClientTerminal() {
            Client *client = new Client();
            client->readTerminal();
            clientList->add(client);
        }

        void remove() {
        }

        void search() {
            char cpf[30];
            File *file = new File();
            Client *client;
            PrimaryIndex *pi;

            printf("Digite cpf a ser buscado: ");
            scanf("%s", cpf);

            if (clientIndex->size() > 0 && strlen(cpf) == 11) {
                pi = binary(clientIndex, 0, clientIndex->size(), cpf);
                client = file->getClientByRRN(pi->getRRN(), (char*) "cliente.dat");
                if (client != NULL)
                    printf("Nome: %s\n", client->getName()->getString());
                else
                    printf("Nao encontrado...\n\n");
            } else {
                printf("Erro ao buscar!\n\n");
            }

            delete file;
        }

        /* HEAPSORT E BINARY SEARCH P/ A PRIMARY INDEX */

        void maxheapify(Vector<PrimaryIndex *> *vector, int position, int length) {
            int largest_id = position;
            int right_id = RIGHT_SON(position);
            int left_id = LEFT_SON(position);

            // descobrindo a posicao do maior elemento
            if (right_id < length && 
                strcmp(vector->get(right_id)->getCpf()->getString(), vector->get(largest_id)->getCpf()->getString()) > 0)
                largest_id = right_id;

            if (left_id < length &&    
                strcmp(vector->get(left_id)->getCpf()->getString(), vector->get(largest_id)->getCpf()->getString()) > 0)
                largest_id = left_id;

            // troca de posicao (position, largest_id)
            if (largest_id != position) {
                // realmente o maior naum eh o pai
                vector->swap(position, largest_id);
                maxheapify(vector, largest_id, length);
            }
        }

        void maxheapify(Vector<SecondaryIndex *> *vector, int position, int length) {
            int largest_id = position;
            int right_id = RIGHT_SON(position);
            int left_id = LEFT_SON(position);

            // descobrindo a posicao do maior elemento
            if (right_id < length && 
                strcmp(vector->get(right_id)->getStr()->getString(), vector->get(largest_id)->getStr()->getString()) > 0)
                largest_id = right_id;

            if (left_id < length &&    
                strcmp(vector->get(left_id)->getStr()->getString(), vector->get(largest_id)->getStr()->getString()) > 0)
                largest_id = left_id;

            // troca de posicao (position, largest_id)
            if (largest_id != position) {
                // realmente o maior naum eh o pai
                vector->swap(position, largest_id);
                maxheapify(vector, largest_id, length);
            }
        }

        void buildMaxheap(Vector<PrimaryIndex *> *vector, int length) {
            int elements = length / 2 - 1;

            while (elements >= 0) {
                maxheapify(vector, elements, length);
                elements--;
            }
        }

        void buildMaxheap(Vector<SecondaryIndex *> *vector, int length) {
            int elements = length / 2 - 1;

            while (elements >= 0) {
                maxheapify(vector, elements, length);
                elements--;
            }
        }


        void heapsort(Vector<PrimaryIndex *> *vector, int length) {
            int i;

            // construindo o max heap
            buildMaxheap(vector, length);

            for (i = 1; i < length; i++) {
                vector->swap(0, length-i);

                // garantindo que o max heap eh valido!
                maxheapify(vector, 0, length-i);
            }
        }

        void heapsort(Vector<SecondaryIndex *> *vector, int length) {
            int i;

            // construindo o max heap
            buildMaxheap(vector, length);

            for (i = 1; i < length; i++) {
                vector->swap(0, length-i);

                // garantindo que o max heap eh valido!
                maxheapify(vector, 0, length-i);
            }
        }

        PrimaryIndex * binary(Vector<PrimaryIndex *> *vector, int start, int end, char *key) {
            int central = start + ((end - start + 1) / 2);

            int cmp = strcmp(key, vector->get(central)->getCpf()->getString());

            if (cmp == 0) return vector->get(central);
            if (start >= end) return NULL;

            if (cmp < 0) {
                return binary(vector, start, central-1, key);
            } else if (cmp > 0) {
                return binary(vector, central+1, end, key);
            }
        }

        SecondaryIndex * binary(Vector<SecondaryIndex *> *vector, int start, int end, char *key) {
            int central = start + ((end - start + 1) / 2);

            int cmp = strcmp(key, vector->get(central)->getStr()->getString());

            if (cmp == 0) return vector->get(central);
            if (start >= end) return NULL;

            if (cmp < 0) {
                return binary(vector, start, central-1, key);
            } else if (cmp > 0) {
                return binary(vector, central+1, end, key);
            }
        }
};

#endif
