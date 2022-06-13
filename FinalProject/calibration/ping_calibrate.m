filename = fopen('50mm.txt' ,'r');
data= textscan(filename, '%f', 'Delimiter', {'mm'}, 'CollectOutput', 1);
fclose(filename);
d50=cell2mat(data);

filename = fopen('100mm.txt' ,'r');
data= textscan(filename, '%f', 'Delimiter', {'mm'}, 'CollectOutput', 1);
fclose(filename);
d100=cell2mat(data);

filename = fopen('150mm.txt' ,'r');
data= textscan(filename, '%f', 'Delimiter', {'mm'}, 'CollectOutput', 1);
fclose(filename);
d150=cell2mat(data);

filename = fopen('200mm.txt' ,'r');
data= textscan(filename, '%f', 'Delimiter', {'mm'}, 'CollectOutput', 1);
fclose(filename);
d200=cell2mat(data);

filename = fopen('250mm.txt' ,'r');
data= textscan(filename, '%f', 'Delimiter', {'mm'}, 'CollectOutput', 1);
fclose(filename);
d250=cell2mat(data);

filename = fopen('300mm.txt' ,'r');
data= textscan(filename, '%f', 'Delimiter', {'mm'}, 'CollectOutput', 1);
fclose(filename);
d300=cell2mat(data);

filename = fopen('350mm.txt' ,'r');
data= textscan(filename, '%f', 'Delimiter', {'mm'}, 'CollectOutput', 1);
fclose(filename);
d350=cell2mat(data);

filename = fopen('400mm.txt' ,'r');
data= textscan(filename, '%f', 'Delimiter', {'mm'}, 'CollectOutput', 1);
fclose(filename);
d400=cell2mat(data);

filename = fopen('450mm.txt' ,'r');
data= textscan(filename, '%f', 'Delimiter', {'mm'}, 'CollectOutput', 1);
fclose(filename);
d450=cell2mat(data);

filename = fopen('500mm.txt' ,'r');
data= textscan(filename, '%f', 'Delimiter', {'mm'}, 'CollectOutput', 1);
fclose(filename);
d500=cell2mat(data);

filename = fopen('600mm.txt' ,'r');
data= textscan(filename, '%f', 'Delimiter', {'mm'}, 'CollectOutput', 1);
fclose(filename);
d600=cell2mat(data);

filename = fopen('700mm.txt' ,'r');
data= textscan(filename, '%f', 'Delimiter', {'mm'}, 'CollectOutput', 1);
fclose(filename);
d700=cell2mat(data);

filename = fopen('800mm.txt' ,'r');
data= textscan(filename, '%f', 'Delimiter', {'mm'}, 'CollectOutput', 1);
fclose(filename);
d800=cell2mat(data);

filename = fopen('900mm.txt' ,'r');
data= textscan(filename, '%f', 'Delimiter', {'mm'}, 'CollectOutput', 1);
fclose(filename);
d900=cell2mat(data);

filename = fopen('1000mm.txt' ,'r');
data= textscan(filename, '%f', 'Delimiter', {'mm'}, 'CollectOutput', 1);
fclose(filename);
d1000=cell2mat(data);

avg1 = mean(d100);
avg2 = mean(d200);
avg3 = mean(d300);
avg4 = mean(d400);
avg5 = mean(d500);
avg6 = mean(d600);
avg7 = mean(d700);
avg8 = mean(d800);
avg9 = mean(d900);
avg10 = mean(d1000);

x=[0, 1;
    50, 1;
    100, 1;
    150, 1;
    200, 1;
    250, 1;
    300, 1;
    350, 1;
    400, 1;
    450, 1;
    500, 1;
    600, 1;
    700, 1;
    800, 1;
    900, 1;
    1000, 1];


y=[1;
    mean(d50);
    avg1;
    mean(d150);
    avg2;
    mean(d250);
    avg3;
    mean(d350);
    avg4; 
    mean(d450);
    avg5;
    avg6;
    avg7;
    avg8;
    avg9;
    avg10];

c0=(d50-p(2))./p(1);
c1=(d100-p(2))./p(1);
c2=(d150-p(2))./p(1);
c3=(d200-p(2))./p(1);
c4=(d250-p(2))./p(1);
c5=(d300-p(2))./p(1);
c6=(d350-p(2))./p(1);
c7=(d400-p(2))./p(1);
c8=(d450-p(2))./p(1);
c9=(d500-p(2))./p(1);
c10=(d600-p(2))./p(1);
c11=(d700-p(2))./p(1);
c12=(d800-p(2))./p(1);
c13=(d900-p(2))./p(1);
c14=(d1000-p(2))./p(1);

cp = [0;
    mean(c0);
    mean(c1);
    mean(c2);
    mean(c3);
    mean(c4);
    mean(c5);
    mean(c6);
    mean(c7);
    mean(c8);
    mean(c9);
    mean(c10);
    mean(c11);
    mean(c12);
    mean(c13);
    mean(c14)];
mean(d450)
mean(c8)

p=x\y;

plot(x(:,1),y)
xlabel("True Distance (mm)")
xlim([0 1000])
ylabel("Sensor Distance (mm)")
ylim([0 1000])
title("ToF Sensor Calibration")
hold on
plot (x(:,1), cp)
legend("Uncalibrated Readings", "Calibrated Readings")
grid on