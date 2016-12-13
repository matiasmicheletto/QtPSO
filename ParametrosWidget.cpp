#include "ParametrosWidget.h"

ParametrosWidget::ParametrosWidget(QWidget *parent) :
    QWidget(parent)
{
    // Mostrar mensaje por consola
    cout << "Creando la pestania de parametros..." << endl;

	// Dimensiones de la funcion (columnas de las tablas)
	dims = 0;

	// Puntero para el enjambre
	enjambre = 0;

	// Criterios de terminacion
	criterioTerm[0] = true;
	criterioTerm[1] = false;
	criterioTerm[2] = false;
	criterioTerm[3] = false;

	posInicial = new Vector(1);
	limiteInf = new Vector(1);
	limiteSup = new Vector(1);


	/// TABLA DE PARAMETROS ///

	// Tabla para seleccionar los paramentros de comportamiento
	QTableWidget *tablaParametros = new QTableWidget(3,4);

	// Cabeceras
	QStringList hHeaders, vHeaders;

	hHeaders << "Cantidad" << "Parámetro G" << "Inercia" << "Parámetro P";
	vHeaders << "Partículas VPG" << "Partículas VG" << "Partículas G";

	tablaParametros->setHorizontalHeaderLabels(hHeaders);
	tablaParametros->setVerticalHeaderLabels(vHeaders);

	// SpinBoxes para la tabla
	nVPGSpin = new QSpinBox;
	nVPGSpin->setMinimum(0);
	nVPGSpin->setMaximum(500);
        nVPGSpin->setValue(100);

	nVGSpin = new QSpinBox;
	nVGSpin->setMinimum(0);
	nVGSpin->setMaximum(500);
	nVGSpin->setValue(0);

	nGSpin = new QSpinBox;
	nGSpin->setMinimum(0);
	nGSpin->setMaximum(500);
	nGSpin->setValue(0);

	connect(nVPGSpin, SIGNAL(editingFinished()), this,
			SLOT(poblacionVPGChanged()));
	connect(nVGSpin, SIGNAL(editingFinished()), this,
			SLOT(poblacionVGChanged()));
	connect(nGSpin, SIGNAL(editingFinished()), this,
			SLOT(poblacionGChanged()));

	for(int i = 0; i < 3; i++)
	{
		gSpin[i] = new QDoubleSpinBox;
		gSpin[i]->setDecimals(4);
		gSpin[i]->setSingleStep(0.1);
		gSpin[i]->setValue(1.5);
		tablaParametros->setCellWidget(i,1,gSpin[i]);
	}

	vSpin[0] = new QDoubleSpinBox;
	vSpin[0]->setDecimals(4);
	vSpin[0]->setSingleStep(0.1);
	vSpin[0]->setValue(0.5);

	vSpin[1] = new QDoubleSpinBox;
	vSpin[1]->setDecimals(4);
	vSpin[1]->setSingleStep(0.1);
	vSpin[1]->setValue(0.5);

	pSpin = new QDoubleSpinBox;
	pSpin->setDecimals(4);
	pSpin->setSingleStep(0.1);
	pSpin->setValue(1.5);

	// Agregar los SpinBoxes
	tablaParametros->setCellWidget(0,0,nVPGSpin);
	tablaParametros->setCellWidget(1,0,nVGSpin);
	tablaParametros->setCellWidget(2,0,nGSpin);

	tablaParametros->setCellWidget(0,2,vSpin[0]);
	tablaParametros->setCellWidget(1,2,vSpin[1]);

	tablaParametros->setCellWidget(0,3,pSpin);

	tablaParametros->setShowGrid(false);
	tablaParametros->setSelectionMode(QAbstractItemView::NoSelection);

	tablaParametros->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
	tablaParametros->setMaximumHeight(4*tablaParametros->rowHeight(0));

	// SpinBoxes: SIGNALS -> SLOTS
	connect(gSpin[0], SIGNAL(editingFinished()), this,
			SLOT(parametroG0Changed()));
	connect(gSpin[1], SIGNAL(editingFinished()), this,
			SLOT(parametroG1Changed()));
	connect(gSpin[2], SIGNAL(editingFinished()), this,
			SLOT(parametroG2Changed()));

	connect(vSpin[0], SIGNAL(editingFinished()), this,
			SLOT(inercia0Changed()));
	connect(vSpin[1], SIGNAL(editingFinished()), this,
			SLOT(inercia1Changed()));

	connect(pSpin, SIGNAL(editingFinished()), this,
			SLOT(parametroPChanged()));

    // Mostrar mensaje por consola
    cout << "Tabla de parametros creada correctamente." << endl;

	/// SELECTORES ///

	// ComboBox para seleccionar el tipo de optimizacion
	optimizacionCombo = new QComboBox;
	// Iconos para el tipo de optimizacion
	optimizacionCombo->setIconSize(QSize(30,30));
	// Agregar los items
	optimizacionCombo->addItem(QIcon("Minimo.png"),tr("Minimizar"));
	optimizacionCombo->addItem(QIcon("Maximo.png"),tr("Maximizar"));

	connect(optimizacionCombo, SIGNAL(activated(int)),
			this, SLOT(tipoOptChanged(int)));

	// Etiqueta para el ComboBox
	QLabel *optimizacionLabel = new QLabel(tr("Tipo de optimización:"));

	// SpinBox para seleccionar la dispersion inicial del enjambre
	dispInicialSpin = new QSpinBox;
	// Setear extremos del intervalo de seleccion (0..50)
	dispInicialSpin->setMinimum(0);
	dispInicialSpin->setMaximum(50);
	// Setear valor predeterminado
	dispInicialSpin->setValue(5);
	// Etiqueta para el SpinBox
	QLabel *dispInicialLabel = new QLabel(tr("Dispersión inicial:"));

	connect(dispInicialSpin, SIGNAL(valueChanged(int)), this,
			SLOT(dispChanged(int)));

	// SpinBox para seleccionar la velocidad maxima de las particulas
	limitVelSpin = new QSpinBox;
	// Setear extremos del intervalo de seleccion (0..200)
	limitVelSpin->setMinimum(0);
	limitVelSpin->setMaximum(100);
	// Etiqueta para el SpinBox
	limitVelLabel = new QLabel(tr("Limitar velocidad:"));
	// Desactivar (por defecto)
	limitVelSpin->setEnabled(false);
	limitVelLabel->setEnabled(false);
	// CheckBox para seleccionar este item
	QCheckBox *limitarVelChck = new QCheckBox;
	// Deseleccionar (por defecto)
	limitarVelChck->setChecked(false);

	connect(limitVelSpin, SIGNAL(valueChanged(int)), this,
			SLOT(limitarVelCanged(int)));
	connect(limitarVelChck, SIGNAL(toggled(bool)), this,
			SLOT(limitarVelChecked(bool)));

    // Mostrar mensaje por consola
    cout << "Area opciones de optimizacion creada correctamente" << endl;

	/// CRITERIOS DE TERMINACION ///

	// GroupBox con los criterios de terminacion
	critTermGroup = new QGroupBox(tr("Criterio de terminación"));

	// SpinBox para seleccionar la cantidad de iteraciones
	ctIterSpin = new QSpinBox;
	// Setear extremos del intervalo de seleccion (1..1000)
	ctIterSpin->setMinimum(1);
	ctIterSpin->setMaximum(1000);
	// Setear valor predeterminado
	ctIterSpin->setValue(15);
	// Etiqueta para el SpinBox
	ctIterLabel = new QLabel(tr("Iteraciones a realizar:"));
	// CheckBox para seleccionar este item
	ctIterChck = new QCheckBox;
	// Seleccionar (por defecto)
	ctIterChck->setChecked(true);

	// DoubleSpinBox para seleccionar la dispersion de finalizacion
	ctDispSpin = new QDoubleSpinBox;
	// Setear a 4 decimales
	ctDispSpin->setDecimals(4);
	// Setear extremos del intervalo de seleccion (0..10)
	ctDispSpin->setMinimum(0.0);
	ctDispSpin->setMaximum(10.0);
	// Setear valor predeterminado
	// Etiqueta para el DoubleSpinBox
	ctDispLabel = new QLabel(tr("Dispersión del enjambre:"));
	// Desactivar (por defecto)
	ctDispSpin->setEnabled(false);
	ctDispLabel->setEnabled(false);
	// CheckBox para seleccionar este item
	ctDispChck = new QCheckBox;
	// Deseleccionar (por defecto)
	ctDispChck->setChecked(false);

	// DoubleSpinBox para seleccionar el error estimado de finalizacion
	ctEaSpin = new QDoubleSpinBox;
	// Setear a 4 decimales
	ctEaSpin->setDecimals(4);
	// Setear extremos del intervalo de seleccion (0..10)
	ctEaSpin->setMinimum(0.0);
	ctEaSpin->setMaximum(10.0);
	// Etiqueta para el DoubleSpinBox
	ctEaLabel = new QLabel(tr("Error estimado relativo:"));
	// Desactivar (por defecto)
	ctEaSpin->setEnabled(false);
	ctEaLabel->setEnabled(false);
	// CheckBox para seleccionar este item
	ctEaChck = new QCheckBox;
	// Deseleccionar (por defecto)
	ctEaChck->setChecked(false);

	// SpinBox para seleccionar el tiempo maximo de ejecucion
	ctTiempoSpin = new QSpinBox;
	// Agregar sufijo "segs"
	ctTiempoSpin->setSuffix(" segs");
	// Setear extremos del intervalo de seleccion (0..300)
	ctTiempoSpin->setMinimum(0);
	ctTiempoSpin->setMaximum(300);
	// Etiqueta para el SpinBox
	ctTiempoLabel = new QLabel(tr("Tiempo de ejecución:"));
	// Desactivar (por defecto)
	ctTiempoSpin->setEnabled(false);
	ctTiempoLabel->setEnabled(false);
	// CheckBox para seleccionar este item
	ctTiempoChck = new QCheckBox;
	// Deseleccionar (por defecto)
	ctTiempoChck->setChecked(false);

	// CheckBoxes: SGNALS -> SLOTS
	connect(ctIterChck, SIGNAL(toggled(bool)),
			this, SLOT(iterChckChanged(bool)));
	connect(ctDispChck, SIGNAL(toggled(bool)),
			this, SLOT(dispChckChanged(bool)));
	connect(ctEaChck, SIGNAL(toggled(bool)),
			this, SLOT(eaChckChanged(bool)));
	connect(ctTiempoChck, SIGNAL(toggled(bool)),
			this, SLOT(tiempoChckChanged(bool)));

	// Boton para optimizacion rapida
	QPushButton *btnOptimizar = new QPushButton(tr(" Optimización rápida"));
	btnOptimizar->setIcon(QIcon("Play.png"));

	// SIGNAL -> SLOT
	connect(btnOptimizar, SIGNAL(clicked()), this, SLOT(btnOptimizarClicked()));


	// Widgets -> GridLayouts -> GroupBoxes
	QGridLayout *critTermLayout = new QGridLayout;
	critTermLayout->addWidget(ctIterChck,0,0);
	critTermLayout->addWidget(ctIterLabel,0,1);
	critTermLayout->addWidget(ctIterSpin,0,2);
	critTermLayout->addWidget(ctDispChck,1,0);
	critTermLayout->addWidget(ctDispLabel,1,1);
	critTermLayout->addWidget(ctDispSpin,1,2);
	critTermLayout->addWidget(ctEaChck,2,0);
	critTermLayout->addWidget(ctEaLabel,2,1);
	critTermLayout->addWidget(ctEaSpin,2,2);
	critTermLayout->addWidget(ctTiempoChck,3,0);
	critTermLayout->addWidget(ctTiempoLabel,3,1);
	critTermLayout->addWidget(ctTiempoSpin,3,2);
	critTermLayout->addWidget(btnOptimizar,4,1,1,2);
	critTermGroup->setLayout(critTermLayout);

    // Mostrar mensaje por consola
    cout << "Area criterios de terminacion creada correctamente" << endl;

	/// TABLAS DE POSICION INICIAL Y LIMITES ///

	// Tabla de posicion inicial
	tablaPosInicial = new QTableWidget(1,0);

	// Cabeceras de la tabla
	vHeaders.clear();

	vHeaders << tr("Posición: ");

	tablaPosInicial->setVerticalHeaderLabels(vHeaders);
	tablaPosInicial->setMaximumHeight(2*tablaPosInicial->rowHeight(0));

	tablaPosInicial->setSelectionMode(QAbstractItemView::NoSelection);

	// Tabla de limites
	tablaLimites = new QTableWidget(2,1);

	// Cabeceras de la tabla
	hHeaders.clear();
	vHeaders.clear();

	hHeaders << "";
	vHeaders << "Desde: ";
	vHeaders << "Hasta: ";

	tablaLimites->setHorizontalHeaderLabels(hHeaders);
	tablaLimites->setVerticalHeaderLabels(vHeaders);

	// CheckBoxes para activar-desactivar los limites
	limiteInfChck = new QCheckBox;
	limiteSupChck = new QCheckBox;

	// CheckBoxes: SIGNALS -> SLOTS
	connect(limiteInfChck, SIGNAL(clicked(bool)), this,
			SLOT(limiteEspacioChecked(bool)));
	connect(limiteSupChck, SIGNAL(clicked(bool)), this,
			SLOT(limiteEspacioChecked(bool)));

	tablaLimites->setCellWidget(0,0,limiteInfChck);
	tablaLimites->setCellWidget(1,0,limiteSupChck);

	tablaLimites->setMaximumHeight(3*tablaLimites->rowHeight(0));
	tablaLimites->resizeColumnToContents(0);

	tablaLimites->setSelectionMode(QAbstractItemView::NoSelection);

	connect(tablaPosInicial, SIGNAL(cellChanged(int,int)), this,
			SLOT(celdaPosChanged(int,int)));
	connect(tablaLimites, SIGNAL(cellChanged(int,int)), this,
			SLOT(celdaLimitesChanged(int,int)));

    // Mostrar mensaje por consola
    cout << "Tablas de posicion inicial y limites creada correctamente" << endl;

	/// VENTANA ///

	// Distribucion: Widgets -> GridLayouts -> GroupBoxes
	QGridLayout *enjambreLayout = new QGridLayout;
	enjambreLayout->addWidget(optimizacionLabel,0,1);
	enjambreLayout->addWidget(optimizacionCombo,0,2);
	enjambreLayout->addWidget(dispInicialLabel,1,1);
	enjambreLayout->addWidget(dispInicialSpin,1,2);
	enjambreLayout->addWidget(limitarVelChck,2,0);
	enjambreLayout->addWidget(limitVelLabel,2,1);
	enjambreLayout->addWidget(limitVelSpin,2,2);

	QGridLayout *parametrosLayout = new QGridLayout;
	parametrosLayout->addWidget(tablaParametros,0,1,3,1);
	parametrosLayout->addLayout(enjambreLayout,0,0);
	QGroupBox *parametrosGroup = new QGroupBox("Parámetros");
	parametrosGroup->setLayout(parametrosLayout);

	QHBoxLayout *tablaPosInicialLayout = new QHBoxLayout;
	tablaPosInicialLayout->addWidget(tablaPosInicial);
	QGroupBox *posInicialGroup = new QGroupBox("Posición Inicial");
	posInicialGroup->setLayout(tablaPosInicialLayout);

	QHBoxLayout *tablaLimitesLayout = new QHBoxLayout;
	tablaLimitesLayout->addWidget(tablaLimites);
	QGroupBox *limitesGroup = new QGroupBox("Limites del espacio de búsqueda");
	limitesGroup->setLayout(tablaLimitesLayout);

	// Barra de progreso para el proceso de optimizacion (inicialmente
	// invisible)
	barraDeProgreso = new QProgressBar;
	barraDeProgreso->setVisible(false);
	barraDeProgreso->setRange(0,100);
	barraDeProgreso->setMaximumHeight(15);

	QGridLayout *layout = new QGridLayout;
	layout->addWidget(parametrosGroup,0,0,1,2);
	layout->addWidget(posInicialGroup,1,0);
	layout->addWidget(limitesGroup,2,0);
	layout->addWidget(critTermGroup,1,1,2,1);
	layout->addWidget(barraDeProgreso,3,0,1,2);

	setLayout(layout);

	// Mostrar mensaje por consola
    cout << "Pestania de parametros creada correctamente" << endl;
}

