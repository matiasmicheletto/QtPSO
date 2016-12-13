#include <QApplication>
#include <QElapsedTimer>
#include <getopt.h>

#include "VentanaPrincipal.h"
#include "FuncionesNumericas.h"

Vector str2vector(QString qstr);
Funcion* interpretarFc(QString fc);

int main(int argc, char *argv[])
{
    qsrand(time(NULL));

	// Codificacion de caracteres
	QTextCodec *linuxCodec = QTextCodec::codecForName("UTF-8");
	QTextCodec::setCodecForTr(linuxCodec);
	QTextCodec::setCodecForCStrings(linuxCodec);
	QTextCodec::setCodecForLocale(linuxCodec);

    int resultado = 0;

	QApplication pso(argc,argv);

    if( argc == 1 ) // Sin argumentos -> GUI
    {
        VentanaPrincipal *ventana = new VentanaPrincipal;
        ventana->show();
        resultado = pso.exec();
    }
    else // Argumentos -> Ejecutar en consola sin GUI
    {
        // Opciones largas
        const struct option opcLargas[] =
        { //{ "opc_larga", tipo_arg, NULL, 'opc_corta'}
            { "help",         1,  NULL,   'h'},
            { "funcion",      1,  NULL,   'f'},
            { "posinic",      1,  NULL,   'p'},
            { "disper",       1,  NULL,   'd'},
            { "iters",        1,  NULL,   'i'},
            { "maximizar",    0,  NULL,   'x'},
            { "ngvp",         1,  NULL,   'm'},
            { NULL,           0,  NULL,    0 }
        };

        // Opciones ingresadas por el usuario
        char opcion;
        QString argumento;

        // Construir enjambre y funcion
        Funcion *funcion = new FunMatematica("X1");
        Enjambre *enjambre = new Enjambre(funcion);
        Vector *vector = new Vector(funcion->getDimension());
        // Por defecto se realizaran 15 iteraciones
        int iter = 15;

        // Interpretacion de comandos
        while(1)
        {
            // Leer opciones
            opcion = getopt_long(argc,argv,"f:p:d:i:xh", opcLargas, NULL);
            // Fin de lista de opciones
            if(opcion < 0)
                break;
            // Parametros de las funciones
            argumento = optarg;
            switch(opcion)
            {
            case 'f':   // Funcion objetivo
            {                
                delete funcion;
                // Reemplazo de constantes
                while( argumento.contains("PI") )
                    argumento.replace("PI","3.141592");

                // Caso funciones numericas
                if(argumento.startsWith("&"))
                    // Obtener tipo de fc y sus parametros
                    funcion = interpretarFc(argumento);
                else
                    // Crear funcion simbolica
                    funcion = new FunMatematica(argumento);

                // Setear funcion al enjambre
                enjambre->setFuncion(funcion);
                break;
            }
            case 'p':   // Posicion inicial
            {          
                Vector posicion(1);
                // Obtener vector a partir del string
                posicion = str2vector(argumento);
                // Controlar las dimensiones del vector
                if( funcion->getDimension() != posicion.getDimension() )
                {
                    cout << "El numero de componentes del vector de posicion "
                         << "inicial indicado no es correcta" << endl;
                    exit(1);
                }
                // Setear posicion inicial al enjambre
                enjambre->setPosicionInicial(&posicion);
                // Reiniciar el enjambre a la posicion incial
                enjambre->reiniciar();
                break;
            }
            case 'd':   // Dispersion inicial
            {
                enjambre->setDispersionInicial(argumento.toDouble());
                enjambre->reiniciar();
                break;
            }
            case 'i':   // Numero de iteraciones
            {
                iter = argumento.toInt();
                break;
            }
            case 'm':   // Modificar poblacion y parametros
            {
                Vector parametros(1);
                // Obtener parametros
                parametros = str2vector(argumento);
                // Diferenciar casos
                switch(parametros.getDimension())
                {
                case 2: // Particulas G
                {
                    enjambre->setNroParticulas(Enjambre::G,parametros[0]);
                    enjambre->setFactorG(Enjambre::G,parametros[1]);
                    break;
                }
                case 3: // Particulas VG
                {
                    enjambre->setNroParticulas(Enjambre::VG,parametros[0]);
                    enjambre->setFactorG(Enjambre::VG,parametros[1]);
                    enjambre->setInercia(Enjambre::VG,parametros[2]);
                    break;
                }
                case 4: // Particulas VPG
                {
                    enjambre->setNroParticulas(Enjambre::VPG,parametros[0]);
                    enjambre->setFactorG(Enjambre::VPG,parametros[1]);
                    enjambre->setInercia(Enjambre::VPG,parametros[2]);
                    enjambre->setFactorP(parametros[3]);
                    break;
                }
                default: // Cantidad de argumentos incorrecta
                {
                    cout << "Poblacion o parametros de ajuste incorrecta."
                         << endl;
                    exit(1);
                }
                }
                break;
            }
            case 'h':   // Ayuda
            {             
                system("clear");
                cout << "Uso: QtPSO -f [funcion] [opciones]" << endl << endl;
                cout << "Opciones:" << endl;
                cout << "\t-i  --iters <numero>    Numero de iteraciones"
                     << endl;
                cout << "\t-d  --disper <valor>    Dispersion Inicial" << endl;
                cout << "\t-p  --posinic <vector>  Posicion Inicial. Ej:(1,2,3)"
                     << endl;
                cout << "\t-m  --ngvp <vector>     Poblacion y parametros"
                     << endl;
                cout << "\t-x  --maximizar         Maximizacion" << endl;
                cout << "\t-h  --help              Ayuda" << endl;
                cout << endl;
                cout << "Ejemplos:" << endl;
                cout << "\tOpciones: posicion inicial en (10,10,10), 200"
                     << " iteraciones" << endl;
                cout << "\tQtPSO -f \"(X1-10)^2+(X2-3)^2+(X3+2)+1\""
                     << " -p \"(10,10,10)\" -i 200" << endl << endl;
                cout << "\tOpciones: dispersion inicial 15, maximizar funcion"
                     << endl;
                cout << "\tQtPSO -f \"sin(X1)*cos(X2)/(X1^2+X2^2+1)\""
                     << " -d 15 -x" << endl << endl;
                cout << "\tOpciones: 100 particulas vpg (g=1.5, w=0.5 y p=0.7)"
                     << " y 30 de tipo g (g=1.6)" << endl;
                cout << "\tQtPSO -f \"&Easom(3)\" --ngvp \"(100,1.5,0.5,0.7)\""
                     << " --ngvp \"(30,1.6)\" "
                     << endl;
                exit(0);
                break;
            }
            case 'x':   // Maximizar
            {
                enjambre->setTipoDeOpt(true);
                break;
            }
            default:    // Opcion invalida
            {
                cout << "Uso: QtPSO -f [funcion] [opciones]" << endl;               
                exit(1);
            }
            }
        }

        // Mostrar funcion
        cout << "Funcion: " << qPrintable(funcion->getExpresion()) << endl;

        // Iniciar cronometro
        QElapsedTimer *timer = new QElapsedTimer;
        timer->start();

        // Optimizar funcion
        for(int i=0; i < iter; i++)
            enjambre->iterar();

        // Detener cronometro
        double tiempo = timer->elapsed();

        // Borrar puntero del cronometro
        delete timer;

        // Mostrar resultados:
        *vector = enjambre->getOptimo();
        cout << "Posicion del optimo: " << endl;
        for(int i = 0; i < vector->getDimension(); i++)
            cout << "\tX" << i+1 << " = " << vector->getComponente(i) << endl;
        cout << endl;

        cout << "Valor optimo: " << enjambre->getFOptimo() << endl;
        cout << "Iteraciones realizadas: " << iter << endl;
        cout << "Dispersion del enjambre: " << enjambre->calcularDispersion()
             << endl;
        cout << "Error estimado relativo: " << enjambre->getError() << endl;
        cout << "Tiempo de ejecucion: " << tiempo << " ms" << endl << endl;
        cout << "Posicion Inicial: (";
        for(int i = 0; i < enjambre->getPosInicial().getDimension(); i++)
        {
            cout << enjambre->getPosInicial().getComponente(i);
            if( i < enjambre->getPosInicial().getDimension()-1 )
                cout << ",";
            else
                cout << ")" << endl;
        }
        cout << endl;

        cout << "Particula:\t\tVPG\tVG\tG" << endl;
        cout << "Cantidad:\t\t";
        cout << enjambre->getCantidadParticulas(Enjambre::VPG) << "\t";
        cout << enjambre->getCantidadParticulas(Enjambre::VG) << "\t";
        cout << enjambre->getCantidadParticulas(Enjambre::G) << endl;
        cout << "Parametro G:\t\t";
        cout << enjambre->getFactorG(Enjambre::VPG) << "\t";
        cout << enjambre->getFactorG(Enjambre::VG) << "\t";
        cout << enjambre->getFactorG(Enjambre::G) << endl;
        cout << "Inercia:\t\t";
        cout << enjambre->getInercia(Enjambre::VPG) << "\t";
        cout << enjambre->getInercia(Enjambre::VG) << endl;
        cout << "Parametro P:\t\t";
        cout << enjambre->getFactorP() << endl;
    }

	cout << "Programa terminado." << endl;
	return resultado;
}

Vector str2vector(QString qstr)
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

Funcion* interpretarFc(QString fc)
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
