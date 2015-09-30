#ifndef UTILITIES_UNITS_QUANTITYCONVERTER_I
#define UTILITIES_UNITS_QUANTITYCONVERTER_I

%{
  #include <utilities/units/QuantityConverter.hpp>
%}

// hide shared_ptrs, expose helper functions
%ignore QuantityConverterSingleton;
%ignore QuantityConverter;
%include <utilities/units/QuantityConverter.hpp>

#endif // UTILITIES_UNITS_QUANTITYCONVERTER_I
