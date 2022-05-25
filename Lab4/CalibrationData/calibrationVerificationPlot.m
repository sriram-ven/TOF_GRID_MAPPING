data = table2array(readtable('calibrationVerificationData.csv'));

accel = data(:,1:3);
mag = data(:,4:6);

figure()
scatter3(accel(:,1), accel(:,2), accel(:,3))
hold on
scatter3(mag(:,1), mag(:,2), mag(:,3))