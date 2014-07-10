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

#ifndef SHAREDGUICOMPONENTS_HEADERVIEWS_HPP
#define SHAREDGUICOMPONENTS_HEADERVIEWS_HPP

#include "OSCollapsibleView.hpp"
#include <QPushButton>

class QLabel;

namespace openstudio{

class MeasureType;

class LightHeaderToggleButton : public QPushButton
{
  public:

  LightHeaderToggleButton(QWidget * parent = nullptr);
  virtual ~LightHeaderToggleButton() {}
};

class HeaderToggleButton : public QPushButton
{
  public:

  HeaderToggleButton(QWidget * parent = nullptr);
  virtual ~HeaderToggleButton() {}
};

class DarkGradientHeader : public OSHeader
{
  Q_OBJECT

  public:

  QLabel * label;

  DarkGradientHeader(QWidget * parent = nullptr);
  virtual ~DarkGradientHeader() {}
};

class LightGradientHeader : public OSHeader
{
  Q_OBJECT

  public:

  QLabel * label;

  LightGradientHeader(QWidget * parent = nullptr);
  void setMeasureType(MeasureType measureType);
  virtual ~LightGradientHeader() {}

  private:

  QLabel * m_measureTypeBadge;
};

class LightHeader : public OSHeader
{
  Q_OBJECT

  public:

  QLabel * label;

  LightHeader(QWidget * parent = nullptr);
  virtual ~LightHeader() {}
};

} // openstudio

#endif // SHAREDGUICOMPONENTS_HEADERVIEWS_HPP

