cd /Users/GustavoMV/Documents/ScootPlayer/FreePass/1513542893
M = load('event-tratado.txt');
x = M(:,1); y = M(:,2);
cd /Users/GustavoMV/Documents/ScootPlayer/FreePass/1513558962
M = load('event-tratado.txt');
X = M(:,1); Y = M(:,2);
subplot(2,1,1);
set(plot(x,y),'LineWidth',[2]);hold on;
set(plot(X,Y),'LineWidth',[2]);hold off; 
xlabel('Time(s)','FontSize',12); 
ylabel('Data Received (bytes)','FontSize',12);
title('Without Application');
legend('Simulation01','Simulation02','Location','NorthEastOutside');

cd /Users/GustavoMV/Documents/ScootPlayer/MpegDash/1513587749
M = load('event-tratado.txt');
x_ = M(:,1); y_ = M(:,2);
cd /Users/GustavoMV/Documents/ScootPlayer/MpegDash/1513593156
M = load('event-tratado.txt');
X_ = M(:,1); Y_ = M(:,2);
subplot(2,1,2);
set(plot(x_,y_,'Color','green'),'LineWidth',[2]);hold on;
set(plot(X_,Y_,'Color','red'),'LineWidth',[2]);hold off; 
xlabel('Time(s)','FontSize',12); 
ylabel('Data Received (bytes)','FontSize',12);
title('With Application');
legend('Simulation01','Simulation02','Location','NorthEastOutside');
