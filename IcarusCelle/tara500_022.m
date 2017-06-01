%%cella 500_022 on pin A0
clear all; close all; clc;
dati=load('dati500_022.mat');
plot(dati.dati_500_022(:,1),dati.dati_500_022(:,2),'*')
hold on
c=polyfit(dati.dati_500_022(:,1),dati.dati_500_022(:,2),1);
xx=linspace(dati.dati_500_022(1,1),dati.dati_500_022((length(dati.dati_500_022)),1));
yy=polyval(c,xx);
plot(xx,yy,'r')
