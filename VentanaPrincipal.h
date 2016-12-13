#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include <QMainWindow>
#include <QTabWidget>
#include <QLineEdit>
#include <QLabel>
#include <QGroupBox>
#include <QPushButton>
#include <QCompleter>

#include "AnimacionWidget.h"
#include "ParametrosWidget.h"
#include "ResultadosWidget.h"
#include "VentanaError.h"
#include "FunMatematica.h"
#include "FuncionesNumericas.h"

class VentanaPrincipal : public QMainWindow
{
    Q_OBJECT
public:
    explicit VentanaPrincipal(QWidget *parent = 0);
	~VentanaPrincipal();

private slots:
	void funcionChanged();
	void tamanioEnjambreChanged();
	void parametroChanged();
	void btnFcClicked();
	void mostrarResultados( uint tiempoEjec );

private:
    Vector str2vector(QString qstr);

	QLineEdit *campoFuncionLine;
	QPushButton *btnCargarFuncion;
	AnimacionWidget *animacion;
	ParametrosWidget *parametros;
	ResultadosWidget *resultados;
	QTabWidget *tabs;
	QStringList comandos;

	Funcion *funcion;
	Enjambre *enjambre;

	Funcion* interpretarFc(QString fc);
};

#endif // VENTANAPRINCIPAL_H
