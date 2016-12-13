#ifndef TERMINAL_H
#define TERMINAL_H

#include <QtOpenGL/QGLWidget>
#include <QStringList>
#include <QList>
#include <iostream>

using namespace std;

class Terminal : public QGLWidget
{
	Q_OBJECT

public:
	explicit Terminal(QWidget *parent = 0);
	void output(QString texto, QFont fuente);
	void limpiar();

protected:
	void initializeGL();
	void resizeGL(int w, int h);
	void paintGL();

private:
	QStringList salida;
	QList<QFont> fuentes;

};

#endif // TERMINAL_H
