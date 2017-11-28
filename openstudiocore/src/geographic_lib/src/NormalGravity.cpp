/**
 * \file NormalGravity.cpp
 * \brief Implementation for GeographicLib::NormalGravity class
 *
 * Copyright (c) Charles Karney (2011-2015) <charles@karney.com> and licensed
 * under the MIT/X11 License.  For more information, see
 * http://geographiclib.sourceforge.net/
 **********************************************************************/

#include <GeographicLib/NormalGravity.hpp>

#if defined(_MSC_VER)
// Squelch warnings about constant conditional expressions
#  pragma warning (disable: 4127)
#endif

namespace GeographicLib {

  using namespace std;

  NormalGravity::NormalGravity(real a, real GM, real omega, real f, real J2)
    : _a(a)
    , _GM(GM)
    , _omega(omega)
    , _f(f)
    , _J2(J2)
    , _omega2(Math::sq(_omega))
    , _aomega2(Math::sq(_omega * _a))
    {
      if (!(Math::isfinite(_a) && _a > 0))
        throw GeographicErr("Major radius is not positive");
      if (!(Math::isfinite(_GM) && _GM > 0))
        throw GeographicErr("Gravitational constant is not positive");
      if (!(_omega == 0 && _f == 0 && _J2 == 0)) {
        bool flatp = _f > 0 && Math::isfinite(_f);
        if (_J2 > 0 && Math::isfinite(_J2) && flatp)
          throw GeographicErr("Cannot specify both f and J2");
        if (!(_J2 > 0 && Math::isfinite(_J2)) && !flatp)
          throw GeographicErr("Must specify one of f and J2");
        if (!(Math::isfinite(_omega) && _omega != 0))
          throw GeographicErr("Angular velocity is not non-zero");
        if (flatp)
          _J2 = FlatteningToJ2(_a, _GM, _omega, _f);
        else
          _f = J2ToFlattening(_a, _GM, _omega, _J2);
      } // else we have a sphere: omega = f = J2 = 0
      _e2 = _f * (2 - _f);
      _ep2 = _e2 / (1 - _e2);
      _q0 = qf(_ep2);
      _earth = Geocentric(_a, _f);
      _b = _a * (1 - _f);
      _E = _a * sqrt(_e2);      // H+M, Eq 2-54
      // H+M, Eq 2-61
      _U0 = _GM / (_E ? _E / atan(sqrt(_ep2)) : _b) + _aomega2 / 3;
      // The approximate ratio of the centrifugal acceleration (at the equator)
      // to gravity.
      _m = _aomega2 * _b / _GM; // H+M, Eq 2-70
      real
        Q = _m * (_q0 ? sqrt(_ep2) * qpf(_ep2) / (3 * _q0) : 1),
        G = (1 - _m - Q / 2);
      _gammae = _GM / (_a * _b) * G;       // H+M, Eq 2-73
      _gammap = _GM / (_a * _a) * (1 + Q); // H+M, Eq 2-74
      // k = b * gammap / (a * gammae) - 1
      _k = (_m + 3 * Q / 2 - _e2 * (1 + Q)) / G;
      // f* = (gammap - gammae) / gammae
      _fstar = (_m + 3 * Q / 2 - _f * (1 + Q)) / G;
    }

  const NormalGravity& NormalGravity::WGS84() {
    static const NormalGravity wgs84(Constants::WGS84_a(),
                                     Constants::WGS84_GM(),
                                     Constants::WGS84_omega(),
                                     Constants::WGS84_f(), 0);
    return wgs84;
  }

  const NormalGravity& NormalGravity::GRS80() {
    static const NormalGravity grs80(Constants::GRS80_a(),
                                     Constants::GRS80_GM(),
                                     Constants::GRS80_omega(),
                                     0, Constants::GRS80_J2());
    return grs80;
  }

