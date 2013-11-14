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

/** The purpose of OSItemDelegate is to create a visual representation of an OSListItem and to connect the data provided by an 
 * OSListItem to the view.  OSItemDelegate should be subclassed, and the view() method should be reimplemented to provide a
 * QWidget that is not empty.  This class will be commonly subclassed and is a member of the controller logic.  It is particular
 * to the view and the data source.  If the view proivded by OSItemDelegate is used often or if the view has a signficant amount 
 * of detail, a seperate view class should be defined outside of the OSItemDelegate and merely instantiated here.  On the other hand 
 * if the view very specific with little opportunity for reuse, and if the design is simple, it is acceptable for OSItemDelegate::view()
 * to build up a widget conglomeration on the fly from primitive widget types like QLabel, QWidget, etc.
 */
class OSItemDelegate : public QObject
{
  Q_OBJECT

  public:

  virtual ~OSItemDelegate() {}

  virtual QWidget * view(QSharedPointer<OSListItem> dataSource);
};

} // openstudio

#endif // OPENSTUDIO_OSLISTVIEW_H

