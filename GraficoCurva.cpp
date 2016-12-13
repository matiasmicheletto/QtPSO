#include "GraficoCurva.h"

using namespace std;

GraficoCurva::GraficoCurva( QString titulo, QWidget *parent ) : QGLWidget(parent)
{
	setMinimumSize(400,100);

	leyenda = titulo;

	normalizador = 1;

    // Color de la curva por defecto
	cr = 1;
	cg = 0.3;
	cb = 0.3;

	// Mensaje por consola
	cout << "Nuevo grafico de curva." << endl;
}


void GraficoCurva::initializeGL()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glClearColor(1, 1, 1, 1); // Fondo blanco
    glClearColor(0, 0, 0, 0); // Fondo negro
	glEnable( GL_POINT_SMOOTH );
	glEnable( GL_BLEND );
//	glLineWidth(2);
}

void GraficoCurva::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLineWidth(2);
}

void GraficoCurva::paintGL()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	// Escala
	double valor;
	double r = normalizador/8;
	QString temp;
	QFont fuente("",7,-1);

	temp.setNum(datos.last());
	renderText(0.5,0.85,0,leyenda+temp);

	valor = 0.0;
	for( double i = -.9; i < 1; i = i+.2 )
	{
		temp.setNum((double)qFloor(10*valor)/10);
		renderText( -0.99,i,0,temp,fuente);
		valor = valor + r;
	}

	// Curva
	glBegin(GL_LINES);	

    glColor3f( cr, cg, cb );    // El color de la curva es "seteable"

	int N = datos.size();
	for( double i = 0; i < N-1; i++ )
	{
		glVertex2f( i * paso - .9, 1.8*datos.at(i)/normalizador - .9 );
		glVertex2f( (i+1) * paso - .9, 1.8*datos.at(i+1)/normalizador - .9 );
	}

	// Ejes

    //glColor3f( 0, 0, 0 );   // Fondo blanco
    glColor3f( 1, 1, 1 ); // Fondo negro

	glVertex2f(-0.9,-0.9);
	glVertex2f(0.9,-0.9);

	glVertex2f(-0.9,-0.9);
	glVertex2f(-0.9,0.9);

	glEnd();
}

void GraficoCurva::nvoDato(double dato)
{
	// Separacion horizontal entre los datos
	paso = 1.8/(datos.size()-1);
	// Agregar dato a la lista
	datos << dato;

	// Actualizar grafica
	updateGL();
}

void GraficoCurva::reiniciar()
{
	datos.clear();
	normalizador = 1;
}

void GraficoCurva::setColor(double r, double g, double b)
{
	cr = r;
	cg = g;
	cb = b;
}

// EOF