  // (atan(y)-(y-y^3/3+y^5/5))/y^7 (y = sqrt(x)) = -1/7+x/9-x^2/11+x^3/13...
  Math::real NormalGravity::atan7(real x) {
    if (abs(x) >= real(0.5)) {
      real y = sqrt(abs(x)), x2 = Math::sq(x);
      return ((x > 0 ? atan(y) : Math::atanh(y))- y * (1 - x / 3 + x2 / 5)) /
        (x * x2 * y);
    } else {
      real xn = -1, q = 0;
      for (int n = 7; ; n += 2) {
        real qn = q + xn / n;
        if (qn == q)
          break;
        q = qn;
        xn *= -x;
      }
      return q;
    }
  }

  // (atan(y)-(y-y^3/3))/y^5 (y = sqrt(x)) = 1/5-x/7+x^2/9-x^3/11...
  Math::real NormalGravity::atan5(real x)
  { return 1/real(5) + x * atan7(x); }

  Math::real NormalGravity::qf(real ep2) {
    // Compute
    //
    //   ((1 + 3/e'^2) * atan(e') - 3/e')/2
    //
    // See H+M, Eq 2-57, with E/u = e'.  This suffers from two levels of
    // cancelation.  The e'^-1 and e'^1 terms drop out, so that the leading
    // term is O(e'^3).  Substitute atan(e') = e' - e'^3/3 + e'^5*atan5(e'^2)
    return sqrt(ep2) * ep2 * (3 * (3 + ep2) * atan5(ep2) - 1) / 6;
  }

  Math::real NormalGravity::dq(real ep2) {
    // Compute d qf(ep2) / d ep2 and substitute
    // atan(e') = e' - e'^3/3 + e'^5/5 + e'^7*atan7(e'^2)
    return sqrt(ep2) * (5 - 3 * (1 + ep2) * (1 + 5 * ep2 * atan7(ep2))) /
      (10 * (1 + ep2));
  }

  Math::real NormalGravity::qpf(real ep2) {
    // Compute
    //
    //   3*(1 + 1/e'^2) * (1 - atan(e')/e') - 1
    //
    // See H+M, Eq 2-67, with E/u = e'.  This suffers from two levels of
    // cancelation.  The e'^-2 and e'^0 terms drop out, so that the leading
    // term is O(e'^2).
    return ep2 * (1 - 3 * (1 + ep2) * atan5(ep2));
  }

  Math::real NormalGravity::Jn(int n) const {
    // Note Jn(0) = -1; Jn(2) = _J2; Jn(odd) = 0
    if (n & 1 || n < 0)
      return 0;
    n /= 2;
    real e2n = 1;            // Perhaps this should just be e2n = pow(-_e2, n);
    for (int j = n; j--;)
      e2n *= -_e2;
    return                      // H+M, Eq 2-92
      -3 * e2n * ((1 - n) + 5 * n * _J2 / _e2) / ((2 * n + 1) * (2 * n + 3));
  }

  Math::real NormalGravity::SurfaceGravity(real lat) const {
    real sphi = Math::sind(Math::LatFix(lat));
    // H+M, Eq 2-78
    return _gammae * (1 + _k * Math::sq(sphi)) / sqrt(1 - _e2 * Math::sq(sphi));
  }

