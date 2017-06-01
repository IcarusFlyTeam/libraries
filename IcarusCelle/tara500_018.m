%% cella500Kg_018 on pin A1
clear all; close all; clc;
dati=load('dati500_018.mat');
plot(dati.dati_500_018(:,1),dati.dati_500_018(:,2),'*')
hold on
c=polyfit(dati.dati_500_018(:,1),dati.dati_500_018(:,2),1);
xx=linspace(dati.dati_500_018(1,1),dati.dati_500_018((length(dati.dati_500_018)),1));
yy=polyval(c,xx);
plot(xx,yy,'r')


