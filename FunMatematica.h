#ifndef FUNMATEMATICA_H
#define FUNMATEMATICA_H

#include <QStringList>

#include "Funcion.h"

#define NRO_FC 5 // Numero de funciones trascendentes

// Funciones reconocidas
const QString funciones[NRO_FC] = {"sin","cos","log","exp","abs"};

class FunMatematica : public Funcion
{
public:
	FunMatematica( QString expr, bool inv = false );

	double evaluar(const Vector *x);
	void resultadoConstructor();

private:
	// Lista de variables de la funcion y lista de la expresion en notacion
	// postfija
	QStringList variables, postfija;

	// Metodos de la clase
	QString operacion(QString arg1, QString arg2, QString op);
	QString operacion(QString arg, QString func);
	bool esOperador(QCharRef c);
};

#endif // FUNMATEMATICA_H
