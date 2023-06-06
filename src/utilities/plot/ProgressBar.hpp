/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_PLOT_PROGRESSBAR_HPP
#define UTILITIES_PLOT_PROGRESSBAR_HPP

#include "../UtilitiesAPI.hpp"
#include "../core/Macro.hpp"
#include "../core/String.hpp"

#include <nano/nano_signal_slot.hpp>

#include <memory>

namespace openstudio {

/** ProgressBar is a pure virtual class providing virtual methods setRange, setValue, and setWindowTitle   
   *  which may be overridden.
   **/
class UTILITIES_API ProgressBar
{

 public:
  /// virtual destructor
  virtual ~ProgressBar() = default;

  /// get min
  virtual int minimum() const = 0;

  /// set min
  virtual void setMinimum(int min) = 0;

  /// get max
  virtual int maximum() const = 0;

  /// set max
  virtual void setMaximum(int max) = 0;

  /// get value
  virtual int value() const = 0;

  /// get the window title
  virtual std::string windowTitle() const = 0;

  /// set the window title
  virtual void setWindowTitle(const std::string& title) = 0;

  /// get the text
  virtual std::string text() const = 0;

  /// get if visible
  virtual bool isVisible() const = 0;

  /// set if visible
  virtual void setVisible(bool visible) = 0;

  /// virtual method called every time percentageUpdated fires
  virtual void onPercentageUpdated(double percentage);

  // public slots:

  /// set range
  virtual void setRange(int min, int max) = 0;

  /// set value
  virtual void setValue(int value) = 0;

  // signals:

  /// called every time progress increases by 1% more than last progress
  Nano::Signal<void(double)> percentageUpdated;

 protected:
  ProgressBar();

  void updatePercentage();

 private:
  double m_percentage;
};

}  // namespace openstudio

#endif  //UTILITIES_PLOT_PROGRESSBAR_HPP
