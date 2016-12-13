#include "AnimacionWidget.h"

AnimacionWidget::AnimacionWidget( QWidget *parent ) :	QWidget(parent)
{
	// Mostrar mensaje por consola
    cout << "Creando la pestania de animacion..." << endl;

	enjambre = 0;

	// Inicialmente animacion detenida
	play = false;
	dispMax = 0;
	errorMax = 0;

	// Timer (controla la velocidad de la animacion)
	timer = new QTimer;
	delay = 500;
	timer->start(delay);
	connect(timer,SIGNAL(timeout()),this,SLOT(refrescarEnjambre()));

	// Cronometro para medir el tiempo de ejecucion de la optimizacion
	cronometro = new QElapsedTimer;
	tiempoEjec = 0;

	// Botones
	btnPlay = new QPushButton;
	btnPlay->setIcon(QIcon("Play2.png"));
	QPushButton *btnReset = new QPushButton;
	btnReset->setIcon(QIcon("Reset.png"));
	QPushButton *btnSalir = new QPushButton;
	btnSalir->setIcon(QIcon("ButtonCancel.png"));

	// Atajos
	btnPlay->setShortcut(QKeySequence("Ctrl+P"));
	btnReset->setShortcut(QKeySequence("Ctrl+R"));
	btnSalir->setShortcut(QKeySequence("Ctrl+Q"));

	// Botones: SIGNALS -> SLOTS
	connect(btnReset,SIGNAL(clicked()),this,SLOT(btnResetClicked()));
	connect(btnPlay,SIGNAL(clicked()),this,SLOT(btnPlayClicked()));
	connect(btnSalir,SIGNAL(clicked()),this,SLOT(btnSalirClicked()));

	// CheckBox para activar desactivar el cubo de coordenadas
	QCheckBox *cuboChk = new QCheckBox("Ejes  ");
	cuboChk->setChecked(true);

	// CheckBox para activar desactivar la informacion de grafico
	QCheckBox *infoChk = new QCheckBox("Leyenda  ");
	infoChk->setChecked(true);

	// CheckBox: SIGNALS -> SLOTS
	connect(cuboChk,SIGNAL(toggled(bool)),this,SLOT(activarCubo(bool)));

	connect(infoChk,SIGNAL(toggled(bool)),this,SLOT(activarInfo(bool)));
	// Slider para controlar la velocidad de la animacion
	QSlider *sliderVelocidad = new QSlider(Qt::Vertical);
	// Setear rango
	sliderVelocidad->setRange(100,2000);
	// Setear paso
	sliderVelocidad->setSingleStep(100);
	// Setear valor inicial
	sliderVelocidad->setValue(delay);
	// Invertir direccion
	sliderVelocidad->setInvertedAppearance(true);

	// Slider para controlar la escala del grafico
	sliderEscala = new QSlider(Qt::Vertical);
	// Setear rango
	sliderEscala->setRange(1,200);
	// Setear valor inicial
	sliderEscala->setValue(10);
	// Invertir direccion
	sliderEscala->setInvertedAppearance(true);

	// Sliders: SIGNAL ->SLOT
	connect(sliderVelocidad, SIGNAL(valueChanged(int)), this,
			SLOT(sliderVelValueChanged(int)));
	connect(sliderEscala, SIGNAL(valueChanged(int)), this,
			SLOT(sliderEscValueChanged(int)));

	// Etiquetas con el numero de iteraciones y optimo global
	QString temp;
	temp.setNum(0);
	iterLabel = new QLabel("  Iteración "+temp);
	temp.setNum(0);
	optLabel = new QLabel("  Optimo = "+temp);

	// Barra de herramientas
	QToolBar *barra = new QToolBar("Barra");
	barra->setFloatable(true);

	// Agregar widgets a la barra de herramientas
	barra->addWidget(btnPlay);
	barra->addWidget(btnReset);
	barra->addWidget(btnSalir);
	barra->addSeparator();
	barra->addWidget(cuboChk);
	barra->addWidget(infoChk);
	barra->addSeparator();
	barra->addWidget(iterLabel);
	barra->addSeparator();
	barra->addWidget(optLabel);

	// Mensaje por consola
	cout << "Barra de herramientas creada correctamente." << endl;

	// Centro del grafico de las particulas, incialmente (0,0,0)
	centroCubo = new Vector(3);

	// QGLWidget para el grafico de las particulas
	graficoEnjambre = new GraficoEnjambre(centroCubo,&delay);
	actualizarGraficoEnjambre();

	// Grafico de enjambre: SIGNAL -> SLOT
	connect(graficoEnjambre, SIGNAL(escalaChanged(int)), this,
			SLOT(escalaChanged(int)));

	// Mensaje por consola
	cout << "Grafico para el enjambre creado correctamente." << endl;

	// Graficos de dispersion y de error estimado
	graficoDispersion = new GraficoCurva("Disp.= ");
	graficoDispersion->setColor(0.5,0.9,0.5);	
	disp = 0;
	graficoDispersion->nvoDato(disp);
	// Mostrar mensaje por consola
	cout << "Grafico de dispersion creado correctamente." << endl;

	graficoError = new GraficoCurva("Error= ");
	graficoError->setColor(0.9,.2,0.2);
	error = 0;
	graficoError->nvoDato(error);
	// Mostrar mensaje por consola
	cout << "Grafico de error estimado creado correctamente." << endl;

	// Etiquetas para los graficos
	temp.setNum(disp);
	dispLabel = new QLabel( "Dispersión = "+temp );
	temp.setNum(error);
	errorLabel = new QLabel( "Error estimado relativo = "+temp );

	// Layouts
	QVBoxLayout *vLayout = new QVBoxLayout;
//	vLayout->addWidget(dispLabel);
	vLayout->addWidget(graficoDispersion);
//	vLayout->addWidget(errorLabel);
	vLayout->addWidget(graficoError);

	QHBoxLayout *hLayout = new QHBoxLayout;
	hLayout->addWidget(sliderVelocidad);
	hLayout->addWidget(sliderEscala);
	hLayout->addWidget(graficoEnjambre);
	hLayout->addLayout(vLayout);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(barra);
	layout->addLayout(hLayout);

	setLayout(layout);
	setWindowTitle("Movimiento del enjambre de partículas");
	setWindowIcon(QIcon("Graficar.png"));

	// Mostrar mensaje por consola
    cout << "Pestania de animacion creada correctamente." << endl;
}

