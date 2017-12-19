cd /Users/GustavoMV/Documents/ScootPlayer/FreePass/1513542893/report
M = csvread('download-tratado.txt');
A = transpose(M);
x = unique(A);
N = numel(x);

count = zeros(N,1);

for k = 1:N
 count(k) = sum(A==x(k));
end
disp([ x(:) count ]);