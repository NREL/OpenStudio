#ifndef UTILITIES_PLOT_PLOT2D_I
#define UTILITIES_PLOT_PLOT2D_I

%{
  #include <utilities/plot/Plot2D.hpp>
%}

// DLM@20100120: values of this enum conflict with the class names LinePlot and FloodPlot in ruby
%ignore Plot2DType;

#if defined SWIGJAVA
%ignore openstudio::Plot2D::windowTitle;
#endif


%include <utilities/plot/Plot2D.hpp>

#endif //UTILITIES_PLOT_PLOT2D_I
