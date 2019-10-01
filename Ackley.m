% Funcion Ackley's Path
% Autor del script: Micheletto, Matias
% Parametros recomendados:
% a = 20
% b = 0.2
% c = 2*pi

function r = Ackley(x, a, b, c)

n = size(x,2);
r = -a * exp( -b*sqrt( sum(x.^2)/n ) ) - exp( sum(cos(c*x))/n ) + a + exp(1);

end
