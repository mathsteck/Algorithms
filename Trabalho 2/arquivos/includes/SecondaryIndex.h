#ifndef _SECONDARY_IDX_H_
#define _SECONDARY_IDX_H_

#include "Sstring.h"
#include "Vector.h"
#include "Index.h"

class SecondaryIndex : public Index {
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

		virtual ~SecondaryIndex() {
			if (this->cpf != NULL)
				delete cpf;
			if (this->str != NULL)
				delete str;
		}

		String *getCpf() { return this->cpf; }
		String *getStr() { return this->str; }

		void setStr(String *str) {
			if (this->str != NULL)
				delete this->str;
			this->str = str;
		}

		void setStr(char *str) {
			if (this->str != NULL)
				delete this->str;
			this->str = new String(str);
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

		void set(Index *index) {
			SecondaryIndex *param = (SecondaryIndex *) index;
			this->setCpf(param->getCpf());
			this->setStr(param->getStr());
		}

		String *get() { 
			return this->str; 
		}
};

#endif
