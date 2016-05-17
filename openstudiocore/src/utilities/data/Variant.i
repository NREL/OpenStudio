#ifndef UTILITIES_DATA_VARIANT_I
#define UTILITIES_DATA_VARIANT_I

%{
  #include <utilities/data/Variant.hpp>
%}

// create an instantiation of the optional class
%template(OptionalVariant) boost::optional<openstudio::Variant>;

// create an instantiation of the vector class
%ignore std::vector<openstudio::Variant>::vector(size_type);
%ignore std::vector<openstudio::Variant>::resize(size_type);
%template(VariantVector) std::vector<openstudio::Variant>;

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
