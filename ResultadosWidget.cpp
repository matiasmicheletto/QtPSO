#include "ResultadosWidget.h"

ResultadosWidget::ResultadosWidget(QWidget *parent) : QWidget(parent)
{
    // Mostrar mensaje por consola
    cout << "Creando la pestania de resultados..." << endl;

	// Etiquetas
	QLabel *resultadosLabel = new QLabel("Resultados de la optimziación");
	QLabel *parametrosLabel = new QLabel("Parámetros seleccionados");
	QFont fuente("",11);
	fuente.setBold(true);
	resultadosLabel->setFont(fuente);
	resultadosLabel->setMaximumHeight(12);
	parametrosLabel->setFont(fuente);
	parametrosLabel->setMaximumHeight(12);

	// Terminales de salida
	termResultados = new Terminal;
	termParametros = new Terminal;

	// Campo de texto para ingresar directorio de archivo
	campoTexto = new QLineEdit;
	// Fuente: cursiva
	campoTexto->setFont(QFont("",-1,-1,true));
	// Agregar el directorio actual
	campoTexto->setText(QDir::currentPath());
	// Ocultar
	campoTexto->setVisible(false);
	// Ok para guardar
	guardar = false;

	// Boton de guardar
	QPushButton *botonGuardar = new QPushButton(tr("  Guardar"));

	// Agregar icono al boton
	botonGuardar->setIcon(QIcon("Guardar.png"));
	botonGuardar->setIconSize(QSize(20,20));

	// Boton: SIGNAL -> SLOT
	connect(botonGuardar, SIGNAL(clicked()), this, SLOT(btnGuardarClicked()));

	// Distribucion de los widgets
	QGridLayout *layout = new QGridLayout;
	layout->addWidget(resultadosLabel,0,0);
	layout->addWidget(parametrosLabel,0,1);
	layout->addWidget(termResultados,1,0);
	layout->addWidget(termParametros,1,1,1,3);
	layout->addWidget(campoTexto,2,0,1,3);
	layout->addWidget(botonGuardar,2,3);

	// La ventana se habilita al cargar resultados
	setEnabled(false);

	setLayout(layout);

    // Mostrar mensaje por consola
    cout << "Pestania de resultados creada correctamente" << endl;
}

void ResultadosWidget::btnGuardarClicked()
// SLOT: Guardar los resultados en formato texto
{
	if( !guardar )
	{
		// Aun no se indico el directorio del archivo
		campoTexto->setVisible(true);
		campoTexto->selectAll();
		campoTexto->setFocus();
		// Habilitar la escritura del archivo
		guardar = true;
	}
	else
	{
		// Crear un archivo de texto en el directorio indicado
		QFile archivo(campoTexto->text()+"/ResultadosPSO.txt");
		// Crear si no existe y abrir
		archivo.open(QIODevice::Append | QIODevice::Text);
		// Interfaz para lectura-escritura de texto
		QTextStream salida(&archivo);

		// Obtener fecha y hora del sistema
		QString sTimeInfo;
		time_t rawtime;
		struct tm * timeinfo;
                time( &rawtime );
		timeinfo = localtime ( &rawtime );
		// sTimeInfo contiene la fecha y la hora
		sTimeInfo = asctime(timeinfo);

		// Enviar todos los datos al archivo de texto
		salida << sTimeInfo << "\n";
		for( int i = 0; i < listaResultados.size(); i++ )
			salida << listaResultados.at(i) << endl;
		salida << "\n\n";

		// Cerrar archivo
		archivo.close();

		guardar = false;
		campoTexto->setVisible(false);
	}
}

void ResultadosWidget::mostrarResultados(QStringList resultados)
{
	// Habilitar ventana
	setEnabled(true);

	// Agregar resultados a la lista para guardar
	listaResultados = resultados;

	QFont negrita("",13);
	negrita.setBold(true);

	QString temp;
	// Mostrar los resultados en el terminal
	for( int i = 0; i < resultados.size(); i++ )
	{
		temp = resultados.at(i);
		if( temp.contains("óptimo") || temp.contains("objetivo") )
			termResultados->output( temp, negrita );
		else
			termResultados->output( temp, QFont("",13) );
	}

}

void ResultadosWidget::mostrarParametros(QStringList parametros)
{
	// Agregar resultados a la lista para imprimir
	listaResultados += parametros;

	QFont negrita("",13);
	negrita.setBold(true);

	QString temp;
	// Mostrar resultados en el terminal
	for( int i = 0; i < parametros.size(); i++ )
	{
		temp = parametros.at(i);
		if( temp.contains("Tipo") )
			termParametros->output( temp, negrita );
		else
			termParametros->output( temp, QFont("",13) );
	}
}

void ResultadosWidget::desactivar()
{
	// Borrar terminales y desactivar ventana
	termResultados->limpiar();
	termParametros->limpiar();
	setEnabled(false);
}

// EOF
