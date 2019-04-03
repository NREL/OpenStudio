#ifndef UTILITIES_UNITS_QUANTITYREGEX_I
#define UTILITIES_UNITS_QUANTITYREGEX_I

%{
  #include <utilities/units/QuantityRegex.hpp>
%}

// hide regexes, and decomposition methods used by UnitFactory
namespace openstudio {

bool isFixedPrecisionValue(const std::string& s);
bool containsFixedPrecisionValue(const std::string& s);
bool isScientificNotationValue(const std::string& s);
bool containsScientificNotationValue(const std::string& s);
bool isAtomicUnit(const std::string& s);
bool containsAtomicUnit(const std::string& s);
bool isCompoundUnit(const std::string& s);
bool containsCompoundUnit(const std::string& s);
bool isScaledUnit(const std::string& s);
bool containsScaledUnit(const std::string& s);
bool isDirectScaledUnit(const std::string& s);
bool containsDirectScaledUnit(const std::string& s);
bool isUnit(const std::string& s);
bool containsUnit(const std::string& s);
bool isQuantity(const std::string& s);
bool containsQuantity(const std::string& s);

}

#endif // UTILITIES_UNITS_QUANTITYREGEX_I
