#ifndef FUNNUMERICAS_H
#define FUNNUMERICAS_H

#include "FunMatematica.h"
#include "Enjambre.h"


// Funciones para testear optimziacion
class Ackley : public Funcion
{
public:
	Ackley( int N, bool inv = false, double a = 20, double b = 0.2,
			  double c = 2*PI );
	double evaluar(const Vector *x);

private:
	double aa, bb, cc;
};

class Easom : public Funcion
{
public:
	Easom( int N, bool inv = false );
	double evaluar(const Vector *x);
};

class Rosenbrock : public Funcion
{
public:
	Rosenbrock( int N, bool inv = false, double a = 100 );
	double evaluar(const Vector *x);

private:
	double aa;
};


// Funcion para testear parametros
class BuscaMaximos : public Funcion
{
public:
	BuscaMaximos( int N, bool inv = false, int nroPicos = 5, int lim = 50 );
	double evaluar(const Vector *x);
	Vector getGlobal();

private:
	int p, l;
	QList<Vector> optimos;
};



class Enjambre;

// Funcion para ajustar parametros para optimizar funciones definidas
// simbolicamente
class Optimizador : public Funcion
{
public:
	Optimizador( QString expr, Vector *_optVerdadero );
    double evaluar(const Vector *x);

	inline double getError() const;
	inline double getDispersion() const;
	inline double getFOptimo() const;
	inline Vector getPosOptima() const;

private:
	Enjambre *enjambre;
	Vector *optEncontrado, *optVerdadero;
};

inline double Optimizador::getError() const
{
	return enjambre->getError();
}

inline double Optimizador::getDispersion() const
{
	return enjambre->calcularDispersion();
}

inline double Optimizador::getFOptimo() const
{
	return enjambre->getFOptimo();
}

inline Vector Optimizador::getPosOptima() const
{
	return enjambre->getOptimo();
}


#endif // FUNNUMERICAS_H
