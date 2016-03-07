%-----------------------------------------------
% Author: Andreas Langhammer and John Tengvall
% Date: 2016-03-06
%
% Main program.
% Handles all communacation between matlab and
% Arduino Due.
% Structure so far, Statemachine
% with states to either send or receive data
% from the Arduino Due, via serial communication.
%------------------------------------------------
function [] = pingis()

%-------------------------------------------------
% FOR THE USER
% Values to set: PID-values, Setpoint-value(Dist),
% Timer and the PlottingTime.
%-------------------------------------------------
P = 1;
I = 0;
D = 0;
% Ball's distance to dictanse sensor [mm]
Dist = 300;
Timer = 50;
PlottingTime = 20;
%-------------------------------------------------
% Sets up a serial communication to arduino.
%
% Nesiserry variables declared, setting
% states on statemachine and calculating values
% that is used to regulate the ball and plot the
% result.
%-------------------------------------------------
% create serial communication object on port.
a = serial(comport, 'BaudRate', 115200); 
set(a, 'Terminator', 10);
fopen(a);
disp('Arduino connected');

%-------------------------------------------------
RUNNING = 1;
SEND = 1;
RECEIVE = 0;
flushinput(a);
% read in values from chart
ADCValue = importdata('ADCB4.txt');
ADCindex = 1;

%-------------------------------------------------
% frequency calculated
Freq = 1000/Timer;
% Big matrix for holding alla values for final plot
ValueMatrix = rand(Freq*PlotingTime,3);
% Smaler matrix for holding
DataValueMatrix = zeros(Freq,3);
x = 1:(Freq * PlottingTime);

%-------------------------------------------------
%Prepare plot and setting up lables and tiltle.
%-------------------------------------------------
Title = 'pingis';    % title of the plot
xLabel = 'Time';     % x label
yLabel = 'Distance'; % y label
title(Title,'FontSize',20);
xlabel(xLabel,'FontSize',12);
ylabel(yLabel,'FontSize',12);

%-------------------------------------------------
% Statemachine used to determine if we
% should send data or receive data.
%-------------------------------------------------
while (RUNNING)
    %-------------------------------------------------
    % Send data to arduino
    % Each ADCindex value sent by it's own.
    %-------------------------------------------------
    while(SEND)
        %flush serial output buffer before sending.
        flushoutput(a);
        while(ADCindex < 101)
            % number from ADCB4.txt to string
            str = num2str(ADCValue(ADCindex));
            %print string from object
            fprint(a, str);
            ADCindex = ADCindex + 1;
            %flush buffer again
            flushoutput(a);
        end
        SEND = 0;
        RECEIVE = 1;
    end
    %-------------------------------------------------
    % Writing values to arduino
    %-------------------------------------------------
    fprintf(a, num2str(P));
    fprintf(a, num2str(I));
    fprintf(a, num2str(D));
    fprintf(a, num2str(Dist));
    fprintf(a, num2str(Timer));
    
    %-------------------------------------------------
    % Receive data from arduino
    %-------------------------------------------------
    while(RECEIVE)
        disp(['Arduino says: ', DataValueMatrix]);
        %-------------------------------------------------
        % First reading values from object then
        % inserting them in DataValueMatrix as doubles
        % insted of strings.
        %-------------------------------------------------
        Temp = 1;
        CSconst = 1;
        while(Temp < Freq)
            % Distance data
            DataValueMatrix(1,1) = str2double(fscan(a, '%c'));
            % error data
            DataValueMatrix(1,2) = str2double(fscan(a, '%c'));
            % control value data
            DataValueMatrix(1,3) = str2double(fscan(a, '%c'));
            % circshift, circularly shifts the elements in matrix
            % by CSconst elements.
            DataValueMatrix = circshift(DataValueMatrix,CSconst);
            Temp = Temp + 1;
        end
        % write out the values from the DataValueMatrix.
        disp(DataValueMatrix);
        ValueMatrix = circshift(ValueMatrix, Freq);
        ValueMatrix(1:Freq, 1:3) = DataValueMatrix;
        plot(x,ValueMatrix);
        legend('Distance', 'Err', 'Control-Value');
        pause(1);
    end % End of receive.
    
    % Plot the finished graph.
    plot(x,ValueMatrix);
    legend('Distance', 'Err', 'Control-Value');
end
end


