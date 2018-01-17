#ifndef UTILITIES_DATA_ATTRIBUTE_I
#define UTILITIES_DATA_ATTRIBUTE_I

%{
  #include <utilities/data/Attribute.hpp>
  #include <utilities/data/Attribute_Impl.hpp>
%}

%ignore QDomElement;

// create an instantiation of the optional class
%template(OptionalAttribute) boost::optional< openstudio::Attribute >;

// create an instantiation of the vector class
%ignore std::vector<openstudio::Attribute>::vector(size_type);
%ignore std::vector<openstudio::Attribute>::resize(size_type);
%template(AttributeVector) std::vector< openstudio::Attribute >;

%ignore openstudio::Attribute::Attribute(const std::string& name, bool value, const boost::optional<std::string>& units);
%ignore openstudio::Attribute::Attribute(const std::string& name, double value, const boost::optional<std::string>& units);
%ignore openstudio::Attribute::Attribute(const std::string& name, int value, const boost::optional<std::string>& units);
%ignore openstudio::Attribute::Attribute(const std::string& name, unsigned value, const boost::optional<std::string>& units);
%ignore openstudio::Attribute::Attribute(const std::string& name, const char* value, const boost::optional<std::string>& units);
%ignore openstudio::Attribute::Attribute(const std::string& name, const std::string& value, const boost::optional<std::string>& units);
%ignore openstudio::Attribute::Attribute(const std::string& name, const std::vector<openstudio::Attribute>& value, const boost::optional<std::string>& units);

#if defined(SWIGRUBY)
  // cannot distinguish between int and unsigned
  %ignore openstudio::Attribute::Attribute(const std::string& name, unsigned value);
  %ignore openstudio::Attribute::Attribute(const std::string& name, unsigned value, const std::string& units);
  %ignore openstudio::Attribute::Attribute(const std::string& name, unsigned value, const boost::optional<std::string>& units);
#endif

%include <utilities/data/Attribute.hpp>

// extend class
%extend openstudio::Attribute {

  std::string __str__() {
    std::ostringstream os;
    os << *self;
    return os.str();
  }

};

#endif //UTILITIES_DATA_ATTRIBUTE_I
