data = table2array(readtable('section7VerificationData.csv'));

accel = data(:,1:3);
mag = data(:,4:6);

figure()
scatter3(accel(:,1), accel(:,2), accel(:,3))
hold on
scatter3(mag(:,1), mag(:,2), mag(:,3))

figure()
subplot(3, 1, 1)
plot(accel(:,1))
hold on
plot(mag(:,1))
title("Roll")

subplot(3,1, 2)
plot(accel(:,2))
hold on
plot(mag(:,2))
title("Pitch")

subplot(3,1, 3)
plot(accel(:,3))
hold on
plot(mag(:,3))
title("Yaw")