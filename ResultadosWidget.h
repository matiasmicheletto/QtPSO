#ifndef RESULTADOSWIDGET_H
#define RESULTADOSWIDGET_H

#include <QGridLayout>
#include <QPushButton>
#include <QTextStream>
#include <QStringList>
#include <QLineEdit>
#include <QWidget>
#include <QLabel>
#include <QFile>
#include <QDir>
#include <ctime>

#include "Terminal.h"

class ResultadosWidget : public QWidget
{
	Q_OBJECT
public:
	ResultadosWidget(QWidget *parent = 0);
	void mostrarResultados(QStringList resultados);
	void mostrarParametros(QStringList parametros);
	void desactivar();

private slots:
	void btnGuardarClicked();

private:
	QLineEdit *campoTexto;
	QStringList listaResultados;
	bool guardar;

	Terminal *termResultados, *termParametros;
};

#endif // RESULTADOSWIDGET_H
