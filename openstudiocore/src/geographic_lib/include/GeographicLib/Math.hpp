/**
 * \file Math.hpp
 * \brief Header for GeographicLib::Math class
 *
 * Copyright (c) Charles Karney (2008-2015) <charles@karney.com> and licensed
 * under the MIT/X11 License.  For more information, see
 * http://geographiclib.sourceforge.net/
 **********************************************************************/

// Constants.hpp includes Math.hpp.  Place this include outside Math.hpp's
// include guard to enforce this ordering.
#include <GeographicLib/Constants.hpp>

#if !defined(GEOGRAPHICLIB_MATH_HPP)
#define GEOGRAPHICLIB_MATH_HPP 1

/**
 * Are C++11 math functions available?
 **********************************************************************/
#if !defined(GEOGRAPHICLIB_CXX11_MATH)
// Recent versions of g++ -std=c++11 (4.7 and later?) set __cplusplus to 201103
// and support the new C++11 mathematical functions, std::atanh, etc.  However
// the Android toolchain, which uses g++ -std=c++11 (4.8 as of 2014-03-11,
// according to Pullan Lu), does not support std::atanh.  Android toolchains
// might define __ANDROID__ or ANDROID; so need to check both.  With OSX the
// version is GNUC version 4.2 and __cplusplus is set to 201103, so remove the
// version check on GNUC.
#  if defined(__GNUC__) && __cplusplus >= 201103 && \
  !(defined(__ANDROID__) || defined(ANDROID) || defined(__CYGWIN__))
#    define GEOGRAPHICLIB_CXX11_MATH 1
// Visual C++ 12 supports these functions
#  elif defined(_MSC_VER) && _MSC_VER >= 1800
#    define GEOGRAPHICLIB_CXX11_MATH 1
#  else
#    define GEOGRAPHICLIB_CXX11_MATH 0
#  endif
#endif

#if !defined(GEOGRAPHICLIB_WORDS_BIGENDIAN)
#  define GEOGRAPHICLIB_WORDS_BIGENDIAN 0
#endif

#if !defined(GEOGRAPHICLIB_HAVE_LONG_DOUBLE)
#  define GEOGRAPHICLIB_HAVE_LONG_DOUBLE 0
#endif

#if !defined(GEOGRAPHICLIB_PRECISION)
/**
 * The precision of floating point numbers used in %GeographicLib.  1 means
 * float (single precision); 2 (the default) means double; 3 means long double;
 * 4 is reserved for quadruple precision.  Nearly all the testing has been
 * carried out with doubles and that's the recommended configuration.  In order
 * for long double to be used, GEOGRAPHICLIB_HAVE_LONG_DOUBLE needs to be
 * defined.  Note that with Microsoft Visual Studio, long double is the same as
 * double.
 **********************************************************************/
#  define GEOGRAPHICLIB_PRECISION 2
#endif

#include <cmath>
#include <algorithm>
#include <limits>

#if GEOGRAPHICLIB_PRECISION == 4
#include <boost/version.hpp>
#if BOOST_VERSION >= 105600
#include <boost/cstdfloat.hpp>
#endif
#include <boost/multiprecision/float128.hpp>
#include <boost/math/special_functions.hpp>
__float128 fmaq(__float128, __float128, __float128);
#elif GEOGRAPHICLIB_PRECISION == 5
#include <mpreal.h>
#endif

#if GEOGRAPHICLIB_PRECISION > 3
// volatile keyword makes no sense for multiprec types
#define GEOGRAPHICLIB_VOLATILE
// Signal a convergence failure with multiprec types by throwing an exception
// at loop exit.
#define GEOGRAPHICLIB_PANIC \
  (throw GeographicLib::GeographicErr("Convergence failure"), false)
#else
#define GEOGRAPHICLIB_VOLATILE volatile
// Ignore convergence failures with standard floating points types by allowing
// loop to exit cleanly.
#define GEOGRAPHICLIB_PANIC false
#endif

namespace GeographicLib {

