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
#define GENRE 5
#define OCCUPATION 6
#define EXIT 7

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
            int option, hideOptions;
            do {
                hideOptions = checkDatFiles();
                printf("======= MENU =======\n");
                printf("\t1. Ler arquivo de clientes\n");
                printf("\t2. Inserir cliente pelo terminal\n");

                if(!hideOptions) {
                    printf("\t3. Remover cliente\n");
                    printf("\t4. Buscar cliente\n");
                    printf("\t5. Genero\n");
                    printf("\t6. Profissao\n");
                }

                printf("\t7. Sair\n");

                printf("Digite: ");
                scanf("%d", &option);

                printf("\n\n");

                if(!hideOptions) {
                    switch(option) {
                        case READFILE:
                            this->insertClientTxt();
                        break;
                        case INSERT:
                            this->insertClientTerminal();
                        break;
                        case REMOVE:
                            this->remove();
                        break;
                        case SEARCH:
                            this->searchPrimaryIndex();
                        break;
                        case GENRE:
                            this->genreMenu();
                        break;
                        case OCCUPATION:
                            this->occupationMenu();
                        break;
                    }
                }
                else {
                    switch(option) {
                        case READFILE:
                            this->insertClientTxt();
                        break;
                        case INSERT:
                            this->insertClientTerminal();
                        break;
                    }
                }

            } while (option != EXIT);
        }

    private:
        int checkDatFiles() {
            // Se não existir o arquivo
            if(access("cliente.dat", F_OK) == -1)
                return 1;
            if(access("clientes_generos.dat", F_OK) == -1)
                return 1;
            if(access("genero.dat", F_OK) == -1)
                return 1;
            return 0;
        }

        // insere clientes a partir de um arquivo txt
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
            delete file;

            this->createIndexes();
        }

        void createIndexes() {
            File *file = new File();

            this->clientList = new Vector<Client *>();
            this->clientIndex = file->createClientIdx();
            this->genreIndex = file->createGenreIndex();
            this->occupationIndex = file->createOccupationIndex();
            this->heapsort(this->clientIndex, this->clientIndex->size());
            this->heapsort(this->genreIndex, this->genreIndex->size());
            this->heapsort(this->occupationIndex, this->occupationIndex->size());

            // escreve nos arquivos
            file->writePrimaryIndex(this->clientIndex, "cliente.idx");
            file->writeSecondaryIndex(this->occupationIndex, "oficio.idx");
            file->writeSecondaryIndex(this->genreIndex, "genero.idx");

            delete file;
        }

        void insertClientTerminal() {
            Client *client = new Client();
            client->readTerminal();
            clientList->add(client);

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
            delete file;

            this->createIndexes();
        }

        // TODO
        void remove() {
            char cpf[30];
            File *file = new File();
            Vector<Index *> *result;
            PrimaryIndex *pi;

            printf("Digite cpf a ser removido: ");
            scanf("%s", cpf);

            if (clientIndex->size() > 0 && strlen(cpf) == 11) {
                result = binary(clientIndex, 0, clientIndex->size(), cpf);
                if (result != NULL) {
                    for (int i = 0; i < result->size(); i++) {
                        pi = (PrimaryIndex *) result->get(i);
                        file->removeClient(pi->getCpf(), pi->getRRN());
                    }
                    delete result;
                } else
                    printf("Nao existe tal cpf...\n\n");
            } else {
                printf("Erro ao buscar!\n\n");
            }

            this->createIndexes();

            delete file;
        }

        void occupationMenu() {
            char occupation[30];
            char genre[30];
            int option;
            Vector<Index *> *result1, *result2, *result;

            printf("Digite a profissao: ");
            scanf("%s", occupation);

            result1 = searchSecondaryIndexOrderly(2, occupation);

            if (result1 != NULL) {
                printf("1. Retornar o top 10 generos mais populares para essa profissao\n");
                printf("2. Dado genero, receber lista de CPFs de que gostam desse genero com essa profissao\nDigite: ");
                scanf("%d", &option);

                if (option == 1) {
                    //Lista de cpfs com uma ocupação específica
                    result2 = new Vector<Index *>();
                    for(int i = 0; i < result1->size(); i++) {
                        for(int j = 0; j < genreIndex->size(); j++) {
                            if(strcmp(genreIndex->get(j)->getCpf()->getString(), result1->get(i)->getCpf()->getString()) == 0)
                                result2->add(genreIndex->get(j));
                        }
                    }
                    this->heapsort(result2, result2->size());

                    Vector<PrimaryIndex *> *top_genre = findTopGenres(result2);

                    // imprime os top generos na tela
                    printf("\n");
                    for(int i = 0; i < top_genre->size() && i < 10; i++) {
                        printf("%d) %s\n", i+1, top_genre->get(i)->getCpf()->getString());
                    }
                    delete top_genre;
                }

                if (option == 2) {
                    printf("Digite o genero: ");
                    scanf("%s", genre);
                    result2 = searchSecondaryIndexOrderly(1, genre);    // busca lista de cpfs do genero

                    if (result2 != NULL) {
                        if (result1->size() > 0 && result2->size() > 0) {    // faz matching das listas
                            result = matchingCpf(result1, result2);
                            for(int i = 0; i < result->size(); i++)
                                printf("%s\n", result->get(i)->getCpf()->getString());
                            delete result;
                        }
                    }
                }
                delete result1;
                delete result2;
            }
        }

        Vector<PrimaryIndex *> * findTopGenres(Vector<Index*> *result) {
            Vector<PrimaryIndex *> *top_genre = new Vector<PrimaryIndex *> ();
            int id = 0;

            for(int i = 0; i < result->size(); i++) {
                //printf("%s %s\n", result2->get(i)->getCpf()->getString(), result2->get(i)->getStr()->getString());
                PrimaryIndex *pi = new PrimaryIndex();
                pi->setCpf(result->get(i)->getStr());

                if(top_genre->size() == 0) {
                    pi->setRRN(1);
                    top_genre->add(pi);
                }
                else if(strcmp(top_genre->get(id)->getCpf()->getString(), pi->getCpf()->getString()) == 0){
                    top_genre->get(id)->setRRN(top_genre->get(id)->getRRN() + 1);
                }
                else {
                    pi->setRRN(1);
                    top_genre->add(pi);
                    id++;
                }
            }
            this->bubblesort(top_genre, top_genre->size(), 1);

            return top_genre;
        }

        Vector <Index *> * matchingCpf(Vector <Index *> *result1, Vector <Index *> *result2) {
            Vector <Index *> *result = new Vector <Index *> ();
            for(int i = 0, j = 0; i < result1->size() && j < result2->size();) {
                int cmp = strcmp(result1->get(i)->getCpf()->getString(), result2->get(j)->getCpf()->getString());
                if (cmp < 0) {
                    i++;
                } else if (cmp > 0) {
                    j++;
                } else {
                    result->add(result1->get(i));
                    i++;
                    j++;
                }
            }

            return result;
        }

        void genreMenu() {
            int option, n = 0;
            char token[256];
            while(n <= 0) {
                printf("Digite o numero de generos que deseja pesquisar:\n");
                scanf("%d", &n);
            }
            // Lista de gêneros inseridos pelo usuário
            Vector <SecondaryIndex*> *genres = new Vector<SecondaryIndex *>();
            for(int i = 0; i < n; i++) {
                printf("Digite o genero:\n");
                scanf("%s", token);
                SecondaryIndex *si = new SecondaryIndex();
                si->setStr(token);
                genres->add(si);
            }

            printf("1. Retornar os 3 generos mais populares entre as pessoas que gostam dos generos deste conjunto.\n");
            printf("2. Os dados dos 10 clientes mais jovens que gostam destes gêneros.\nDigite:");
            scanf("%d", &option);

            this->bubblesort(genres, genres->size(), 0);

            // Filtra os CPFs que gostam de todos os generos listados
            Vector <Index*> *cpf_list;

            // Um vetor de vetores de index
            Vector <Vector <Index *>* > *results = new Vector<Vector<Index *>* >();
            // Para cada gênero inserido pelo usuário
            for(int i = 0; i < genres->size(); i++) {
                // Procura todos os CPFs que gostam deste genero
                cpf_list = searchSecondaryIndexOrderly(1, genres->get(i)->getStr()->getString());
                results->add(cpf_list);
            }

            // Matching
            Vector<Index *> *result;

            for(int i = 0; i + 1 < results->size(); i++) {
                Vector<Index *> *left, *right;

                if(i == 0)
                    left = results->get(i);
                else
                    left = result;
                right = results->get(i + 1);

                result = matchingCpf(left, right);
            }

            if(results->size() == 1) {
                result = results->get(0);
            }

            if(option == 1) {
                Vector<Index *> *genre_cpf_list = new Vector<Index *>();

                // Procura todos os gêneros a partir do cpf
                for(int j = 0; j < result->size(); j++) {
                    for(int k = 0; k < genreIndex->size(); k++) {
                        if(strcmp(genreIndex->get(k)->getCpf()->getString(), result->get(j)->getCpf()->getString()) == 0)
                            genre_cpf_list->add(genreIndex->get(k));
                    }
                }

                this->heapsort(genre_cpf_list, genre_cpf_list->size());
                //Procura os top gêneros dessa lista de cpf/genero
                Vector<PrimaryIndex *> *top_genres = findTopGenres(genre_cpf_list);

                int flag = 0;
                for(int k = 0, j = 0; k < top_genres->size() && j < 3; k++) {
                    for(int i = 0; i < genres->size(); i++) {
                        if(strcmp(top_genres->get(k)->getCpf()->getString(), genres->get(i)->getStr()->getString()) == 0)
                            flag = 1;
                    }

                    if(flag == 0) {
                        printf("%d) %s\n", j+1, top_genres->get(k)->getCpf()->getString());
                        j++;
                    }
                    flag = 0;
                }
            }

            if(option == 2) {
                Vector <Client *> *clients = new Vector<Client *>();
                File *file = new File();
                Vector<Index *> *search_result = NULL;

                for(int i = 0; i < result->size(); i++) {
                    search_result = binary(clientIndex, 0, clientIndex->size(), result->get(i)->getCpf()->getString());
                    if(search_result != NULL) {
                        for(int j = 0; j < search_result->size(); j++) {
                            PrimaryIndex *pi = (PrimaryIndex *) search_result->get(j);
                            Client *client = file->getClientByRRN(pi->getRRN());

                            if(client != NULL) {
                                clients->add(client);
                            }
                        }
                    }
                }

                this->bubblesort(clients, clients->size(), 1);
                for(int i = 0; i < clients->size() && i < 10; i++) {
                    clients->get(i)->print();
                }
            }
        }

        // se 1 busca no genreIndex se 2 busca na occupationIndex
        // retorna sempre um vetor de CPFs resultantes da busca ordenado com contadores zerados
        Vector<Index *> *searchSecondaryIndexOrderly(int type, char *aux) {
            File *file = new File();
            Client *client;
            Vector<Index *> *searchResult, *resultList, *secondary;
            SecondaryIndex *si;

            if (type == 1)
                secondary = genreIndex;
            else if (type == 2)
                secondary = occupationIndex;

            if (secondary->size() > 0 && strlen(aux) > 0) {
                searchResult = binary(secondary, 0, secondary->size(), aux);
                if (searchResult != NULL) {
                    resultList = new Vector<Index *>();
                    for (int i = 0; i < searchResult->size(); i++) {
                        PrimaryIndex *pi = new PrimaryIndex(searchResult->get(i)->getCpf(), 0);
                        resultList->add(pi);
                        //printf("%s %s\n", si->getCpf()->getString(), si->get()->getString());
                    }
                    delete searchResult;
                    this->heapsort(resultList, resultList->size());
                    return resultList;
                } else
                    return NULL;
            } else {
                return NULL;
            }

            delete file;
        }

        void searchPrimaryIndex() {
            char cpf[30];
            File *file = new File();
            Client *client;
            Vector<Index *> *result;
            PrimaryIndex *pi;

            printf("Digite cpf a ser buscado: ");
            scanf("%s", cpf);

            if (clientIndex->size() > 0 && strlen(cpf) == 11) {
                result = binary(clientIndex, 0, clientIndex->size(), cpf);
                if (result != NULL) {
                    for (int i = 0; i < result->size(); i++) {
                        pi = (PrimaryIndex *) result->get(i);
                        client = file->getClientByRRN(pi->getRRN());
                        client->print();
                    }
                    delete result;
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

        // BINARY SEARCH RECURSIVO - pode haver algum bug aqui...

        Vector<Index *> *binary(Vector<Index *> *vector, int start, int end, char *key) {
            int central = start + ((end - start + 1) / 2);

            int cmp = strcmp(key, vector->get(central - 1)->get()->getString());

            if (cmp == 0) {
                Vector<Index *> *result = new Vector<Index *>();
                while (central-1 > 0) {
                    if (strcmp(key, vector->get(central-1)->get()->getString()) == 0) {
                        central--;
                    } else {
                        break;
                    }
                }
                while (strcmp(key, vector->get(central)->get()->getString()) == 0) {
                    result->add(vector->get(central));
                    if (central + 1 != vector->size())
                        central++;
                    else
                        break;
                }
                return result;
            }

            if (start >= end) return NULL;

            if (cmp < 0) {
                return binary(vector, start, central-1, key);
            } else if (cmp > 0) {
                return binary(vector, central+1, end, key);
            }
        }


        // mode = 0 crescente mode = 1 decrescente
        void bubblesort(Vector<PrimaryIndex *> *vector, int size, int mode) {
            for(int i = size - 1; i >= 1; i--) {
                for(int j = 0; j < i ; j++) {
                    if(mode == 0) {
                        if(vector->get(j)->getRRN() > vector->get(j+1)->getRRN()) {
                            vector->swap(j, j+1);
                        }
                    }
                    if(mode == 1) {
                        if(vector->get(j)->getRRN() < vector->get(j+1)->getRRN()) {
                            vector->swap(j, j+1);
                        }
                    }
                }
            }
        }

        // mode = 0 crescente mode = 1 decrescente
        void bubblesort(Vector<SecondaryIndex *> *vector, int size, int mode) {
            for(int i = size - 1; i >= 1; i--) {
                for(int j = 0; j < i ; j++) {
                    if(mode == 0) {
                        if(strcmp(vector->get(j)->getStr()->getString(), vector->get(j+1)->getStr()->getString()) > 0) {
                            vector->swap(j, j+1);
                        }
                    }
                    if(mode == 1) {
                        if(strcmp(vector->get(j)->getStr()->getString(), vector->get(j+1)->getStr()->getString()) < 0) {
                            vector->swap(j, j+1);
                        }
                    }
                }
            }
        }

        // mode = 0 crescente mode = 1 decrescente
        void bubblesort(Vector<Client *> *vector, int size, int mode) {
            for(int i = size - 1; i >= 1; i--) {
                for(int j = 0; j < i ; j++) {
                    if(mode == 0) {
                        if(atoi(vector->get(j)->getAge()->getString()) < atoi(vector->get(j+1)->getAge()->getString())) {
                            vector->swap(j, j+1);
                        }
                    }
                    if(mode == 1) {
                        if(atoi(vector->get(j)->getAge()->getString()) > atoi(vector->get(j+1)->getAge()->getString())) {
                            vector->swap(j, j+1);
                        }
                    }
                }
            }
        }
};

#endif
