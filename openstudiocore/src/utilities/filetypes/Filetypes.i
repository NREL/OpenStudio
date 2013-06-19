#ifndef UTILITIES_FILETYPES_I
#define UTILITIES_FILETYPES_I

%{
  namespace openstudio{}
  using namespace openstudio;
  #include <utilities/filetypes/EpwFile.hpp>
  #include <utilities/filetypes/TimeDependentValuationFile.hpp>
  #include <utilities/units/Quantity.hpp>
  #include <utilities/idf/IdfObject.hpp>
  #include <utilities/idf/WorkspaceObject.hpp>
%}

%import <utilities/units/Quantity.i>

%ignore std::vector<openstudio::EpwFile>::vector(size_type);
%ignore std::vector<openstudio::EpwFile>::resize(size_type);
%template(EpwFileVector) std::vector<openstudio::EpwFile>;
%template(OptionalEpwFile) boost::optional<openstudio::EpwFile>;
%ignore std::vector<openstudio::TimeDependentValuationFile>::vector(size_type);
%ignore std::vector<openstudio::TimeDependentValuationFile>::resize(size_type);
%template(TimeDependentValuationFileVector) std::vector<openstudio::TimeDependentValuationFile>;
%template(OptionalTimeDependentValuationFile) boost::optional<openstudio::TimeDependentValuationFile>;

%include <utilities/filetypes/EpwFile.hpp>
%include <utilities/filetypes/TimeDependentValuationFile.hpp>

#endif //UTILITIES_FILETYPES_I 
