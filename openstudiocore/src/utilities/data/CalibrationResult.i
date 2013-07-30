#ifndef UTILITIES_DATA_CALIBRATIONRESULT_I
#define UTILITIES_DATA_CALIBRATIONRESULT_I
%{
  #include <utilities/data/CalibrationResult.hpp>
%}

// create an instantiation of the optional class
%template(OptionalCalibrationResult) boost::optional< openstudio::CalibrationResult >;

// create an instantiation of the vector class
%template(CalibrationResultVector) std::vector< openstudio::CalibrationResult >;

%include <utilities/data/CalibrationResult.hpp>

#endif //UTILITIES_DATA_CALIBRATIONRESULT_I 