ParametrosWidget::~ParametrosWidget()
{
	delete nVPGSpin;
	delete nVGSpin;
	delete nGSpin;
	delete dispInicialSpin;
	delete limitVelSpin;
	delete gSpin[0], delete gSpin[1], delete gSpin[2];
	delete vSpin[0], delete gSpin[1];
	delete pSpin;
	delete optimizacionCombo;
	delete tablaPosInicial;
	delete tablaLimites;
}

void ParametrosWidget::setEnjambre(Enjambre *_enjambre)
{
	// Actualizar punteros y cantidad de columnas en las tablas

	enjambre = _enjambre;

	int dim = enjambre->getDimension();

	if( dim != dims )
	{
		// Resetear dimensiones de los vectores
		posInicial->setDimension(dim);
		limiteInf->setDimension(dim);
		limiteSup->setDimension(dim);

		*posInicial = 0;
		*limiteInf = -__DBL_MAX__;
		*limiteSup = __DBL_MAX__;

		QStringList hHeaders;
		QString temp;

		// Remover todas las columnas
		for( int i = 0; i < dims; i++ )
		{
			tablaPosInicial->removeColumn(0);
			tablaLimites->removeColumn(0);
		}

		// Agregar nuevas
		for(int i = 0; i < dim; i++)
		{
			temp.setNum(i+1);
			hHeaders << "X"+temp;

			tablaPosInicial->insertColumn(0);
			QTableWidgetItem *celda = new QTableWidgetItem("0");
			tablaPosInicial->setItem(0,0,celda);

			tablaLimites->insertColumn(0);
			celda = new QTableWidgetItem("-Inf");
			tablaLimites->setItem(0,0,celda);
			celda = new QTableWidgetItem("Inf");
			tablaLimites->setItem(1,0,celda);
		}

		tablaPosInicial->setHorizontalHeaderLabels(hHeaders);
		hHeaders << "";
		tablaLimites->setHorizontalHeaderLabels(hHeaders);

		dims = dim;
	}
}

