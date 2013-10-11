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

#ifndef OPENSTUDIO_OSCOLLAPSIBLEVIEW_H
#define OPENSTUDIO_OSCOLLAPSIBLEVIEW_H

#include <QWidget>
#include <QPointer>
#include <QPushButton>

namespace openstudio {

class OSViewSwitcher;

// OSHeader is an empty QWidget that must be subclassed to show something interesting.
// The purpose of this class is to ensure that there is a toggle button to support OSCollapsibleView. 
// It is the responsibility of the subclass to insert the toggle button into a layout.
class OSHeader : public QPushButton
{
  Q_OBJECT

  public:

  OSHeader(QPushButton * button);

  virtual ~OSHeader() {}

  QPushButton * toggleButton;

  protected:

  void paintEvent(QPaintEvent *);
};

class OSCollapsibleView : public QWidget
{
  Q_OBJECT

  public:

  OSCollapsibleView(QWidget * parent=0);

  virtual ~OSCollapsibleView() {}

  void setHeader(QWidget * header);

  void setContent(QWidget * content);

  public slots:

  void expand();

  void collapse();

  void setExpanded(bool expanded);

  bool isExpanded();

  private:

  QPointer<OSHeader> m_osHeader;
  
  OSViewSwitcher * m_headerContainer;

  OSViewSwitcher * m_contentContainer;
};

} // openstudio

#endif // OPENSTUDIO_OSCOLLAPSIBLEVIEW_H

