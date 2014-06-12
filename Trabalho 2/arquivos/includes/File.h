#ifndef _FILE_H_
#define _FILE_H_

#include "Vector.h"
#include "Sstring.h"
#include "PrimaryIndex.h"
#include "SecondaryIndex.h"

// classe auxiliar para gerenciar os arquivos

class File {

    public:
        File() {
        }

        virtual ~File() {

        }

        // metodo auxiliar
        int getFileSize(char *filename) {
            FILE *fp = fopen(filename, "rb");

            if (fp == NULL) return 0;

            fseek(fp, 0, SEEK_END);
            int size = ftell(fp);
            fseek(fp, 0, SEEK_SET);

            fclose(fp);

            if (strcmp(filename, "cliente.dat") == 0 || strcmp(filename, "clientes_generos.dat")) {
                size = size - sizeof(int);
            }

            return size;
        }

        // sobrecarga de metodo
        int getFileSize(const char *filename) {
            FILE *fp = fopen(filename, "rb");

            if (fp == NULL) return 0;

            fseek(fp, 0, SEEK_END);
            int size = ftell(fp);
            fseek(fp, 0, SEEK_SET);

            fclose(fp);

            return size;
        }

        void setClient(Vector<Client *> *clientList) {
            FILE *fp;
            int headPointer, i;

            fp = fopen("cliente.dat", "rb+");

            if (fp == NULL) {                                    // nao havia o arquivo criado ainda
                fp = fopen("cliente.dat", "ab+");

                headPointer = -1;                                // nao ha registros removidos
                fwrite(&headPointer, sizeof(int), 1, fp);        // escreve head pointer dos registros removidos

                for(i = 0; i < clientList->size(); i++) {
                    Client *client;
                    client = clientList->get(i);

                    fwrite(client->getClientDat()->getString(), sizeof(char), REGISTER_SIZE, fp);
                }
            } else {
                // escreve primeiro no lugar dos registros removidos
                fread(&headPointer, sizeof(int), 1, fp);             // pega endereco do primeiro registro removido
                for(i = 0; headPointer != -1 && i < clientList->size(); i++) {
                    Client *client;
                    client = clientList->get(i);
                    fseek(fp, REGISTER_SIZE*headPointer + sizeof(int), SEEK_SET);            // vai onde ta o registro
                    fread(&headPointer, sizeof(int), 1, fp);                                // pega endereco do proximo removido
                    fseek(fp, -sizeof(int), SEEK_CUR);                                        // volta
                    fwrite(client->getClientDat()->getString(), sizeof(char), REGISTER_SIZE, fp);    // sobreescreve
                }

                // se nao couber, completa no final
                if (i < clientList->size()) {
                    // atualiza a head pointer para dizer q n tem mais registros removidos
                    headPointer = -1;
                    fseek(fp, 0, SEEK_SET);
                    fwrite(&headPointer, sizeof(int), 1, fp);
                    fseek(fp, 0, SEEK_END);
                    for(; i < clientList->size(); i++) {
                        Client *client;
                        client = clientList->get(i);

                        fwrite(client->getClientDat()->getString(), sizeof(char), REGISTER_SIZE, fp);
                    }
                } else {
                    // caso contrario, atualiza a head pointer para o ultimo valor lido
                    fseek(fp, 0, SEEK_SET);
                    fwrite(&headPointer, sizeof(int), 1, fp);
                }
            }

            fclose(fp);
        }

        Client * getClientByRRN(int rrn) {
            FILE *fp = fopen("cliente.dat", "rb+");

            int size = getFileSize("cliente.dat");

            if (REGISTER_SIZE * rrn > size)
                return NULL;

            char reg[REGISTER_SIZE];

            fseek(fp, rrn*REGISTER_SIZE + sizeof(int), SEEK_SET);        // o + sizeof(int) eh para pular o
            fread(reg, sizeof(char), REGISTER_SIZE, fp);                // head pointer dos registros removidos
            Client *client = new Client();
            client->set(reg, 2);
            fclose(fp);

            return client;
        }

        void setGenre(Vector<Client *> *clientes) {
            FILE *fp;
            
            fp = fopen("genero.dat", "rb+");
            int correct, result;
    
            if(fp == NULL) {
                fp = fopen("genero.dat", "wb+");
            }

            for(int i = 0; i < clientes->size(); i++) {
                Client *client = clientes->get(i);
                genreVerify(client->getPreferredGenresList());
            }

            fclose(fp);
        }

        void genreVerify(Vector<String *> *generos) {
            FILE *fp;
            int key, size, aux, cmp;
            char genero_fp[30];

            fp = fopen ("genero.dat", "rb+");

            if(fp == NULL)
                fp = fopen ("genero.dat", "wb+");

            fseek(fp, 0, SEEK_END);
            size = getFileSize("genero.dat");

            if (size > 0) {
                fseek(fp, -sizeof(int)+sizeof(char)*30, SEEK_END);
                fread(&key, sizeof(int), 1, fp);
            } else {
                key = 0;
            }

            fseek(fp, 0, SEEK_SET);

            for(int i = 0; i < generos->size(); i++) {
                String *genero = generos->get(i);
                cmp = 1;
                if (size > 0) {
                    fseek(fp, 0, SEEK_SET);
                    while(!feof(fp)) {
                        fread(&aux, sizeof(int), 1, fp);
                        fread(&genero_fp, sizeof(char), 30, fp);
                        if (!feof(fp)) {
                            cmp = strcmp(genero->getString(), genero_fp);
                            if (!cmp)
                                break;
                        }
                    }
                }
                if (cmp) {
                    fwrite(&key, sizeof(int), 1, fp);
                    fwrite(genero->getString(), sizeof(char), 30, fp);
                    //printf("%d %s\n", key, genero->getString());
                    key++;
                }
            }
            fclose(fp);
        }

