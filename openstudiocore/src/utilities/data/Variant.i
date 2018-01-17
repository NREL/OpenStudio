#ifndef UTILITIES_DATA_VARIANT_I
#define UTILITIES_DATA_VARIANT_I

%{
  #include <utilities/data/Variant.hpp>
%}

%template(OptionalVariant) boost::optional<openstudio::Variant>;
%template(VariantVector) std::vector<openstudio::Variant>;
%template(StringVariantMap) std::map<std::string, openstudio::Variant>;

%ignore openstudio::Variant::Variant(const char* value);

%include <utilities/data/Variant.hpp>

// extend class
%extend openstudio::Variant{

  std::string __str__() {
    std::ostringstream os;
    os << *self;
    return os.str();
  }

  int __int__() const{
    return self->valueAsInteger();
  }

  float __float__() const{
    return self->valueAsDouble();
  }
};

#endif //UTILITIES_DATA_ATTRIBUTE_I