void ParametrosWidget::tipoOptChanged( int tipo )
{
	enjambre->setTipoDeOpt(tipo == 1);
	emit variableChanged();
}

void ParametrosWidget::dispChanged( int disp )
{
	enjambre->setDispersionInicial(disp);
	emit variableChanged();
}

void ParametrosWidget::limitarVelChecked( bool checked )
{
	enjambre->setLimiteVelocidad(checked);
	limitVelSpin->setEnabled(checked);
	limitVelLabel->setEnabled(checked);
	emit variableChanged();
}

void ParametrosWidget::limitarVelCanged( int valor )
{
	enjambre->setVelocidadMaxima(valor);
	emit variableChanged();
}

void ParametrosWidget::poblacionVPGChanged()
{
	int valor = nVPGSpin->value();

	enjambre->setNroParticulas(Enjambre::VPG,valor);

	emit tamanioEnjambreChanged();
}

void ParametrosWidget::poblacionVGChanged()
{
	int valor = nVGSpin->value();

	enjambre->setNroParticulas(Enjambre::VG,valor);

	emit tamanioEnjambreChanged();
}

void ParametrosWidget::poblacionGChanged()
{
	int valor = nGSpin->value();

	enjambre->setNroParticulas(Enjambre::G,valor);

	emit tamanioEnjambreChanged();
}

