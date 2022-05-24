close all

[Acc,Mag,wGyro,Eul] = CreateTrajectoryData(0.02,0)

dcm  = angle2dcm(0, 0, 0)
% dcm = angle2dcm( Eul(1, 1) * pi / 180, Eul(1, 2) * pi / 180, Eul(1, 3 ) * pi / 180);
x_error = zeros();
y_error = zeros();
z_error = zeros();


GscaleFactor = (1/250)*(2^15-1);    % bits/(deg/s)
wGyro = pi .* wGyro ./ (GscaleFactor * 180);

phi_arr = zeros();
theta_arr = zeros();
psi_arr = zeros();

for i = 1:length(Eul)
    currGyro = [wGyro(i,1); wGyro(i,2) ; wGyro(i,3)];
    dcm = IntegrateOpenLoop(dcm, currGyro, 0.02);
    
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
title("Calculated vs Actual Roll Angle")
legend("actual", "expected")

subplot(3,1, 2)
% plot(y_error)
plot(theta_arr)
hold on,
plot(Eul(:,2))
title("Calculated vs Actual Pitch Angle")

subplot(3,1, 3)
% plot(z_error)
plot(psi_arr)
hold on,
plot(Eul(:,3))
title("Calculated vs Actual Yaw Angle")

hold off;


figure()
subplot(3, 1, 1)
plot(x_error)
title("Roll error")


subplot(3,1, 2)
plot(y_error)
title("Pitch error")


subplot(3,1, 3)
plot(z_error)
title("Yaw error")


hold off



figure()

subplot(3, 1, 1)
% plot(x_error)
histfit(x_error)
title("Roll error distribution")

subplot(3,1, 2)
% plot(y_error)
histfit(y_error)
title("Pitch error distribution")


subplot(3,1, 3)
% plot(z_error)
histfit(z_error)
title("Yaw error distribution")


hold off

mean(x_error)
std(x_error)
mean(y_error)
std(y_error)
mean(z_error)
std(z_error)