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

#include "ProgressBar.hpp"

#include "../core/Application.hpp"

#include <cmath>
#include <nano/nano_signal_slot.hpp>

namespace openstudio{

  /// constructor
  ProgressBar::ProgressBar(QWidget* parent)
    : m_percentage(0.0)
  {
    // make sure a QApplication exists
    openstudio::Application::instance().application(true);

    // create a QProgressBar
    m_impl = std::shared_ptr<QProgressBar>(new QProgressBar(parent));

    // if no parent assume we want this visible
    if (parent == nullptr){
      m_impl->setVisible(true);
    }

    updatePercentage();
  }

  /// constructor
  ProgressBar::ProgressBar(bool visible, QWidget* parent)
    : m_percentage(0.0)
  {
    // make sure a QApplication exists
    openstudio::Application::instance().application(true);

    // create a QProgressBar
    m_impl = std::shared_ptr<QProgressBar>(new QProgressBar(parent));

    // set visibility
    m_impl->setVisible(visible);

    updatePercentage();
  }

  /// constructor from impl
  //ProgressBar::ProgressBar(const std::shared_ptr<QProgressBar>& impl)
  //  : m_impl(impl), m_percentage(0.0)
  //{}

  /// virtual destructor
  ProgressBar::~ProgressBar()
  {
    m_impl->setVisible(false);
  }

  /// get min
  int ProgressBar::minimum() const
  {
    return m_impl->minimum();
  }

  /// set min
  void ProgressBar::setMinimum(int min)
  {
    m_impl->setMinimum(min);
    updatePercentage();
  }

  /// get max
  int ProgressBar::maximum() const
  {
    return m_impl->maximum();
  }

  /// set max
  void ProgressBar::setMaximum(int max)
  {
    m_impl->setMaximum(max);
    updatePercentage();
  }

  /// get value
  int ProgressBar::value() const
  {
    return m_impl->value();
  }

  /// get the window title
  std::string ProgressBar::windowTitle() const
  {
    return toString(m_impl->windowTitle());
  }

  /// set the window title
  void ProgressBar::setWindowTitle(const std::string& title)
  {
    this->setWindowTitle(toQString(title));
  }

  /// get the text
  std::string ProgressBar::text() const
  {
    return toString(m_impl->text());
  }

  /// get if visible
  bool ProgressBar::isVisible() const
  {
    return m_impl->isVisible();
  }

  /// set if visible
  void ProgressBar::setVisible(bool visible)
  {
    m_impl->setVisible(visible);
  }

  /// virtual method called every time progressUpdated fires
  void ProgressBar::onPercentageUpdated(double percentage)
  {
  }

  /// set range
  void ProgressBar::setRange(int min, int max)
  {
    m_impl->setMinimum(min);
    m_impl->setMaximum(max);
    updatePercentage();
  }

  /// set value
  void ProgressBar::setValue(int value)
  {
    m_impl->setValue(value);
    updatePercentage();
  }

  /// set window title
  void ProgressBar::setWindowTitle(const QString& windowTitle)
  {
    m_impl->setWindowTitle(windowTitle);
  }

  //std::shared_ptr<QProgressBar> ProgressBar::impl() const
  //{
  //  return m_impl;
  //}

  void ProgressBar::updatePercentage()
  {
    double value = this->value();
    double max = this->maximum();
    double min = this->minimum();
    double range = max - min;

    double newPercentage = 0.0;
    if (range > 0.0){
      newPercentage = 100.0 * (value - min) / range;
    }

    if (fabs(newPercentage-m_percentage) >= 1.0){
      m_percentage = newPercentage;
      this->percentageUpdated.nano_emit(m_percentage);
      onPercentageUpdated(m_percentage);
    }
  }

} // openstudio
