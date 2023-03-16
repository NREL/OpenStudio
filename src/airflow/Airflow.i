#ifndef AIRFLOW_I
#define AIRFLOW_I

#ifdef SWIGPYTHON
%module openstudioairflow
#endif

#define UTILITIES_API
#define AIRFLOW_API

%include <utilities/core/CommonInclude.i>
%import <utilities/core/CommonImport.i>
%import <utilities/Utilities.i>
%import <model/Model.i>

%{
  //#include <airflow/ReverseTranslator.hpp>
  #include <airflow/contam/ContamEnums.hpp>
  #include <airflow/contam/ForwardTranslator.hpp>
  using namespace openstudio::contam;
  using namespace openstudio;

  #include <model/Model.hpp>
  #include <model/ModelObject.hpp>
%}

%ignore IndexModelImpl;
%ignore IndexModel(Reader &input);
%template(OptionalContamIndexModel) boost::optional<openstudio::contam::IndexModel>;

// All the vectors
%template(ContamSpeciesVector) std::vector<openstudio::contam::Species>;
%template(ContamLevelVector) std::vector<openstudio::contam::Level>;
%template(ContamDayScheduleVector) std::vector<openstudio::contam::DaySchedule>;
%template(ContamWeekScheduleVector) std::vector<openstudio::contam::WeekSchedule>;
%template(ContamWindPressureProfileVector) std::vector<openstudio::contam::WindPressureProfile>;
%template(ContamPressureCoefficientPointVector) std::vector<openstudio::contam::PressureCoefficientPoint>;
%template(ContamPlrTest1Vector) std::vector<openstudio::contam::PlrTest1>;
%template(ContamPlrTest2Vector) std::vector<openstudio::contam::PlrTest2>;
%template(ContamCvfDatVector) std::vector<openstudio::contam::CvfDat>;
%template(ContamSimpleAhsVector) std::vector<openstudio::contam::Ahs>;
%template(ContamZoneVector) std::vector<openstudio::contam::Zone>;
%template(ContamAirflowPathVector) std::vector<openstudio::contam::AirflowPath>;
%template(ContamSketchPadIconVector) std::vector<openstudio::contam::Icon>;
%template(ContamSchedulePointVector) std::vector<openstudio::contam::SchedulePoint>;
%template(ContamFanDataPointVector) std::vector<openstudio::contam::FanDataPoint>;
%template(ContamXyDataPointVector) std::vector<openstudio::contam::XyDataPoint>;
%template(ContamAirflowSubelementDataVector) std::vector<openstudio::contam::AirflowSubelementData>;
%template(AirflowElementTypeVector) std::vector<openstudio::contam::AirflowElementType>;
%template(OptionalAirflowElementType) boost::optional<openstudio::contam::AirflowElementType>;
%template(ControlNodeTypeVector) std::vector<openstudio::contam::ControlNodeType>;
%template(OptionalControlNodeType) boost::optional<openstudio::contam::ControlNodeType>;

// #ifdef SWIGCSHARP
//%rename(ContamReverseTranslator) openstudio::contam::ReverseTranslator;
%rename(ContamForwardTranslator) openstudio::contam::ForwardTranslator;
// #endif

%include <airflow/contam/ContamEnums.hpp>
%include <airflow/contam/PrjSubobjects.hpp>
%include <airflow/contam/PrjObjects.hpp>
%include <airflow/contam/PrjAirflowElements.hpp>
%include <airflow/contam/PrjModel.hpp>
//%include <airflow/ReverseTranslator.hpp>
%include <airflow/contam/ForwardTranslator.hpp>

#endif //AIRFLOW_I
