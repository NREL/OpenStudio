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

#ifndef OPENSTUDIO_HEADERVIEWS_H
#define OPENSTUDIO_HEADERVIEWS_H

#include "OSCollapsibleView.hpp"
#include <QPushButton>

class QLabel;

namespace openstudio{

class MeasureType;

class LightHeaderToggleButton : public QPushButton
{
  public:

  LightHeaderToggleButton(QWidget * parent = 0);
  virtual ~LightHeaderToggleButton() {}
};

class HeaderToggleButton : public QPushButton
{
  public:

  HeaderToggleButton(QWidget * parent = 0);
  virtual ~HeaderToggleButton() {}
};

class DarkGradientHeader : public OSHeader
{
  Q_OBJECT

  public:

  QLabel * label;

  DarkGradientHeader(QWidget * parent = 0);
  virtual ~DarkGradientHeader() {}
};

class LightGradientHeader : public OSHeader
{
  Q_OBJECT

  public:

  QLabel * label;

  LightGradientHeader(QWidget * parent = 0);
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

  LightHeader(QWidget * parent = 0);
  virtual ~LightHeader() {}
};

} // openstudio

#endif // OPENSTUDIO_HEADERVIEWS_H

