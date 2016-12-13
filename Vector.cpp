#include "Vector.h"

Vector::Vector( int dimension )
// Se definen las dimensiones del vector
{
	dim = dimension;
	for(int i = 0; i < dimension; i++)
		componentes << 0.0;
}

void Vector::setDimension(int dimension)
{
	for( int i = dim; i < dimension; i++ )
		componentes << 0.0;
	for( int i = dim; i > dimension; i--)
		componentes.removeLast();
	dim = dimension;
}

double Vector::getModulo() const
// Modulo o magnitud del vector
{
	double modulo = 0;
	for( int i = 0; i < dim; i++ )
		modulo += componentes[i]*componentes[i];
	return qSqrt(modulo);
}

void Vector::sumar( Vector vector )
// Sumar otro vector a este
{
	if( dim == vector.getDimension())
		for( int i = 0; i < dim; i++)
			componentes[i] += vector.getComponente(i);
}

void Vector::sumar( double constante )
// Sumar una constante a cada componente de este vector
{
	for( int i = 0; i < dim; i++)
		componentes[i] += constante;
}

void Vector::restar( Vector vector )
// Restar otro vector a este
{
	if( dim == vector.getDimension())
		for( int i = 0; i < dim; i++)
			componentes[i] -= vector.getComponente(i);
}

void Vector::restar( double constante )
// Restar una constante a cada componente de este vector
{
	for( int i = 0; i < dim; i++)
		componentes[i] -= constante;
}

void Vector::multiplicar( double constante )
// Multiplicar este vector por una constante
{
	for( int i = 0; i < dim; i++)
		componentes[i] *= constante;
}

void Vector::dividir( double constante )
// Dividir este vector entre una constante
{
	for( int i = 0; i < dim; i++)
		componentes[i] /= constante;
}

Vector Vector::operator + ( Vector vector ) const
// Operador suma vectorial
{
	Vector resultado(dim);
	resultado = *this;
	resultado.sumar( vector );
	return resultado;
}

Vector Vector::operator + ( double constante ) const
// Operador suma de valores constantes
{
	Vector resultado(dim);
	resultado = *this;
	resultado.sumar( constante );
	return resultado;
}

Vector Vector::operator - ( Vector vector ) const
// Operador resta vectorial
{
	Vector resultado(dim);
	resultado = *this;
	resultado.restar( vector );
	return resultado;
}

Vector Vector::operator - ( double constante ) const
// Operador resta constantes
{
	Vector resultado(dim);
	resultado = *this;
	resultado.restar( constante );
	return resultado;
}

Vector Vector::operator * ( double constante ) const
// Operador producto por una constante
{
	Vector resultado(dim);
	resultado = *this;
	resultado.multiplicar( constante );
	return resultado;
}

Vector Vector::operator / ( double constante ) const
// Operador cociente entre constante
{
	Vector resultado(dim);
	resultado = *this;
	resultado.dividir( constante );
	return resultado;
}

double Vector::operator []( int i ) const
// Acceder a componente (manipular vector como arreglo)
{
	return componentes.at(i);
}

void Vector::operator = ( double valor )
// Asignar mismo valor a todas las componentes
{
	for( int i = 0; i < dim; i++ )
		componentes[i] = valor;
}

// EOF
