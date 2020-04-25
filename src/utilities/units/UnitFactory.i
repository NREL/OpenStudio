#ifndef UTILITIES_UNITS_UNITFACTORY_I
#define UTILITIES_UNITS_UNITFACTORY_I

%{
  #include <utilities/units/UnitFactory.hpp>
%}

// hide shared_ptrs from users, just export helper functions
%ignore UnitFactorySingleton;
%ignore UnitFactory;
%include <utilities/units/UnitFactory.hpp>

#endif // UTILITIES_UNITS_UNITFACTORY_I
