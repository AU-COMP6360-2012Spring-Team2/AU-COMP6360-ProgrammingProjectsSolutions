clear; clear all; clc

% data
mpr= [
9.932307692	6.809230769	89.14153846	7.683076923
9.936056838	9.557726465	128.7779751	19.41563055
9.928176796	16.44972376	177.7834254	34.32596685
10.57013575	26.84615385	235.9230769	51.70588235
9.56954612	86.26207906	270.4758419	77.81551977
9.827900913	201.8370274	280.0769231	85.1642764
];
nompr = [
9.902777778	11.30416667	89.37777778	0
9.945668135	25.18061674	138.6842878	0
10.00877193	46.36549708	187.6988304	0
10.00921659	73.59677419	236.3341014	0
9.725222146	198.202324	274.1927546	0
9.999159664	346.905042	278.3336134	0
];

locations = [
120	50
120	110
60	120
120	205
70	240
140	240
220	240
240	260
240	200
240	310
220	360
260	360
260	240
240	140
340	240
360	280
340	360
240	440
200	480
130	480
280	480
360	200
360	130
360	50
360	470
400	120
470	120
480	200
480	40
480	280
];

% indeces
eebl = 1;
eebl_rebr = 2;
beacon = 3;
hello = 4;
x=[10 15 20 25 30];
dynamic=[1:5];
static=6;



figure(1) % dynamic traffic
subplot(2,2,1)
plot(x,mpr(dynamic, eebl_rebr),'-*',x,nompr(dynamic, eebl_rebr),'--o');
legend('With MPR', 'Without MPR','Location', 'NorthWest');
xlabel('Number of Nodes');
ylabel('Packet Per Second');
title('Rebroadcasted EEBL Packet Rate');
grid on;

subplot(2,2,2)
plot(x,mpr(dynamic, beacon),'-*',x,nompr(dynamic, beacon),'--o');
legend('With MPR', 'Without MPR','Location', 'NorthWest');
xlabel('Number of Nodes');
ylabel('Packet Per Second');
title('Beacon Packet Rate');
grid on;

subplot(2,2,3)
plot(x,mpr(dynamic, eebl)+mpr(dynamic, eebl_rebr),'-*',x,nompr(dynamic, eebl)+nompr(dynamic, eebl_rebr),'--o');
legend('With MPR', 'Without MPR','Location', 'NorthWest');
xlabel('Number of Nodes');
ylabel('Packet Per Second');
title('Total EEBL Packet Rate');
grid on;

subplot(2,2,4)
plot(x,sum(mpr(dynamic,:)')','-*',x,sum(nompr(dynamic,:)')','--o');
legend('With MPR', 'Without MPR','Location', 'NorthWest');
xlabel('Number of Nodes');
ylabel('Packet Per Second');
title('Total Throughput Packet Rate');
grid on;


figure(2) % static traffic
barplot = [
    mpr(static, eebl_rebr), mpr(static, beacon), mpr(static, eebl)+mpr(static, eebl_rebr), sum(mpr(static,:)')';
    nompr(static, eebl_rebr), nompr(static, beacon), nompr(static, eebl)+nompr(static, eebl_rebr), sum(nompr(static,:)')';
]';
h=bar(barplot);
set(gca,'XTickLabel',['Rebroadcasted EEBL'; '      Beacon      '; '    Total EEBL    '; ' Total Throughput '])
ylabel('Packet Per Second');
legend('With MPR', 'Without MPR','Location','NorthWest');
set(h(1),'LineWidth',1,'LineStyle','-');
set(h(2),'LineWidth',1,'LineStyle','--');
colormap Summer;
grid on;


figure(3) % locations
scatter(locations(:,1), locations(:,2),'o');
xlabel('longitude (meters)')
ylabel('latitude (meters)')
grid on;
set(gca, 'XTickMode', 'manual');
set(gca, 'XTick', [0 120 240 360 480]);
set(gca, 'YTickMode', 'manual');
set(gca, 'YTick', [0 120 240 360 480]);