  Math::real NormalGravity::V0(real X, real Y, real Z,
                               real& GammaX, real& GammaY, real& GammaZ)
    const {
    // See H+M, Sec 6-2
    real
      p = Math::hypot(X, Y),
      clam = p ? X/p : 1,
      slam = p ? Y/p : 0,
      r = Math::hypot(p, Z),
      Q = Math::sq(r) - Math::sq(_E),
      t2 = Math::sq(2 * _E * Z),
      disc = sqrt(Math::sq(Q) + t2),
      // This is H+M, Eq 6-8a, but generalized to deal with Q negative
      // accurately.
      u = sqrt((Q >= 0 ? (Q + disc) : t2 / (disc - Q)) / 2),
      uE = Math::hypot(u, _E),
      // H+M, Eq 6-8b
      sbet = Z * uE,
      cbet = p * u,
      s = Math::hypot(cbet, sbet);
    cbet = s ? cbet/s : 0;
    sbet = s ? sbet/s : 1;
    real
      invw = uE / Math::hypot(u, _E * sbet), // H+M, Eq 2-63
      ep = _E/u,
      ep2 = Math::sq(ep),
      q = _q0 ? qf(ep2) / _q0 : pow(_a / u, 3),
      qp = qpf(ep2) / _q0,
      // H+M, Eqs 2-62 + 6-9, but omitting last (rotational) term .
      Vres = (_GM / (_E ? _E / atan(_E / u) : u)
              + _aomega2 * q * (Math::sq(sbet) - 1/real(3)) / 2),
      // H+M, Eq 6-10
      gamu = - invw * (_GM
                       + (_aomega2 * (_q0 ? _E * qp : 3 * q * u)
                          * (Math::sq(sbet) - 1/real(3)) / 2)) / Math::sq(uE),
      gamb = _aomega2 * q * sbet * cbet * invw / uE,
      t = u * invw / uE;
    // H+M, Eq 6-12
    GammaX = t * cbet * gamu - invw * sbet * gamb;
    GammaY = GammaX * slam;
    GammaX *= clam;
    GammaZ = invw * sbet * gamu + t * cbet * gamb;
    return Vres;
  }

  Math::real NormalGravity::Phi(real X, real Y, real& fX, real& fY)
    const {
    fX = _omega2 * X;
    fY = _omega2 * Y;
    // N.B. fZ = 0;
    return _omega2 * (Math::sq(X) + Math::sq(Y)) / 2;
  }

  Math::real NormalGravity::U(real X, real Y, real Z,
                              real& gammaX, real& gammaY, real& gammaZ)
    const {
    real fX, fY;
    real Ures = V0(X, Y, Z, gammaX, gammaY, gammaZ) + Phi(X, Y, fX, fY);
    gammaX += fX;
    gammaY += fY;
    return Ures;
  }

  Math::real NormalGravity::Gravity(real lat, real h,
                                    real& gammay, real& gammaz)
    const {
    real X, Y, Z;
    real M[Geocentric::dim2_];
    _earth.IntForward(lat, 0, h, X, Y, Z, M);
    real gammaX, gammaY, gammaZ,
      Ures = U(X, Y, Z, gammaX, gammaY, gammaZ);
    // gammax = M[0] * gammaX + M[3] * gammaY + M[6] * gammaZ;
    gammay = M[1] * gammaX + M[4] * gammaY + M[7] * gammaZ;
    gammaz = M[2] * gammaX + M[5] * gammaY + M[8] * gammaZ;
    return Ures;
  }

  Math::real NormalGravity::J2ToFlattening(real a, real GM,
                                           real omega, real J2) {
    real
      K = 2 * Math::sq(a * omega) * a / (15 * GM),
      e2 = 3 * J2;              // See Moritz (1980), p 398.
    // Solve using Newton's method
    for (int j = 0; j < maxit_ || GEOGRAPHICLIB_PANIC; ++j) {
      real e2a = e2,
        ep2 = e2 / (1 - e2),
        q0 = qf(ep2),
        dq0 = dq(ep2) / Math::sq(1 - e2),
        h = e2 * (1 - sqrt(e2) * K / q0) - 3 * J2,
        dh = 1 - sqrt(e2) * K * (3 * q0 - 2 * e2 * dq0) / (2 * Math::sq(q0)),
        de2 = - h / dh;
      e2 = e2a + de2;
      if (e2 == e2a)
        break;
    }
    return e2 / (1 + sqrt(1 - e2));
  }

  Math::real NormalGravity::FlatteningToJ2(real a, real GM,
                                           real omega, real f) {
    real
      K = 2 * Math::sq(a * omega) * a / (15 * GM),
      e2 = f * (2 - f),
      q0 = qf(e2 / (1 - e2));
    return e2 * (1 - K * sqrt(e2) / q0) / 3; // H+M, Eq 2-90
  }

} // namespace GeographicLib
