#ifndef UTILITIES_IDD_I
#define UTILITIES_IDD_I

%{
  #include <utilities/idd/IddFieldProperties.hpp>
  #include <utilities/idd/IddKeyProperties.hpp>
  #include <utilities/idd/IddKey.hpp>
  #include <utilities/idd/IddField.hpp>
  #include <utilities/idd/IddObject.hpp>
  #include <utilities/idd/IddFile.hpp>
  #include <utilities/idd/IddFactory.hxx>
  #include <utilities/idd/IddFileAndFactoryWrapper.hpp>
  #include <utilities/idd/ExtensibleIndex.hpp>
  #include <utilities/idd/IddObjectProperties.hpp>
  #include <utilities/idd/Comments.hpp>

  #include <utilities/units/Quantity.hpp>
  #include <utilities/units/Unit.hpp>
  #include <utilities/units/BTUUnit.hpp>
  #include <utilities/units/CelsiusUnit.hpp>
  #include <utilities/units/CFMUnit.hpp>
  #include <utilities/units/FahrenheitUnit.hpp>
  #include <utilities/units/GPDUnit.hpp>
  #include <utilities/units/IPUnit.hpp>
  #include <utilities/units/Misc1Unit.hpp>
  #include <utilities/units/MPHUnit.hpp>
  #include <utilities/units/SIUnit.hpp>
  #include <utilities/units/ThermUnit.hpp>
  #include <utilities/units/WhUnit.hpp>
%}

%include <utilities/idd/Comments.hpp>
%include <utilities/idd/IddFieldProperties.hpp>
%include <utilities/idd/ExtensibleIndex.hpp>
%include <utilities/idd/IddObjectProperties.hpp>

// create an instantiation of the optional classes

%template(OptionalIddFileType) boost::optional<openstudio::IddFileType>;
%template(OptionalIddObjectType) boost::optional<openstudio::IddObjectType>;
%template(OptionalIddKey) boost::optional<openstudio::IddKey>;
%template(OptionalIddField) boost::optional<openstudio::IddField>;
%template(OptionalIddObject) boost::optional<openstudio::IddObject>;
%template(OptionalIddFile) boost::optional<openstudio::IddFile>;

// create an instantiation of the vector classes
%template(IddKeyVector) std::vector<openstudio::IddKey>;
%template(IddFieldVector) std::vector<openstudio::IddField>;
%template(IddObjectVector) std::vector<openstudio::IddObject>;
%template(IddFileVector) std::vector<openstudio::IddFile>;
%template(IddObjectTypeVector) std::vector<openstudio::IddObjectType>;

%template(OptionalIddObjectTypeVector) boost::optional<std::vector<openstudio::IddObjectType> >;

// ignore detail namespace
%ignore openstudio::detail;

// ignore ostream related functions
%ignore print(std::ostream&, bool) const;

// include the headers into the swig interface directly
%include <utilities/idd/IddEnums.hpp>

#if defined(SWIGCSHARP) || defined(SWIGJAVA)
  %{
    #include <utilities/idd/IddFieldEnums.hxx>
  %}
  %include <utilities/idd/IddFieldEnums.ixx>
#endif

%template(IddObjectTypeSet) std::set<openstudio::IddObjectType>;

%include <utilities/idd/IddKeyProperties.hpp>
%include <utilities/idd/IddKey.hpp>
%include <utilities/idd/IddField.hpp>
%include <utilities/idd/IddObject.hpp>
%include <utilities/idd/IddFile.hpp>
%include <utilities/idd/IddFileAndFactoryWrapper.hpp>

%extend openstudio::IddKey{
  std::string __str__() const{
    std::ostringstream os;
    os << *self;
    return os.str();
  }
};

%extend openstudio::IddObject{
  std::string __str__() const{
    std::ostringstream os;
    os << *self;
    return os.str();
  }
};

%extend openstudio::IddFile{
  std::string __str__() const{
    std::ostringstream os;
    os << *self;
    return os.str();
  }
};

#endif // UTILITIES_IDD_I
