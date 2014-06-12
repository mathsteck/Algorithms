#ifndef _INDEX_H_
#define _INDEX_H_

#include "Sstring.h"
#include "Vector.h"

// Classe abstrata indice

class Index {
    public:

        virtual ~Index() {}
        virtual String * get() = 0;
        virtual String * getCpf() {}
        virtual String * getStr() {}
        virtual void set(Index *index) = 0;
};

#endif
