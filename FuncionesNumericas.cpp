#include "FuncionesNumericas.h"

/// ACKLEY'S PATH

Ackley::Ackley( int N, bool inv, double a, double b, double c ) :
	Funcion("Ackley's Path",inv)
{
	// Orden de la funcion
	dim = N;

	// Parametros
	aa = a;
	bb = b;
	cc = c;
}

double Ackley::evaluar(const Vector *x)
/*        -           ____________  -          -  n            -
		 /           /----n          \        /  ----           \
-a · exp |-b · _    / \       xi^2    | - exp |  \    cos( c·xi )| + a + e
		 |      \  /  /      ------   |       |  /    ---------- |
		 \       \/   ----i=1  n     /        \  ----     n     /
		  -                         -          -  i=1          -
*/
{
	double s1 = 0.0, s2 = 0.0;
	for( int i = 0; i < dim; i++ )
	{
		s1 += (*x)[i] * (*x)[i];
		s2 += qCos( cc * (*x)[i] );
	}

	double r = aa*qExp( -bb*qSqrt( s1/dim ) ) + qExp( s2/dim ) - aa - qExp(1);

	// Cambiar signo ( inversa )
	return invertir ? r : -r;
}


/// EASOM'S FUNCTION ///

Easom::Easom( int N, bool inv ) : Funcion("Easom's function",inv)
{
	// Orden de la funcion
	dim = N;
}

double Easom::evaluar(const Vector *x)
/* Easom's ction (extendida para n variables)
			  n
			-----
			|   |
 (-1)^(n+1) |   | cos(Xi)
			 i=1
 -------------------------
	 -   n           -
	/   ----          \
	|	\              |
exp |	/    (Xi-Pi)^2 |
	\   ----          /
	 -   i=1         -
*/
{
	double p = 1.0, s = 0.0;
	for( int i = 0; i < dim; i++ )
	{
		p *= qCos((*x)[i]);
		s += qPow((*x)[i] - PI, 2);
	}
	double r = p / qExp(s);

	if( dim % 2 == 0 )
		r = -r;

	return invertir ? -r : r;
}


/// ROSENBROCK'S VALLEY ///

Rosenbrock::Rosenbrock( int N, bool inv, double a ) :
	Funcion("Rosenbrock's Valley", inv)
{
	// Orden de la funcion
	dim = N;
	aa = a;
}

double Rosenbrock::evaluar( const Vector *x )
/*
i=n-1
----
\
/    (xj - xi^2)^2 + (1-xi)^2
----
i = 1
j=i+1
*/
{
	double r = 0.0, xi;
	for( int i = 0; i < dim-1; i++ )
	{
		xi = (*x)[i];
		r += aa*qPow( (*x)[i+1] - xi*xi ,2) + qPow( 1-xi ,2);
	}

	return invertir ? -r : r;
}


/// BUSCA MAXIMOS ///
// Funcion que contiene p maximos distribuidos aleatoriamente en una region
// delimitada por -lim..lim en cada direccion. Contiene un unico optimo global
// que vale 2 y p-1 optimos locales que valen 1 cada uno (salvo que haya
// superposiciones de maximos, aunque poco probable)

BuscaMaximos::BuscaMaximos(int N, bool inv, int nroPicos, int lim) :
	Funcion( "Busca Maximos", inv)
{
	dim = N;
	p = nroPicos;
	l = lim;

	// Valores aleatorios entre -l y l
	for(int i = 0; i < p; i++)
	{
		Vector nuevo(dim);
		for(int j = 0; j < dim; j++)
			nuevo.setComponente(j,qrand()%(2*l)-l);
		optimos << nuevo;
	}
}

double BuscaMaximos::evaluar(const Vector *x)
/*            -                -
   p         /  n               \
  ----	  	/  ----              \
  \	        |  \                 |
  /     exp |  /    -(xj + rj)^2 |
  ----      \  ----              /
  i=1	     \ j=1              /
			  -                -
*/
{
	double r = 0.0, s, xj;
	for(int i = 0; i < p; i++)
	{
		s = 0.0;
		for( int j = 0; j < dim; j++ )
		{
			// Termino cuadratico
			xj = (*x)[j] - optimos.at(i)[j];
			s -= xj*xj;
		}
		r += qExp(s);
	}
	// El ultimo pico mide el doble de altura
	r += qExp(s);

	return invertir ? -r : r;
}

Vector BuscaMaximos::getGlobal()
// Retornar la posicion del optimo global
{
	return optimos.last();
}


/// OPTIMIZADOR ///
// Funcion para realizar metaoptimizaciones
Optimizador::Optimizador( QString expr, Vector *_optVerdadero )
	: Funcion(expr)
{
    // Cantidad de variables que toma la funcion como argumento
	dim = 3;

    expresion = expr + " M.O.";

	// Crear funcion
	FunMatematica *funcion = new FunMatematica(expr);
	int dims = funcion->getDimension();
	Vector *posInic = new Vector(dim);

	// Crear un enjambre
	enjambre = new Enjambre(funcion);
	enjambre->setPosicionInicial(posInic);

	// Inicializar vectores a emplear
    optEncontrado = new Vector(dims);
    optVerdadero = new Vector(dims);
    *optVerdadero = *_optVerdadero;
}

double Optimizador::evaluar(const Vector *x)
{
	enjambre->reiniciar();

	// Setear parametros del enjambre
	enjambre->setInercia(Enjambre::VPG,(*x)[0]);
	enjambre->setFactorP((*x)[1]);
	enjambre->setFactorG(Enjambre::VPG,(*x)[2]);

	// Optimizar (15 iteraciones)
	for(int i = 0; i < 15; i++)
		enjambre->iterar();

	// Obtener el optimo
	*optEncontrado = enjambre->getOptimo();

	// Error = ValorVerdadero - ValorCalculado
	// Como en este caso las magnitudes son vectoriales, devolver el modulo
	double resultado = (*optVerdadero - *optEncontrado).getModulo();

	return resultado;
}
