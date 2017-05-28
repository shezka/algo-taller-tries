#ifndef DICC_STRING_H_
#define DICC_STRING_H_

#include <string>
#include <ostream>
#include <iostream>
#include <cassert>

using std::string;


template<typename T>
class DiccString {
public:
    /**
    CONSTRUCTOR
    * Construye un diccionario vacio.
    **/
    DiccString();

    /**
    CONSTRUCTOR POR COPIA
    * Construye un diccionario por copia.
    **/
    DiccString(const DiccString<T> &);

    /**
    DESTRUCTOR
    **/
    ~DiccString();

    /**
    DEFINIR
    * Recibe una clave con su significado de tipo T y la define.
    * Si ya estaba definida, la reescribe.
    **/
    void Definir(const string &clave, const T &significado);

    /**
    DEFINIDO?
    * Devuelve un bool, que es true si la clave pasada está definida en
    * el diccionario.
    **/
    bool Definido(const string &clave) const;

    /**
    OBTENER
    * Dada una clave, devuelve su significado.
    * PRE: La clave está definida.
    --PRODUCE ALIASING--
    -- Versión modificable y no modificable
    **/
    const T &Obtener(const string &clave) const;

    T &Obtener(const string &clave);


    /**
    BORRAR
    * Dada una clave, la borra del diccionario junto a su significado.
    * PRE: La clave está definida.
    --PRODUCE ALIASING--
    **/
    void Borrar(const string &clave);

private:

    struct Nodo {
        Nodo **siguientes;
        T *definicion;

        Nodo() {
            siguientes = new Nodo *[256];
            for (int i = 0; i < 256; i++) {
                siguientes[i] = nullptr;
            }
            definicion = nullptr;
      //      std::cout << "crear" << std::endl;
        }
        Nodo(const Nodo &n) {

            definicion = n.definicion == nullptr ? nullptr : new T(*n.definicion);
            siguientes = new Nodo *[256];
            for (int i = 0; i < 256; ++i) {
                siguientes[i] = n.siguientes[i] == nullptr ? nullptr : new Nodo(*n.siguientes[i]);
            }
        }

        ~Nodo() {
            if (definicion) delete definicion;
         //   std::cout << "pobre nodito" << std::endl;
         //   std::cout << "pobre nodito" << std::endl;
            for (int i = 0; i < 256; ++i){
                delete siguientes[i];
            }
            delete [] siguientes;
        }
    };

    // Completar con funciones privadas

    Nodo *raiz;

    void BorrarTodos(Nodo **todos);

    Nodo* seBorra(const string &clave, int i, DiccString<T>::Nodo *miNodo);

    const int DiccString<T>::cantSiguientes(const typename DiccString<T>::Nodo &nodo) const;
};

template<typename T>
DiccString<T>::DiccString() : raiz(nullptr) {
   raiz = new Nodo();
}

template<typename T>
DiccString<T>::DiccString(const DiccString &d) {
    raiz = d.raiz != nullptr ? new Nodo(*d.raiz) : nullptr;
}

template<typename T>
DiccString<T>::~DiccString() {
    //BorrarTodos(raiz->siguientes);
    delete raiz;
}

template<class T>
void DiccString<T>::BorrarTodos(Nodo **todos) {

    for (int i = 0; i < 256; i++) {
        if (todos[i] != nullptr) BorrarTodos(todos[i]->siguientes);
        delete todos[i];
    }
}



template<typename T>
void DiccString<T>::Definir(const string &clave, const T &significado) {
    if (raiz == nullptr)
    {
        raiz = new Nodo();
    }

    int size = clave.length();
    int i = 0;
    Nodo *actual = raiz;
    while (i < size) { //bajo por los caracteres hasta llevar a null
        if (actual->siguientes[(int)clave[i]] == nullptr) {
            actual->siguientes[(int)clave[i]] = new Nodo();
          //  std::cout << "un nuevo miembro" << std::endl;
        }

        actual = actual->siguientes[(int)clave[i]];
        i++;
    }
    if (actual->definicion != nullptr) delete actual->definicion;
    actual->definicion = new T(significado);
   // std::cout << *actual->definicion << std::endl;
}

