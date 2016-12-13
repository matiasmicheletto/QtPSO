#include "VentanaPrincipal.h"

VentanaPrincipal::VentanaPrincipal(QWidget *parent) : QMainWindow(parent)
{
    // Mostrar mensaje por consola
    cout << "Creando la ventana principal..." << endl;

	funcion = 0;
	enjambre = 0;

	// Barra de herramientas

	// Campo para ingresar la funcion
	campoFuncionLine = new QLineEdit;
	campoFuncionLine->setFont(QFont("",12));

	// Autocompletamiento de palabras
    comandos << "&Ackley(N,20,0.2,6.283185307)"
             << "&BuscaMaximos(N,5,50)"
             << "&Easom(N)"
             << "&Metaoptimizar(F,Xo_1,Xo_2,...,Xo_n)"
             << "&Rosenbrock(N,100)";

	QCompleter *completador = new QCompleter(comandos,this);
	campoFuncionLine->setCompleter(completador);

	connect(campoFuncionLine, SIGNAL(textChanged(QString)), this,
			SLOT(funcionChanged()));

	// Etiqueta del campo de texto
	QLabel *funcionLabel = new QLabel(tr("F(X1, X2, ..., Xn) = "));
	funcionLabel->setFont(QFont("",-1,-1,true));

	// Boton para actualizar la funcion
	btnCargarFuncion = new QPushButton(QIcon("RightArrow.png"),"");
	// Atajo por teclado (Tecla Enter)
	btnCargarFuncion->setShortcut(QKeySequence::InsertParagraphSeparator);
	// SIGNAL -> SLOT
	connect(btnCargarFuncion,SIGNAL(clicked()),this,SLOT(btnFcClicked()));

	// Layout para el campo de texto y la etiquta
	QHBoxLayout *funcionLayout = new QHBoxLayout;
	funcionLayout->addWidget(funcionLabel);
	funcionLayout->addWidget(campoFuncionLine);
	funcionLayout->addWidget(btnCargarFuncion);

	// Grupo que encienrra el contenido de la barra de herramientas
	QGroupBox *grupoFuncion = new QGroupBox(tr("Función Objetivo"));
	grupoFuncion->setLayout(funcionLayout);

	// Barra de herramientas
	QToolBar *bar = new QToolBar(tr("Función Objetivo"));
	bar->addWidget(grupoFuncion);

	// Agregar barra
	addToolBar(bar);

    // Mostrar mensaje por consola
    cout << "Barra de herramientas creada correctamente." << endl;

    // Mostrar mensaje por consola
    cout << "Agregando widgets a las pestanias..." << endl;

	// Construir los widgets principales de la ventana
	animacion = new AnimacionWidget(this);
	parametros = new ParametrosWidget(this);
	resultados = new ResultadosWidget(this);

	// Widgets: SIGNALS -> SLOTS
	connect(animacion, SIGNAL(optimizacionTerminada( uint )), this,
			SLOT(mostrarResultados(uint)));

	connect(parametros, SIGNAL(tamanioEnjambreChanged()), this,
			SLOT(tamanioEnjambreChanged()));
	connect(parametros,SIGNAL(variableChanged()), this,
			SLOT(parametroChanged()));
	connect(parametros, SIGNAL(optimizacionIniciada()), this,
			SLOT(parametroChanged()));
	connect(parametros,SIGNAL(optimizacionTerminada(uint)), this,
			SLOT(mostrarResultados(uint)));

	// Widgets distribuidos en penstanias
	tabs = new QTabWidget();
	tabs->addTab(animacion,QIcon("Graficar.png"),tr("Animación"));
	tabs->addTab(parametros,QIcon("Configuracion.png"),tr("Configuración"));
	tabs->addTab(resultados,QIcon("Tabla.png"),tr("Resultados"));

	// Indicar widget central de la ventana principal
	setCentralWidget(tabs);

	// Inicialmente widgets desactivados porque no existe funcion
	tabs->setEnabled(false);

	// Icono de la aplicacion
	setWindowIcon(QIcon("PSO.png"));

	// Titulo de la aplicacion
	setWindowTitle("QtPSO");

    // Mostrar ventana principal
    cout << "Ventana principal creada correctamente." << endl;
}

