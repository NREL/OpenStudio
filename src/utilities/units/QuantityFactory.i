#ifndef UTILITIES_UNITS_QUANTITYFACTORY_I
#define UTILITIES_UNITS_QUANTITYFACTORY_I

%{
  #include <utilities/units/QuantityFactory.hpp>
%}

// hide shared_ptrs, expose helper functions
%ignore QuantityFactorySingleton;
%ignore QuantityFactory;
%include <utilities/units/QuantityFactory.hpp>

#endif // UTILITIES_UNITS_QUANTITYFACTORY_I
