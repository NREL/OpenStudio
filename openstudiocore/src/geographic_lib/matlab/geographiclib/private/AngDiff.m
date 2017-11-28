function d = AngDiff(x, y)
%ANGDIFF  Compute angle difference accurately
%
%   d = ANGDIFF(x, y) computes y - x, reduces the result to (-180,180] and
%   rounds the result.  x and y must be in [-180,180].  x and y can be any
%   compatible shapes.

  [d, t] = sumx(AngNormalize(x), AngNormalize(-y));
  d = - AngNormalize(d);
  d(d == 180 & t < 0) = -180;
  d = d - t;
end