VentanaPrincipal::~VentanaPrincipal()
// Destructor de la clase
{
	delete campoFuncionLine;
	delete btnCargarFuncion;
	delete animacion;
	delete parametros;
}

Vector VentanaPrincipal::str2vector(QString qstr)
// Convertir de QString a vector
{
    // Vector unidimensional
    Vector vector(1);
    // Valor de cada componente
    double valor;
    // String temporal
    QString temp("");

    // Indices de caracter y de componente
    int i = 1;
    int j = 1;
    // Hasta alcanzar el final del string, hacer:
    while( i < qstr.size() )
    {
        // Hasta alcanzar una coma "," o un parentesis derecho ")", hacer:
        while( qstr[i] != ',' && qstr[i] != ')' )
        {
            // Agregar caracter al string temporal
            temp.append(qstr[i]);
            // Siguiente caracter
            i++;
        }
        // Convertir string a double
        valor = temp.toDouble();
        // Borrar string temporal
        temp.clear();
        // En caso de que haya agregar otra dimension al vector
        if( vector.getDimension() < j )
            vector.setDimension(j);
        // Setear nueva componente
        vector.setComponente(j-1,valor);
        // Pasar al siguiente caracter y siguiente componente
        j++;
        i++;
    }
    // Devolver vector con todos los valores
    return vector;
}

Funcion* VentanaPrincipal::interpretarFc(QString fc)
// Identificar el tipo de funcion y leer parametros
{
    // Quitar operador
    fc.remove("&");
    // Funcion a retornar
    Funcion *func;
    // Vector para cargar paramentros
    Vector param(1);
    // Calsificar
    if(fc.contains("Ackley"))
    {
        // Quitar nombre de la funcion
        fc.remove("Ackley");
        // Obtener parametros
        param = str2vector(fc);
        // Crear funcion
        func = new Ackley(param[0],false,param[1],param[2],param[3]);
    }
    else if(fc.contains("BuscaMaximos"))
    {
        fc.remove("BuscaMaximos");
        param = str2vector(fc);
        func = new BuscaMaximos(param[0],false,param[1],param[2]);
    }
    else if(fc.contains("Easom"))
    {
        fc.remove("Easom");
        param = str2vector(fc);
        func = new Easom(param[0]);
    }
    else if(fc.contains("Metaoptimizar"))
    {
        fc.remove("Metaoptimizar");
        // Obtener expresion de la funcion a metaoptimizar
        QString temp("");
        while(fc[1] != ',')
        {
            temp.append(fc[1]);
            fc.remove(1,1);
        }
        // Quitar la coma
        fc.remove(1,1);
        param = str2vector(fc);
        func = new Optimizador(temp,&param);

        // Mostrar pestaña de parametros
        tabs->setCurrentIndex(1);
    }
    else if(fc.contains("Rosenbrock"))
    {
        fc.remove("Rosenbrock");
        param = str2vector(fc);
        func = new Rosenbrock(param[0],false,param[1]);
    }
    else return 0;

    return func;
}

void VentanaPrincipal::funcionChanged()
// SLOT
{
	QGraphicsColorizeEffect *color = new QGraphicsColorizeEffect;
	color->setColor(QColor(Qt::red));
	campoFuncionLine->setGraphicsEffect(color);
}

void VentanaPrincipal::tamanioEnjambreChanged()
// SLOT
{
	animacion->pausar();
	animacion->actualizarGraficoEnjambre();
}

void VentanaPrincipal::parametroChanged()
// SLOT
{
	animacion->pausar();
}

