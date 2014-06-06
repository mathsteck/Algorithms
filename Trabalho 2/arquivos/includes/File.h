#ifndef _FILE_H_
#define _FILE_H_

#include "Vector.h"
#include "Sstring.h"
#include "PrimaryIndex.h"

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

		void setClient(Vector<Client *> *clientes){
			FILE *fp;

			fp = fopen("cliente.dat", "ab+");
			int correct[7];

			if(fp == NULL)
				fp = fopen("cliente.dat", "wb+");

			for(int i = 0; i < clientes->size(); i++) {
				Client *client;
				client = clientes->get(i);

				fwrite(client->getClientDat()->getString(), sizeof(char), REGISTER_SIZE, fp);
			}

			fclose(fp);
		}

		Client * getClientByRRN(int rrn) {
			FILE *fp = fopen("cliente.dat", "rb+");

			int size = getFileSize("cliente.dat");

			if (REGISTER_SIZE * rrn > size)
				return NULL;

			char reg[REGISTER_SIZE];

			fseek(fp, rrn*REGISTER_SIZE, SEEK_SET);
			fread(reg, sizeof(char), REGISTER_SIZE, fp);
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
			
			fp2 = fopen("clientes_generos.dat", "rb+");	
			if(fp2 == NULL)
        		fp2 = fopen("clientes_generos.dat", "wb+");

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

		Vector<PrimaryIndex *> *createClientIdx() {
			Vector<PrimaryIndex *> *clientIndex = new Vector<PrimaryIndex *>();

			int size = getFileSize("cliente.dat");
			int total = size/REGISTER_SIZE;

			for (int i = 0; i < total; i++) {
				Client *client = getClientByRRN(i);
				PrimaryIndex *pi = new PrimaryIndex(client->getCpf(), i);
				clientIndex->add(pi);
			}

			return clientIndex;
		}
};

#endif