        void setClientsGenre(Vector<Client *> *clientes) {
            for(int i = 0; i < clientes->size(); i++) {
                Client *client = clientes->get(i);
                insertClientGenre(client->getPreferredGenresList(), client->getCpf());
            }
        }

        void insertClientGenre(Vector<String *> *generos, String *cpf) {
            FILE *fp, *fp2;
            int key, aux, cmp;
            char genero_fp[30];

            fp = fopen("genero.dat", "rb+");    
            if(fp == NULL)
                fp = fopen("genero.dat", "wb+");

            fp2 = fopen("clientes_generos.dat", "ab+");    

            for(int i = 0; i < generos->size(); i++) {
                String *genero = generos->get(i);
                cmp = 1;
                fseek(fp, 0, SEEK_SET);
                while(!feof(fp)) {
                    fread(&key, sizeof(int), 1, fp);
                    fread(&genero_fp, sizeof(char), 30, fp);
                    if (!feof(fp)) {
                        cmp = strcmp(genero->getString(), genero_fp);
                        if (!cmp) {
                            //printf("%s %d\n", cpf->getString(), key);
                            fwrite(cpf->getString(), sizeof(char), cpf->size(), fp2);
                            fwrite(&key, sizeof(int), 1, fp2);
                            break;
                        }
                    }
                }
            }
            fclose(fp);
            fclose(fp2);
        }

        Vector<Index *> *createClientIdx() {
            Vector<Index *> *clientIndex = new Vector<Index *>();

            int size = getFileSize("cliente.dat");
            int total = size/REGISTER_SIZE;

            for (int i = 0; i < total; i++) {
                Client *client = getClientByRRN(i);
                Index *pi = new PrimaryIndex(client->getCpf(), i);
                clientIndex->add(pi);
            }

            return clientIndex;
        }

        Vector<Index *> *createOccupationIndex() {
            Vector<Index *> *occupationIndex = new Vector<Index *>();

            int size = getFileSize("cliente.dat");
            int total = size/REGISTER_SIZE;

            for (int i = 0; i < total; i++) {
                Client *client = getClientByRRN(i);
                Index *pi = new SecondaryIndex(client->getCpf(), client->getOccupation());
                occupationIndex->add(pi);
            }

            return occupationIndex;
        }

        String *getGenreByKey(int key) {
            FILE *fp;
            int aux;
            char genero_fp[30];

            fp = fopen("genero.dat", "rb+");    
            if(fp == NULL)
                fp = fopen("genero.dat", "wb+");

            while(!feof(fp)) {
                fread(&aux, sizeof(int), 1, fp);
                fread(&genero_fp, sizeof(char), 30, fp);
                if (!feof(fp)) {
                    if (key == aux) {
                        String *genre = new String(genero_fp);
                        return genre;
                    }
                }
            }

            fclose(fp);
        }

        Vector<Index *> *createGenreIndex() {
            Vector<Index *> *clientIndex = new Vector<Index *>();
            FILE *fp;
            char aux[11];
            int key;

            int size = getFileSize("clientes_generos.dat");
            int total = size/15;

            fp = fopen("clientes_generos.dat", "rb+");

            for (int i = 0; i < total; i++) {
                fread(aux, sizeof(char), 11, fp);
                fread(&key, sizeof(int), 1, fp);
                aux[11] = '\0';
                String *cpf = new String(aux);
                Index *pi = new SecondaryIndex(cpf, getGenreByKey(key));
                clientIndex->add(pi);
            }

            fclose(fp);

            return clientIndex;
        }

        void writePrimaryIndex(Vector<Index *> *index, const char *indexName) {
            FILE *fp;
            int aux;

            fp = fopen(indexName, "rb+");
            if(fp == NULL)
                fp = fopen(indexName, "wb+");

            for (int i = 0; i < index->size(); i++) {
                PrimaryIndex *item = (PrimaryIndex *) index->get(i);
                fwrite(item->getCpf()->getString(), sizeof(char), 11, fp);
                aux = item->getRRN();
                fwrite(&aux, sizeof(int), 1, fp);
            }

            fclose(fp);
        }

        void writeSecondaryIndex(Vector<Index *> *index, const char *indexName) {
            FILE *fp;

            fp = fopen(indexName, "rb+");
            if(fp == NULL)
                fp = fopen(indexName, "wb+");

            for (int i = 0; i < index->size(); i++) {
                SecondaryIndex *item = (SecondaryIndex *) index->get(i);
                fwrite(item->getCpf()->getString(), sizeof(char), 11, fp);
                fwrite(item->getStr()->getString(), sizeof(char), 30, fp);
            }

            fclose(fp);
        }

        void removeClient(String *cpf, int rrn) {
            FILE *fp = fopen("cliente.dat", "rb+");
            int headPointer;

            fread(&headPointer, sizeof(int), 1, fp);                // le o headPointer
            fseek(fp, 0, SEEK_SET);                                    // volta
            fwrite(&rrn, sizeof(int), 1, fp);                        // escreve novo headPointer
            fseek(fp, rrn*REGISTER_SIZE + sizeof(int), SEEK_SET);    // vai onde estar o registro a ser removido
            fwrite(&headPointer, sizeof(int), 1, fp);                // escreve o que tinha antes no headPointer
            fclose(fp);
        }
};

#endif
