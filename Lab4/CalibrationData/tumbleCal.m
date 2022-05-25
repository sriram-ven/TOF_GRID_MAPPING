data = table2array(readtable('tumbleData.csv'));

accel = data(:,1:3);
mag = data(:,4:6);

figure()
scatter3(accel(:,1), accel(:,2), accel(:,3));
axis equal

figure()
scatter3(mag(:,1), mag(:,2), mag(:,3));
axis equal

[Aaccel, Baccel] = CalibrateEllipsoidData3D(accel(:,1), accel(:,2), accel(:,3), 2, 0);
[Amag, Bmag] = CalibrateEllipsoidData3D(mag(:,1), mag(:,2), mag(:,3), 2, 0);