  /**
   * \brief Mathematical functions needed by %GeographicLib
   *
   * Define mathematical functions in order to localize system dependencies and
   * to provide generic versions of the functions.  In addition define a real
   * type to be used by %GeographicLib.
   *
   * Example of use:
   * \include example-Math.cpp
   **********************************************************************/
  class GEOGRAPHICLIB_EXPORT Math {
  private:
    void dummy() {
      GEOGRAPHICLIB_STATIC_ASSERT(GEOGRAPHICLIB_PRECISION >= 1 &&
                                  GEOGRAPHICLIB_PRECISION <= 5,
                                  "Bad value of precision");
    }
    Math();                     // Disable constructor
  public:

#if GEOGRAPHICLIB_HAVE_LONG_DOUBLE
    /**
     * The extended precision type for real numbers, used for some testing.
     * This is long double on computers with this type; otherwise it is double.
     **********************************************************************/
    typedef long double extended;
#else
    typedef double extended;
#endif

#if GEOGRAPHICLIB_PRECISION == 2
    /**
     * The real type for %GeographicLib. Nearly all the testing has been done
     * with \e real = double.  However, the algorithms should also work with
     * float and long double (where available).  (<b>CAUTION</b>: reasonable
     * accuracy typically cannot be obtained using floats.)
     **********************************************************************/
    typedef double real;
#elif GEOGRAPHICLIB_PRECISION == 1
    typedef float real;
#elif GEOGRAPHICLIB_PRECISION == 3
    typedef extended real;
#elif GEOGRAPHICLIB_PRECISION == 4
    typedef boost::multiprecision::float128 real;
#elif GEOGRAPHICLIB_PRECISION == 5
    typedef mpfr::mpreal real;
#else
    typedef double real;
#endif

    /**
     * @return the number of bits of precision in a real number.
     **********************************************************************/
    static inline int digits() {
#if GEOGRAPHICLIB_PRECISION != 5
      return std::numeric_limits<real>::digits;
#else
      return std::numeric_limits<real>::digits();
#endif
    }

    /**
     * Set the binary precision of a real number.
     *
     * @param[in] ndigits the number of bits of precision.
     * @return the resulting number of bits of precision.
     *
     * This only has an effect when GEOGRAPHICLIB_PRECISION == 5.  See also
     * Utility::set_digits for caveats about when this routine should be
     * called.
     **********************************************************************/
    static inline int set_digits(int ndigits) {
#if GEOGRAPHICLIB_PRECISION != 5
      (void)ndigits;
#else
      mpfr::mpreal::set_default_prec(ndigits >= 2 ? ndigits : 2);
#endif
      return digits();
    }

    /**
     * @return the number of decimal digits of precision in a real number.
     **********************************************************************/
    static inline int digits10() {
#if GEOGRAPHICLIB_PRECISION != 5
      return std::numeric_limits<real>::digits10;
#else
      return std::numeric_limits<real>::digits10();
#endif
    }

    /**
     * Number of additional decimal digits of precision for real relative to
     * double (0 for float).
     **********************************************************************/
    static inline int extra_digits() {
      return
        digits10() > std::numeric_limits<double>::digits10 ?
        digits10() - std::numeric_limits<double>::digits10 : 0;
    }

#if GEOGRAPHICLIB_PRECISION <= 3
    /**
     * Number of additional decimal digits of precision of real relative to
     * double (0 for float).
     *
     * <b>DEPRECATED</b>: use extra_digits() instead
     **********************************************************************/
    static const int extradigits =
      std::numeric_limits<real>::digits10 >
      std::numeric_limits<double>::digits10 ?
      std::numeric_limits<real>::digits10 -
      std::numeric_limits<double>::digits10 : 0;
#endif

    /**
     * true if the machine is big-endian.
     **********************************************************************/
    static const bool bigendian = GEOGRAPHICLIB_WORDS_BIGENDIAN;

    /**
     * @tparam T the type of the returned value.
     * @return &pi;.
     **********************************************************************/
    template<typename T> static inline T pi() {
      using std::atan2;
      static const T pi = atan2(T(0), T(-1));
      return pi;
    }
    /**
     * A synonym for pi<real>().
     **********************************************************************/
    static inline real pi() { return pi<real>(); }

