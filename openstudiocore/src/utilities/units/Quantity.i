#ifndef UTILITIES_UNITS_QUANTITY_I
#define UTILITIES_UNITS_QUANTITY_I

%{
  #include <utilities/units/Quantity.hpp>
  #include <utilities/units/OSOptionalQuantity.hpp>
  #include <utilities/units/OSQuantityVector.hpp>
%}


%import <utilities/units/Scale.i>

%template(QuantityVector) std::vector<openstudio::Quantity>;
%template(OptionalQuantity) boost::optional<openstudio::Quantity>;

%include <utilities/units/Quantity.hpp>
%include <utilities/units/OSOptionalQuantity.hpp>
%include <utilities/units/OSQuantityVector.hpp>

%extend openstudio::Quantity {
  std::string __str__() const{
    std::stringstream ss;
    ss << *self;
    return ss.str();
  }
  openstudio::Quantity operator-() {
    return -(*self);
  }
  openstudio::Quantity operator+(const openstudio::Quantity& rQuantity) {
    return *self + rQuantity;
  }
  openstudio::Quantity operator-(const openstudio::Quantity& rQuantity) {
    return *self - rQuantity;
  }
  openstudio::Quantity operator*(const openstudio::Quantity& rQuantity) {
    return *self * rQuantity;
  }
  openstudio::Quantity operator/(const openstudio::Quantity& rQuantity) {
    return *self / rQuantity;
  }
  openstudio::Quantity operator*(double d) {
    return *self * d;
  }
  openstudio::Quantity operator/(double d) {
    return *self / d;
  }
};

#endif // UTILITIES_UNITS_QUANTITY_I
