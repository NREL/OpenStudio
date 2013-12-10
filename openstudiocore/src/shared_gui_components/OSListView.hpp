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

#ifndef OPENSTUDIO_OSLISTVIEW_H
#define OPENSTUDIO_OSLISTVIEW_H

#include "OSListController.hpp"
#include <QWidget>
#include <QSharedPointer>
#include <map>

class QScrollArea;
class QVBoxLayout;
class QGraphicsObject;

namespace openstudio{

class OSListItem;
class OSItemDelegate;

/** The purpose of OSListView is to display a list of items in a vertical column.
 *  This class's primary resposibility is to visually arrange a list of things.
 *  Data is provided to OSListView by a subclass of OSListController.
 *  For each item provided by the OSListController, OSListView draws a view provided by OSItemDelegate.
 *  It should be uncommon to subclass OSListView.
 */
class OSListView : public QWidget
{
  Q_OBJECT

 public:

  OSListView(bool scrollable = false, QWidget * parent = 0);

  virtual ~OSListView() {}

  void setDelegate(QSharedPointer<OSItemDelegate> delegate);

  void setListController(QSharedPointer<OSListController> listController);

  QSharedPointer<OSListController> listController() const;

  void setSpacing(int spacing);

  void setContentsMargins(int left,int top,int right,int bottom);

  void setHorizontalScrollBarAlwaysOn(bool alwaysOn);

  void setVerticalScrollBarAlwaysOn(bool alwaysOn);

 public slots:

  void refreshAllViews();

 protected:

  void paintEvent(QPaintEvent *);

 private slots:

  void insertItemView(int i);

  void removeItemView(int i);

  void removePair(QObject * object);

  void refreshItemView(int i);

 private:

  QSharedPointer<OSItemDelegate> m_delegate;

  QSharedPointer<OSListController> m_listController;

  QVBoxLayout * m_mainVLayout;

  // Use this to keep the OSListItem classes around for the life of the widget
  std::map<QObject *,QSharedPointer<OSListItem> > m_widgetItemPairs;

  bool m_scrollable;

  QScrollArea * m_scrollArea;
};

} // openstudio

#endif // OPENSTUDIO_OSLISTVIEW_H

