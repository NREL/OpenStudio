#ifndef AIRFLOW_I
#define AIRFLOW_I

#ifdef SWIGPYTHON
%module openstudiocontam
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
