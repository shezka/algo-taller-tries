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
    DiccString(const DiccString<T>&);

    /**
    DESTRUCTOR
    **/
    ~DiccString();

    /**
    DEFINIR
    * Recibe una clave con su significado de tipo T y la define.
    * Si ya estaba definida, la reescribe.
    **/
    void Definir(const string& clave, const T& significado);

    /**
    DEFINIDO?
    * Devuelve un bool, que es true si la clave pasada está definida en
    * el diccionario.
    **/
    bool Definido(const string& clave) const;

    /**
    OBTENER
    * Dada una clave, devuelve su significado.
    * PRE: La clave está definida.
    --PRODUCE ALIASING--
    -- Versión modificable y no modificable
    **/
    const T& Obtener(const string& clave) const;
    T& Obtener(const string& clave);


    /**
    BORRAR
    * Dada una clave, la borra del diccionario junto a su significado.
    * PRE: La clave está definida.
    --PRODUCE ALIASING--
    **/
    void Borrar(const string& clave);

  private:

    struct Nodo {
      Nodo** siguientes;
      T* definicion;

      Nodo() {
        siguientes = new Nodo*[256];
        for (int i = 0; i < 256; i++) {
            siguientes[i] = nullptr;
        }
        definicion = nullptr;
      }

      ~Nodo() {
        delete definicion;
        delete [] siguientes;
      }
    };

    // Completar con funciones privadas

    Nodo* raiz;

    bool seBorra(const string& clave, int i, DiccString<T>::Nodo *miNodo);
};

template <typename T>
DiccString<T>::DiccString() : raiz(nullptr) {
 raiz = new Nodo();
}

template <typename T>
DiccString<T>::DiccString(const DiccString& d) {
 *this = d;
}

template <typename T>
DiccString<T>::~DiccString() {
// while (this->raiz->siguientes != nullptr) this->Borrar(this->raiz->);
}

template <typename T>
void DiccString<T>::Definir(const string& clave, const T& significado) {


    int size = clave.size();
    int i = 0;
    Nodo* actual = raiz;
    while (i < size)
    { //bajo por los caracteres hasta llevar a null
        if (actual->siguientes[int(clave[i])] == nullptr)
        {
            actual->siguientes[int(clave[i])] = new Nodo();
        }

        actual = actual->siguientes[int(clave[i])];
        i++;
    }
    actual->definicion = new T(significado);
 // La función int(char) de c++ devuelve el código ascii de un caracter
}

template <typename T>
bool DiccString<T>::Definido(const string& clave) const{
    int size = clave.size();
    int i = 0;
    Nodo* actual = raiz;
    while (i < size)
    { //bajo por los caracteres hasta llevar a null
        if (actual->siguientes[int(clave[i])] == nullptr)
        {
            return false;
        }

        actual = actual->siguientes[int(clave[i])];
        i++;
    }
    return true;
 // La función int(char) de c++ devuelve el código ascii de un caracter
}

template <typename T>
T& DiccString<T>::Obtener(const string& clave) {
    int size = clave.size();
    int i = 0;
    Nodo* actual = raiz;
    while (i < size)
    { //bajo por los caracteres hasta llevar a null
        actual = actual->siguientes[int(clave[i])];
        i++;
    }
    return *actual->definicion;
 // La función int(char) de c++ devuelve el código ascii de un caracter
}

template <typename T>
const T& DiccString<T>::Obtener(const string& clave) const {
    int size = clave.size();
    int i = 0;
    Nodo* actual = raiz;
    while (i < size)
    { //bajo por los caracteres hasta llevar a null
        actual = actual->siguientes[int(clave[i])];
        i++;
    }
    return *actual->definicion;
 // La función int(char) de c++ devuelve el código ascii de un caracter
}

template <typename T>
void DiccString<T>::Borrar(const string& clave) {


    Nodo* actual = raiz->siguientes[int(clave[0])];
    seBorra(clave, 0, actual);
}
template <typename T>
bool DiccString<T>::seBorra(const string& clave, int i, DiccString<T>::Nodo *miNodo)
{
    if ((miNodo != nullptr) &&
            (miNodo->definicion == nullptr) &&
            (this->seBorra(clave, ++i, miNodo->siguientes[int(clave[++i])])))
    {
        delete miNodo;
        return true;
    } else
    {
        return false;
    }

}
#endif