template<typename T>
bool DiccString<T>::Definido(const string &clave) const {
    //if (raiz == nullptr)
    //{
    //    raiz = new Nodo();
   // }
    int size = clave.length();
    int i = 0;
    Nodo *actuall = raiz;
    while (i < size) { //bajo por los caracteres hasta llevar a null
        if (actuall != nullptr || actuall->siguientes[(int)clave[i]] == nullptr) {
            return false;
        }

        actuall = actuall->siguientes[(int)clave[i]];
        i++;
    }
    if (actuall->definicion != nullptr) return true;
    return false;
    // La función int(char) de c++ devuelve el código ascii de un caracter
}

template<typename T>
T &DiccString<T>::Obtener(const string &clave) {
    int size = clave.size();
    int i = 0;
    Nodo *actual = raiz;
    while (i < size) { //bajo por los caracteres hasta llevar a null
        actual = actual->siguientes[int(clave[i])];
        i++;
    }
    return *actual->definicion;
    // La función int(char) de c++ devuelve el código ascii de un caracter
}

template<typename T>
const T &DiccString<T>::Obtener(const string &clave) const {
    int size = clave.size();
    int i = 0;
    Nodo *actual = raiz;
    while (i < size) { //bajo por los caracteres hasta llevar a null
        actual = actual->siguientes[int(clave[i])];
        i++;
    }
    return *actual->definicion;
    // La función int(char) de c++ devuelve el código ascii de un caracter
}

template<typename T>
void DiccString<T>::Borrar(const string &clave) {


//BORRAR CODIGO DE ABAJO!! es un lio :(
        if(raiz == nullptr)  return;

    //delword helper
    // Recorro toda la clave. Guardo actual en ultimo si tiene 2+ hijos o si tiene una definicion
    Nodo *actual = raiz, *ultimo = raiz;
int indiceBifurcacion = 0;
for(int i = 0; i < clave.length(); ++i) {
actual = actual->siguientes[(int)clave[i]];
if ( (cantSiguientes(*actual) > 1) || (actual->definicion != NULL) && (i != clave.length()-1)) {
ultimo = actual;
indiceBifurcacion = i;
}
}

// Elimino todos los nodos a partir de ultimo
if (cantSiguientes(*actual) == 0) {
// La clave a borrar no es un prefijo de otra clave
if (ultimo == raiz) {
if (raiz) {
// Era la unica clave entonces borro el arbol completo
// La raiz contiene una definicion. Dejo la raiz y borro lo demas
delete raiz->siguientes[(int)clave[0]];
raiz->siguientes[(int)clave[0]] = NULL;
}
    delete raiz;
} else {
// Borro a partir de la ultima bifurcacion
delete ultimo->siguientes[(int)clave[indiceBifurcacion+1]];
ultimo->siguientes[(int)clave[indiceBifurcacion+1]] = NULL;
}
} else {
// La clave a borrar es prefijo de otra clave. Solo borro la definicion
delete actual->definicion;
actual->definicion = nullptr;
}

}

template <typename T>
const int DiccString<T>::cantSiguientes(const typename DiccString<T>::Nodo &nodo) const {
    int cantSiguientes = 0;
    for(int i = 0; i < 256; ++i) {
        if (nodo.siguientes[i] != NULL) ++cantSiguientes;
    }
    return cantSiguientes;
}


}

template<typename T>
typename DiccString<T>::Nodo* DiccString<T>::seBorra(const string &clave, int i, DiccString<T>::Nodo *miNodo) {
//    if ((miNodo != nullptr) &&
//
//        (i == (clave.length() - 1) ||
//                ((miNodo->definicion == nullptr) &&
//        (this->seBorra(clave, i++, miNodo->siguientes[int(clave[i++])]))))) {
//        delete miNodo;
//        return true;
//    } else {
//        return false;
//    }
    if (miNodo != nullptr)
    {
        if (i == clave.length() || i == clave.length() - 1)
        {
           // delete miNodo;
            return miNodo;
        } else
        {
            if (miNodo->definicion == nullptr)
            {
                i++;
                if(seBorra(clave, i, miNodo->siguientes[(int)clave[i]]) ) //borro y dsp vuelvo a borrarrr

                {
                    //delete miNodo;
                    return miNodo;
                } else
                {
                    return seBorra(clave, i, miNodo->siguientes[(int)clave[i]]);
                }
            }
            else
            {
                return seBorra(clave, i, miNodo->siguientes[(int)clave[i]]);
            }
        }
    }
    return miNodo;
}

#endif
