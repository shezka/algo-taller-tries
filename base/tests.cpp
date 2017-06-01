#include <functional>
#include <future>
#include <gtest/gtest.h>
#include <iostream>

#include "DiccString.hpp"


/**
 * Esto es una clase sin operador asignacion. Sirve para testear
 * que estan utilizando el constructor por copia al asignar los elementos
 * de la coleccion.
 */
struct Punto {
    Punto(int x, int y) : x_(x), y_(y) {}
    Punto(const Punto& otro) : x_(otro.x_), y_(otro.y_) {}

    bool operator==(const Punto& otro) const {
        return (this->x_ == otro.x_) && (this->y_ == otro.y_);
    }

    int x_, y_;
    Punto& operator=(const Punto&) = delete;
};

// ========================================================================== //

struct DiccStringTest : public ::testing::Test {

    virtual void SetUp() {
        singleton.Definir("hola", 1);

        sin_prefijos.Definir("hola", 1);
        sin_prefijos.Definir("chau", 2);
        sin_prefijos.Definir("adios", 3);

        con_prefijos.Definir("c", 1);
        con_prefijos.Definir("casa", 2);
        con_prefijos.Definir("casona", 3);

        puntos.Definir("hola", Punto(2,3));
        puntos.Definir("casa", Punto(100,4));
        puntos.Definir("chau", Punto(57,3));
        puntos.Definir("chausa", Punto(570,30));

        dicc_dicc.Definir("vacio", vacio);
        dicc_dicc.Definir("singleton", singleton);
        dicc_dicc.Definir("sin_prefijos", sin_prefijos);
        dicc_dicc.Definir("con_prefijos", con_prefijos);
    }

    DiccString<int> vacio;
    DiccString<int> singleton;
    DiccString<int> sin_prefijos;
    DiccString<int> con_prefijos;
    DiccString<Punto> puntos;
    DiccString<DiccString<int>> dicc_dicc;
};


TEST_F(DiccStringTest, Definido) {
    EXPECT_FALSE(vacio.Definido("a"));
    EXPECT_FALSE(vacio.Definido("aaaa"));
    EXPECT_FALSE(vacio.Definido("adios"));

    EXPECT_TRUE(sin_prefijos.Definido("hola"));
    EXPECT_TRUE(sin_prefijos.Definido("chau"));
    EXPECT_TRUE(sin_prefijos.Definido("adios"));

    EXPECT_FALSE(sin_prefijos.Definido("h"));
    EXPECT_FALSE(sin_prefijos.Definido("ho"));
    EXPECT_FALSE(sin_prefijos.Definido("hol"));

    EXPECT_TRUE(con_prefijos.Definido("c"));
    EXPECT_TRUE(con_prefijos.Definido("casa"));
    EXPECT_TRUE(con_prefijos.Definido("casona"));
    EXPECT_FALSE(con_prefijos.Definido("ca"));
    EXPECT_FALSE(con_prefijos.Definido("cas"));
    EXPECT_FALSE(con_prefijos.Definido("caso"));
}

TEST_F(DiccStringTest, Obtener) {
    EXPECT_EQ(sin_prefijos.Obtener("hola"), 1);
    EXPECT_EQ(sin_prefijos.Obtener("chau"), 2);
    EXPECT_EQ(sin_prefijos.Obtener("adios"), 3);

    EXPECT_EQ(con_prefijos.Obtener("c"), 1);
    EXPECT_EQ(con_prefijos.Obtener("casa"), 2);
    EXPECT_EQ(con_prefijos.Obtener("casona"), 3);
}

TEST_F(DiccStringTest, Redefinir) {
    con_prefijos.Definir("c", 2);
    EXPECT_EQ(con_prefijos.Obtener("c"), 2);
    con_prefijos.Definir("c", 3);
    EXPECT_EQ(con_prefijos.Obtener("c"), 3);
}

