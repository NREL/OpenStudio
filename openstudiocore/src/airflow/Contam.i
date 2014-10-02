#ifndef CONTAM_I
#define CONTAM_I

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
  #include <airflow/ForwardTranslator.hpp>
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

%include <airflow/PrjSubobjects.hpp>
%include <airflow/PrjObjects.hpp>
%include <airflow/PrjAirflowElements.hpp>
%include <airflow/PrjModel.hpp>
//%include <airflow/ReverseTranslator.hpp>
%include <airflow/ForwardTranslator.hpp>

#endif //CONTAM_I 