    /**
     * @tparam T the type of the returned value.
     * @return the number of radians in a degree.
     **********************************************************************/
    template<typename T> static inline T degree() {
      static const T degree = pi<T>() / 180;
      return degree;
    }
    /**
     * A synonym for degree<real>().
     **********************************************************************/
    static inline real degree() { return degree<real>(); }

    /**
     * Square a number.
     *
     * @tparam T the type of the argument and the returned value.
     * @param[in] x
     * @return <i>x</i><sup>2</sup>.
     **********************************************************************/
    template<typename T> static inline T sq(T x)
    { return x * x; }

    /**
     * The hypotenuse function avoiding underflow and overflow.
     *
     * @tparam T the type of the arguments and the returned value.
     * @param[in] x
     * @param[in] y
     * @return sqrt(<i>x</i><sup>2</sup> + <i>y</i><sup>2</sup>).
     **********************************************************************/
    template<typename T> static inline T hypot(T x, T y) {
#if GEOGRAPHICLIB_CXX11_MATH
      using std::hypot; return hypot(x, y);
#else
      using std::abs; using std::sqrt;
      x = abs(x); y = abs(y);
      if (x < y) std::swap(x, y); // Now x >= y >= 0
      y /= (x ? x : 1);
      return x * sqrt(1 + y * y);
      // For an alternative (square-root free) method see
      // C. Moler and D. Morrision (1983) https://dx.doi.org/10.1147/rd.276.0577
      // and A. A. Dubrulle (1983) https://dx.doi.org/10.1147/rd.276.0582
#endif
    }

    /**
     * exp(\e x) &minus; 1 accurate near \e x = 0.
     *
     * @tparam T the type of the argument and the returned value.
     * @param[in] x
     * @return exp(\e x) &minus; 1.
     **********************************************************************/
    template<typename T> static inline T expm1(T x) {
#if GEOGRAPHICLIB_CXX11_MATH
      using std::expm1; return expm1(x);
#else
      using std::exp; using std::abs; using std::log;
      GEOGRAPHICLIB_VOLATILE T
        y = exp(x),
        z = y - 1;
      // The reasoning here is similar to that for log1p.  The expression
      // mathematically reduces to exp(x) - 1, and the factor z/log(y) = (y -
      // 1)/log(y) is a slowly varying quantity near y = 1 and is accurately
      // computed.
      return abs(x) > 1 ? z : (z == 0 ? x : x * z / log(y));
#endif
    }

    /**
     * log(1 + \e x) accurate near \e x = 0.
     *
     * @tparam T the type of the argument and the returned value.
     * @param[in] x
     * @return log(1 + \e x).
     **********************************************************************/
    template<typename T> static inline T log1p(T x) {
#if GEOGRAPHICLIB_CXX11_MATH
      using std::log1p; return log1p(x);
#else
      using std::log;
      GEOGRAPHICLIB_VOLATILE T
        y = 1 + x,
        z = y - 1;
      // Here's the explanation for this magic: y = 1 + z, exactly, and z
      // approx x, thus log(y)/z (which is nearly constant near z = 0) returns
      // a good approximation to the true log(1 + x)/x.  The multiplication x *
      // (log(y)/z) introduces little additional error.
      return z == 0 ? x : x * log(y) / z;
#endif
    }

    /**
     * The inverse hyperbolic sine function.
     *
     * @tparam T the type of the argument and the returned value.
     * @param[in] x
     * @return asinh(\e x).
     **********************************************************************/
    template<typename T> static inline T asinh(T x) {
#if GEOGRAPHICLIB_CXX11_MATH
      using std::asinh; return asinh(x);
#else
      using std::abs; T y = abs(x); // Enforce odd parity
      y = log1p(y * (1 + y/(hypot(T(1), y) + 1)));
      return x < 0 ? -y : y;
#endif
    }

    /**
     * The inverse hyperbolic tangent function.
     *
     * @tparam T the type of the argument and the returned value.
     * @param[in] x
     * @return atanh(\e x).
     **********************************************************************/
    template<typename T> static inline T atanh(T x) {
#if GEOGRAPHICLIB_CXX11_MATH
      using std::atanh; return atanh(x);
#else
      using std::abs; T y = abs(x); // Enforce odd parity
      y = log1p(2 * y/(1 - y))/2;
      return x < 0 ? -y : y;
#endif
    }

