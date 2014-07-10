#ifndef UTILITIES_DATA_VECTOR_I
#define UTILITIES_DATA_VECTOR_I

%{
  #include <utilities/data/Vector.hpp>
%}

%template(DoubleFromVectorFunctor) boost::function1<double, const openstudio::Vector &>;


// create an instantiation of the vector class
%template(VectorVector) std::vector< openstudio::Vector >;

namespace openstudio{

class Vector{
public:

  // constructors
  Vector();
  Vector(unsigned N);
  Vector(unsigned N, double x0);
  Vector(const Vector& v);

  // sizing
  unsigned size() const;
  void resize(unsigned N, bool preserve);
  
};

%extend Vector{

  unsigned __len__() const {
    return self->size();
  }

  double __getitem__(unsigned i) const {
    return (*self)(i);
  }

  void __setitem__(unsigned i, double x) {
    (*self)(i) = x;
  }
  
  bool __eq__(const Vector& other) {
    return operator==((*self), other);
  }
  
  Vector __add__(const Vector& v) const {
    return ((*self) + v);
  }
  
  Vector __add__(double d) const {
    return ((*self) + openstudio::ScalarVector(self->size(), d));
  }
  
  Vector __sub__(const Vector& v) const {
    return ((*self) - v);
  }
  
  Vector __sub__(double d) const {
    return ((*self) - openstudio::ScalarVector(self->size(), d));
  }
 
  Vector __mul__(double d) const {
    return ((*self) * d);
  }
  
  Vector __div__(double d) const {
    return ((*self) / d);
  }
  
  std::string __str__() const{
    std::ostringstream os;
    os << *self;
    return os.str();
  }
};




//////////////////////////////////////////////////////////////////////////
// Copy and paste free function declarations here from Vector.hpp
//////////////////////////////////////////////////////////////////////////

/// Helper function to construct Vector from std::vector<double>.
UTILITIES_API Vector createVector(const std::vector<double> values);

/** @name Operators */
//@{

UTILITIES_API bool operator==(const Vector& lhs, const Vector& rhs);
UTILITIES_API bool operator!=(const Vector& lhs, const Vector& rhs);

//@}
/** @name Interpolation */
//@{

// The following link has hints for interpolation
// http://o2scl.sourceforge.net/o2scl/html/index.html#intp_section

/** Enum to specify the interpoloation method. */
enum InterpMethod{LinearInterp, NearestInterp, HoldLastInterp, HoldNextInterp};

/** Enum to specify the extrapolation method. */
enum ExtrapMethod{NoneExtrap, NearestExtrap};

/** Data structure for holding interpolation information. */
struct UTILITIES_API InterpInfo{
  bool extrapolated; // was point out of range
  unsigned ia, ib; // indices of two nearest points
  double wa, wb; // weights of two nearest points
};

/** Linear interpolation of the function y = f(x) at point xi. Assumes that x is strictly 
 *  increasing. */
UTILITIES_API InterpInfo interpInfo(const Vector& x, double xi);

/** Linear interpolation of the function y = f(x) at point xi. Assumes that x is strictly 
 *  increasing */
UTILITIES_API double interp(const Vector& x, const Vector& y, double xi, 
                            InterpMethod interpMethod = LinearInterp, 
                            ExtrapMethod extrapMethod = NoneExtrap);

/** Linear interpolation of the function y = f(x) at points xi. Assumes that x is strictly 
 *  increasing. */
UTILITIES_API Vector interp(const Vector& x, const Vector& y, const Vector& xi, 
                            InterpMethod interpMethod = LinearInterp, 
                            ExtrapMethod extrapMethod = NoneExtrap);

//@}
/** @name Common Methods and Vector Operations */
//@{

/** Generates a Vector of N points randomly drawn between and including a and b. */
UTILITIES_API Vector randVector(double a, double b, unsigned N);

/** Generates a Vector of N points linearly spaced between and including a and b. */
UTILITIES_API Vector linspace(double a, double b, unsigned N);

/** Generates a Vector linearly spaced points starting at a and ending before or at b with 
 *  interval delta. */
UTILITIES_API Vector deltaSpace(double a, double b, double delta);

/** Generates a Vector of N points logorithmically spaced between and including base^a and 
 *  base^b. */
UTILITIES_API Vector logspace(double a, double b, unsigned N, double base = 10.0);

/** Take the natural logarithm of elements of a Vector. */
UTILITIES_API Vector log(const Vector& x);

/** Take the logarithm of elements of a Vector with certain base. */
UTILITIES_API Vector log(const Vector& x, double base);

/** Compute the cumulative sum of a Vector. */
UTILITIES_API Vector cumsum(const Vector& x, double runningSum = 0.0);

/** Returns the dot product between lhs and rhs. */
UTILITIES_API double dot(const Vector& lhs, const Vector& rhs);

/** Returns the sum of vector's values. */
UTILITIES_API double sum(const Vector& vector);

/** Returns the largest element of vector. */
UTILITIES_API double maximum(const Vector& vector);

/** Returns the smallest element of vector. */
UTILITIES_API double minimum(const Vector& vector);

/** Returns the mean of vector's values */
UTILITIES_API double mean(const Vector& vector);

/** Returns the sample variance of vector's values. */
UTILITIES_API double variance(const Vector& vector);

/** Returns the standard deviation of vector's values. */
UTILITIES_API double stdDev(const Vector& vector);

/** Returns std::function pointer to sum(const Vector&). */
UTILITIES_API boost::function1<double, const Vector &> sumVectorFunctor();

/** Returns std::function pointer to maxiumum(const Vector&). */
UTILITIES_API boost::function1<double, const Vector &> maximumVectorFunctor();

/** Returns std::function pointer to miniumum(const Vector&). */
UTILITIES_API boost::function1<double, const Vector &> minimumVectorFunctor();

/** Returns std::function pointer to mean(const Vector&). */
UTILITIES_API boost::function1<double, const Vector &> meanVectorFunctor();

/** Returns std::function pointer to variance(const Vector&). */
UTILITIES_API boost::function1<double, const Vector &> varianceVectorFunctor();

/** Returns std::function pointer to stdDev(const Vector&). */
UTILITIES_API boost::function1<double, const Vector &> stdDevVectorFunctor();

/** Evaluates functor(vector). For use in SWIG bindings. */
UTILITIES_API double evaluateDoubleFromVectorFunctor(
    const boost::function1<double, const Vector &>& functor,
    const Vector& vector);

//@}


} // openstudio

#endif //UTILITIES_DATA_VECTOR_I 