void AnimacionWidget::setEnjambre( Enjambre *_enjambre )
{
	enjambre = _enjambre;
	setEnabled(true);
	actualizarGraficoEnjambre();
}

void AnimacionWidget::actualizarGraficoEnjambre()
// Enviar posiciones de las particulas al widget que las grafica
{
	// Si el enjambre existe
	if( enjambre!=0 )
	{
		int N = enjambre->getCantidadParticulas();
		graficoEnjambre->setTamanio(N);
		// Para cada particula, pasar vector de posicion
		for(int i = 0; i < N; i++)
			graficoEnjambre->setPunto(i, enjambre->getPosicionParticula(i));
	}

	// Actualizar grafico
	graficoEnjambre->updateGL();
}

void AnimacionWidget::refrescarEnjambre()
// SLOT: Realizar iteracion y actualizar grafico
{
	if(play)
	{
		// Realizar iteracion
		enjambre->iterar();

		Vector promedio = enjambre->calcularPromedio();

		double escala = graficoEnjambre->getEscala();

		for( int i = 0; i < promedio.getDimension() && i < 3; i++ )
			if( promedio[i] > (*centroCubo)[i] + escala/2 ||
					promedio[i] < (*centroCubo)[i] - escala/2)
				centroCubo->setComponente(i,promedio[i]);

		actualizarGraficoEnjambre();

		// Normalizar datos de las curvas
		disp = enjambre->calcularDispersion();
		if( disp > dispMax )
		{
			dispMax = disp;
			graficoDispersion->setNormalizador(disp);
		}
		error = enjambre->getError();
		if( error > errorMax )
		{
			errorMax = error;
			graficoError->setNormalizador(error);
		}

		// Agregar datos a las curvas
		graficoDispersion->nvoDato(disp);
		graficoError->nvoDato(error);

		// Actualizar etiquetas
		QString temp;
		temp.setNum(disp);
		dispLabel->setText( "Dispersión = "+temp );
		temp.setNum(error);
		errorLabel->setText( "Error estimado relativo = "+temp );
		temp.setNum(enjambre->getNroIteracion());
		iterLabel->setText("  Iteración "+temp);
		temp.setNum(enjambre->getFOptimo());
		optLabel->setText("  Optimo = "+temp);
	}
}

void AnimacionWidget::pausar()
{
	play = true;
	btnPlayClicked();
}

void AnimacionWidget::btnPlayClicked()
// SLOT: Iniciar - Pausar animacion
{
	if(play)
	{
		// Pausar
		btnPlay->setIcon(QIcon("Play2.png"));
		play = false;
		tiempoEjec += cronometro->elapsed();
	}
	else
	{
		// Iniciar
		btnPlay->setIcon(QIcon("Pause.png"));
		play = true;		
		cronometro->start();
	}
}

void AnimacionWidget::btnResetClicked()
// SLOT: Reiniciar animacion
{
	// Reiniciar la posicion de las particulas
	enjambre->reiniciar();
	actualizarGraficoEnjambre();
	// Reiniciar el cronometro
	tiempoEjec = 0;
	// Reiniciar graficos de curvas
	graficoDispersion->reiniciar();
	graficoError->reiniciar();
	// Reiniciar escalas
	dispMax = 0;
	errorMax = 0;
	// Reiniciar centro del cubo
	*centroCubo = 0;
	// Agregar primeros datos a las curvas
	disp = enjambre->calcularDispersion();
	error = enjambre->getError();
	graficoDispersion->nvoDato(disp);
	graficoError->nvoDato(error);
	QString temp;
	temp.setNum(enjambre->getNroIteracion());
	iterLabel->setText("  Iteración "+temp);
	temp.setNum(enjambre->getFOptimo());
	optLabel->setText("  Optimo = "+temp);
}

void AnimacionWidget::btnSalirClicked()
{
	play = true;
	emit btnPlayClicked();
	int temp = tiempoEjec - enjambre->getNroIteracion()*delay;
	emit optimizacionTerminada( temp );
}

void AnimacionWidget::sliderVelValueChanged(int valor)
// SLOT
{
	delay = valor;
	timer->start(valor);
	graficoEnjambre->updateGL();
}

void AnimacionWidget::sliderEscValueChanged(int valor)
// SLOT
{
	graficoEnjambre->setEscala(valor);	
}

void AnimacionWidget::escalaChanged( int valor )
// SLOT
{
	sliderEscala->setValue(valor);
}

void AnimacionWidget::activarCubo( bool activar )
{
	graficoEnjambre->activarCubo(activar);
}

void AnimacionWidget::activarInfo( bool activar )
{
	graficoEnjambre->activarInfo(activar);
}

// EOF