void ParametrosWidget::parametroG0Changed()
{
	double valor = gSpin[0]->value();
	enjambre->setFactorG(Enjambre::VPG, valor);
	emit variableChanged();
}

void ParametrosWidget::parametroG1Changed()
{
	double valor = gSpin[1]->value();
	enjambre->setFactorG(Enjambre::VG, valor);
	emit variableChanged();
}

void ParametrosWidget::parametroG2Changed()
{
	double valor = gSpin[2]->value();
	enjambre->setFactorG(Enjambre::G, valor);
	emit variableChanged();
}

void ParametrosWidget::inercia0Changed()
{
	double valor = vSpin[0]->value();
	enjambre->setInercia(Enjambre::VPG, valor);
	emit variableChanged();

}

void ParametrosWidget::inercia1Changed()
{
	double valor = vSpin[0]->value();
	enjambre->setInercia(Enjambre::VG, valor);
	emit variableChanged();
}

void ParametrosWidget::parametroPChanged()
{
	double valor = pSpin->value();
	enjambre->setFactorP(valor);
	emit variableChanged();
}

void ParametrosWidget::limiteEspacioChecked( bool checked )
{
	enjambre->setLimitesEspacio(checked);
	limiteInfChck->setChecked(checked);
	limiteSupChck->setChecked(checked);
	if( checked )
		tablaLimites->setSelectionMode(QAbstractItemView::SingleSelection);
	else
		tablaLimites->setSelectionMode(QAbstractItemView::NoSelection);
	emit variableChanged();
}

