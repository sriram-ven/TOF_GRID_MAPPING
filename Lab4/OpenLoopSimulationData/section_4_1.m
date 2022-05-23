[Acc,Mag,wGyro,Eul] = CreateTrajectoryData(0.02,0)

%dcm  = angle2dcm(0, 0, 0)
dcm = angle2dcm( Eul(1, 1) * pi / 180, Eul(1, 2) * pi / 180, Eul(1, 3 ) * pi / 180);
x_error = zeros();
y_error = zeros();
z_error = zeros();

wGyro = pi .* wGyro ./ (GscaleFactor * 180);

phi_arr = zeros();
theta_arr = zeros();
psi_arr = zeros();

for i = 1:length(Eul)
    currGyro = [wGyro(i,1); wGyro(i,2) ; wGyro(i,3)];
    dcm = IntegrateOpenLoop(dcm, currGyro, 0.02);
    [phi, theta, psi] = dcm2angle(dcm);
    x_error(i) = Eul(i, 1)*pi/180 - phi;
    y_error(i) = Eul(i, 2)*pi/180 - theta;
    z_error(i) = Eul(i, 3)*pi/180 - psi;

    phi_arr(i) = phi * 180 / pi;
    theta_arr(i) = theta * 180 / pi;
    psi_arr(i) = psi * 180 / pi;
    
end
hold on;

figure()

subplot(3, 1, 1)
% plot(x_error)
plot(phi_arr)
hold on,
plot(Eul(:,1))
legend("actual", "expected")

subplot(3,1, 2)
% plot(y_error)
plot(theta_arr)
hold on,
plot(Eul(:,2))

subplot(3,1, 3)
% plot(z_error)
plot(psi_arr)
hold on,
plot(Eul(:,3))

hold off;