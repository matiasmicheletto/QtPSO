#include "GraficoEnjambre.h"

GraficoEnjambre::GraficoEnjambre(Vector *_centro, int *delay, QWidget *parent) :
	QGLWidget(parent)
{
	centro = _centro;
	tamanio = 0;
	xRot = 250;
	yRot = 250;
	zRot = 0;
	escala = 10;
	mostrarCubo = true;
	mostrarInfo = true;
	demora = delay;

	setMinimumSize(500,250);

	// Mostrar mensaje por consola
	cout << "Nuevo grafico de puntos." << endl;
}

GraficoEnjambre::~GraficoEnjambre()
// Destructor, borrar punteros a vectores
{
	while(!puntos.isEmpty())
		delete puntos.takeFirst();

	delete centro;
}

void GraficoEnjambre::setPunto(int i, Vector posicion)
{
	// Se grafican hasta las 3 primeras componentes de las particulas
	puntos.at(i)->setComponente(0,posicion.getComponente(0));

	puntos.at(i)->setComponente(1, posicion.getDimension() > 1 ?
									posicion.getComponente(1) : 0);
	puntos.at(i)->setComponente(2, posicion.getDimension() > 2 ?
								posicion.getComponente(2) : 0);
}

void GraficoEnjambre::setTamanio(int N)
{
	tamanio = N;

	while( !puntos.empty() )
		delete puntos.takeFirst();

	Vector *nuevoVector;
	for(int i = 0; i < N; i++)
	{
		nuevoVector = new Vector(3);
		puntos << nuevoVector;
	}
}

void GraficoEnjambre::setEscala(double esc)
{
	// Los valores de la escala deben estar entre 1 y 200
	if(esc < 1)	escala = 1;
	else if(esc > 200)	escala = 200;
	else escala = esc;

	emit escalaChanged(escala);

	updateGL();
}

void GraficoEnjambre::setXRotation(double angle)
{
	normalizeAngle(&angle);
	if (angle != xRot) {
		xRot = angle;
		updateGL();
	}
}

void GraficoEnjambre::setYRotation(double angle)
{
	normalizeAngle(&angle);
	if (angle != yRot) {
		yRot = angle;
		updateGL();
	}
}

void GraficoEnjambre::setZRotation(double angle)
{
	normalizeAngle(&angle);
	if (angle != zRot) {
		zRot = angle;
		updateGL();
	}
}

void GraficoEnjambre::mousePressEvent(QMouseEvent *event)
{
	lastPos = event->pos();
	if(event->button() == 4)
		setEscala(10);
}

void GraficoEnjambre::mouseMoveEvent(QMouseEvent *event)
{
	int dx = event->x() - lastPos.x();
	int dy = event->y() - lastPos.y();

	if (event->buttons() & Qt::LeftButton)
	{
		setXRotation(xRot + 8 * dy);
		setYRotation(yRot + 8 * dx);
	}
	else if (event->buttons() & Qt::RightButton)
	{
		setXRotation(xRot + 8 * dy);
		setZRotation(zRot + 8 * dx);
	}
	lastPos = event->pos();
}

void GraficoEnjambre::wheelEvent(QWheelEvent *event)
{
	setEscala(escala-event->delta()/60);
}

void  GraficoEnjambre::initializeGL()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable( GL_POINT_SMOOTH );
	glEnable( GL_BLEND );
	glEnable( GL_NORMALIZE );
    //glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // Fondo blanco
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   // Fondo negro
	glPointSize(2);
}