void ParametrosWidget::pintarGrupoCriterios()
// Cuando los criterios de terminacion seleccionados no son correctos, mostrar
// advertencia pintando el groupBox en color rojo
{
	QGraphicsColorizeEffect *color = new QGraphicsColorizeEffect;
	color->setColor(QColor(Qt::red));
	critTermGroup->setGraphicsEffect(color);
}

void ParametrosWidget::iterChckChanged(bool check)
// SLOT: Activar/Desactivar seleccion de este criterio de terminacion
{
	// Habilitar criterio de terminacion
	criterioTerm[0] = check;
	ctIterSpin->setEnabled(check);
	ctIterLabel->setEnabled(check);
	// Deben estar habilitadas las casillas 1 o 4
	if( criterioTerm[0] || criterioTerm[3] )
		critTermGroup->setGraphicsEffect(NULL);
	else
		pintarGrupoCriterios();
}

void ParametrosWidget::dispChckChanged(bool check)
// SLOT: Activar/Desactivar seleccion de este criterio de terminacion
{
	// Habilitar criterio de terminacion
	criterioTerm[1] = check;
	ctDispSpin->setEnabled(check);
	ctDispLabel->setEnabled(check);
	// Deben estar habilitadas las casillas 1 o 4
	if( criterioTerm[0] || criterioTerm[3] )
		critTermGroup->setGraphicsEffect(NULL);
	else
		pintarGrupoCriterios();
}

