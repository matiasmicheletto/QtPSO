#ifndef GRAFICOENJAMBRE_H
#define GRAFICOENJAMBRE_H

#include <QtGui>
#include <QtOpenGL/QGLWidget>
#include <QList>
#include "Vector.h"

class GraficoEnjambre : public QGLWidget
{
    Q_OBJECT
public:
	explicit GraficoEnjambre(Vector *_centro, int *delay, QWidget *parent = 0);
	~GraficoEnjambre();

	void setPunto( int i, Vector posicion );
	void setTamanio( int N );
	void setEscala( double esc );
	void activarCubo( bool activar );
	void activarInfo( bool activar );
	inline double getEscala() const;

signals:
	void escalaChanged(int valor);

public slots:
	void setXRotation(double angle);
	void setYRotation(double angle);
	void setZRotation(double angle);

private:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);

	void normalizeAngle(double *angle);

	int tamanio, *demora;
	double escala, xRot, yRot, zRot;
	QPoint lastPos;
	QList<Vector*> puntos;
	Vector *centro;
	bool mostrarCubo, mostrarInfo;
};

inline double GraficoEnjambre::getEscala() const
{
	return escala;
}

#endif // GRAFICOENJAMBRE_H
