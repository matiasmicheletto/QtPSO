#include "Enjambre.h"

Enjambre::Enjambre(Funcion *objetivo)
{
	// PARAMETROS INICIALES


	// Funcion objetivo
	funcion = objetivo;

	// Tipo de optimizacion
	max = false;

	// Dimension del espacio de busqueda
	dim = funcion->getDimension();


	// Punteros e indices para las listas de vectores
	nVPG = 0;
	nVG = 0;
	nG = 0;
	iG = 0;
	N = 0;


    // Limites del espacio
    limMin = new Vector(dim);
    *limMin = -__DBL_MAX__;
    limMax = new Vector(dim);
    *limMax = __DBL_MAX__;


	// Limitadores de velocidad y posicion
	espacioLimitado = false;
	velocidadLimitada = false;

	// Velocidad maxima
	velMax = __DBL_MAX__;

	// Factor de correccion G (peso optimo global)
	factorG[G] = 1.5;
	factorG[VG] = 1.5;
	factorG[VPG] = 1.5;

	// Factor de correccion P (peso optimo local)
	factorP = 1.5;

	// Inercia
	inercia[VG] = 0.5;
	inercia[VPG] = 0.5;

	// Dispersion inicial
	d0 = 5;
	// Posicion inicial {0,0,...,0} (origen)
	p0 = new Vector(dim);

	// Optimo global
	fg = __DBL_MAX__;
	g = new Vector(dim);

	// Error estimado relativo inicialmente 1
	ea = 1;

	// Numero de iteraciones
	iter = 0;


	// Agregar particulas
    // Inicialmente 100 particulas VPG
        for( int i = 0; i < 100; i++ )
    {
		agregarParticula(VPG);
		// Actualizar optimos
		actualizarOptimos(i);
	}
}

Enjambre::~Enjambre()
{
	// Eliminar punteros
}

void Enjambre::iterar()
{
	double valor = 0, temp = 0;

	int i, j;

	// Para cada particula VPG
	for(i = 0; i < nVPG; i++)
	{
		// Actualizar velocidad y posicion para cada componente
		for(j = 0; j < dim; j++)
		{
			// Formula del algoritmo PSO
			valor = v.at(i)[j] * inercia[VPG] +
					( p.at(i)[j] - x.at(i)[j] ) * factorP * ran() +
					((*g)[j] - x.at(i)[j]) * factorG[VPG] * ran();

			// Limites de velocidad
			if(velocidadLimitada)
				if( qAbs(valor) > velMax )
					valor = valor > 0 ? velMax : -velMax;

			// Setear valor
			v[i].setComponente(j, valor);

			// Actualizar posicion
			temp = x.at(i)[j] + v.at(i)[j];

			// Limites del espacio
			if( espacioLimitado )
			{
				if( temp > (*limMax)[j] )
					temp = (*limMax)[j];
				else if( temp < (*limMin)[j] )
						temp = (*limMin)[j];
			}

			// Setear valor
			x[i].setComponente( j, temp );
		}

		// Actualizar optimos
		actualizarOptimos(i);
	}
	// Para cada particula VG
	for(i = nVPG; i < iG; i++)
	{
		// Actualizar velocidad y posicion para cada componente
		for(j = 0; j < dim; j++ )
		{
			// Formula del algoritmo PSO
			valor = v.at(i)[j] * inercia[VG] +
					((*g)[j] - x.at(i)[j]) * factorG[VG] * ran();

			// Limites de velocidad
			if(velocidadLimitada)
				if( qAbs(valor) > velMax )
					valor = valor > 0 ? velMax : -velMax;

			// Setear valor
			v[i].setComponente(j, valor);

			// Actualizar posicion
			temp = x.at(i)[j] + v.at(i)[j];

			// Limites del espacio
			if( espacioLimitado )
			{
				if( temp > (*limMax)[j] )
					temp = (*limMax)[j];
				else if( temp < (*limMin)[j] )
						temp = (*limMin)[j];
			}

			// Actualizar posicion
			x[i].setComponente(j, temp );
		}

		// Actualizar optimos
		actualizarOptimos(i);
	}
	// Para cada particula G
	for(i = iG; i < N; i++)
	{
		// Actualizar posicion para cada componente
		for(j = 0; j < dim; j++ )
		{
			// "Velocidad"
			valor = ((*g)[j] - x.at(i)[j]) * factorG[G] * ran();

			// Limites de velocidad
			if(velocidadLimitada)
				if( qAbs(valor) > velMax )
					valor = valor > 0 ? velMax : -velMax;

			temp = x.at(i)[j] + valor;

			// Limites del espacio
			if( espacioLimitado )
			{
				if( temp > (*limMax)[j] )
					temp = (*limMax)[j];
				else if( temp < (*limMin)[j] )
						temp = (*limMin)[j];
			}

			// Actualizar posicion
			x[i].setComponente(j, temp);
		}

		// Actualizar optimos
		actualizarOptimos(i);
	}

	// Incrementar numero de iteraciones
	iter++;
}

void Enjambre::actualizarOptimos( int i )
// Actualizar los optimos para la particula i-esima
{
	// Valor de la funcion en la posicion de la particula
	double fx = funcion->evaluar(&x.at(i));

	// Si el valor de la funcion es menor que el optimo local -> actualizar
	if( i < nVPG ) // (Solo para particulas VPG)
		if( fx < fp.at(i) )
		{
			fp[i] = fx;
			p[i] = x.at(i);
		}

	// Si es menor que el optimo global -> actualizar g
	if( fx < fg )
	{
		// Error estimado
		if( fg != 0 )
			ea = qAbs( (fg - fx)/fg );

		fg = fx;
		*g = x.at(i);
	}
}

