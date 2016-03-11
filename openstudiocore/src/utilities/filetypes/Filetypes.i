#ifndef UTILITIES_FILETYPES_I
#define UTILITIES_FILETYPES_I

%{
  namespace openstudio{}
  using namespace openstudio;
  #include <utilities/filetypes/EpwFile.hpp>
  #include <utilities/units/Quantity.hpp>
  #include <utilities/idf/IdfObject.hpp>
  #include <utilities/idf/WorkspaceObject.hpp>
%}

%import <utilities/units/Quantity.i>

%template(EpwDataPointVector) std::vector<openstudio::EpwDataPoint>;
%template(OptionalEpwDataPoint) boost::optional<openstudio::EpwDataPoint>;
%template(OptionalAirState) boost::optional<openstudio::AirState>;

%ignore std::vector<openstudio::EpwFile>::vector(size_type);
%ignore std::vector<openstudio::EpwFile>::resize(size_type);
%template(EpwFileVector) std::vector<openstudio::EpwFile>;
%template(OptionalEpwFile) boost::optional<openstudio::EpwFile>;

%include <utilities/filetypes/EpwFile.hpp>

#endif //UTILITIES_FILETYPES_I 
