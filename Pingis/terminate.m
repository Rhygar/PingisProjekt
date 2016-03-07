function [ output_args ] = terminate( a )

%--------------------------------------
% This function simply ends the program
% whenever it's called.
%--------------------------------------
fclose(a);
delete(a)
clear a
end

