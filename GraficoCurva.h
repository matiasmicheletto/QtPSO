#ifndef GRAFICOCURVA_H
#define GRAFICOCURVA_H

#include <QtOpenGL/QGLWidget>
#include <QList>
#include <QtCore/qmath.h>
#include <iostream>

class GraficoCurva : public QGLWidget
{
	Q_OBJECT
public:
	explicit GraficoCurva( QString titulo = "", QWidget *parent = 0 );
	void nvoDato( double dato );
	void reiniciar();
	void inline setNormalizador( double valor );
	void setColor( double r, double g, double b );

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

	QList<double> datos;
	double paso, normalizador, cr, cg, cb;
	QString leyenda;
};

void inline GraficoCurva::setNormalizador(double valor)
{
	normalizador = valor;
}

#endif // GRAFICOCURVA_H
