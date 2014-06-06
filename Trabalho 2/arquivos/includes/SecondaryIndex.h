#ifndef _SECONDARY_IDX_H_
#define _SECONDARY_IDX_H_

#include "Sstring.h"
#include "Vector.h"

class SecondaryIndex {
	private:
		String *cpf;
		String *str;

	public:
		SecondaryIndex() {
			this->cpf = NULL;
			this->str = NULL;
		}

		SecondaryIndex(String *cpf, String *str) {
			this->cpf = cpf;
			this->str = str;
		}

		~SecondaryIndex() {
			if (this->cpf != NULL)
				delete cpf;
		}

		String *getCpf() { return this->cpf; }
		String *getStr() { return this->str; }

		void setStr(String *str) {
			if (this->cpf != NULL)
				delete this->cpf;
			this->cpf = cpf;
		}

		void setCpf(String *cpf) {
			if (this->cpf != NULL)
				delete this->cpf;
			this->cpf = cpf;
		}

		void setCpf(char *cpf) {
			if (this->cpf != NULL)
				delete this->cpf;
			this->cpf = new String(cpf);
		}

};

#endif
