data = table2array(readtable('convergenceData.csv'));

figure()
subplot(3, 1, 1)
plot(data(:,1))
title("Roll")


subplot(3,1, 2)
plot(data(:,2))
title("Pitch")


subplot(3,1, 3)
plot(data(:,3))
title("Yaw")