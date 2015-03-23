#ifndef UTILITIES_CLOUD_CLOUD_I
#define UTILITIES_CLOUD_CLOUD_I

// does not turn on directors for all classes, just enables them
%module(directors="1") OpenStudio

%ignore openstudio::detail;

%{
  #include <utilities/cloud/OSServer.hpp>
  #include <utilities/cloud/CloudProvider.hpp>
  #include <utilities/cloud/CloudProvider_Impl.hpp>
  #include <utilities/cloud/VagrantProvider.hpp>
  #include <utilities/cloud/VagrantProvider_Impl.hpp>
  #include <utilities/cloud/AWSProvider.hpp>
  #include <utilities/cloud/AWSProvider_Impl.hpp>
  
  // to be ignored
  class QComboBox;
  class QMainWindow;
  class QNetworkAccessManager;
  namespace openstudio{
    class PathWatcher;
    class UpdateManager;
    class ProgressBar;
  }  
%}

// it seems that SWIG tries to create conversions of QObjects to these
%ignore QComboBox;
%ignore QMainWindow;
%ignore QNetworkAccessManager;
%ignore openstudio::PathWatcher;
%ignore openstudio::UpdateManager;
%ignore openstudio::ProgressBar;

%define SWIG_CLOUD_TEMPLATES(_name)
  %ignore std::vector<openstudio::_name>::vector(size_type);
  %ignore std::vector<openstudio::_name>::resize(size_type);
  %template(_name##Vector) std::vector<openstudio::_name>;
  %template(Optional##_name) boost::optional<openstudio::_name>;
%enddef


SWIG_CLOUD_TEMPLATES(OSServer);
SWIG_CLOUD_TEMPLATES(CloudSettings);
SWIG_CLOUD_TEMPLATES(CloudSession);
SWIG_CLOUD_TEMPLATES(CloudProvider);
SWIG_CLOUD_TEMPLATES(VagrantSettings);
SWIG_CLOUD_TEMPLATES(VagrantSession);
SWIG_CLOUD_TEMPLATES(VagrantProvider);
SWIG_CLOUD_TEMPLATES(AWSSettings);
SWIG_CLOUD_TEMPLATES(AWSSession);
SWIG_CLOUD_TEMPLATES(AWSProvider);

#if defined SWIGRUBY
  %define SWIG_CLOUDPROVIDER_EXTENSION(_name)
    %init %{
      rb_eval_string("OpenStudio::CloudSettings.class_eval { define_method(:to_" #_name "Settings) { OpenStudio::to" #_name "Settings(self); } }");
      rb_eval_string("OpenStudio::CloudSession.class_eval  { define_method(:to_" #_name "Session)  { OpenStudio::to" #_name "Session(self); } }");
      rb_eval_string("OpenStudio::CloudProvider.class_eval { define_method(:to_" #_name "Provider) { OpenStudio::to" #_name "Provider(self); } }");
    %}
  %enddef
#elif defined SWIGCSHARP
  #define SWIG_CLOUDPROVIDER_EXTENSION(_name) 
#else
  #define SWIG_CLOUDPROVIDER_EXTENSION(_name) 
#endif


%define SWIG_CLOUDPROVIDER(_name)

  #if defined SWIGCSHARP
    //%typemap(csclassmodifiers) openstudio::##_nameSettings "public partial class"
    //%typemap(csclassmodifiers) openstudio::##_nameSession "public partial class"
    //%typemap(csclassmodifiers) openstudio::##_nameProvider "public partial class"
    //SWIG_CLOUDPROVIDER_EXTENSION(_name)
  #endif
  
  namespace openstudio {
    boost::optional<##_name##Settings> to##_name##Settings(const openstudio::CloudSettings& settings);
    boost::optional<##_name##Session>  to##_name##Session(const openstudio::CloudSession& session);
    boost::optional<##_name##Provider> to##_name##Provider(const openstudio::CloudProvider& provider);
  }
    
  %inline {
    namespace openstudio {
      boost::optional<##_name##Settings> to##_name##Settings(const openstudio::CloudSettings& settings) {
        return settings.optionalCast<##_name##Settings>();
      }
      boost::optional<##_name##Session> to##_name##Session(const openstudio::CloudSession& session) {
        return session.optionalCast<##_name##Session>();
      }
      boost::optional<##_name##Provider> to##_name##Provider(const openstudio::CloudProvider& provider) {
        return provider.optionalCast<##_name##Provider>();
      }
    }
  }
  
  #if defined SWIGRUBY
    SWIG_CLOUDPROVIDER_EXTENSION(_name)
  #endif
  
%enddef

%include <utilities/cloud/OSServer.hpp>
%include <utilities/cloud/CloudProvider.hpp>
%include <utilities/cloud/VagrantProvider.hpp>
%include <utilities/cloud/AWSProvider.hpp>

SWIG_CLOUDPROVIDER(Vagrant);
SWIG_CLOUDPROVIDER(AWS);

#endif // UTILITIES_CLOUD_CLOUD_I
