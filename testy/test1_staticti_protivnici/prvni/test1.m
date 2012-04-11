data0 = importdata('D:\MICHAL\Škola FEL\NNSA\testy\test1_staticti_protivnici\prvni\data30.txt')
data1 = importdata('D:\MICHAL\Škola FEL\NNSA\testy\test1_staticti_protivnici\prvni\data31.txt')
data2 = importdata('D:\MICHAL\Škola FEL\NNSA\testy\test1_staticti_protivnici\prvni\data32.txt')
data3 = importdata('D:\MICHAL\Škola FEL\NNSA\testy\test1_staticti_protivnici\prvni\data33.txt')
data4 = importdata('D:\MICHAL\Škola FEL\NNSA\testy\test1_staticti_protivnici\prvni\data34.txt')
data5 = importdata('D:\MICHAL\Škola FEL\NNSA\testy\test1_staticti_protivnici\prvni\data35.txt')
data6 = importdata('D:\MICHAL\Škola FEL\NNSA\testy\test1_staticti_protivnici\prvni\data36.txt')
data7 = importdata('D:\MICHAL\Škola FEL\NNSA\testy\test1_staticti_protivnici\prvni\data37.txt')
data8 = importdata('D:\MICHAL\Škola FEL\NNSA\testy\test1_staticti_protivnici\prvni\data38.txt')
data9 = importdata('D:\MICHAL\Škola FEL\NNSA\testy\test1_staticti_protivnici\prvni\data39.txt')

vysledek = sort(data0.data(end, 1:end))
vysledek(2,:) = sort(data1.data(end, 1:end))
vysledek(3,:) = sort(data2.data(end, 1:end))
vysledek(4,:) = sort(data3.data(end, 1:end))
vysledek(5,:) = sort(data4.data(end, 1:end))
vysledek(6,:) = sort(data5.data(end, 1:end))
vysledek(7,:) = sort(data6.data(end, 1:end))
vysledek(8,:) = sort(data7.data(end, 1:end))
vysledek(9,:) = sort(data8.data(end, 1:end))
vysledek(10,:) = sort(data9.data(end, 1:end))



vysledek1 = sort(vysledek);

plot(vysledek1(1,:),'r')
hold on
plot(vysledek1(10,:),'g')

plot(mean(vysledek1),'b')

xlabel('jedinci')
ylabel('fitness')

legend('nejhorsi', 'nejlepsi', 'prumer z 10', 2)
