/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include <QProgressBar>
#include <nano/nano_signal_slot.hpp>

#include <memory>

namespace openstudio{

  /** ProgressBar wraps a QProgressBar and provides virtual methods setRange, setValue, and setWindowTitle(QString)
   *  which may be overridden.
   *
   *  ProgressBar an atypical QObject because it is designed to be stack allocated.  In many cases it
   *  would be preferred to connect your own heap allocated QObject to the signals directly rather
   *  than using this convenience class.
   **/
  class UTILITIES_API ProgressBar {

  public:

    /// constructor
    ProgressBar(QWidget* parent = nullptr);

    /// constructor
    ProgressBar(bool visible, QWidget* parent = nullptr);

    /// constructor from impl
    //ProgressBar(const std::shared_ptr<QProgressBar>& impl);

    /// virtual destructor
    virtual ~ProgressBar();

    /// get min
    int minimum() const;

    /// set min
    void setMinimum(int min);

    /// get max
    int maximum() const;

    /// set max
    void setMaximum(int max);

    /// get value
    int value() const;

    /// get the window title
    std::string windowTitle() const;

    /// set the window title
    void setWindowTitle(const std::string& title);

    /// get the text
    std::string text() const;

    /// get if visible
    bool isVisible() const;

    /// set if visible
    void setVisible(bool visible);

    /// virtual method called every time percentageUpdated fires
    virtual void onPercentageUpdated(double percentage);

  // public slots:

    /// set range
    void setRange(int min, int max);

    /// set value
    void setValue(int value);

    /// set window title
    void setWindowTitle(const QString& windowTitle);

  // signals:

    /// called every time progress increases by 1% more than last progress
    Nano::Signal<void(double)> percentageUpdated;


  protected:

    /// return the impl
    //std::shared_ptr<QProgressBar> impl() const;

  private:
    /// impl
    std::shared_ptr<QProgressBar> m_impl;

    void updatePercentage();

    double m_percentage;
  };

} // openstudio

#endif //UTILITIES_PLOT_PROGRESSBAR_HPP
