#ifndef ANIMACIONWIDGET_H
#define ANIMACIONWIDGET_H

#include <QWidget>
#include <QTimer>
#include <QElapsedTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolBar>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QSlider>
#include <QCheckBox>
#include <QLabel>

#include "GraficoEnjambre.h"
#include "GraficoCurva.h"
#include "Enjambre.h"

class AnimacionWidget : public QWidget
{
    Q_OBJECT
public:
	explicit AnimacionWidget( QWidget *parent = 0 );
	void setEnjambre( Enjambre *_enjambre );

signals:
	void optimizacionTerminada( unsigned int );

public slots:
	void pausar();

	void refrescarEnjambre();

	void actualizarGraficoEnjambre();

	void btnPlayClicked();
	void btnResetClicked();
	void btnSalirClicked();	

private slots:
	void sliderVelValueChanged( int valor );
	void sliderEscValueChanged( int valor );

	void escalaChanged( int valor );

	void activarCubo( bool activar );
	void activarInfo( bool activar );

private:
	Enjambre *enjambre;
	QTimer *timer;
	QElapsedTimer *cronometro;
	GraficoEnjambre *graficoEnjambre;
	GraficoCurva *graficoDispersion, *graficoError;
	QLabel *dispLabel, *errorLabel, *iterLabel, *optLabel;
	QPushButton *btnPlay;	
	Vector *centroCubo;
	QSlider *sliderEscala;

	int delay;
	bool play;
	uint tiempoEjec;
	double dispMax, errorMax, disp, error;
};

#endif // ANIMACIONWIDGET_H
