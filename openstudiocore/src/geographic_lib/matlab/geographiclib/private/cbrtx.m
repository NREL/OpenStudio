function y = cbrtx(x)
%CBRTX   The real cube root
%
%   CBRTX(x) is the real cube root of x (assuming x is real).  x
%   can be any shape.

  y = abs(x).^(1/3);
  y(x < 0) = -y(x < 0);
end
