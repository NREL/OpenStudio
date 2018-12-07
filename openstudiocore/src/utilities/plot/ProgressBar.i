#ifndef UTILITIES_PLOT_PROGRESSBAR_I
#define UTILITIES_PLOT_PROGRESSBAR_I

%{
  #include <utilities/plot/ProgressBar.hpp>
%}

%feature("director") ProgressBar;

%ignore percentageUpdated;

%include <utilities/plot/ProgressBar.hpp>

#endif //UTILITIES_PLOT_PROGRESSBAR_I