    /**
     * The cube root function.
     *
     * @tparam T the type of the argument and the returned value.
     * @param[in] x
     * @return the real cube root of \e x.
     **********************************************************************/
    template<typename T> static inline T cbrt(T x) {
#if GEOGRAPHICLIB_CXX11_MATH
      using std::cbrt; return cbrt(x);
#else
      using std::abs; using std::pow;
      T y = pow(abs(x), 1/T(3)); // Return the real cube root
      return x < 0 ? -y : y;
#endif
    }

    /**
     * Fused multiply and add.
     *
     * @tparam T the type of the arguments and the returned value.
     * @param[in] x
     * @param[in] y
     * @param[in] z
     * @return <i>xy</i> + <i>z</i>, correctly rounded (on those platforms with
     *   support for the <code>fma</code> instruction).
     *
     * On platforms without the <code>fma</code> instruction, no attempt is
     * made to improve on the result of a rounded multiplication followed by a
     * rounded addition.
     **********************************************************************/
    template<typename T> static inline T fma(T x, T y, T z) {
#if GEOGRAPHICLIB_CXX11_MATH
      using std::fma; return fma(x, y, z);
#else
      return x * y + z;
#endif
    }

    /**
     * Normalize a two-vector.
     *
     * @tparam T the type of the argument and the returned value.
     * @param[in,out] x on output set to <i>x</i>/hypot(<i>x</i>, <i>y</i>).
     * @param[in,out] y on output set to <i>y</i>/hypot(<i>x</i>, <i>y</i>).
     **********************************************************************/
    template<typename T> static inline void norm(T& x, T& y)
    { T h = hypot(x, y); x /= h; y /= h; }

    /**
     * The error-free sum of two numbers.
     *
     * @tparam T the type of the argument and the returned value.
     * @param[in] u
     * @param[in] v
     * @param[out] t the exact error given by (\e u + \e v) - \e s.
     * @return \e s = round(\e u + \e v).
     *
     * See D. E. Knuth, TAOCP, Vol 2, 4.2.2, Theorem B.  (Note that \e t can be
     * the same as one of the first two arguments.)
     **********************************************************************/
    template<typename T> static inline T sum(T u, T v, T& t) {
      GEOGRAPHICLIB_VOLATILE T s = u + v;
      GEOGRAPHICLIB_VOLATILE T up = s - v;
      GEOGRAPHICLIB_VOLATILE T vpp = s - up;
      up -= u;
      vpp -= v;
      t = -(up + vpp);
      // u + v =       s      + t
      //       = round(u + v) + t
      return s;
    }

    /**
     * Evaluate a polynomial.
     *
     * @tparam T the type of the arguments and returned value.
     * @param[in] N the order of the polynomial.
     * @param[in] p the coefficient array (of size \e N + 1).
     * @param[in] x the variable.
     * @return the value of the polynomial.
     *
     * Evaluate <i>y</i> = &sum;<sub><i>n</i>=0..<i>N</i></sub>
     * <i>p</i><sub><i>n</i></sub> <i>x</i><sup><i>N</i>&minus;<i>n</i></sup>.
     * Return 0 if \e N &lt; 0.  Return <i>p</i><sub>0</sub>, if \e N = 0 (even
     * if \e x is infinite or a nan).  The evaluation uses Horner's method.
     **********************************************************************/
    template<typename T> static inline T polyval(int N, const T p[], T x)
    { T y = N < 0 ? 0 : *p++; while (--N >= 0) y = y * x + *p++; return y; }

    /**
     * Normalize an angle.
     *
     * @tparam T the type of the argument and returned value.
     * @param[in] x the angle in degrees.
     * @return the angle reduced to the range [&minus;180&deg;, 180&deg;).
     *
     * The range of \e x is unrestricted.
     **********************************************************************/
    template<typename T> static inline T AngNormalize(T x) {
#if GEOGRAPHICLIB_CXX11_MATH && GEOGRAPHICLIB_PRECISION != 4
      using std::remainder;
      x = remainder(x, T(360)); return x != 180 ? x : -180;
#else
      using std::fmod;
      x = fmod(x, T(360));
      return x < -180 ? x + 360 : (x < 180 ? x : x - 360);
#endif
    }

