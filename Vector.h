#ifndef VECTOR_H
#define VECTOR_H

#include <QList>
#include <QtCore/qmath.h>
#include <cstdlib>
#include <iostream>

using namespace std;

class Vector
{
public:
	Vector( int dimension );

	// Getters & Setters
	void setDimension( int dimension );
	inline int getDimension() const;
	inline void setComponente( int i, double valor );
	inline double getComponente( int i ) const;

	double getModulo() const;

	// Operaciones sobre vectores
	Vector operator + ( Vector vector ) const;
	Vector operator - ( Vector vector ) const;
	Vector operator + ( double constante ) const;
	Vector operator - ( double constante ) const;
	Vector operator * ( double constante ) const;
	Vector operator / ( double constante ) const;
	double operator [] ( int i ) const;
	void operator = ( double valor );

private:
	int dim;
	QList<double> componentes;

	void sumar( Vector vector );
	void sumar( double constante );
	void restar( Vector vector );
	void restar( double constante );
	void multiplicar( double constante );
	void dividir( double constante );
};

inline int Vector::getDimension() const
{
	return dim;
}

inline void Vector::setComponente( int i, double valor )
{
	if( i <= dim ) componentes[i] = valor;
}

inline double Vector::getComponente(int i) const
{
	return i<= dim ? componentes.at(i) : 0;
}

#endif // VECTOR_H
