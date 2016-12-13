#include "Funcion.h"

Funcion::Funcion(QString expr, bool inv)
{
	// Expresion que define la funcion
	expresion = expr;
	// Invertir resultados (para maximizar)
	invertir = inv;
}

double Funcion::evaluar(const Vector *x)
// Metodo a definir por cada subclase
{
    return 0;
}

// EOF
