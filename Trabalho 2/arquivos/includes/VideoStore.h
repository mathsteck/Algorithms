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
#define REMOVE_ARQ 5
#define EXIT 6

class VideoStore {

	private:
		Vector<Client *> *clientList;						// vetor com objetos de clientes
		Vector<PrimaryIndex *> *clientIndex;				// vetor contendo o indice de clientes
		Vector<SecondaryIndex *> *genreIndex;				// vetor contendo o indice de generos
		Vector<SecondaryIndex *> *occupationIndex;			// vetor contendo o indice de oficios

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
				printf("\t5. Remover arquivo\n");
				printf("\t6. Sair\n");

				printf("Digite: ");
				scanf("%d", &option);

				printf("\n\n");

				switch(option) {
					case READFILE: 
						insertClientTxt();
					break;
					case INSERT: 
						insertClientTerminal(); 
					break;			
					case REMOVE_ARQ:
						removerArq();
					break;
					//case: REMOVE: break;
					//case SEARCH: break;
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
			file->setCliente(clientList);
			file->setGenero(clientList);
			file->setClientesGenero(clientList);
			// nessa etapa, nao precisa mais do vetor de clientes pois temos todas as informacoes no dat
			for (int i = 0; i < clientList->size(); i++) {
				Client *client = clientList->get(i);
				delete client;
			}
			delete clientList;

			clientList = new Vector<Client *>();
			clientIndex = file->createClientIdx();
			delete file;
		}

		void insertClientTerminal() {
			Client *client = new Client();
			client->readTerminal();
			clientList->add(client);
			File *file = new File();
			file->setCliente(clientList);
			delete file;
		}

		void removeMenu() {

		}

		void searchMenu() {

		}


		void removerArq() {
    		
    		FILE *arq;
    		int resp;
    		char resp2;
    		char filename[100];

    		printf("\nEscolha o arquivo que deseja excluir: \n");
    		printf("\t1. cliente.dat\n");
    		printf("\t2. genero.dat\n");
			printf("\t3. clientes_generos.dat\n");
			printf("\t4. cliente.idx\n");
			printf("\t5. genero.idx\n");
			printf("\t6. oficio.idx\n");
			scanf("%d", &resp);

    		switch(resp){
				case 1:
					strcpy(filename, "cliente.dat");
				break;
				case 2:
					strcpy(filename, "genero.dat");
				break;
				case 3:
					strcpy(filename, "clientes_generos.dat");
				break;
				case 4:
					strcpy(filename, "cliente.idx");
				break;
				case 5:
					strcpy(filename, "genero.idx");
				break;
				case 6:
					strcpy(filename, "oficio.idx");
				break;
				default:
					printf("\nNumero nao existe!\n");
				break;
    		}
			

		    if(arq == NULL){
		        printf("Nao existe arquivo para remover!!\n");
		        exit(1);
		    }
		    else{
		        printf("\nDeseja realmente deletar o arquivo? (S/N)\n");
		        scanf("%s", &resp2);
		        if(resp2 == 'S' || resp2 == 's'){
		            remove(filename);
		        }
		        else if(resp2 == 'N' || resp2 == 'n'){
		            return;
		        }
		        else{
		            printf("\nResposta incorreta!\n");
		            return;
		        }
		    }
		}

};

#endif
