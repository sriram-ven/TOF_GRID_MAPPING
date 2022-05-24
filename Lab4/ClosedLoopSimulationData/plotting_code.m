GscaleFactor = (1/250)*(2^15-1);    % bits/(deg/s)
wGyro = pi .* wGyro ./ (GscaleFactor * 180);

B = [0, 0, 0];
dcm = angle2dcm( Eul(1, 1) * pi / 180, Eul(1, 2) * pi / 180, Eul(1, 3 ) * pi / 180);

initMag = [Mag(1,1); Mag(1,2); Mag(1,3)];
initAcc = [0; 0; -1];

phi_arr = zeros();
theta_arr = zeros();
psi_arr = zeros();

for i = 1:1:length(Eul)
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
plot(Eul(:,1))
hold off

subplot(3,1, 2)
hold on
plot(theta_arr)
plot(Eul(:,2))
hold off

subplot(3,1, 3)
hold on
plot(psi_arr)
plot(Eul(:,3))
hold off