% Optimizacion por enjambre de particulas
% 
% Autor: Micheletto, Matias
% Fecha: 13/03/2012 
%
% Optimizacion por enjambre de particulas aplicada a funciones de 2 variables
clear
clc

%%__ Declaraciones de los parametros __%%

% Funcion objetivo (picos)
fitness = inline('peaks(x,y)');
% Numero de particulas
N = 10;
% Cantidad de iteraciones
iter = 10;
%Posicion inicial del enjambre
p0 = [2,-2.5];
% Ejes (vista)
ejes = [-2,2.5,-3,1];
% Espacio de busqueda
[xx,yy] = meshgrid(-2:.05:2.5,-3:.05:1);
% Factor de correccion 1
fc1 = 1;
% Factor de correccion 2
fc2 = 1.5;
% Inercia
w = 0.4;
% Dispersion inicial
d = 0.5;


%%___ Inicializar enjambre ___%%

% Optimo global
fOpt = realmax;

% Inicializar posicion de las particulas
for i=1:N
	x(i,1) = p0(1) + 2*d*rand - d;
	x(i,2) = p0(2) + 2*d*rand - d;
	
	% Valores de los optimos locales
	l(i) = fitness( x(i,1), x(i,2) );
	
	% Optimo global	
	if l(i) < fOpt
		g(1:2) = x(i,1:2);
		fOpt = l(i); 
	end	
end

% Optimos locales
p = x;

% Velocidad (inicializar arreglo en cero)
v(N,2) = 0;

% Funcion a optimizar (contornos)
zz  = fitness(xx,yy);

% Colores de las particulas (aleatorios)
for i=1:N
    color(i,1) = rand; % Componente R (rojo)
    color(i,2) = rand; % Componente G (verde)
    color(i,3) = rand; % Componente B (azul)
end

%%__ Iteraciones __%%

% Para cada iteracion
for i=1:iter
    clf % Borrar grafica
    hold on % Mantener dibujos
    axis(ejes) % Fijar posicion de los ejes
    grid % Activar grilla
    title('Movimiento de las partículas','FontWeight','Bold')
    % Texto en el eje X
    cadena = ['Iteración Nº ',num2str(i)];
    xlabel(cadena,'FontWeight','Bold')
    % Curvas de nivel
    contour(xx,yy,zz,20)
    
	% Para cada particula del enjambre, hacer
	for j=1:N
        % Algoritmo PSO
		% Actualizar velocidad
		v(j,1) = w*rand*v(j,1) + fc1*rand*( p(j,1) - x(j,1) ) + ...
										fc2*rand*( g(1) - x(j,1) );
		v(j,2) = w*rand*v(j,2) + fc1*rand*( p(j,2) - x(j,2) ) + ...
										fc2*rand*( g(2) - x(j,2) );
		
		% Actualizar posicion
		x(j,:) = x(j,:) + v(j,:);
		
		% Actualizar optimos
		fx =  fitness(x(j,1),x(j,2));
		if fx < l(j)
			% Optimo local
			p(j,:) = x(j,:);
			l(j) = fx;
			if fx < fOpt
				% Optimo global
				g(:) = p(j,:);
				fOpt = fx;
            end 
        end
        % Graficar particula actual
        plot(x(j,1),x(j,2),'o','MarkerFaceColor',...
            [color(j,1),color(j,2),color(j,3)],...
            'MarkerEdgeColor','k','MarkerSize',5)
    end
    
    % Guardar cada grafico
    %cadena = ['Iteracion',num2str(i),'.png']; % Nombre del archivo
    %print('-dpng',cadena) % Guardar
    pause(0.6) % Pausar animacion
end

%close % Cerrar al terminar animacion
