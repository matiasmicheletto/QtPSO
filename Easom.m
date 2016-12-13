% Funcion Easom
% Autor del script: Micheletto, Matias

function r = Easom(x)

r = -(-1)^size(x,2)*prod(cos(x)) / exp(sum((x-pi).^2));

end
