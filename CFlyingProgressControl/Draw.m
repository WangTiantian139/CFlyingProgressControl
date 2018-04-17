M = csvread('FlyByCmdOut.csv',1,0);
figure
comet3(M(:,2),M(:,3),M(:,4));
M = csvread('FlyPlusOut.csv',1,0);
figure
comet3(M(:,2),M(:,3),M(:,4));
