xbias = 0;
ybias = 0;
zbias = 0;

for i = 1:50
    xbias = xbias + trajectoryData(i,1);
    ybias = ybias + trajectoryData(i,2);
    zbias = zbias + trajectoryData(i,3);
end

xbias = xbias / 50;
ybias = ybias / 50;
zbias = zbias / 50;

trajectoryData(:,1) = trajectoryData(:,1) - xbias;
trajectoryData(:,2) = trajectoryData(:,2) - ybias;
trajectoryData(:,3) = trajectoryData(:,3) - zbias;

x_error(i) =  zeros();
y_error(i) =  zeros();
z_error(i) =  zeros();


phi_arr = zeros();
theta_arr = zeros();
psi_arr = zeros();

dcm = angle2dcm(trajectoryData(1, 1),trajectoryData(1, 2) , trajectoryData(1, 3));

for i = 1:length(trajectoryData)
    currGyro = [trajectoryData(i,1); trajectoryData(i,2) ; trajectoryData(i,3)];
    dcm = IntegrateOpenLoop(dcm, currGyro, 0.02);
    [phi, theta, psi] = dcm2angle(dcm);
    
    phi_arr(i) = phi * 180 / pi;
    theta_arr(i) = theta * 180 / pi;
    psi_arr(i) = psi * 180 / pi;

end

subplot(3, 1, 1)
plot(phi_arr)
hold on,
plot(-trajectoryData(:,6))
title("Calculated vs Actual Roll Angle")
legend("calculated", "actual")

subplot(3,1, 2)
plot(theta_arr)
hold on,
plot(-trajectoryData(:,5))
title("Calculated vs Actual Pitch Angle")

subplot(3,1, 3)
plot(psi_arr)
hold on,
plot(-trajectoryData(:,4))
title("Calculated vs Actual Yaw Angle")

