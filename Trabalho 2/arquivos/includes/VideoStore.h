#ifndef _VIDEO_STORE_H_
#define _VIDEO_STORE_H_

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "Vector.h"
#include "Sstring.h"
#include "Client.h"
#include "File.h"
#include "PrimaryIndex.h"
#include "SecondaryIndex.h"
#include "Index.h"

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
        Vector<Index *> *clientIndex;                        // vetor contendo o indice primario de clientes
        Vector<Index *> *genreIndex;                        // vetor contendo o indice secundario de generos
        Vector<Index *> *occupationIndex;                    // vetor contendo o indice secundario de oficios

    public:

        VideoStore() {
            // Inicializa as listas
            clientList = new Vector<Client *>();
            clientIndex = new Vector<Index *>();
            genreIndex = new Vector<Index *>();
            occupationIndex = new Vector<Index *>();
        }

        virtual ~VideoStore() {
            // Delete os elementos das listas
            for (int i = 0; i < clientList->size(); i++) {
                Client *client = clientList->get(i);
                delete client;
            }

            for (int i = 0; i < clientIndex->size(); i++) {
                Index *index = clientIndex->get(i);
                delete index;
            }

            for (int i = 0; i < genreIndex->size(); i++) {
                Index *index = genreIndex->get(i);
                delete index;
            }

            for (int i = 0; i < occupationIndex->size(); i++) {
                Index *index = occupationIndex->get(i);
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
            int option, showOptions;

            do {
                showOptions = checkDatFiles();
                printf("======= MENU =======\n");
                printf("\t1. Ler arquivo de clientes\n");
                printf("\t2. Inserir cliente pelo terminal\n");

                if(showOptions == 1) {
                    printf("\t3. Remover cliente\n");
                    printf("\t4. Buscar cliente\n");
                }
                printf("\t5. Sair\n");

                printf("Digite: ");
                scanf("%d", &option);

                printf("\n\n");

                if(option == READFILE) {
                    this->insertClientTxt();
                }
                if(option == INSERT) {
                    this->insertClientTerminal();
                }
                if(option == REMOVE && showOptions == 1) {
                    continue;
                }
                if(option == SEARCH && showOptions == 1) {
                    this->search();
                }

            } while (option != EXIT);
        }

    private:

        int checkDatFiles() {
            if(access("cliente.dat", F_OK) == -1)
                return 0;
            if(access("clientes_generos.dat", F_OK) == -1)
                return 0;
            if(access("genero.dat", F_OK) == -1)
                return 0;
            return 1;
        }

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

            // nessa etapa, nao precisa mais do vetor de clientes pois temos todas as informacoes nos dat
            for (int i = 0; i < clientList->size(); i++) {
                Client *client = clientList->get(i);
                delete client;
            }
            delete clientList;

            this->clientList = new Vector<Client *>();
            this->clientIndex = file->createClientIdx();
            this->genreIndex = file->createGenreIndex();
            this->occupationIndex = file->createOccupationIndex();
            this->heapsort(this->clientIndex, this->clientIndex->size());
            this->heapsort(this->genreIndex, this->genreIndex->size());
            this->heapsort(this->occupationIndex, this->occupationIndex->size());
            file->writePrimaryIndex(this->clientIndex, "cliente.idx");
            file->writeSecondaryIndex(this->occupationIndex, "oficio.idx");
            file->writeSecondaryIndex(this->genreIndex, "genero.idx");
            delete file;
            /* imprimir
            for (int i = 0; i < occupationIndex->size(); i++) {
                SecondaryIndex *pi = (SecondaryIndex *) occupationIndex->get(i);
                printf("%s %s\n", pi->getCpf()->getString(), pi->getStr()->getString());
            }*/
        }

        // TODO
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
                pi = (PrimaryIndex *) binary(clientIndex, 0, clientIndex->size(), cpf);
                if (pi != NULL) {
                    client = file->getClientByRRN(pi->getRRN());
                    printf("Nome: %s\n", client->getName()->getString());
                } else
                    printf("Nao encontrado...\n\n");
            } else {
                printf("Erro ao buscar!\n\n");
            }

            delete file;
        }

        // HEAPSORT

        void maxheapify(Vector<Index *> *vector, int position, int length) {
            int largest_id = position;
            int right_id = RIGHT_SON(position);
            int left_id = LEFT_SON(position);

            // descobrindo a posicao do maior elemento
            if (right_id < length && 
                strcmp(vector->get(right_id)->get()->getString(), vector->get(largest_id)->get()->getString()) > 0)
                largest_id = right_id;

            if (left_id < length &&    
                strcmp(vector->get(left_id)->get()->getString(), vector->get(largest_id)->get()->getString()) > 0)
                largest_id = left_id;

            // troca de posicao (position, largest_id)
            if (largest_id != position) {
                // realmente o maior naum eh o pai
                vector->swap(position, largest_id);
                maxheapify(vector, largest_id, length);
            }
        }

        void buildMaxheap(Vector<Index *> *vector, int length) {
            int elements = length / 2 - 1;

            while (elements >= 0) {
                maxheapify(vector, elements, length);
                elements--;
            }
        }

        void heapsort(Vector<Index *> *vector, int length) {
            int i;

            // construindo o max heap
            buildMaxheap(vector, length);

            for (i = 1; i < length; i++) {
                vector->swap(0, length-i);

                // garantindo que o max heap eh valido!
                maxheapify(vector, 0, length-i);
            }
        }

        // BINARY SEARCH RECURSIVO

        Index * binary(Vector<Index *> *vector, int start, int end, char *key) {
            int central = start + ((end - start + 1) / 2);

            int cmp = strcmp(key, vector->get(central)->get()->getString());

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
