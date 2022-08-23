#ifndef UTILITIES_DATA_CALIBRATIONRESULT_I
#define UTILITIES_DATA_CALIBRATIONRESULT_I
%{
  #include <utilities/data/CalibrationResult.hpp>
%}

// create an instantiation of the optional classes
%template(OptionalCalibrationResult) boost::optional< openstudio::CalibrationResult >;
%template(OptionalCalibrationBillingPeriod) boost::optional< openstudio::CalibrationBillingPeriod >;
%template(OptionalCalibrationUtilityBill) boost::optional< openstudio::CalibrationUtilityBill >;

// create an instantiation of the vector class
%template(CalibrationResultVector) std::vector< openstudio::CalibrationResult >;

// We would ideally need to template these vectors... because they don't have a default constructor, we ignore the size_type ones
%ignore std::vector<openstudio::CalibrationBillingPeriod>::vector(size_type);
%ignore std::vector<openstudio::CalibrationBillingPeriod>::resize(size_type);
%template(CalibrationBillingPeriodVector) std::vector< openstudio::CalibrationBillingPeriod >;
%ignore std::vector<openstudio::CalibrationUtilityBill>::vector(size_type);
%ignore std::vector<openstudio::CalibrationUtilityBill>::resize(size_type);
%template(CalibrationUtilityBillVector) std::vector< openstudio::CalibrationUtilityBill >;

%template(AttributeValueTypeVector) std::vector< openstudio::AttributeValueType >;

%include <utilities/data/CalibrationResult.hpp>

#endif //UTILITIES_DATA_CALIBRATIONRESULT_I
