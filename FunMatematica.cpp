#include "FunMatematica.h"

FunMatematica::FunMatematica( QString expr, bool inv ) : Funcion(expr, inv)
{
	// Si la expresion es vacia, no hacer nada
	if( expresion == "" ) return;

	// Quitar espacios
	while( expresion.contains(" ") )
		expresion.remove(" ");

	// Crear lista de variables
	int indice = 1;		// Indice para numerar variables
	QString variable;	// Variables posiblemente presentes en la expresion
	forever
	{
		// Crear el string "Xi", con i = 1,2,3,...
		variable = "X" + variable.setNum(indice);
		// Si se encuentra en la expresion, agregar a la lista
		if( expresion.contains(variable) )
			variables << variable;
		// Si no, terminar
		else break;
		// Pasar a la siguiente variable
		indice++;
	}

	// Obtener la cantidad de variables
	dim = variables.size();

	// Para evitar el error
	expresion.prepend("(");
	expresion.append(")");

	// Recorrer el string separando valores y operadores en una lista
	QStringList lExpr;		// Lista para separar operadores de operandos
	QString temp = "";		// Variable string temporal
	int i = 0;

	while( i < expresion.size() )
	{
		// Si el caracter es un operador
		if( esOperador(expresion[i]) )
		{
			// Asignar caracter a un string
			temp = expresion[i];
			// Agregar string a la lista
			lExpr << temp;
			// Pasar al siguiente
			i++;
		}
		else
		{
			// Caso operandos
			while( !esOperador(expresion[i]) )
				// Notacion cientifica
				if( expresion[i] == 'e' )
				{
					temp += expresion[i];
					temp += expresion[i+1];
					i = i+2;
				}
				else
				{
					temp += expresion[i];
					i++;
					if( i > expresion.size() ) break;
				}
				lExpr << temp;
		}
		temp.clear();
	}

	// Convertir lista en notacion infija a postfija

	// Crear pila
	QStringList pila;

	i = 0;

	// Mientras no se llegue al final de la lista de entrada, hacer:
	while( i < lExpr.size() )
	{
		// Extraer el primer termino de la lista
		temp = lExpr.at(i);

		// Si es una funcion o parentesis izquierdo
		if( temp == "sin" || temp == "cos" || temp == "log"
				|| temp == "exp" || temp == "abs" || temp == "(")
			// Insertar en la pila
			pila.append(temp);

		// Si es un parentesis derecho
		else if( temp == ")" )
		{
			// Mientras la pila no este vacia, y en su cima no haya un
			// parentesis izquierdo, hacer:
			while( pila.last() != "(" )
			{
				// Extraer el elemento de la pila e insertarlo al final de
				// la lista de salida
				postfija << pila.last();
				pila.removeLast();
			}
			// Se llega al parentesis izquierdo, quitarlo de la pila
			pila.removeLast();

			// Si la pila no esta vacia aun
			if( !pila.empty() )
				if( pila.last() == "sin" || pila.last() == "cos"
						|| pila.last() == "log" || pila.last() == "exp"
						|| pila.last() == "abs")
				{
					// Agregar funcion a la lista de salida
					postfija << pila.last();
					pila.removeLast();
				}
		}
		// Si temp es un operador de nivel de precedencia bajo (suma-resta)
		else if( temp == "+" || temp == "-" )
		{
			// Mientras la pila no este vacia, y en su cima haya un operador
			// de mayor o igual nivel de precedencia hacer:
			while( !pila.empty() && (pila.last() == "+" || pila.last() == "-"
									 || pila.last() == "*"
									 || pila.last() == "/"
									 || pila.last() == "^" ))
			{
				// Extraer el elemento de la pila e insertarlo en la lista
				postfija << pila.last();
				pila.removeLast();
			}
			pila.append(temp);
		}
		// Si temp es un operador de nivel de precedencia medio
		// (producto - cociente)
		else if( temp == "*" ||	temp == "/" )
		{
			// Mientras la pila no este vacia, y en su cima haya un operador
			// de mayor o igual nivel de precedencia hacer:
			while( !pila.empty() && (pila.last() == "*" || pila.last() == "/"
									 || pila.last() == "^" ))
			{
				// Extraer el elemento de la pila e insertarlo en la lista
				postfija << pila.last();
				pila.removeLast();
			}
			pila.append(temp);
		}
		// Si temp es un operador de nivel de precedencia alto
		// (potencia - radicacion)
		else if( temp == "^")
		{
			// Mientras la pila no este vacia, y en su cima haya un operador
			// de mayor o igual nivel de precedencia hacer:
			while( !pila.empty() && pila.last() == "^" )
			{
				// Extraer el elemento de la pila e insertarlo en la lista
				postfija << pila.last();
				pila.last();
			}
			pila.append(temp);
		}
		// En cualquier otro caso, temp es un operando (numero)
		else
			// Insertar temp al final de la lista de salida
			postfija << temp;
		i++; // Pasar al siguiente caracter
	}

	// Mientras la pila no este vacia, hacer
	while( !pila.empty() )
	{
		// Extraer el elemento de la pila e insertarlo en la lista de salida
		postfija << pila.last();
		pila.removeLast();
	}
}

