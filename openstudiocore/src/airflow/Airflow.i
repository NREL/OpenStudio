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
  #include <airflow/contam/ForwardTranslator.hpp>
  using namespace openstudio::contam;
  using namespace openstudio;
  
  #include <model/Model.hpp>
  #include <model/ModelObject.hpp>
%}

%ignore IndexModelImpl;
%ignore IndexModel(Reader &input);
%template(OptionalIndexModel) boost::optional<openstudio::contam::IndexModel>;

// All the vectors
%template(SpeciesVector) std::vector<openstudio::contam::Species>;
%template(LevelVector) std::vector<openstudio::contam::Level>;
%template(DayScheduleVector) std::vector<openstudio::contam::DaySchedule>;
%template(WeekScheduleVector) std::vector<openstudio::contam::WeekSchedule>;
%template(WindPressureProfileVector) std::vector<openstudio::contam::WindPressureProfile>;
%template(PlrTest1Vector) std::vector<PlrTest1>;
%template(PlrTest2Vector) std::vector<PlrTest2>;
%template(CvfDatVector) std::vector<openstudio::contam::CvfDat>;
%template(SimpleAhsVector) std::vector<openstudio::contam::Ahs>;
%template(ContamZoneVector) std::vector<openstudio::contam::Zone>;
%template(AirflowPathVector) std::vector<openstudio::contam::AirflowPath>;
%template(IntVectorVector) std::vector<std::vector<int> >;

// #ifdef SWIGCSHARP
//%rename(ContamReverseTranslator) openstudio::contam::ReverseTranslator;
%rename(ContamForwardTranslator) openstudio::contam::ForwardTranslator;
// #endif

%include <airflow/contam/PrjSubobjects.hpp>
%include <airflow/contam/PrjObjects.hpp>
%include <airflow/contam/PrjAirflowElements.hpp>
%include <airflow/contam/PrjModel.hpp>
//%include <airflow/ReverseTranslator.hpp>
%include <airflow/contam/ForwardTranslator.hpp>

#endif //AIRFLOW_I 
