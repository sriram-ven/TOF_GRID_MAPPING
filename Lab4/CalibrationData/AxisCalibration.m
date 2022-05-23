[Ap, Bp] = CalibrateEllipsoidData3D(data(:,1), data(:,2), data(:,3), 20, 0);
[As, Bs] = CalibrateEllipsoidData3D(data(:,1), data(:,2), data(:,3), 20, 0);



[Xp_corr, Yp_corr, Zp_corr] = CorrectEllipsoidData3D(accelData(:,1),accelData(:,2),accelData(:,3),Ap,Bp);
[Xs_corr, Ys_corr, Zs_corr] = CorrectEllipsoidData3D(magData(:,1),magData(:,2),magData(:,3),As,Bs);

figure();
scatter3(Xp_corr, Yp_corr, Zp_corr);
hold on;
scatter3(Xs_corr, Ys_corr, Zs_corr);

pi = [0; 0; -1];
si = [0.4779; 0.1118; 0.8713];

[Rmis, Pbody] = AlignPrimarySecondary()
