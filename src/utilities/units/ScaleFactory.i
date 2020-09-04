#ifndef UTILITIES_UNITS_SCALEFACTORY_I
#define UTILITIES_UNITS_SCALEFACTORY_I

%{
  #include <utilities/units/ScaleFactory.hpp>
%}

// ScaleFactory is kept private as an implementation detail for now.
// Do export a function for printing the registered scales so users know what is available.

namespace openstudio {

/** Wrapper around << for SWIG bindings. Prints each scale registered in ScaleFactory,
 *  and thus available for use in Units, Quantities, and bare Scale operations. */
std::string printScales();

}

#endif // UTILITIES_UNITS_SCALEFACTORY_I
