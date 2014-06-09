#ifndef _CLIENT_H
#define _CLIENT_H_

#include "Sstring.h"
#include "Vector.h"

#define REGISTER_SIZE 250 

class Client {

	private:
		String *cpf;
		String *name;
		String *address;
		String *age;
		String *gender;
		String *occupation;
		Vector<String *> *preferredGenresList;
		String *type;

	public:
		Client() {
			this->cpf = NULL;
			this->name = NULL;
			this->address = NULL;
			this->age = NULL;
			this->gender = NULL;
			this->occupation = NULL;
			this->preferredGenresList = new Vector<String *>();
			this->type = NULL;
		}

		virtual ~Client() {
			if (this->cpf != NULL) free(this->cpf);
			if (this->name != NULL) delete this->name;
			if (this->address != NULL) delete this->address;
			if (this->age != NULL) delete this->age;
			if (this->gender != NULL) delete this->gender;
			if (this->occupation != NULL) delete this->occupation;

			if (this->preferredGenresList != NULL) {
				for (int i = 0; i < preferredGenresList->size(); i++) {
					String *genre = preferredGenresList->get(i);
					delete genre;
				}

				delete this->preferredGenresList;
			}

			if (this->type != NULL) delete this->type;
		}

		// setters
		void setCpf(char *cpf) {
			if (this->cpf != NULL)
				delete this->cpf;

			this->cpf = new String(cpf);
		}

		void setName(char *name) {
			if (this->name != NULL)
				delete this->name;

			this->name = new String(name);
		}

		void setAddress(char *address) {
			if (this->address != NULL)
				delete this->address;

			this->address = new String(address);
		}

		void setAge(char *age) {
			if (this->age != NULL)
				delete this->age;

			this->age = new String(age);
		}

		void setGender(char *gender) {
			if (this->gender != NULL)
				delete this->gender;

			this->gender = new String(gender);
		}

		void setOccupation(char *occupation) {
			if (this->occupation != NULL)
				delete this->occupation;

			this->occupation = new String(occupation);
		}

		void setGenres(char *genres) {
			char *ptr;

			ptr = strtok(genres, "@");
			while (ptr != NULL) {
				String *str = new String(ptr);
				this->preferredGenresList->add(str);
				ptr = strtok(NULL, "@");
			}
		}

		void setType(char *type) {
			if (this->type != NULL)
				delete this->type;

			this->type = new String(type);
		}

		// getters
		String *getCpf() { return this->cpf; }
		String *getName() { return this->name; }
		String *getAddress() { return this->address; }
		String *getAge() { return this->age; }
		String *getGender() { return this->gender; }
		String *getOccupation() { return this->occupation; }
		Vector<String *> * getPreferredGenresList() { return this->preferredGenresList; }
		String *getType() { return this->type; }

		int getSize() {
			int size = 0;

			size += this->cpf->size();
			size += this->name->size();
			size += this->address->size();
			size += this->age->size();
			size += this->gender->size();
			size += this->occupation->size();
			size += this->type->size();
			size += 7;

			return size;
		}

		String * getClientDat() {
			char reg[REGISTER_SIZE];

			strcpy(reg, this->cpf->getString());
			strcat(reg, ",");
			strcat(reg, this->name->getString());
			strcat(reg, ",");
			strcat(reg, this->address->getString());
			strcat(reg, ",");
			strcat(reg, this->age->getString());
			strcat(reg, ",");
			strcat(reg, this->gender->getString());
			strcat(reg, ",");
			strcat(reg, this->occupation->getString());
			strcat(reg, ",");
			strcat(reg, this->type->getString());
			strcat(reg, ",");

			String *str = new String(reg);

			return str;
		}

		void readLine(char *str, int size) {
		    fgets (str, size, stdin);

		    if ((strlen(str) > 0) && (str[strlen (str) - 1] == '\n'))
		        str[strlen (str) - 1] = '\0';
		}

		void print() {
			printf("\nImprimindo Cliente:\n");
			printf("CPF: %s\n", getCpf()->getString());
			printf("Nome: %s\n", getName()->getString());
			printf("Endereco: %s\n", getAddress()->getString());
			printf("Idade: %s\n", getAge()->getString());
			printf("Genero: %s\n", getGender()->getString());
			printf("Oficio: %s\n", getOccupation()->getString());
			printf("Generos: ");
			for (int i = 0; i < getPreferredGenresList()->size(); i++) {
				String *str = getPreferredGenresList()->get(i);
				if (i+1 == getPreferredGenresList()->size())
					printf("%s\n", str->getString());
				else
					printf("%s, ", str->getString());
			}
			if (this->type != NULL) printf("Tipo: %s\n", getType()->getString());
		}

		// source = 1 se for terminal/txt ou 2 se for clientes.dat
		void set(char *reg, int source) {
			char *ptr, *aux;

			ptr = strtok(reg, ",");
			for (int i = 0; ptr != NULL; i++) {
				switch(i) {
					case 0:
						setCpf(ptr);
					break;
					case 1:
						setName(ptr);
					break;
					case 2:
						setAddress(ptr);
					break;
					case 3:
						setAge(ptr);
					break;
					case 4:
						setGender(ptr);
					break;
					case 5:
						setOccupation(ptr);
					break;
					case 6:
						if (source == 2)
							setType(ptr);
						aux = ptr;
					break;
					case 7:
						setType(ptr);
						setGenres(aux);
					break;
				}
				ptr = strtok(NULL, ",");
			}
		}

		// ler cliente do terminal
		void readTerminal() {
			char reg[500];

			printf("Digite novo cliente: ");
			readLine(reg, 500);
			readLine(reg, 500);
			set(reg, 1);
		}
};

#endif
