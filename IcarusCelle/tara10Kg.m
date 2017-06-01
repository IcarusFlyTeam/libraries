%%cella 10Kg on pin A0
clear all; close all; clc;
dati=load('dati10.mat');
plot(dati.dati_10(:,1),dati.dati_10(:,2),'*')
hold on
c=polyfit(dati.dati_10(:,1),dati.dati_10(:,2),1);
xx=linspace(dati.dati_10(1,1),dati.dati_10((length(dati.dati_10)),1));
yy=polyval(c,xx);
plot(xx,yy,'r')
