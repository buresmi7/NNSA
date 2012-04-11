data0 = importdata('D:\MICHAL\Škola FEL\NNSA\testy\test1_staticti_protivnici\prvni\data18.txt')
x = [1:101]
y = [1:300]

[X,Y] = meshgrid(x,y);

mesh(X,Y,sort(data0.data(:,1:end)'))

xlabel('generace')
ylabel('jedinci')
zlabel('fitness')

xlim([0 100])