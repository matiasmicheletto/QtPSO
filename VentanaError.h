#ifndef VENTANAERROR_H
#define VENTANAERROR_H

#include <QWidget>
#include <QPushButton>
#include <QStringList>
#include <QLabel>
#include <QVBoxLayout>
#include <iostream>

using namespace std;

class VentanaError : public QWidget
{
	Q_OBJECT

public:
	VentanaError( QStringList mensaje, QWidget *parent = 0 );

private slots:
	void btnOkClicked();
};

#endif // VENTANAERROR_H