    /**
     * Normalize an arbitrary angle.
     *
     * @tparam T the type of the argument and returned value.
     * @param[in] x the angle in degrees.
     * @return the angle reduced to the range [&minus;180&deg;, 180&deg;).
     *
     * <b>DEPRECATED</b>: use AngNormalize instead.
     **********************************************************************/
    template<typename T> static inline T AngNormalize2(T x)
    { return AngNormalize<T>(x); }

    /**
     * Normalize a latitude.
     *
     * @tparam T the type of the argument and returned value.
     * @param[in] x the angle in degrees.
     * @return x if it is in the range [&minus;90&deg;, 90&deg;], otherwise
     *   return NaN.
     **********************************************************************/
    template<typename T> static inline T LatFix(T x)
    { using std::abs; return abs(x) > 90 ? NaN<T>() : x; }

    /**
     * Difference of two angles reduced to [&minus;180&deg;, 180&deg;]
     *
     * @tparam T the type of the arguments and returned value.
     * @param[in] x the first angle in degrees.
     * @param[in] y the second angle in degrees.
     * @return \e y &minus; \e x, reduced to the range [&minus;180&deg;,
     *   180&deg;].
     *
     * The result is equivalent to computing the difference exactly, reducing
     * it to (&minus;180&deg;, 180&deg;] and rounding the result.  Note that
     * this prescription allows &minus;180&deg; to be returned (e.g., if \e x
     * is tiny and negative and \e y = 180&deg;).
     **********************************************************************/
    template<typename T> static inline T AngDiff(T x, T y) {
#if GEOGRAPHICLIB_CXX11_MATH && GEOGRAPHICLIB_PRECISION != 4
      using std::remainder;
      T t, d = - AngNormalize(sum(remainder( x, T(360)),
                                  remainder(-y, T(360)), t));
#else
      T t, d = - AngNormalize(sum(AngNormalize(x), AngNormalize(-y), t));
#endif
      // Here y - x = d - t (mod 360), exactly, where d is in (-180,180] and
      // abs(t) <= eps (eps = 2^-45 for doubles).  The only case where the
      // addition of t takes the result outside the range (-180,180] is d = 180
      // and t < 0.  The case, d = -180 + eps, t = eps, can't happen, since
      // sum would have returned the exact result in such a case (i.e., given t
      // = 0).
      return (d == 180 && t < 0 ? -180 : d) - t;
    }

    /**
     * Coarsen a value close to zero.
     *
     * @tparam T the type of the argument and returned value.
     * @param[in] x
     * @return the coarsened value.
     *
     * The makes the smallest gap in \e x = 1/16 - nextafter(1/16, 0) =
     * 1/2<sup>57</sup> for reals = 0.7 pm on the earth if \e x is an angle in
     * degrees.  (This is about 1000 times more resolution than we get with
     * angles around 90&deg;.)  We use this to avoid having to deal with near
     * singular cases when \e x is non-zero but tiny (e.g.,
     * 10<sup>&minus;200</sup>).  This also converts -0 to +0.
     **********************************************************************/
    template<typename T> static inline T AngRound(T x) {
      using std::abs;
      const T z = 1/T(16);
      GEOGRAPHICLIB_VOLATILE T y = abs(x);
      // The compiler mustn't "simplify" z - (z - y) to y
      y = y < z ? z - (z - y) : y;
#if GEOGRAPHICLIB_PRECISION == 4
      // With quad precision and x = +/-0, this gives y = -0.  So change test
      // to x <= 0 here to force +0 to be returned.
      return x <= 0 ? 0 - y : y;
#elif GEOGRAPHICLIB_PRECISION == 5
      // With mpfr, 0 - y is a call to +=(int) which doesn't fix the sign of -0
      return x < 0 ? T(0) - y : y;
#else
      return x < 0 ? 0 - y : y;
#endif
    }

