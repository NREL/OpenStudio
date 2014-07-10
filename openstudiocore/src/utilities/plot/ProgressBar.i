#ifndef UTILITIES_PLOT_PROGRESSBAR_I
#define UTILITIES_PLOT_PROGRESSBAR_I

%{
  #include <utilities/plot/ProgressBar.hpp>
%}

%feature("director") ProgressBar;  

%ignore ProgressBar(const std::shared_ptr<QProgressBar>& impl);

%include <utilities/plot/ProgressBar.hpp>

#endif //UTILITIES_PLOT_PROGRESSBAR_I