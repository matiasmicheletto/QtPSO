#ifndef ENJAMBRE_H
#define ENJAMBRE_H

#include <ctime>
#include "Funcion.h"

class Enjambre
{
public:
	Enjambre( Funcion *objetivo );
	~Enjambre();

	// Tipos de particulas
	enum Particulas{ VPG, VG, G, T };

	void iterar();
	void reiniciar();

	double calcularDispersion();
	Vector calcularPromedio();
	inline double getError() const;

	void agregarParticula( Particulas tipo );
	void quitarParticula( Particulas tipo );
	void setNroParticulas( Particulas tipo, int cantidad );

	// Getters & Setters
	void setFuncion( Funcion *objetivo );

	inline void setTipoDeOpt( bool maximizar );
	inline void setDispersionInicial( double valor );
	inline void setPosicionInicial( Vector *posicion );

	inline void setFactorP( double valor );
	inline void setFactorG( Particulas tipo, double valor );
	inline void setInercia( Particulas tipo, double valor );

	inline void setLimitesEspacio( bool limitar );
	inline void setLimiteVelocidad( bool limitar );
	inline void setLimiteInf( Vector *limiteMin );
	inline void setLimiteSup( Vector *limiteMax );
	inline void setVelocidadMaxima( double velocidad );

	inline double getFOptimo() const;
	inline Vector getOptimo() const;
	inline int getDimension() const;

	inline int getCantidadParticulas( Particulas tipo = T ) const;
	inline Vector getPosicionParticula( int i ) const;

	inline int getNroIteracion() const;

	inline double getFactorP() const;
	inline double getFactorG( Particulas tipo ) const;
	inline double getInercia( Particulas tipo ) const;

	inline Vector getPosInicial() const;
	inline double getDispersionInicial() const;

	inline void estaLimitado(bool &espacio, bool &velocidad);

	inline Vector getLimiteInf() const;
	inline Vector getLimiteSup() const;

	inline double getVelMax() const;

private:
	// Funcion objetivo
	Funcion *funcion;
	// Vectores de velocidad, posicion y optimos locales
	QList<Vector> v, p, x;
	// Lista de valores de optimos locales -
	QList<double> fp;
	// Optimo global - posicion inicial - limites inferior y superior
	Vector *g, *p0, *limMin, *limMax;
	// Cantidad de particulas de cada tipo - indices - dimensiones del espacio -
	// numero de iteracion
	int nVPG, nVG, nG, iG, N, dim, iter;
	// Factores de correccion (comportamiento) - optimo global - disp inicial -
	// limite de velocidad - error estimado
	double factorG[3], inercia[2], factorP, fg, d0, velMax, ea;

	bool espacioLimitado, velocidadLimitada, max;

	inline double ran();
	void actualizarOptimos( int i );
	void inicializar( int i );
};

inline double Enjambre::getFOptimo() const
{
	return max? -fg : fg;
}

inline Vector Enjambre::getOptimo() const
{
	return *g;
}

inline double Enjambre::getError() const
{
	return ea;
}

inline int Enjambre::getDimension() const
{
	return dim;
}

inline void Enjambre::setTipoDeOpt(bool maximizar)
{
	max = maximizar;
	funcion->setInv(max);
}

inline void Enjambre::setDispersionInicial( double valor )
{
	d0 = valor;
}

inline void Enjambre::setPosicionInicial( Vector *posicion )
{
	*p0 = *posicion;
}

inline void Enjambre::setFactorP( double valor )
{
	factorP = valor;
}

inline void Enjambre::setFactorG( Particulas tipo, double valor )
{
	factorG[tipo] = valor;
}

inline void Enjambre::setInercia( Particulas tipo, double valor )
{
	inercia[tipo] = valor;
}

inline void Enjambre::setLimitesEspacio( bool limitar )
{
	espacioLimitado = limitar;
}

inline void Enjambre::setLimiteVelocidad( bool limitar )
{
	velocidadLimitada = limitar;
}

inline void Enjambre::setLimiteInf( Vector *limiteMin )
{
	*limMin = *limiteMin;
}

inline void Enjambre::setLimiteSup(Vector *limiteMax)
{
	*limMax = *limiteMax;
}

inline void Enjambre::setVelocidadMaxima( double velocidad )
{
	velMax = velocidad;
}

inline int Enjambre::getCantidadParticulas(Particulas tipo) const
{
	switch( tipo )
	{
	case VPG:
		return nVPG;
	case VG:
		return nVG;
	case G:
		return nG;
	case T:
		return nVPG + nVG + nG;
	}
}

inline Vector Enjambre::getPosicionParticula(int i) const
{
	return x.at(i);
}

inline int Enjambre::getNroIteracion() const
{
	return iter;
}

inline double Enjambre::getFactorP() const
{
	return factorP;
}

inline double Enjambre::getFactorG( Particulas tipo ) const
{
	return factorG[tipo];
}

inline double Enjambre::getInercia( Particulas tipo ) const
{
	return inercia[tipo];
}

inline Vector Enjambre::getPosInicial() const
{
	return *p0;
}

inline double Enjambre::getDispersionInicial() const
{
	return d0;
}

inline void Enjambre::estaLimitado(bool &espacio, bool &velocidad)
{
	espacio = espacioLimitado;
	velocidad = velocidadLimitada;
}

inline Vector Enjambre::getLimiteInf() const
{
	return *limMin;
}

inline Vector Enjambre::getLimiteSup() const
{
	return *limMax;
}

inline double Enjambre::getVelMax() const
{
	return velMax;
}

inline double Enjambre::ran()
// Retorna valores aleatorios entre 0 y 1
{
	return (qrand() % 10000) / 1e4;
}


#endif // ENJAMBRE_H
