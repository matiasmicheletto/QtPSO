clear;
clc;

fitness = inline('(X1-10)^2+(X2-20)^2+1');
verdadero = [10,20];
disp('Paraboloide')
disp('N=25, iter=25')
[t,err] = PSO(fitness,verdadero,25,25,[0,0],10,0)
disp('N=50, iter=100')
[t,err] = PSO(fitness,verdadero,50,100,[0,0],10,0)
disp('N=100, iter=500')
[t,err] = PSO(fitness,verdadero,100,500,[0,0],10,0)



fitness = inline('Easom([X1,X2])');
verdadero = [pi,pi];
disp('Easom')
disp('N=25, iter=25')
[t,err] = PSO(fitness,verdadero,25,25,[0,0],10,0)
disp('N=50, iter=100')
[t,err] = PSO(fitness,verdadero,50,100,[0,0],10,0)
disp('N=100, iter=500')
[t,err] = PSO(fitness,verdadero,100,500,[0,0],10,0)


fitness = inline('Rosenbrock([X1,X2],100)');
verdadero = [1,1];
disp('Rosenbrock')
disp('N=25, iter=25')
[t,err] = PSO(fitness,verdadero,25,25,[0,0],10,0)
disp('N=50, iter=100')
[t,err] = PSO(fitness,verdadero,50,100,[0,0],10,0)
disp('N=100, iter=500')
[t,err] = PSO(fitness,verdadero,100,500,[0,0],10,0)


fitness = inline('Ackley([X1,X2],20,0.2,2*pi)');
verdadero = [0,0];
disp('Ackley')
disp('N=25, iter=25')
[t,err] = PSO(fitness,verdadero,25,25,[0,0],10,0)
disp('N=50, iter=100')
[t,err] = PSO(fitness,verdadero,50,100,[0,0],10,0)
disp('N=100, iter=500')
[t,err] = PSO(fitness,verdadero,100,500,[0,0],10,0)

