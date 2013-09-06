#ifndef UTILITIES_CLOUD_CLOUD_I
#define UTILITIES_CLOUD_CLOUD_I

// does not turn on directors for all classes, just enables them
%module(directors="1") OpenStudio

%ignore openstudio::detail;

%{
  #include <utilities/cloud/OSServer.hpp>
  #include <utilities/cloud/CloudProvider.hpp>
  #include <utilities/cloud/VagrantProvider.hpp>
  
  // to be ignored
  class QComboBox;
  class QMainWindow;
  class QNetworkAccessManager;
  namespace openstudio{
    class PathWatcher;
    class Plot2D;
    class UpdateManager;
    class LinePlot;
    class FloodPlot;
    class ProgressBar;
  }  
%}

// it seems that SWIG tries to create conversions of QObjects to these
%ignore QComboBox;
%ignore QMainWindow;
%ignore QNetworkAccessManager;
%ignore openstudio::PathWatcher;
%ignore openstudio::UpdateManager;
%ignore openstudio::Plot2D;
%ignore openstudio::LinePlot;
%ignore openstudio::FloodPlot;
%ignore openstudio::ProgressBar;

%include <utilities/cloud/OSServer.hpp>
%include <utilities/cloud/CloudProvider.hpp>
%include <utilities/cloud/VagrantProvider.hpp>

#endif // UTILITIES_CLOUD_CLOUD_I