bool FunMatematica::esOperador(QCharRef c)
// Determina si un caracter es operador
{
	return ( c == '+' || c == '-' || c == '*' || c == '/' || c == '^' ||
			c == '(' || c == ')' );
}

void FunMatematica::resultadoConstructor()
{
	cout << "Nueva funcion construida:" << endl;
	cout << "Not. Infija: " << qPrintable(expresion) << endl;
	cout << "Not. Postfija: ";
	for( int i = 0; i < postfija.size(); i++ )
		cout << qPrintable(postfija.at(i)) << " ";
	cout << endl;
}

double FunMatematica::evaluar(const Vector *x)
{
	double resultado;

	// Crear pila y string temporal
	QStringList pila;
	QString temp1;

	int k;
	// Para cada elemento de la lista de la expresion postfija, hacer:
	for( int i = 0; i < postfija.size(); i++ )
	{
		// Tomar elemento
		temp1 = postfija.at(i);

		// Si es una de las variables
		k = variables.indexOf(temp1);
		if( k != -1 )
		{
			temp1.setNum((*x)[k]);
			pila.push_back(temp1);
		}
		// Si es funcion
		else if( temp1 == "cos" || temp1 == "sin" || temp1 == "log"
				|| temp1 == "exp" || temp1 == "abs" )
		{
			// Aplicar función correspondiente
			temp1 = operacion(pila.last(),temp1);
			// Quitar tope de la pila
			pila.pop_back();
			// Agregar resultado a la pila
			pila.push_back(temp1);
		}
		// Si es operador
		else if( temp1 == "+" || temp1 == "-" || temp1 == "*" ||
				temp1 == "/" || temp1 == "^" )
		{
			// Tomar el elemento de la cima de la pila
			QString temp2 = pila.last();
			// Quitar el ultimo
			pila.pop_back();
			// Aplicar operacion correspondiente
			temp1 = operacion(pila.last(),temp2,temp1);
			// Quitar nuevamente el ultimo
			pila.pop_back();
			// Agregar resultado a la pila
			pila.push_back(temp1);
		}
		// Si es un operando, agregar a la pila
		else
			pila.push_back(temp1);
	}
	// La cima de la pila contiene el valor de la funcion
	temp1 = pila.last();
	resultado = temp1.toDouble();

	// Caso devolver valores negativos
	return ( invertir && resultado != 0 ) ? -resultado : resultado;
}

QString FunMatematica::operacion(QString arg1, QString arg2, QString op)
// Efectuar una operacion algebraica entre dos valores
{
	// Convertir a valores numericos
	double a = arg1.toDouble();
	double b = arg2.toDouble();

	// Efectuar operacion correspondiente
	double r;
	// No se puede usar switch
	if( op == "+") r = a+b;
	if( op == "-") r = a-b;
	if( op == "*") r = a*b;
	if( op == "/") r = a/b;
	if( op == "^") r = qPow(a,b);

	// Convetir el resultado a string nuevamente y retornar
	QString valor;
	return valor.setNum(r);
}

QString FunMatematica::operacion(QString arg, QString func)
// Evaluar funcion trascendente
{
	// Convertir argumento a valor numerico
	double s = arg.toDouble();

	// Hallar funcion
	int indice;
	for( int i = 0; i < NRO_FC; i++ )
		if( func == funciones[i] )
		{
			indice = i;
			break;
		}

	// Aplicar funcion correspondiente
	double r;
	switch( indice )
	{
	case 0: r = qSin(s); break;
	case 1: r = qCos(s); break;
	case 2: r = qLn(s); break;
	case 3: r = qExp(s); break;
	case 4: r = qAbs(s); break;
	}

	// Convetir el resultado a string nuevamente y retornar
	QString valor;
	return valor.setNum(r);
}

// EOF