    /**
     * Evaluate the sine and cosine function with the argument in degrees
     *
     * @tparam T the type of the arguments.
     * @param[in] x in degrees.
     * @param[out] sinx sin(<i>x</i>).
     * @param[out] cosx cos(<i>x</i>).
     *
     * The results obey exactly the elementary properties of the trigonometric
     * functions, e.g., sin 9&deg; = cos 81&deg; = &minus; sin 123456789&deg;.
     **********************************************************************/
    template<typename T> static inline void sincosd(T x, T& sinx, T& cosx) {
      // In order to minimize round-off errors, this function exactly reduces
      // the argument to the range [-45, 45] before converting it to radians.
      using std::sin; using std::cos;
      T r; int q;
#if GEOGRAPHICLIB_CXX11_MATH && GEOGRAPHICLIB_PRECISION <= 3 && \
  !defined(__GNUC__)
      // Disable for gcc because of bug in glibc version < 2.22, see
      //   https://sourceware.org/bugzilla/show_bug.cgi?id=17569
      // Once this fix is widely deployed, should insert a runtime test for the
      // glibc version number.
      using std::remquo;
      r = remquo(x, T(90), &q);
#else
      using std::fmod; using std::floor;
      r = fmod(x, T(360));
      q = int(floor(r / 90 + T(0.5)));
      r -= 90 * q;
#endif
      // now abs(r) <= 45
      r *= degree();
      // Possibly could call the gnu extension sincos
      T s = sin(r), c = cos(r);
      switch (unsigned(q) & 3U) {
      case 0U: sinx =     s; cosx =     c; break;
      case 1U: sinx =     c; cosx = 0 - s; break;
      case 2U: sinx = 0 - s; cosx = 0 - c; break;
      default: sinx = 0 - c; cosx =     s; break; // case 3U
      }
    }

    /**
     * Evaluate the sine function with the argument in degrees
     *
     * @tparam T the type of the argument and the returned value.
     * @param[in] x in degrees.
     * @return sin(<i>x</i>).
     **********************************************************************/
    template<typename T> static inline T sind(T x) {
      // See sincosd
      using std::sin; using std::cos;
      T r; int q;
#if GEOGRAPHICLIB_CXX11_MATH && GEOGRAPHICLIB_PRECISION <= 3 && \
  !defined(__GNUC__)
      using std::remquo;
      r = remquo(x, T(90), &q);
#else
      using std::fmod; using std::floor;
      r = fmod(x, T(360));
      q = int(floor(r / 90 + T(0.5)));
      r -= 90 * q;
#endif
      // now abs(r) <= 45
      r *= degree();
      unsigned p = unsigned(q);
      r = p & 1U ? cos(r) : sin(r);
      return p & 2U ? 0 - r : r;
    }

    /**
     * Evaluate the cosine function with the argument in degrees
     *
     * @tparam T the type of the argument and the returned value.
     * @param[in] x in degrees.
     * @return cos(<i>x</i>).
     **********************************************************************/
    template<typename T> static inline T cosd(T x) {
      // See sincosd
      using std::sin; using std::cos;
      T r; int q;
#if GEOGRAPHICLIB_CXX11_MATH && GEOGRAPHICLIB_PRECISION <= 3 && \
  !defined(__GNUC__)
      using std::remquo;
      r = remquo(x, T(90), &q);
#else
      using std::fmod; using std::floor;
      r = fmod(x, T(360));
      q = int(floor(r / 90 + T(0.5)));
      r -= 90 * q;
#endif
      // now abs(r) <= 45
      r *= degree();
      unsigned p = unsigned(q + 1);
      r = p & 1U ? cos(r) : sin(r);
      return p & 2U ? 0 - r : r;
    }

    /**
     * Evaluate the tangent function with the argument in degrees
     *
     * @tparam T the type of the argument and the returned value.
     * @param[in] x in degrees.
     * @return tan(<i>x</i>).
     *
     * If \e x = &plusmn;90&deg;, then a suitably large (but finite) value is
     * returned.
     **********************************************************************/
    template<typename T> static inline T tand(T x) {
      static const T overflow = 1 / sq(std::numeric_limits<T>::epsilon());
      T s, c;
      sincosd(x, s, c);
      return c ? s / c : (s < 0 ? -overflow : overflow);
    }

