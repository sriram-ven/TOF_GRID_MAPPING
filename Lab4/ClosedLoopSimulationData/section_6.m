trajectoryData = csvread('section8RealData.csv');

wGyro = zeros(length(trajectoryData), 3);
wGyro(:,1) = trajectoryData(:,1);
wGyro(:,2) = trajectoryData(:,2);
wGyro(:,3) = trajectoryData(:,3);

Acc = zeros(length(trajectoryData), 3);
Acc(:,1) = trajectoryData(:,4);
Acc(:,2) = trajectoryData(:,5);
Acc(:,3) = trajectoryData(:,6);

Mag = zeros(length(trajectoryData), 3);
Mag(:,1) = trajectoryData(:,7);
Mag(:,2) = trajectoryData(:,8);
Mag(:,3) = trajectoryData(:,9);

xbias = 0;
ybias = 0;
zbias = 0;

for i = 1:50
    xbias = xbias + wGyro(i,1);
    ybias = ybias + wGyro(i,2);
    zbias = zbias + wGyro(i,3);
end

xbias = xbias / 50;
ybias = ybias / 50;
zbias = zbias / 50;

B = [xbias, ybias, zbias];

dcm = angle2dcm(trajectoryData(1, 10) * pi/180, trajectoryData(1, 11) * pi/180, trajectoryData(1, 12) * pi/180);

% initMag = [0.4779; 0.1118; 0.8713];
initMag = [-0.4576; -0.1982; -0.8668];
initAcc = [0; 0; -1];

phi_arr = zeros();
theta_arr = zeros();
psi_arr = zeros();

for i = 1:length(trajectoryData)
    currGyro = [wGyro(i,1); wGyro(i,2) ; wGyro(i,3)];
    currMag = [Mag(i,1), Mag(i,2), Mag(i,3)];
    currAccel = [Acc(i,1), Acc(i,2), Acc(i,3)];
    [dcm, B] = IntegrateClosedLoop(dcm, B, currGyro, currMag, currAccel, initMag, initAcc, 0.02);
    [phi, theta, psi] = dcm2angle(dcm);
    
    phi_arr(i) = phi * 180 / pi;
    theta_arr(i) = theta * 180 / pi;
    psi_arr(i) = psi * 180 / pi;
end

subplot(3, 1, 1)
hold on
plot(phi_arr)
plot(-trajectoryData(:,12))
hold off
title('Calculated vs Actual Yaw (With Magnetometer)')

legend('IntegrateClosedLoop', 'C Implementation')

subplot(3,1, 2)
hold on
plot(theta_arr)
plot(trajectoryData(:,11))
hold off
title('Calculated vs Actual Pitch (With Magnetometer)')

subplot(3,1, 3)
hold on
plot(psi_arr)
plot(trajectoryData(:,10))
hold off
title('Calculated vs Actual Roll (With Magnetometer)')