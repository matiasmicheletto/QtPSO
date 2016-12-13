#ifndef PARAMETROSWIDGET_H
#define PARAMETROSWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QGroupBox>
#include <QComboBox>
#include <QCheckBox>
#include <QStringList>
#include <QHeaderView>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QProgressBar>
#include <QElapsedTimer>
#include <QDoubleSpinBox>
#include <QGraphicsColorizeEffect>

#include "Vector.h"
#include "Enjambre.h"

class ParametrosWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ParametrosWidget(QWidget *parent = 0);
	~ParametrosWidget();

	void setEnjambre( Enjambre *_enjambre );

signals:
	void variableChanged();
	void tamanioEnjambreChanged();
	void optimizacionIniciada();
	void optimizacionTerminada(uint);

private slots:
	void tipoOptChanged( int tipo );
	void dispChanged( int disp );
	void limitarVelChecked( bool checked );
	void limitarVelCanged( int valor );

	void poblacionVPGChanged();
	void poblacionVGChanged();
	void poblacionGChanged();

	void parametroG0Changed();
	void parametroG1Changed();
	void parametroG2Changed();

	void inercia0Changed();
	void inercia1Changed();

	void parametroPChanged();

	void limiteEspacioChecked( bool checked );

	void iterChckChanged(bool check);
	void dispChckChanged(bool check);
	void eaChckChanged(bool check);
	void tiempoChckChanged(bool check);

	void btnOptimizarClicked();

	void celdaPosChanged(int fila, int columna);
	void celdaLimitesChanged(int fila, int columna);

private:
	void pintarGrupoCriterios();

	QSpinBox *nVPGSpin, *nVGSpin, *nGSpin, *dispInicialSpin, *limitVelSpin,
	*ctIterSpin, *ctTiempoSpin;
	QDoubleSpinBox *gSpin[3], *vSpin[2], *pSpin, *ctDispSpin, *ctEaSpin;
	QComboBox *optimizacionCombo;
	QTableWidget *tablaPosInicial, *tablaLimites;
	QLabel *limitVelLabel, *ctDispLabel, *ctEaLabel, *ctIterLabel,
	*ctTiempoLabel;
	QCheckBox *limiteInfChck, *limiteSupChck, *ctDispChck, *ctEaChck,
	*ctIterChck, *ctTiempoChck;
	QGroupBox *critTermGroup;
	QProgressBar *barraDeProgreso;

	Enjambre *enjambre;
	Vector *posInicial, *limiteInf, *limiteSup;

	bool criterioTerm[4];

	int dims;
};

#endif // PARAMETROSWIDGET_H