    /**
     * Evaluate the atan2 function with the result in degrees
     *
     * @tparam T the type of the arguments and the returned value.
     * @param[in] y
     * @param[in] x
     * @return atan2(<i>y</i>, <i>x</i>) in degrees.
     *
     * The result is in the range [&minus;180&deg; 180&deg;).  N.B.,
     * atan2d(&plusmn;0, &minus;1) = &minus;180&deg;; atan2d(+&epsilon;,
     * &minus;1) = +180&deg;, for &epsilon; positive and tiny;
     * atan2d(&plusmn;0, 1) = &plusmn;0&deg;.
     **********************************************************************/
    template<typename T> static inline T atan2d(T y, T x) {
      // In order to minimize round-off errors, this function rearranges the
      // arguments so that result of atan2 is in the range [-pi/4, pi/4] before
      // converting it to degrees and mapping the result to the correct
      // quadrant.
      using std::atan2; using std::abs;
      int q = 0;
      if (abs(y) > abs(x)) { std::swap(x, y); q = 2; }
      if (x < 0) { x = -x; ++q; }
      // here x >= 0 and x >= abs(y), so angle is in [-pi/4, pi/4]
      T ang = atan2(y, x) / degree();
      switch (q) {
        // Note that atan2d(-0.0, 1.0) will return -0.  However, we expect that
        // atan2d will not be called with y = -0.  If need be, include
        //
        //   case 0: ang = 0 + ang; break;
        //
        // and handle mpfr as in AngRound.
      case 1: ang = (y > 0 ? 180 : -180) - ang; break;
      case 2: ang =  90 - ang; break;
      case 3: ang = -90 + ang; break;
      }
      return ang;
    }

    /**
     * Evaluate the atan function with the result in degrees
     *
     * @tparam T the type of the argument and the returned value.
     * @param[in] x
     * @return atan(<i>x</i>) in degrees.
     **********************************************************************/
    template<typename T> static inline T atand(T x)
    { return atan2d(x, T(1)); }

    /**
     * Evaluate <i>e</i> atanh(<i>e x</i>)
     *
     * @tparam T the type of the argument and the returned value.
     * @param[in] x
     * @param[in] es the signed eccentricity =  sign(<i>e</i><sup>2</sup>)
     *    sqrt(|<i>e</i><sup>2</sup>|)
     * @return <i>e</i> atanh(<i>e x</i>)
     *
     * If <i>e</i><sup>2</sup> is negative (<i>e</i> is imaginary), the
     * expression is evaluated in terms of atan.
     **********************************************************************/
    template<typename T> static T eatanhe(T x, T es);

    /**
     * tan&chi; in terms of tan&phi;
     *
     * @tparam T the type of the argument and the returned value.
     * @param[in] tau &tau; = tan&phi;
     * @param[in] es the signed eccentricity = sign(<i>e</i><sup>2</sup>)
     *   sqrt(|<i>e</i><sup>2</sup>|)
     * @return &tau;&prime; = tan&chi;
     *
     * See Eqs. (7--9) of
     * C. F. F. Karney,
     * <a href="https://dx.doi.org/10.1007/s00190-011-0445-3">
     * Transverse Mercator with an accuracy of a few nanometers,</a>
     * J. Geodesy 85(8), 475--485 (Aug. 2011)
     * (preprint <a href="http://arxiv.org/abs/1002.1417">arXiv:1002.1417</a>).
     **********************************************************************/
    template<typename T> static T taupf(T tau, T es);

    /**
     * tan&phi; in terms of tan&chi;
     *
     * @tparam T the type of the argument and the returned value.
     * @param[in] taup &tau;&prime; = tan&chi;
     * @param[in] es the signed eccentricity = sign(<i>e</i><sup>2</sup>)
     *   sqrt(|<i>e</i><sup>2</sup>|)
     * @return &tau; = tan&phi;
     *
     * See Eqs. (19--21) of
     * C. F. F. Karney,
     * <a href="https://dx.doi.org/10.1007/s00190-011-0445-3">
     * Transverse Mercator with an accuracy of a few nanometers,</a>
     * J. Geodesy 85(8), 475--485 (Aug. 2011)
     * (preprint <a href="http://arxiv.org/abs/1002.1417">arXiv:1002.1417</a>).
     **********************************************************************/
    template<typename T> static T tauf(T taup, T es);