void Enjambre::reiniciar()
{
	// Borrar numero de iteraciones
	iter = 0;

	// Optimo global
	fg = __DBL_MAX__;

	// Borrar lista de optimos locales
	fp.clear();

	double temp, fx;
	// Para cada particula, asignar posicion inicial
	for( int i = 0; i < N; i++ )
	{
		// Asignar valores iniciales a cada componente de los vectores x y p
		for( int j = 0; j < dim; j++ )
		{
			// Valores aleatorios dentro del area inicial
			temp = (*p0)[j] + (2*ran()-1)*d0;
			// Posicion
			x[i].setComponente(j,temp);
			// Optimo local
			if(i < nVPG)
				p[i].setComponente(j,temp);
		}

		// Resetear velocidades
		if( i < iG )
			v[i] = 0;

		// Valor de la funcion en la posicion de la particula
		fx = funcion->evaluar(&x[i]);

		// Optimos locales
		if( i < nVPG )
			fp << fx;

		// Optimo global
		if( fx < fg )
		{
			fg = fx;
			*g = x.at(i);
		}
	}
}

Vector Enjambre::calcularPromedio()
// Posicion media del enjambre
{
	Vector promedio(dim);
	for( int i = 0; i < N; i++ )
		promedio = promedio + x.at(i);

	return promedio/N;
}

double Enjambre::calcularDispersion()
// Dispersion del enjambre (escalar)
{
	Vector promedio = calcularPromedio();
	Vector s(dim);
	for( int i = 0; i < N; i++ )
		for( int j = 0; j < dim; j++ )
			s.setComponente(j, s[j] + qPow( x.at(i)[j] - promedio[j],2 ) );

	s = s/N;

	return qSqrt(s.getModulo());
}

void Enjambre::agregarParticula( Particulas tipo )
// Se "crean" particulas en su posicion inicial
{
	// Nuevo vector de posicion
	Vector nuevaPos(dim);

	double temp;

	// Asignar posicion inicial
	for( int i = 0; i < dim; i++ )
	{
		temp = (*p0)[i] + ( 2*ran()-1 )*d0;
		nuevaPos.setComponente(i,temp);
	}

	switch(tipo)
	{
	case VPG: // Agregar particula VPG
	{
		// Nuevos vectores de velocidad y optimo local
		Vector nuevaVel(dim);
		Vector nuevoOpt(dim);

		// El optimo local inicialmente en la posicion actual
		nuevoOpt = nuevaPos;
		temp = funcion->evaluar(&nuevaPos);
		fp.prepend(temp);

		// Agregar vectores
		x.prepend(nuevaPos);
		v.prepend(nuevaVel);
		p.prepend(nuevoOpt);
		// Incrementar indices
		nVPG++;
		iG++;
		N++;
		break;
	}
	case VG: // Agregar particula VG
	{
		// Nuevo vector de optimo local
		Vector nuevaVel(dim);
		// Agregar vectores
		x.insert(nVPG,nuevaPos);
		v.insert(nVPG,nuevaVel);
		// Incrementar indices
		nVG++;
		iG++;
		N++;
		break;
	}
	case G: // Agregar particula G
	{
		// Agregar vector
		x << nuevaPos;
		// Incrementar indices
		nG++;
		N++;
		break;
	}
	}
}

void Enjambre::quitarParticula( Particulas tipo )
// Elimina una de las particulas del enjambre
{
	switch(tipo)
	{
	case VPG:
		if( nVPG > 0 )
		{
			// Borrar punteros y quitar de las listas
			x.removeFirst();
			v.removeFirst();
			p.removeFirst();
			fp.removeFirst();
			// Decrementar inidices
			nVPG--;
			iG--;
			N--;
		}
		break;
	case VG:
		if( nVG > 0)
		{
			// Borrar punteros y quitar de las listas
			x.removeAt(nVPG);
			v.removeAt(nVPG);
			// Decrementar inidices
			nVG--;
			iG--;
			N--;
		}
		break;
	case G:
		if( nG > 0 )
		{
			// Borrar punteros y quitar de las listas
			x.removeLast();
			// Decrementar indices
			nG--;
			N--;
		}
		break;
	}
}

void Enjambre::setNroParticulas(Particulas tipo, int cantidad)
// Setear numero de particulas por cantidad
{
	switch(tipo)
	{
	case VPG:
		while( nVPG < cantidad )
			agregarParticula(VPG);
		while( nVPG > cantidad )
			quitarParticula(VPG);
		break;
	case VG:
		while( nVG < cantidad )
			agregarParticula(VG);
		while( nVG > cantidad )
			quitarParticula(VG);
		break;
	case G:
		while( nG < cantidad )
			agregarParticula(G);
		while( nG > cantidad )
			quitarParticula(G);
		break;
	}
}

void Enjambre::setFuncion(Funcion *objetivo)
// Cambiar la funcion objetivo
{
	funcion = objetivo;
	funcion->setInv(max);
	// Cambiar la dimension de todos los vectores
	if( funcion->getDimension() != dim )
	{
		dim = funcion->getDimension();
		int i = 0;
		while( i < nVPG )
		{
			x[i].setDimension(dim);
			v[i].setDimension(dim);
			p[i].setDimension(dim);
			i++;
		}
		while( i < iG )
		{
			x[i].setDimension(dim);
			v[i].setDimension(dim);
			i++;
		}
		while( i < N )
		{
			x[i].setDimension(dim);
			i++;
		}
		g->setDimension(dim);
		p0->setDimension(dim);
		limMin->setDimension(dim);
		limMax->setDimension(dim);
		*limMin = -__DBL_MAX__;
		*limMax = __DBL_MAX__;
	}
	reiniciar();
}

// EOF
