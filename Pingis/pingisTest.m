function [ a ] = pingisTest( port, P, I, D, b, r )

valuearr = importdata('ADCB4.txt');
a = serial(port,'BaudRate',115200);
set(a, 'Terminator', 10);
fopen(a);
pause(4);
flushinput(a);
flushoutput(a);


for k = 1:100
    fprintf(a, num2str(valuearr(k)));
end

matrixLiten = zeros(r, 3);
matrixPlott = rand(r*30, 3);


fprintf(a, num2str(P*1000));

fprintf(a, num2str(I*1000));

fprintf(a, num2str(D*1000));

fprintf(a, num2str(b));

fprintf(a, num2str(r));


%test
fgetl(a)
fgetl(a)
fgetl(a)
fgetl(a)
fgetl(a)
%slut p� test


while(1)
      for i = 1 : r  
          %fgetl(a)
          %fgetl(a);
          %fgetl(a);
          matrixLiten(1,1) = str2double(fgetl(a));
          matrixLiten(1,2) = str2double(fgetl(a));
          matrixLiten(1,3) = str2double(fgetl(a));
          matrixPlott = circshift(matrixLiten, 1);
      end
     matrixPlott = circshift(matrixPlott, r);
     matrixPlott(1:r,1:3) = matrixLiten;
     plot1 = plot(1:r*30, matrixPlott);
     legend('m�tv�rde', 'felv�rde', 'utv�rde')
     pause(0.5)
end

end
