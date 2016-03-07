a = arduino_com('COM6');

analog_data = zeros(1, 100);

procent = 1:100;

for k=1:100    
    analogWrite(a, round(k*2.55),'DAC0');
    display(k)
    display('Tryck enter')
    pause()
    sum = 0;
    for m=1:100
        sum = sum + analogRead(a,'A0');
    end
    analog_data(k) = sum/100;
end

plot(procent, analog_data)
grid on
xlswrite('linjariseringsresultat.xlsx',analog_data);

delete(a)