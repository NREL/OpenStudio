/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
*  All rights reserved.
*  
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*  
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*  
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include "ProgressBar.hpp"

#include "../core/Application.hpp"

#include <cmath>

namespace openstudio{

  /// constructor
  ProgressBar::ProgressBar(QWidget* parent)
    : m_percentage(0.0)
  {
    // make sure a QApplication exists
    openstudio::Application::instance().application();

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
    openstudio::Application::instance().application();

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
      emit percentageUpdated(m_percentage);
      onPercentageUpdated(m_percentage);
    }
  }

} // openstudio
