#ifndef _SSTRING_H_
#define _SSTRING_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Classe auxiliar para lidar com vetor de char

class String {

	private:
		char *string;

	public:
		String() {
			this->string = NULL;
		}

		String(char *string) {
			this->string = NULL;
			setString(string);
		}

		String(const char *string) {
			this->string = NULL;
			setString(string);
		}

		virtual ~String() {
			if (this->string != NULL)
				free(this->string);
		}

		void setString(char *string) {
			if (this->string != NULL)
				free(this->string);

			this->string = (char *) malloc(sizeof(char)*(strlen(string)+1));
			strcpy(this->string, string);
		}

		void setString(const char *string) {
			if (this->string != NULL)
				free(this->string);

			this->string = (char *) malloc(sizeof(char)*(strlen(string)+1));
			strcpy(this->string, string);
		}

		int size() { 
			return strlen(this->string); 
		}

		char * getString() {
			return this->string;
		}
};

#endif
