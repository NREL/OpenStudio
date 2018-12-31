/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "OSProgressBar.hpp"

#include "Application.hpp"

#include <cmath>
#include <nano/nano_signal_slot.hpp>

/// constructor
OSProgressBar::OSProgressBar(QWidget* parent)
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
OSProgressBar::OSProgressBar(bool visible, QWidget* parent)
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
//OSProgressBar::OSProgressBar(const std::shared_ptr<QProgressBar>& impl)
//  : m_impl(impl), m_percentage(0.0)
//{}

/// virtual destructor
OSProgressBar::~OSProgressBar()
{
  m_impl->setVisible(false);
}

/// get min
int OSProgressBar::minimum() const
{
  return m_impl->minimum();
}

/// set min
void OSProgressBar::setMinimum(int min)
{
  m_impl->setMinimum(min);
  updatePercentage();
}

/// get max
int OSProgressBar::maximum() const
{
  return m_impl->maximum();
}

/// set max
void OSProgressBar::setMaximum(int max)
{
  m_impl->setMaximum(max);
  updatePercentage();
}

/// get value
int OSProgressBar::value() const
{
  return m_impl->value();
}

/// get the window title
std::string OSProgressBar::windowTitle() const
{
  return openstudio::toString(m_impl->windowTitle());
}

/// set the window title
void OSProgressBar::setWindowTitle(const std::string& title)
{
  this->setWindowTitle(openstudio::toQString(title));
}

/// get the text
std::string OSProgressBar::text() const
{
  return openstudio::toString(m_impl->text());
}

/// get if visible
bool OSProgressBar::isVisible() const
{
  return m_impl->isVisible();
}

/// set if visible
void OSProgressBar::setVisible(bool visible)
{
  m_impl->setVisible(visible);
}

/// virtual method called every time progressUpdated fires
void OSProgressBar::onPercentageUpdated(double percentage)
{
}

/// set range
void OSProgressBar::setRange(int min, int max)
{
  m_impl->setMinimum(min);
  m_impl->setMaximum(max);
  updatePercentage();
}

/// set value
void OSProgressBar::setValue(int value)
{
  m_impl->setValue(value);
  updatePercentage();
}

/// set window title
void OSProgressBar::setWindowTitle(const QString& windowTitle)
{
  m_impl->setWindowTitle(windowTitle);
}

//std::shared_ptr<QProgressBar> OSProgressBar::impl() const
//{
//  return m_impl;
//}