    /**
     * Test for finiteness.
     *
     * @tparam T the type of the argument.
     * @param[in] x
     * @return true if number is finite, false if NaN or infinite.
     **********************************************************************/
    template<typename T> static inline bool isfinite(T x) {
#if GEOGRAPHICLIB_CXX11_MATH
      using std::isfinite; return isfinite(x);
#else
      using std::abs;
      return abs(x) <= (std::numeric_limits<T>::max)();
#endif
    }

    /**
     * The NaN (not a number)
     *
     * @tparam T the type of the returned value.
     * @return NaN if available, otherwise return the max real of type T.
     **********************************************************************/
    template<typename T> static inline T NaN() {
      return std::numeric_limits<T>::has_quiet_NaN ?
        std::numeric_limits<T>::quiet_NaN() :
        (std::numeric_limits<T>::max)();
    }
    /**
     * A synonym for NaN<real>().
     **********************************************************************/
    static inline real NaN() { return NaN<real>(); }

    /**
     * Test for NaN.
     *
     * @tparam T the type of the argument.
     * @param[in] x
     * @return true if argument is a NaN.
     **********************************************************************/
    template<typename T> static inline bool isnan(T x) {
#if GEOGRAPHICLIB_CXX11_MATH
      using std::isnan; return isnan(x);
#else
      return x != x;
#endif
    }

    /**
     * Infinity
     *
     * @tparam T the type of the returned value.
     * @return infinity if available, otherwise return the max real.
     **********************************************************************/
    template<typename T> static inline T infinity() {
      return std::numeric_limits<T>::has_infinity ?
        std::numeric_limits<T>::infinity() :
        (std::numeric_limits<T>::max)();
    }
    /**
     * A synonym for infinity<real>().
     **********************************************************************/
    static inline real infinity() { return infinity<real>(); }

    /**
     * Swap the bytes of a quantity
     *
     * @tparam T the type of the argument and the returned value.
     * @param[in] x
     * @return x with its bytes swapped.
     **********************************************************************/
    template<typename T> static inline T swab(T x) {
      union {
        T r;
        unsigned char c[sizeof(T)];
      } b;
      b.r = x;
      for (int i = sizeof(T)/2; i--; )
        std::swap(b.c[i], b.c[sizeof(T) - 1 - i]);
      return b.r;
    }

#if GEOGRAPHICLIB_PRECISION == 4
    typedef boost::math::policies::policy
      < boost::math::policies::domain_error
        <boost::math::policies::errno_on_error>,
        boost::math::policies::pole_error
        <boost::math::policies::errno_on_error>,
        boost::math::policies::overflow_error
        <boost::math::policies::errno_on_error>,
        boost::math::policies::evaluation_error
        <boost::math::policies::errno_on_error> >
      boost_special_functions_policy;

    static inline real hypot(real x, real y)
    { return boost::math::hypot(x, y, boost_special_functions_policy()); }

    static inline real expm1(real x)
    { return boost::math::expm1(x, boost_special_functions_policy()); }

    static inline real log1p(real x)
    { return boost::math::log1p(x, boost_special_functions_policy()); }

    static inline real asinh(real x)
    { return boost::math::asinh(x, boost_special_functions_policy()); }

    static inline real atanh(real x)
    { return boost::math::atanh(x, boost_special_functions_policy()); }

    static inline real cbrt(real x)
    { return boost::math::cbrt(x, boost_special_functions_policy()); }

    static inline real fma(real x, real y, real z)
    { return fmaq(__float128(x), __float128(y), __float128(z)); }

    static inline bool isnan(real x) { return boost::math::isnan(x); }

    static inline bool isfinite(real x) { return boost::math::isfinite(x); }
#endif
  };

} // namespace GeographicLib

#endif  // GEOGRAPHICLIB_MATH_HPP
