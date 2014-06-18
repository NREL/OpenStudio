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

#ifndef RESULTSVIEWER_TABBARDRAG_HPP
#define RESULTSVIEWER_TABBARDRAG_HPP

#include <QTabBar>
#include <QDropEvent>
#include <QMimeData>
#include <QWidget>


namespace resultsviewer{

/** WidgetMimeData is a mime data holder to support dragging to and from tabs
*/
class WidgetMimeData : public QMimeData
{

public:
  WidgetMimeData(QWidget *widget)
  {
    m_widget = widget;
  }

  QWidget *widget() const {return m_widget;}
private:
  QWidget *m_widget;
};

/** TabBarDrag is a tab bar widget that supports dragging
*/
class TabBarDrag : public QTabBar
{
  Q_OBJECT

 public:

  TabBarDrag(QWidget* parent=nullptr);

 private:
//  void dropEvent(QDropEvent *evt);
//  void dragEnterEvent(QDragEnterEvent *e);

 protected:
  // drag event

  void mousePressEvent(QMouseEvent *e);

  void mouseMoveEvent(QMouseEvent *e);

  void performDrag();

  QPoint m_startDragPos;

 signals:

  void signalPerformDrag();
};


}; // resultsviewer namespace

#endif // RESULTSVIEWER_TABBARDRAG_HPP
