data0 = importdata('D:\MICHAL\Škola FEL\NNSA\testy\test1_staticti_protivnici\prvni\data38.txt')


vysledek = sortrows(data0.data',fliplr(1:301))

plot(vysledek(1,1:301),'r')

hold on

plot(vysledek(300,1:301),'g')

plot(mean(vysledek),'b')

xlim([0 301])

legend('nejhorsi', 'nejlepsi', 'prumer cele generace', 2)

xlabel('iterace')
ylabel('fitness')
title('evoluce z testu 38')