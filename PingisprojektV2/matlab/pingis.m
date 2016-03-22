%------------------------------------------------%
% Author: Andreas Langhammer and John Tengvall   %
% Date: 2016-03-06                               %                        
% Main program.                                  %
% Handles all communacation between matlab and
% Arduino Due.
% Structure so far, Statemachine
% with states to either send or receive data
% from the Arduino Due, via serial communication.
%-------------------------------------------------%
function [] = pingis()
delete(instrfind)
%-------------------------------------------------%
% FOR THE USER                                    %
% Values to set: PID-values, Setpoint-value(Dist),%
% Timer and the PlottingTime.                     %
%-------------------------------------------------%
P = 1.05;
I = 1.1242;
D = 0.2774;
% Ball's distance to dictanse sensor [mm]
Setpoint = 300;
CalcFreq = 20;
PlottingTime = 20;
%-------------------------------------------------%
% Sets up a serial communication to arduino.      %
%                                                 %
% Necessery variables declared, setting           %
% states on statemachine and calculating values   %
% that is used to regulate the ball and plot the  %
% result.                                         %
%-------------------------------------------------%
% create serial communication object on port.
a = serial('com14', 'BaudRate', 115200); 
set(a, 'Terminator', 10);
fopen(a);
disp('Arduino connected');

%------------------------------------------------%

flushinput(a);
% read in values from chart
ADCValue = importdata('ADCB4.txt');
% ADCindex = 1;
pause(4)
%-------------------------------------------------%
% frequency calculated
Freq = 1000/CalcFreq;
% Big matrix for holding all values for final plot
ValueMatrix = rand(Freq*PlottingTime,3);
% Smaller matrix for holding
DataValueMatrix = zeros(Freq,3);
x = 1:(Freq * PlottingTime);
% Setpoint
ValueMatrix(:,4) = ones(Freq*PlottingTime,1).*Setpoint;

%--------------------------------------------------%
RUNNING = 1;
SEND = 1;               
RECEIVE = 0;                                       
%--------------------------------------------------%
% Statemachine used to determine if we             %
% should send data or receive data.                %
%--------------------------------------------------%


while (RUNNING)
    %----------------------------------------------%
    % Send data to arduino                         %
    % Each ADCindex value sent by it's own.        %
    %----------------------------------------------%
    while(SEND)
        %flush serial output buffer before sending.
        flushoutput(a);
        flushinput(a);
        for k = 1:100
            fprintf(a, num2str(ADCValue(k)));
        end
    %-----------------------------------------------%
    % Writing values to arduino                     %
    %-----------------------------------------------%
    fprintf(a, num2str(P*1000));
    fprintf(a, num2str(I*1000));
    fprintf(a, num2str(D*1000));
    fprintf(a, num2str(Setpoint));
    fprintf(a, num2str(Freq)); 
    
    SEND = 0;
    RECEIVE = 1;
    end
    
    %-------------------------------------------------%
    % Receive data from arduino                       %
    %-------------------------------------------------%
    while(RECEIVE)
        
        %-------------------------------------------------%
        % First reading values from object then           %
        % inserting them in DataValueMatrix as doubles    %
        % insted of strings.                              %
        %-------------------------------------------------%
        Temp = 1;
        while(Temp <= Freq)
        
            % circshift, circularly shifts the elements in matrix
            DataValueMatrix = circshift(DataValueMatrix,1);
            % Distance data
            DataValueMatrix(1,1) = str2double(fgetl(a));
            % error data
            DataValueMatrix(1,2) = str2double(fgetl(a));
            % control value data
            DataValueMatrix(1,3) = str2double(fgetl(a));
            
            Temp = Temp + 1;
        end
        % write out the values from the DataValueMatrix.
        disp(DataValueMatrix);
        ValueMatrix = circshift(ValueMatrix, Freq);
        ValueMatrix(1:Freq, 1:3) = DataValueMatrix;
        plot(x,ValueMatrix)
        legend('Distance', 'Err', 'Control-Value','Setpoint')
        xlabel =('Time');     % x label
        ylabel =('Distance'); % y label
        title('pingis')
       
        pause(1)
    end % End of receive.
end
end


