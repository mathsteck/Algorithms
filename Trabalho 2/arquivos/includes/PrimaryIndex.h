#ifndef _PRIMARY_IDX_H_
#define _PRIMARY_IDX_H_

#include "Sstring.h"
#include "Vector.h"

class PrimaryIndex {
	private:
		String *cpf;
		int rrn;

	public:
		PrimaryIndex() {
			this->cpf = NULL;
			this->rrn = 0;
		}

		PrimaryIndex(String *cpf, int rrn) {
			this->cpf = cpf;
			this->rrn = rrn;
		}

		~PrimaryIndex() {
			if (this->cpf != NULL)
				delete cpf;
		}

		String *getCpf() { return this->cpf; }
		int getRRN() { return this->rrn; }

		void setRRN(int rrn) {
			this->rrn = rrn;
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