TEST_F(DiccStringTest, DefinirSinAsignacion) {
    EXPECT_EQ(puntos.Obtener("hola"), Punto(2,3) );
    EXPECT_EQ(puntos.Obtener("casa"), Punto(100,4));
    EXPECT_EQ(puntos.Obtener("chau"), Punto(57,3) );
    EXPECT_EQ(puntos.Obtener("chausa"), Punto(570,30) );
}

TEST_F(DiccStringTest, Copiar) {
    DiccString<int> copia(sin_prefijos);

    EXPECT_TRUE(copia.Definido("hola"));
    EXPECT_TRUE(copia.Definido("chau"));
    EXPECT_TRUE(copia.Definido("adios"));

    EXPECT_FALSE(copia.Definido("h"));
    EXPECT_FALSE(copia.Definido("ho"));
    EXPECT_FALSE(copia.Definido("hol"));

    EXPECT_EQ(sin_prefijos.Obtener("hola"), copia.Obtener("hola"));
    EXPECT_EQ(sin_prefijos.Obtener("chau"), copia.Obtener("chau"));
    EXPECT_EQ(sin_prefijos.Obtener("adios"), copia.Obtener("adios"));

    DiccString<int> copia2(con_prefijos);

    EXPECT_TRUE(copia2.Definido("c"));
    EXPECT_TRUE(copia2.Definido("casa"));
    EXPECT_TRUE(copia2.Definido("casona"));
    EXPECT_FALSE(copia2.Definido("ca"));
    EXPECT_FALSE(copia2.Definido("cas"));
    EXPECT_FALSE(copia2.Definido("caso"));

    EXPECT_EQ(copia2.Obtener("c"), 1);
    EXPECT_EQ(copia2.Obtener("casa"), 2);
    EXPECT_EQ(copia2.Obtener("casona"), 3);
}

TEST_F(DiccStringTest, NoAliasing) {
    DiccString<int> copia(singleton);
    copia.Definir("hare", 2);
    ASSERT_FALSE(singleton.Definido("hare"));
}


TEST_F(DiccStringTest, TrieDeTries) {
    EXPECT_FALSE(dicc_dicc.Obtener("vacio").Definido("hola"));

    EXPECT_EQ(dicc_dicc.Obtener("singleton").Obtener("hola"), 1);

    EXPECT_EQ(dicc_dicc.Obtener("sin_prefijos").Obtener("hola"), 1);
    EXPECT_EQ(dicc_dicc.Obtener("sin_prefijos").Obtener("chau"), 2);
    EXPECT_EQ(dicc_dicc.Obtener("sin_prefijos").Obtener("adios"), 3);

    EXPECT_EQ(dicc_dicc.Obtener("con_prefijos").Obtener("c"), 1);
    EXPECT_EQ(dicc_dicc.Obtener("con_prefijos").Obtener("casa"), 2);
    EXPECT_EQ(dicc_dicc.Obtener("con_prefijos").Obtener("casona"), 3);

}

TEST_F(DiccStringTest, BorrarUnicaCLave) {
    EXPECT_TRUE(singleton.Definido("hola"));

    singleton.Borrar("hola");

    EXPECT_FALSE(singleton.Definido("hola"));
}


TEST_F(DiccStringTest, Borrar) {
    con_prefijos.Borrar("c");
    EXPECT_FALSE(con_prefijos.Definido("c"));

    EXPECT_TRUE(con_prefijos.Definido("casa"));
    EXPECT_TRUE(con_prefijos.Definido("casona"));

    con_prefijos.Borrar("casona");
    EXPECT_FALSE(con_prefijos.Definido("casona"));

    EXPECT_TRUE(con_prefijos.Definido("casa"));

    con_prefijos.Borrar("casa");
    EXPECT_FALSE(con_prefijos.Definido("casa"));

    sin_prefijos.Borrar("hola");
    EXPECT_FALSE(sin_prefijos.Definido("hola"));

    sin_prefijos.Borrar("chau");
    EXPECT_FALSE(sin_prefijos.Definido("chau"));

    sin_prefijos.Borrar("adios");
    EXPECT_FALSE(sin_prefijos.Definido("adios"));
}


int main(int argc, char* argv[]) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}