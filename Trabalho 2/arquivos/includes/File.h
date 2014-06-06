#ifndef _FILE_H_
#define _FILE_H_

#include "Vector.h"
#include "Sstring.h"
#include "PrimaryIndex.h"

// classe auxiliar para gerenciar os arquivos

class File {

	private:
		String *filename;

	public:
		File() {
			this->filename = NULL;
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

			return size;
		}

		int getFileSize(const char *filename) {
			FILE *fp = fopen(filename, "rb");

			if (fp == NULL) return 0;

			fseek(fp, 0, SEEK_END);
			int size = ftell(fp);
			fseek(fp, 0, SEEK_SET);

			fclose(fp);

			return size;
		}

		void setCliente(Vector<Client *> *clientes){
			FILE *arq;

			arq = fopen("cliente.dat", "ab+");
			int correct[7];

			if(arq == NULL){
        		printf("Nao existe arquivo!!\n");
        		exit(1);
    		}

			for(int i = 0; i < clientes->size(); i++) {
				Client *client;
				client = clientes->get(i);

				fwrite(client->getClientDat()->getString(), sizeof(char), REGISTER_SIZE, arq);
			}

			fclose(arq);
		}

		Client * getClientByRRN(int rrn) {
			FILE *arq = fopen("cliente.dat", "rb+");

			int size = getFileSize("cliente.dat");

			if (REGISTER_SIZE * rrn > size)
				return NULL;

			char reg[REGISTER_SIZE];

			fseek(arq, rrn*REGISTER_SIZE, SEEK_SET);
			fread(reg, sizeof(char), REGISTER_SIZE, arq);
			Client *client = new Client();
			client->set(reg, 2);
			fclose(arq);

			return client;
		}

		void setGenero(Vector<Client *> *clientes) {
			FILE *arq;
			
			arq = fopen("genero.dat", "rb+");
			int correct, result;
	
			if(arq == NULL) {
        		arq = fopen("genero.dat", "wb+");
    		}

			for(int i = 0; i < clientes->size(); i++) {
				Client *client = clientes->get(i);
				verificaGenero(client->getPreferredGenresList());
			}

			fclose(arq);
		}

		void verificaGenero(Vector<String *> *generos) {
			FILE *arq;
			int key, size, aux, cmp;
			char genero_arq[30];

			arq = fopen ("genero.dat", "rb+");

			if(arq == NULL){
        		printf("Nao existe arquivo!!\n");
        		exit(1);
    		}

			fseek(arq, 0, SEEK_END);
			size = getFileSize("genero.dat");

			if (size > 0) {
				fseek(arq, -sizeof(int)+sizeof(char)*30, SEEK_END);
				fread(&key, sizeof(int), 1, arq);
			} else {
				key = 0;
			}

			fseek(arq, 0, SEEK_SET);

			for(int i = 0; i < generos->size(); i++) {
				String *genero = generos->get(i);
				cmp = 1;
				if (size > 0) {
					fseek(arq, 0, SEEK_SET);
					while(!feof(arq)) {
						fread(&aux, sizeof(int), 1, arq);
						fread(&genero_arq, sizeof(char), 30, arq);
						if (!feof(arq)) {
							cmp = strcmp(genero->getString(), genero_arq);
							if (!cmp)
								break;
	            		}
					}
				}
				if (cmp) {
					fwrite(&key, sizeof(int), 1, arq);
					fwrite(genero->getString(), sizeof(char), 30, arq);
					//printf("%d %s\n", key, genero->getString());
					key++;
				}
			}
			fclose(arq);
		}

		void setClientesGenero(Vector<Client *> *clientes) {
			for(int i = 0; i < clientes->size(); i++) {
				Client *client = clientes->get(i);
				insereClienteGenero(client->getPreferredGenresList(), client->getCpf());
			}
		}

		void insereClienteGenero(Vector<String *> *generos, String *cpf) {
			FILE *arq, *arq2;
			int key, aux, cmp;
			char genero_arq[30];

			arq = fopen("genero.dat", "rb+");	
			if(arq == NULL)
        		arq = fopen("genero.dat", "wb+");
			
			arq2 = fopen("clientes_generos.dat", "rb+");	
			if(arq2 == NULL)
        		arq2 = fopen("clientes_generos.dat", "wb+");

			for(int i = 0; i < generos->size(); i++) {
				String *genero = generos->get(i);
				cmp = 1;
				fseek(arq, 0, SEEK_SET);
				while(!feof(arq)) {
					fread(&key, sizeof(int), 1, arq);
					fread(&genero_arq, sizeof(char), 30, arq);
					if (!feof(arq)) {
						cmp = strcmp(genero->getString(), genero_arq);
						if (!cmp) {
							//printf("%s %d\n", cpf->getString(), key);
							fwrite(cpf->getString(), sizeof(char), cpf->size(), arq2);
							fwrite(&key, sizeof(int), 1, arq2);
							break;
						}
            		}
				}
			}
			fclose(arq);
			fclose(arq2);
		}

		Vector<PrimaryIndex *> *createClientIdx() {
			Vector<PrimaryIndex *> *clientIndex = new Vector<PrimaryIndex *>();

			int size = getFileSize("cliente.dat");
			int total = size/REGISTER_SIZE;

			for (int i = 0; i < total; i++) {
				Client *client = getClientByRRN(i);
				//printf("cpf: %s rrn: %d\n", client->getCpf()->getString(), i);
				PrimaryIndex *pi = new PrimaryIndex(client->getCpf(), i);
				clientIndex->add(pi);
			}

			return clientIndex;
		}
};

#endif
