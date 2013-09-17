/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#ifndef OPENSTUDIO_PAT_LOCALLIBRARYVIEW_H
#define OPENSTUDIO_PAT_LOCALLIBRARYVIEW_H

#include "OSDragableView.hpp"
#include "HeaderViews.hpp"
#include <QWidget>
#include <QPoint>

class QPushButton;
class QLabel;

namespace openstudio{

class OSViewSwitcher;
  
class LocalLibraryView : public QWidget
{
  Q_OBJECT

  public:

  LocalLibraryView(QWidget * parent = 0);

  virtual ~LocalLibraryView() {}

  OSViewSwitcher * mainViewSwitcher;

  QPushButton * bclMeasuresButton;

  QPushButton * myMeasuresButton;

  QPushButton * duplicateMeasureButton;

  QPushButton * addMeasureButton;

  QPushButton * myMeasuresFolderButton;

  QPushButton * updateMyMeasuresButton;

  QPushButton * updateBCLMeasuresButton;

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

  LibraryItemView(QWidget * parent = 0);

  virtual ~LibraryItemView() {}

  QLabel * label;
  QLabel * m_measureTypeBadge;
  QLabel * m_bclBadge;

  public slots:

  void setHasEmphasis(bool hasEmphasis);

  protected:

  void paintEvent(QPaintEvent *);
};

} // openstudio

#endif // OPENSTUDIO_PAT_LOCALLIBRARYVIEW_H