void  GraficoEnjambre::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void  GraficoEnjambre::paintGL()
{
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glPushMatrix();

	/// LEYENDAS ///

	QString temp;

	if( mostrarInfo )
	{
        //glColor3f( 0, 0, 0 ); // Fondo blanco
        glColor3f( 1, 1, 1 );   // Fondo negro

		// Velocidad de refresco
		QFont fuente("",8);

		temp.setNum(qRound((double) (*demora)/-200 + 10.5));
		temp.prepend("Frecuencia = ");
		temp.append(" fps");
		renderText(-.95,.95,0,temp,fuente);

		// Escala en porcentajes
		temp.setNum(int (escala*-.49+100.5));
		temp.prepend("Zoom = ");
		temp.append("%");
		renderText(-.95,.9,0,temp,fuente);

		// Rotacion
		temp.setNum(xRot/25.0);
		temp.prepend("@X1 = ");
		glColor3f( 1, 0, 0 );
		renderText(-.95,-.85,0,temp,fuente);

		temp.setNum(yRot/25.0);
		temp.prepend("@X2 = ");
		glColor3f( 0, 1, 0 );
		renderText(-.95,-.9,0,temp,fuente);

		temp.setNum(zRot/25.0);
		temp.prepend("@X3 = ");
		glColor3f( 0, 0, 1 );
		renderText(-.95,-.95,0,temp,fuente);
	}

	// Rotar grafico

	glRotated(xRot / 25.0, 1.0, 0.0, 0.0);
	glRotated(yRot / 25.0, 0.0, 1.0, 0.0);
	glRotated(zRot / 25.0, 0.0, 0.0, 1.0);


	/// PUNTOS ///

	glBegin( GL_POINTS);

	double px,py,pz;

	// Graficar todos los puntos
	for ( int i = 0; i < tamanio; i++ )
	{
		px = ((*puntos.at(i))[0] - (*centro)[0]) / escala;
		py = ((*puntos.at(i))[1] - (*centro)[1]) / escala;
		pz = ((*puntos.at(i))[2] - (*centro)[2]) / escala;
        glColor3f(0.5+px,0.5+py,0.5+pz);    // Colores variables
		glVertex3f( px, py, pz);
	}

	glEnd();


	/// EJES ///

	// Dibujar cubo
	if( mostrarCubo )
	{
		// Nombres de los ejes
        //glColor3f( 0,0,0 );   // Fondo blanco
        glColor3f( 1, 1, 1 );   // Fondo negro

		temp = "X1";
		renderText(.55,-.55,-.55,temp);
		temp = "X2";
		renderText(-.55,.55,-.55,temp);
		temp = "X3";
		renderText(-.55,-.55,.55,temp);

		// Escalas
		double valor;
		QFont fuente("",7,-1);
		for( double i = -.4; i < .5; i = i+.1 )
		{
			valor = i*escala + (*centro)[0];
			if( valor > 0)
				temp.setNum((double)qFloor(10*valor)/10);
			else
				temp.setNum((double)qCeil(10*valor)/10);
			renderText( i, -0.55, -0.55,temp,fuente);

			valor = i*escala + (*centro)[1];
			if( valor > 0)
				temp.setNum((double)qFloor(10*valor)/10);
			else
				temp.setNum((double)qCeil(10*valor)/10);
			renderText( -0.55, i, -0.55,temp,fuente);

			valor = i*escala + (*centro)[2];
			if( valor > 0)
				temp.setNum((double)qFloor(10*valor)/10);
			else
				temp.setNum((double)qCeil(10*valor)/10);
			renderText( -0.55, -0.55, i,temp,fuente);
		}

		// Lineas
		glBegin( GL_LINES );

        // Ejes principales de colores
		glColor3f( 1, 0, 0 );

		glVertex3f(-0.5f, -0.5f, -0.5f);
		glVertex3f(0.5f, -0.5f, -0.5f);

		glColor3f( 0, 1, 0 );

		glVertex3f(-0.5f, -0.5f, -0.5f);
		glVertex3f(-0.5f, 0.5f, -0.5f);

		glColor3f( 0, 0, 1 );

		glVertex3f(-0.5f, -0.5f, -0.5f);
		glVertex3f(-0.5f, -0.5f, 0.5f);

        // Resto de los ejes

        //glColor3f(0,0,0);     // Fondo blanco
        glColor3f( 1, 1, 1 );   // Fondo negro

		glVertex3f(0.5f, -0.5f, -0.5f);
		glVertex3f(0.5f, 0.5f, -0.5f);

		glVertex3f(0.5f, 0.5f, -0.5f);
		glVertex3f(-0.5f, 0.5f, -0.5f);

		glVertex3f(0.5f, -0.5f, -0.5f);
		glVertex3f(0.5f, -0.5f, 0.5f);

		glVertex3f(0.5f, 0.5f, -0.5f);
		glVertex3f(0.5f, 0.5f, 0.5f);

		glVertex3f(-0.5f, 0.5f, -0.5f);
		glVertex3f(-0.5f, 0.5f, 0.5f);

		glVertex3f(-0.5f, -0.5f, 0.5f);
		glVertex3f(0.5f, -0.5f, 0.5f);

		glVertex3f(0.5f, -0.5f, 0.5f);
		glVertex3f(0.5f, 0.5f, 0.5f);

		glVertex3f(0.5f, 0.5f, 0.5f);
		glVertex3f(-0.5f, 0.5f, 0.5f);

		glVertex3f(-0.5f, 0.5f, 0.5f);
		glVertex3f(-0.5f, -0.5f, 0.5f);

		glEnd();
	}

	glPopMatrix();
}

void GraficoEnjambre::activarCubo( bool activar )
{
	mostrarCubo = activar;
	updateGL();
}

void GraficoEnjambre::activarInfo( bool activar )
{
	mostrarInfo = activar;
	updateGL();
}

void GraficoEnjambre::normalizeAngle( double *angle )
{
	while (*angle < 0)
		*angle += 360 * 25;
	while (*angle > 360 * 25)
		*angle -= 360 * 25;
}

// EOF
