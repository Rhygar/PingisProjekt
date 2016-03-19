function [ a ] = PingisTest( port, P, I, D, b, r )
%COM Summary of this function goes here
% port: the COM-port that arduino is connected to.
% P: Proportionality constant for regulation.
% I: Integration constant for regulation.
% D: Derivative constant for regulation.
% b: set value for regulation
% r: rate of execution for regulation task.
%   Detailed explanation goes here
valuearr = importdata('ADCB4.txt');
a = serial(port,'BaudRate',115200);
set(a, 'Terminator', 10);
fopen(a);
pause(4);
flushinput(a);
flushoutput(a);

for k = 1:100
    fprintf(a, valuearr(k));
end

fprintf(a, num2str(P * 1000) );

fprintf(a, num2str(I * 1000));

fprintf(a, num2str(D * 1000));

fprintf(a, num2str(b * 1000));

fprintf(a, num2str(r * 1000));

while(1)
    fgetl(a);
end

end

