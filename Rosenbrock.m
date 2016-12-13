% Funcion Rosenbrock's Valley
% Autor del script: Micheletto, Matias
% Parametros recomendados:
% a = 100

function r = Rosenbrock(x, a)

r = 0;
for i = 1:size(x,2)-1
	r = r + a*( x(i+1) - x(i)^2 )^2 + ( 1 - x(i) )^2;
end

end
