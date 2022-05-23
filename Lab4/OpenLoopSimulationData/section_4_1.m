dcm = angle2dcm( Eul(1, 1), Eul(1, 2), Eul(1, 3 ));
x_error = zeros();
y_error = zeros();
z_error = zeros();

wGyro = wGyro ./ GscaleFactor;

for i = 1:length(Eul)
    currGyro = [wGyro(i,1); wGyro(i,2) ; wGyro(i,3)];
    dcm = IntegrateOpenLoop(dcm, currGyro, 0.02);
    [phi, theta, psi] = dcm2angle(dcm);
    x_error(i) = Eul(i, 1) - phi;
    y_error(i) = Eul(i, 2) - theta;
    z_error(i) = Eul(i, 3) - psi;
    
end

figure()
subplot(3, 1, 1)
plot(x_error)
subplot(3,1, 2)
plot(y_error)
subplot(3,1, 3)
plot(z_error)