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

    bool seBorra(const string &clave, int i, DiccString<T>::Nodo *miNodo);

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
    Nodo *actual = raiz->siguientes[(int)clave[i]];
    while (i < size - 1) { //bajo por los caracteres hasta llevar a null
        if (actual == nullptr) {
            return false;
        }
        i++;
        actual = actual->siguientes[(int)clave[i]];
    }
    if (actual && actual->definicion != nullptr) return true;
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

    //if (raiz != nullptr) seBorra(clave, 0, raiz->siguientes[(int)clave[0]]);

    int j = 1;
    Nodo* papi = raiz;
    Nodo* actual = raiz->siguientes[(int)clave[0]];
    while(seBorra(clave, j, actual) && j < clave.size())
    {
        if(j == clave.size() - 1)
        {
            T* def = actual->definicion;
            actual->definicion = nullptr;
            delete def;
            break;
        }
        papi = actual;
        actual = actual->siguientes[(int)clave[j]];
        j++; 
    }
       if (!seBorra(clave, j, actual))
       {
           delete actual;
           papi->siguientes[(int)clave[j-1]] = nullptr;
       }
        if (clave.size() == 1)
        {
            if (!seBorra(clave, 0, raiz->siguientes[(int)clave[0]])) {
                delete raiz->siguientes[(int) clave[0]];
                raiz->siguientes[(int) clave[0]] = nullptr;
            } else{
                delete raiz->siguientes[(int)clave[0]]->definicion;
                raiz->siguientes[(int)clave[0]]->definicion = nullptr;
            }
        }
}






template<typename T>
bool DiccString<T>::seBorra(const string &clave, int i, DiccString<T>::Nodo *miNodo) {
    int j = 0;
    while(miNodo && j < 256)
    {
        if (miNodo->siguientes[j] && (!miNodo->siguientes[j]->definicion || (*miNodo->siguientes[j]->definicion != Obtener(clave)))) return true;
        j++;
    }
    return false;

}

#endif
