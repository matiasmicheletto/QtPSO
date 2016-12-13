#include "VentanaError.h"

VentanaError::VentanaError(QStringList mensaje, QWidget *parent) :
	QWidget(parent)
{
	// Disposicion de los widgets: vertical
	QVBoxLayout *layout = new QVBoxLayout;

	// Agregar linea por linea del mensaje a la ventana
	for( int i = 0; i < mensaje.size(); i++ )
	{
		// Nueva etiqueta
		QLabel *mensajeLabel = new QLabel(mensaje.at(i));
		// Setear fuente
		mensajeLabel->setFont(QFont("",12,60));
		// Alineacion: centrado
		mensajeLabel->setAlignment(Qt::AlignHCenter);
		// Agregar al layout
		layout->addWidget(mensajeLabel);
	}
    cout << endl;

	// Boton aceptar
	QPushButton *botonOk = new QPushButton(tr("Aceptar"));
	// Boton: SIGNAL -> SLOT
	connect(botonOk,SIGNAL(clicked()),this,SLOT(btnOkClicked()));
	// Asignar tecla de atajo (tecla Enter)
	botonOk->setShortcut(QKeySequence::InsertParagraphSeparator);

	// Agregar el boton a la ventana
	layout->addWidget(botonOk);

	setLayout(layout);
	setWindowTitle("Error!");
	setWindowIcon(QIcon("Error.png"));

}

void VentanaError::btnOkClicked()
{
	emit close();
}

// EOF