void VentanaPrincipal::btnFcClicked()
// SLOT
{
	animacion->pausar();
	QString input = campoFuncionLine->text();

	if( input != "" ) // Si el campo de texto no esta vacio
	{
		// Agregar palabra para autocompletamiento
		if( !comandos.contains(input) )
		{
			comandos << input;
			QCompleter *completador = new QCompleter(comandos,this);
			campoFuncionLine->setCompleter(completador);
		}

		// Construir nueva funcion
		if( funcion != 0)
			delete funcion;

		// Sustituir constantes
		while( input.contains("PI") )
			input.replace("PI","3.141592");

		// Funciones numericas
        if( input.startsWith("&") )
		{
			funcion = interpretarFc(input);

			QGraphicsColorizeEffect *color = new QGraphicsColorizeEffect;
			color->setColor(QColor(Qt::darkBlue));
			campoFuncionLine->setGraphicsEffect(color);
		}
		else
		{
			// Funciones simbolicas
			funcion = new FunMatematica(input);
			campoFuncionLine->setGraphicsEffect(NULL);
		}

		// Construir enjambre (solo la primera vez)
		if( enjambre == 0 )
			enjambre = new Enjambre(funcion);
		else
			enjambre->setFuncion(funcion);

		animacion->setEnjambre(enjambre);

		parametros->setEnjambre(enjambre);

		tabs->setEnabled(true);

        // Mostrar mensaje por consola
        cout << "Nueva funcion ingresada." << endl;
        cout << "Expresion: " << qPrintable(funcion->getExpresion())
             << endl;
	}
	else
	{
        QStringList mensaje;
		mensaje << tr("No se ha ingresado") << tr("ninguna función");
		VentanaError *error = new VentanaError(mensaje);
		error->show();
		error->move(500,300);
		tabs->setEnabled(false);
        qWarning("Se debe ingresar una expresion en el campo de texto.");
		campoFuncionLine->setGraphicsEffect(NULL);
	}
}

