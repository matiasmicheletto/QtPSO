#ifndef FUNCION_H
#define FUNCION_H

#include "QString"
#include "Vector.h"

class Funcion
{
public:
	Funcion(QString expr, bool inv = false);

	virtual double evaluar(const Vector *x);

	inline int getDimension() const;
	inline QString getExpresion() const;
	inline void setInv( bool inv );
	inline bool esInv() const;

protected:
    static const double PI = 3.14159265;
	// Cantidad de variables de la funcion
	int dim;
	// Expresion matematica que define la funcion
	QString expresion;
	// Variable para inertir el valor de la funcion (para maximizar)
	bool invertir;
};

inline int Funcion::getDimension() const
{
	// Si el numero de variables es nulo, devolver 1
	return dim == 0 ? 1 : dim;
}

inline QString Funcion::getExpresion() const
{
	return expresion;
}

inline void Funcion::setInv(bool inv)
// Activar inversion de valores (para hallar maximos)
{
	invertir = inv;
}

inline bool Funcion::esInv() const
{
	return invertir;
}

#endif // FUNCION_H
