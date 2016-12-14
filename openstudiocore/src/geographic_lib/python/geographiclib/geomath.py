"""geomath.py: transcription of GeographicLib::Math class."""
# geomath.py
#
# This is a rather literal translation of the GeographicLib::Math class to
# python.  See the documentation for the C++ class for more information at
#
#    http://geographiclib.sourceforge.net/html/annotated.html
#
# Copyright (c) Charles Karney (2011-2015) <charles@karney.com> and
# licensed under the MIT/X11 License.  For more information, see
# http://geographiclib.sourceforge.net/
######################################################################

import sys
import math

class Math(object):
  """
  Additional math routines for GeographicLib.

  This defines constants:
    epsilon, difference between 1 and the next bigger number
    digits, the number of digits in the fraction of a real number
    minval, minimum normalized positive number
    maxval, maximum finite number
    nan, not a number
    inf, infinity
  """

  digits = 53
  epsilon = math.pow(2.0, 1-digits)
  minval = math.pow(2.0, -1022)
  maxval = math.pow(2.0, 1023) * (2 - epsilon)
  inf = float("inf") if sys.version_info > (2, 6) else 2 * maxval
  nan = float("nan") if sys.version_info > (2, 6) else inf - inf

  def sq(x):
    """Square a number"""

    return x * x
  sq = staticmethod(sq)

  def cbrt(x):
    """Real cube root of a number"""

    y = math.pow(abs(x), 1/3.0)
    return y if x >= 0 else -y
  cbrt = staticmethod(cbrt)

  def log1p(x):
    """log(1 + x) accurate for small x (missing from python 2.5.2)"""

    if sys.version_info > (2, 6):
      return math.log1p(x)

    y = 1 + x
    z = y - 1
    # Here's the explanation for this magic: y = 1 + z, exactly, and z
    # approx x, thus log(y)/z (which is nearly constant near z = 0) returns
    # a good approximation to the true log(1 + x)/x.  The multiplication x *
    # (log(y)/z) introduces little additional error.
    return x if z == 0 else x * math.log(y) / z
  log1p = staticmethod(log1p)

  def atanh(x):
    """atanh(x) (missing from python 2.5.2)"""

    if sys.version_info > (2, 6):
      return math.atanh(x)

    y = abs(x)                  # Enforce odd parity
    y = Math.log1p(2 * y/(1 - y))/2
    return -y if x < 0 else y
  atanh = staticmethod(atanh)

  def norm(x, y):
    """Private: Normalize a two-vector."""
    r = math.hypot(x, y)
    return x/r, y/r
  norm = staticmethod(norm)

  def sum(u, v):
    """Error free transformation of a sum."""
    # Error free transformation of a sum.  Note that t can be the same as one
    # of the first two arguments.
    s = u + v
    up = s - v
    vpp = s - up
    up -= u
    vpp -= v
    t = -(up + vpp)
    # u + v =       s      + t
    #       = round(u + v) + t
    return s, t
  sum = staticmethod(sum)

  def polyval(N, p, s, x):
    """Evaluate a polynomial."""
    y = float(0 if N < 0 else p[s]) # make sure the returned value is a float
    while N > 0:
      N -= 1; s += 1
      y = y * x + p[s]
    return y
  polyval = staticmethod(polyval)

  def AngRound(x):
    """Private: Round an angle so that small values underflow to zero."""
    # The makes the smallest gap in x = 1/16 - nextafter(1/16, 0) = 1/2^57
    # for reals = 0.7 pm on the earth if x is an angle in degrees.  (This
    # is about 1000 times more resolution than we get with angles around 90
    # degrees.)  We use this to avoid having to deal with near singular
    # cases when x is non-zero but tiny (e.g., 1.0e-200).
    z = 1/16.0
    y = abs(x)
    # The compiler mustn't "simplify" z - (z - y) to y
    if y < z: y = z - (z - y)
    return 0 - y if x < 0 else y
  AngRound = staticmethod(AngRound)

  def AngNormalize(x):
    """reduce angle to [-180,180)"""

    x = math.fmod(x, 360)
    return (x + 360 if x < -180 else
            (x if x < 180 else x - 360))
  AngNormalize = staticmethod(AngNormalize)

  def LatFix(x):
    """replace angles outside [-90,90] by NaN"""

    return Math.nan if abs(x) > 90 else x
  LatFix = staticmethod(LatFix)

  def AngDiff(x, y):
    """compute y - x and reduce to [-180,180] accurately"""

    d, t = Math.sum(Math.AngNormalize(x), Math.AngNormalize(-y))
    d = - Math.AngNormalize(d)
    return (-180 if d == 180 and t < 0 else d) - t
  AngDiff = staticmethod(AngDiff)

  def sincosd(x):
    """Compute sine and cosine of x in degrees."""

    r = math.fmod(x, 360)
    q = Math.nan if Math.isnan(r) else int(math.floor(r / 90 + 0.5))
    r -= 90 * q; r = math.radians(r)
    s = math.sin(r); c = math.cos(r)
    q = q % 4
    if q == 1:
      s, c =   c, 0-s
    elif q == 2:
      s, c = 0-s, 0-c
    elif q == 3:
      s, c = 0-c,   s
    return s, c
  sincosd = staticmethod(sincosd)

  def atan2d(y, x):
    """compute atan2(y, x) with the result in degrees"""

    if abs(y) > abs(x):
      q = 2; x, y = y, x
    else:
      q = 0
    if x < 0:
      q += 1; x = -x
    ang = math.degrees(math.atan2(y, x))
    if q == 1:
      ang = (180 if y > 0 else -180) - ang
    elif q == 2:
      ang =  90 - ang
    elif q == 3:
      ang = -90 + ang
    return ang
  atan2d = staticmethod(atan2d)

  def isfinite(x):
    """Test for finiteness"""

    return abs(x) <= Math.maxval
  isfinite = staticmethod(isfinite)

  def isnan(x):
    """Test if nan"""

    return math.isnan(x) if sys.version_info > (2, 6) else x != x
  isnan = staticmethod(isnan)
