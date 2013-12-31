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

#ifndef OPENSTUDIO_OSGRIDVIEW_H
#define OPENSTUDIO_OSGRIDVIEW_H

#include "OSGridController.hpp"

#include <QSharedPointer>
#include <QWidget>

#include <map>

class QGraphicsObject;
class QScrollArea;
class QGridLayout;

namespace openstudio{

class OSGridItem;
class OSGridItemDelegate;

/** The purpose of OSGridView is to display a group of items in a grid view.
 *  Data is provided to OSGridView by a subclass of OSGridController.
 *  For each item provided by the OSGridController, OSGridView draws a view provided by OSGridItemDelegate.
 *  It should be uncommon to subclass OSGridView.
 */
class OSGridView : public QWidget
{
  Q_OBJECT

 public:

  OSGridView(bool scrollable = false, QWidget * parent = 0);

  virtual ~OSGridView() {}

  void setDelegate(QSharedPointer<OSGridItemDelegate> delegate);

  void setGridController(QSharedPointer<OSGridController> gridController);

  QSharedPointer<OSGridController> gridController() const;

  void setSpacing(int spacing);

  void setContentsMargins(int left,int top,int right,int bottom);

  void setHorizontalScrollBarAlwaysOn(bool alwaysOn);

  void setVerticalScrollBarAlwaysOn(bool alwaysOn);

 public slots:

  void refreshAllViews(); // Note: Use sparingly for best performance

 protected:

  void paintEvent(QPaintEvent *);

 private slots:

  void insertItemView(int row, int column);

  void removeItemView(int row, int column);

  void removePair(QObject * object);

  void refreshItemView(int row, int column);

 private:

  QSharedPointer<OSGridItemDelegate> m_delegate;

  QSharedPointer<OSGridController> m_gridController;

  QGridLayout * m_gridLayout;

  // Use this to keep the OSGridItem classes around for the life of the widget
  std::map<QObject *,QSharedPointer<OSGridItem> > m_widgetItemPairs;

  bool m_scrollable;

  QScrollArea * m_scrollArea;
};

} // openstudio

#endif // OPENSTUDIO_OSGRIDVIEW_H

