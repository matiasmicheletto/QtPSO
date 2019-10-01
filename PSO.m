% Optimizacion por enjambre de particulas
% 
% Autor: Micheletto, Matias
% Fecha: 13/03/2012 
%
% Optimizacion por enjambre de particulas aplicada a funciones de 2 variables

% Ejemplos

% Funcion a optimizar
%fitness = inline('(X1-10)^2+(X2-20)^2+1');
%verdadero = [10,20];

%fitness = inline('Easom([X1,X2])');
%verdadero = [pi,pi];

%fitness = inline('Rosenbrock([X1,X2])');
%verdadero = [1,1];

%fitness = inline('Ackley([X1,X2])');
%verdadero = [0,0];

function [t,err] = PSO(fitness,verdadero, N, iter, p0, d, outpt)

if nargin <= 2
    % Cantidad de particulas
    N = 25;
    % Posicion inicial
    p0 = [0,0];
    % Dispersion inicial
    d = 5;
    % Numero de iteraciones
    iter = 25;
    % Salida texto
    outpt = 1;
end

%%__ Declaraciones de los parametros __%%

% Factor de correccion 1
fc1 = 1.5;
% Factor de correccion 2
fc2 = 1.5;
% Inercia
w = 0.5;


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

% Velocidad
v(N,2) = 0;
	
	
	

%%__ Iteraciones __%%

% Iniciar cronometro
tic

% Para cada iteracion
for i=1:iter
	% Para cada particula del enjambre
	for j=1:N
		% Actualizar velocidad
		v(j,1) = w*rand*v(j,1) + fc1*rand*( p(j,1) - x(j,1) ) + fc2*rand*( g(1) - x(j,1) );
		v(j,2) = w*rand*v(j,2) + fc1*rand*( p(j,2) - x(j,2) ) + fc2*rand*( g(2) - x(j,2) );
		
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
	end
end

% Detener cronometro
t = toc;

% Pasar a ms
t = t*1000;




%%__ Calculo de dispersion __%%

prom(1) = sum(x(:,1))/N;
prom(2) = sum(x(:,2))/N;
% Dispersión de los datos
s(1) = sqrt(sum((x(:,1) - prom(1)).^2)/N);
s(2) = sqrt(sum((x(:,2) - prom(2)).^2)/N);
S = sqrt(s(1)^2 + s(2)^2);



%%__ Calculo de error __%%
err = ((g(1)-verdadero(1))/g(1))^2 + ((g(2)-verdadero(2))/g(2))^2;
err = sqrt(err)*100;


%%__ Mostrar resultados __%%
if(outpt)
	disp ''
	disp 'Resultados de la optimizacion'
	disp '-----------------------------'
	disp 'Funcion objetivo:'
	disp(fitness)
	disp 'Posicion del optimo:'
	disp(g)
	disp 'Valor optimo:'
	disp(fOpt)
	disp 'Dispersion:'
	disp(S)
	disp 'Duracion del proceso de optimizacion (ms)'
	disp(t)
	disp ''
end

end

