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

#ifndef UTILITIES_PLOT_PROGRESSBAR_HPP
#define UTILITIES_PLOT_PROGRESSBAR_HPP

#include "../UtilitiesAPI.hpp"
#include "../core/Macro.hpp"
#include "../core/String.hpp"

#include <QProgressBar>

#include <memory>

namespace openstudio{

  /** ProgressBar wraps a QProgressBar and provides virtual methods setRange, setValue, and setWindowTitle(QString)
   *  which may be overridden. 
   *
   *  ProgressBar an atypical QObject because it is designed to be stack allocated.  In many cases it 
   *  would be preferred to connect your own heap allocated QObject to the signals directly rather
   *  than using this convenience class.
   **/
  class UTILITIES_API ProgressBar : public QObject {
  
    Q_OBJECT
  
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

  public slots:

    /// set range
    void setRange(int min, int max);

    /// set value
    void setValue(int value);

    /// set window title
    void setWindowTitle(const QString& windowTitle);

  signals:

    /// called every time progress increases by 1% more than last progress
    void percentageUpdated(double percentage);

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
