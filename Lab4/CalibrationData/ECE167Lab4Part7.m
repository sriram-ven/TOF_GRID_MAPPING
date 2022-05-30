data = table2array(readtable('section7AxisData.csv'));

Primary_Accelerometer_data = data(:,1:3);
Secondary_Magnetometer_data = data(:,4:6);

figure();
scatter3(Primary_Accelerometer_data(:,1), Primary_Accelerometer_data(:,2), Primary_Accelerometer_data(:,3));
hold on;
scatter3(Secondary_Magnetometer_data(:,1), Secondary_Magnetometer_data(:,2), Secondary_Magnetometer_data(:,3));

mi = [0; 0; 1];
si = [Secondary_Magnetometer_data(1,1); Secondary_Magnetometer_data(1,2); Secondary_Magnetometer_data(1,3)];

Primary_Accelerometer_data = transpose(Primary_Accelerometer_data);
Secondary_Magnetometer_data = transpose(Secondary_Magnetometer_data);


[Rmis, Pbody] = AlignPrimarySecondary(Primary_Accelerometer_data, Secondary_Magnetometer_data, mi, si, eye(3))

Rmis = Rmis .* -1

e1 = -asin(Rmis(1,3))*(180/pi)
e2 = atan2(Rmis(2,3), Rmis(3,3))*(180/pi)
e3 = atan2(Rmis(1,2), Rmis(1,1)) *(180/pi)

accel = transpose(Primary_Accelerometer_data);
mag = Rmis * Secondary_Magnetometer_data;

mag = transpose(mag);

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
