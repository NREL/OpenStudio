/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ProgressBar.hpp"

#include <cmath>
#include <nano/nano_signal_slot.hpp>

namespace openstudio {

/// constructor
ProgressBar::ProgressBar() : m_percentage(0.0) {}

/// virtual destructor

/// virtual method called every time progressUpdated fires
void ProgressBar::onPercentageUpdated(double percentage) {}

void ProgressBar::updatePercentage() {
  double value = this->value();
  double max = this->maximum();
  double min = this->minimum();
  double range = max - min;

  double newPercentage = 0.0;
  if (range > 0.0) {
    newPercentage = 100.0 * (value - min) / range;
  }

  if (std::fabs(newPercentage - m_percentage) >= 1.0) {
    m_percentage = newPercentage;
    this->percentageUpdated.nano_emit(m_percentage);
    onPercentageUpdated(m_percentage);
  }
}

}  // namespace openstudio
