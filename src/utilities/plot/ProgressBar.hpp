/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
