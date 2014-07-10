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

#ifndef SHAREDGUICOMPONENTS_LOCALLIBRARYVIEW_HPP
#define SHAREDGUICOMPONENTS_LOCALLIBRARYVIEW_HPP

#include "OSDragableView.hpp"

#include "HeaderViews.hpp"

#include <QPoint>
#include <QWidget>

class QPushButton;
class QLabel;

namespace openstudio{

class MeasureBadge;
class OSViewSwitcher;
  
class LocalLibraryView : public QWidget
{
  Q_OBJECT

  public:

  LocalLibraryView(QWidget * parent = nullptr);

  virtual ~LocalLibraryView() {}

  OSViewSwitcher * mainViewSwitcher;

  QPushButton * duplicateMeasureButton;

  QPushButton * addMeasureButton;

  QPushButton * myMeasuresFolderButton;

  QPushButton * addBCLMeasureButton;
};

class LibraryGroupItemHeader : public LightGradientHeader
{
  Q_OBJECT

  public:

  LibraryGroupItemHeader();

  virtual ~LibraryGroupItemHeader() {}

  public slots:

  void setCount(int count);

  private:

  QLabel * countLabel;
};

class LibrarySubGroupItemHeader : public LightHeader
{
  Q_OBJECT

  public:

  LibrarySubGroupItemHeader();

  virtual ~LibrarySubGroupItemHeader() {}

  public slots:

  void setCount(int count);

  private:

  QLabel * countLabel;
};

class LibraryItemView : public OSDragableView
{
  Q_OBJECT

  public:

  LibraryItemView(QWidget * parent = nullptr);

  virtual ~LibraryItemView() {}

  QLabel * label;
  QLabel * m_measureTypeBadge;
  MeasureBadge * m_measureBadge;

  public slots:

  void setHasEmphasis(bool hasEmphasis);

  protected:

  void paintEvent(QPaintEvent *);
};

} // openstudio

#endif // SHAREDGUICOMPONENTS_LOCALLIBRARYVIEW_HPP

