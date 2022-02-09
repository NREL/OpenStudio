#ifndef UTILITIES_DATA_MATRIX_I
#define UTILITIES_DATA_MATRIX_I

%{
  #include <utilities/data/Matrix.hpp>
%}

// create an instantiation of the vector class
%template(MatrixVector) std::vector< openstudio::Matrix >;

namespace openstudio{

class Matrix{
public:

  #ifdef SWIGPYTHON
    %typemap(in) (unsigned i, unsigned j) {
      long a, b;

      if (PyTuple_Check($input)) {
        if (!PyArg_ParseTuple($input,"ll", &a, &b)) {
          PyErr_SetString(PyExc_TypeError,"tuple must have 2 elements");
          return NULL;
        }
        $1 = a;
        $2 = b;
      } else {
        PyErr_SetString(PyExc_TypeError,"expected a tuple.");
        return NULL;
      }
    }
  #endif

  // constructors
  Matrix();
  Matrix(unsigned M, unsigned N);
  Matrix(unsigned M, unsigned N, double x0);
  Matrix(const Matrix& m);

  // sizing
  unsigned size1() const;
  unsigned size2() const;
  void resize(unsigned M, unsigned N, bool preserve);

};

%extend Matrix{

  double __getitem__(unsigned i, unsigned j) const {
    return (*self)(i,j);
  }

  void __setitem__(unsigned i, unsigned j, double x) {
    (*self)(i,j) = x;
  }

  bool __eq__(const Matrix& other) {
    return operator==((*self), other);
  }

  Matrix __add__(const Matrix& m) const {
    return ((*self) + m);
  }

  Matrix __add__(double d) const {
    return ((*self) + Matrix(self->size1(), self->size2(), d));
  }

  Matrix __sub__(const Matrix& m) const {
    return ((*self) - m);
  }

  Matrix __sub__(double d) const {
    return ((*self) - Matrix(self->size1(), self->size2(), d));
  }

  Matrix __mul__(const Matrix& m) const {
    return prod(*self, m);
  }

  Vector __mul__(const Vector& v) const {
    return prod(*self, v);
  }

  Matrix __mul__(double d) const {
    return ((*self) * d);
  }

  Matrix __div__(double d) const {
    return ((*self) / d);
  }

  std::string __str__() const {
    std::ostringstream os;

    // Don't use boost ostream operator, we want to show something matrix-like, not inline
    // os << *self;
    typedef Matrix::size_type size_type;
    size_type size1 = self->size1();
    size_type size2 = self->size2();

    os << "Matrix of size (" << size1 << ", " << size2 << ")";
    if ((size1 == 0) || (size2 == 0)) {
      os << ": Nothing to show, at least one dimension is zero";
      return os.str();
    } else {
      os << ":\n[";
      for (size_type i = 0; i < size1; ++i) {
        os << '[';
        for (size_type j = 0; j < size2; ++j) {
          os << (*self)(i, j);
          if (j != size2 - 1) {
            os << ", ";
          }
        }
        os << ']';
        if (i != size1 - 1) {
          os << ",\n ";
        }
      }
      os << ']';
    }
    return os.str();
  }

}; // End %extend Matrix

//////////////////////////////////////////////////////////////////////////
// Copy and paste free function declarations here from Matrix.hpp
//////////////////////////////////////////////////////////////////////////

/// new operators

//bool operator==(const Matrix& lhs, const Matrix& rhs);
//bool operator!=(const Matrix& lhs, const Matrix& rhs);

/// common methods

/// linear interpolation of the function v = f(x, y) at point xi, yi
/// assumes that x and y are strictly increasing
double interp(const Vector& x, const Vector& y, const Matrix& v, double xi, double yi, InterpMethod interpMethod = linearInterp, ExtrapMethod extrapMethod = noneExtrap);

/// linear interpolation of the function v = f(x, y) at points xi, yi
/// assumes that x and y are strictly increasing
Matrix interp(const Vector& x, const Vector& y, const Matrix& v, const Vector& xi, const Vector& yi, InterpMethod interpMethod = linearInterp, ExtrapMethod extrapMethod = noneExtrap);

/// outer product
Matrix outerProd(const Vector& lhs, const Vector& rhs);

/// take the natural logarithm of a Matrix
Matrix log(const Matrix& v);

/// take the logarithm of a Matrix with certain base
Matrix log(const Matrix& v, double base);

/// generates a Matrix of N points randomly drawn between and including a and b.
Matrix randMatrix(double a, double b, unsigned M, unsigned N);

/// sum
double sum(const Matrix& matrix);

/// maximum
double maximum(const Matrix& matrix);

/// minimum
double minimum(const Matrix& matrix);

/// mean
double mean(const Matrix& matrix);

} // openstudio

#endif //UTILITIES_DATA_MATRIX_I

