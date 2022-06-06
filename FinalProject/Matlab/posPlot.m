pose = table2array(readtable("poselog3t.csv"));
xpos = pose(:,1);
ypos = pose(:,2);

close all;
%% 
scatter(xpos, ypos);
axis equal;

figure();

plot(pose(:,4));
hold on;
plot(pose(:,5));