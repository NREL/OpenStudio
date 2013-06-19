#ifndef OPENSUTDIO_PROCESSEVENTSPROGRESSBAR_HPP
#define OPENSUTDIO_PROCESSEVENTSPROGRESSBAR_HPP

#include <utilities/plot/ProgressBar.hpp>

namespace openstudio {


// uses the progress bar interface in energy plus and version translators
// to call process events
class ProcessEventsProgressBar : public openstudio::ProgressBar
{
public:
  ProcessEventsProgressBar();
  virtual void onPercentageUpdated(double percentage);
};

}

#endif