void VentanaPrincipal::mostrarResultados( uint tiempoEjec )
// SLOT: resultados de la optimizacion
{
    // Mostrar mensaje por consola
    cout << "Resultados de la optimizacion:" << endl;

	// Borrar resultados (si habia)
	resultados->desactivar();

	int dim = enjambre->getDimension();

	// Crear la lista de strings con todos los resultados de la
	// optimizacion
	QStringList listaResultados;
	QString temp1, temp2;
	Vector posOptima = enjambre->getOptimo();


	// RESULTADOS //

	// Expresion matematica de la funcion
	listaResultados << "Función objetivo:";
	listaResultados << "\t"+funcion->getExpresion();
	listaResultados << "";

	// Posicion del optimo
	listaResultados << "Posición del óptimo:";
	for(int i = 0; i < dim; i++)
	{
		// Componente i-esima -> string
		temp1.setNum(posOptima[i]);
		temp2.setNum(i+1);
		// Agregar a temp
		temp1.prepend("\tX"+temp2+" = ");
		listaResultados << temp1;
	}

	// Valor del optimo
	// Obs. se podria usar el atrib. "maximizar" en lugar de
	// "funcion->esInv()", pero en caso de metaoptimizaciones, estas
	// variables no coinciden
	temp1.setNum( enjambre->getFOptimo() );
	temp1.append( funcion->esInv() ? tr(" (máximo)") : tr(" (mínimo)") );
	temp1.prepend("Valor del óptimo = ");
	listaResultados << temp1;

	listaResultados << "";

	// Iteraciones realizadas
	temp1.setNum(enjambre->getNroIteracion());
	temp1.prepend("Iteraciones realizadas = ");
	listaResultados << temp1;

	// Dispersion del enjambre
	temp1.setNum(enjambre->calcularDispersion());
	temp1.prepend("Dispersión del enjambre = ");
	listaResultados << temp1;

	// Error estimado relativo
	temp1.setNum(enjambre->getError());
	temp1.prepend("Error estimado relativo = ");
	listaResultados << temp1;

	// Tiempo de ejecucion
	temp1.setNum(tiempoEjec);
	temp1.prepend("Tiempo de ejecución = ");
	temp1.append(" ms");
	listaResultados << temp1;

	listaResultados << "";

	resultados->mostrarResultados(listaResultados);
	listaResultados.clear();


	// PARAMETROS //

	// Posicion inicial
	Vector posInicial = enjambre->getPosInicial();
	temp1 = "Posición Inicial = (";
	for(int i = 0; i < dim; i++ )
	{
		temp2.setNum(posInicial[i]);
		if( i != dim-1 )
			temp1.append(temp2+",");
		else
			temp1.append(temp2+")");
	}
	listaResultados << temp1;

	// Dispersion inicial
	temp1.setNum(enjambre->getDispersionInicial());
	temp1.prepend("Dispersión inicial = ");
	listaResultados << temp1;

	listaResultados << "";

	// Tamanio del enjambre
	listaResultados << "Tipo de partícula:\tVPG\tVG\tG";
	temp1 = "Cantidad =\t\t";
	temp2.setNum(enjambre->getCantidadParticulas(Enjambre::VPG));
	temp1.append(temp2+"\t");
	temp2.setNum(enjambre->getCantidadParticulas(Enjambre::VG));
	temp1.append(temp2+"\t");
	temp2.setNum(enjambre->getCantidadParticulas(Enjambre::G));
	temp1.append(temp2);
	listaResultados << temp1;

	// Parametro G
	temp1 = "Parámetro G =\t\t";
	temp2.setNum(enjambre->getFactorG(Enjambre::VPG));
	temp1.append(temp2+"\t");
	temp2.setNum(enjambre->getFactorG(Enjambre::VG));
	temp1.append(temp2+"\t");
	temp2.setNum(enjambre->getFactorG(Enjambre::G));
	temp1.append(temp2);
	listaResultados << temp1;

	// Inercia
	temp1 = "Inercia =   \t\t";
	temp2.setNum(enjambre->getInercia(Enjambre::VPG));
	temp1.append(temp2+"\t");
	temp2.setNum(enjambre->getInercia(Enjambre::VG));
	temp1.append(temp2);
	listaResultados << temp1;

	// Parametro P
	temp1.setNum(enjambre->getFactorP());
	temp1.prepend("Parámetro P =\t\t");
	listaResultados << temp1;

	listaResultados << "";

	// Velocidad maxima
	bool velLimitada, posLimitada;
	enjambre->estaLimitado(posLimitada,velLimitada);

	if( !velLimitada )
		listaResultados << "Velocidad de partículas ilimitada";
	else
	{
		temp1.setNum(enjambre->getVelMax());
		temp1.prepend("Velocidad máxima = ");
		listaResultados << temp1;
	}

	listaResultados << "";

	// Limites del espacio
	if( !posLimitada )
		listaResultados << "Espacio de búsqueda ilimitado";
	else
	{
		listaResultados << "Límites del espacio:";
		temp1 = "Límite inferior = (";
		Vector lim = enjambre->getLimiteInf();
		for(int i = 0; i < dim; i++ )
		{
			temp2.setNum(lim[i]);
			if( i != dim-1 )
				temp1.append(temp2+",");
			else
				temp1.append(temp2+")");
		}
		listaResultados << temp1;

		temp1 = "Límite superior = (";
		lim = enjambre->getLimiteSup();
		for(int i = 0; i < dim; i++ )
		{
			temp2.setNum(lim[i]);
			if( i != dim-1 )
				temp1.append(temp2+",");
			else
				temp1.append(temp2+")");
		}
		listaResultados << temp1;
	}

	// Mostrar resultados
	resultados->mostrarParametros(listaResultados);

	// Pasar a la pestania de resultados
	tabs->setCurrentIndex(2);
}

// EOF