void ParametrosWidget::eaChckChanged(bool check)
// SLOT: Activar/Desactivar seleccion de este criterio de terminacion
{
	// Habilitar criterio de terminacion
	criterioTerm[2] = check;
	ctEaSpin->setEnabled(check);
	ctEaLabel->setEnabled(check);
	// Deben estar habilitadas las casillas 1 o 4
	if( criterioTerm[0] || criterioTerm[3] )
		critTermGroup->setGraphicsEffect(NULL);
	else
		pintarGrupoCriterios();
}

void ParametrosWidget::tiempoChckChanged(bool check)
// SLOT: Activar/Desactivar seleccion de este criterio de terminacion
{
	// Habilitar criterio de terminacion
	criterioTerm[3] = check;
	ctTiempoSpin->setEnabled(check);
	ctTiempoLabel->setEnabled(check);
	// Deben estar habilitadas las casillas 1 o 4
	if( criterioTerm[0] || criterioTerm[3] )
		critTermGroup->setGraphicsEffect(NULL);
	else
		pintarGrupoCriterios();
}

void ParametrosWidget::btnOptimizarClicked()
{
	// Los criterios de terminacion deben garantizar que el metodo se detenga
	if( !criterioTerm[0] && !criterioTerm[3] )
		return;

	// SIGNAL
	emit optimizacionIniciada();

	// Reiniciar el enjambre antes de comenzar la optimizacion
	enjambre->reiniciar();

	// Activar la barra de progreso
	barraDeProgreso->setVisible(true);

	// Criterios de terminacion
	int iterMax = ctIterSpin->value();
	double dispMin = ctDispSpin->value();
	double eaMin = ctEaSpin->value();
	uint tiempoMax = 1000*ctTiempoSpin->value();

	// Porcentajes de progreso
	double progresoIter, progresoTiempo;

	// Iniciar cronometro para medir el tiempo de ejecucion
	QElapsedTimer *timer = new QElapsedTimer;
	timer->start();
	// Iterar hasta que se cumpla el criterio de terminacion
	forever
	{
		// Realizar una iteracion
		enjambre->iterar();

		// Criterios de terminacion. Usar doble "if" para evitar
		// llamar a los "getters" en cada iteracion

		// Criterio de terminacion 1: Numero de iteraciones
		if( criterioTerm[0] )
		{
			if( enjambre->getNroIteracion() >= iterMax )
				break;
			progresoIter = ((double) enjambre->getNroIteracion() - 1) /
					((double) iterMax - 1);
		}
		// Criterio de terminacion 2: Dispersion alcanzada
		if( criterioTerm[1] )
			if( enjambre->calcularDispersion() < dispMin )
				break;
		// Criterio de terminacion 3: Error estimado alcanzado
		if( criterioTerm[2] )
			if( enjambre->getError() < eaMin )
				break;
		// Criterio de terminacion 4: Tiempo de ejecucion superado
		if( criterioTerm[3] )
		{
			if( timer->elapsed() > tiempoMax )
				break;
			progresoTiempo = (double) timer->elapsed() /
					(double) tiempoMax;
		}

		// Mostrar porcentaje de progreso en la barra
		barraDeProgreso->setValue( qMax(progresoTiempo,progresoIter) * 100 );
	}

	// Obtener el tiempo de ejecucion
	uint tiempoEjec = timer->elapsed();
	delete timer;

	// Ocultar barra de progreso al finalizar la optimizacion
	barraDeProgreso->setVisible(false);

	// SIGNAL
	emit optimizacionTerminada(tiempoEjec);
}

void ParametrosWidget::celdaPosChanged(int fila, int columna)
{
	// String para obtener el dato de la celda
	QString texto;
	// Valor del dato
	double valor;
	// Extraer el texto
	texto = tablaPosInicial->item(fila, columna)->text();
	// Convertir a double
	valor = texto.toDouble();
	// Setear la componente del vector
	posInicial->setComponente(columna,valor);
	enjambre->setPosicionInicial(posInicial);
}

void ParametrosWidget::celdaLimitesChanged(int fila, int columna)
{
	// String para obtener el dato de la celda
	QString texto;
	// Valor del dato
	double valor;
	// Extraer el texto
	texto = tablaLimites->item(fila, columna)->text();
	// Convertir a double
	valor = texto.toDouble();

	// Setear la componente del vector correspondiente y agregar al enjambre
	if(fila == 0)
	{
		limiteInf->setComponente(columna,valor);
		enjambre->setLimiteInf(limiteInf);
	}
	else
	{
		limiteSup->setComponente(columna,valor);
		enjambre->setLimiteSup(limiteSup);
	}
}

// EOF
