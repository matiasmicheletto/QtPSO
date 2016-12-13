#include "Terminal.h"

Terminal::Terminal(QWidget *parent) : QGLWidget(parent)
{
	setMinimumSize(500,250);
	move(200,200);
}

void Terminal::output(QString texto, QFont fuente)
{
	salida << texto;
	fuentes << fuente;

    // Mostrar resultados por consola
    cout << qPrintable(texto) << endl;
	if( salida.size() > 18 )
	{
		salida.removeFirst();
		fuentes.removeFirst();
	}
	updateGL();
}

void Terminal::limpiar()
{
	salida.clear();
	fuentes.clear();
	updateGL();
}

void Terminal::initializeGL()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable( GL_NORMALIZE );
	glEnable( GL_BLEND );
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
}

void Terminal::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void Terminal::paintGL()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glColor3f( 0, 0, 0 );
	int i = 0;
	for(double d = 0.9; d > -.9 && i < salida.size(); d -= 0.11 )
	{
		renderText(-.9,d,0,salida.at(i),fuentes.at(i));
		i++;
	}
}

// EOF
