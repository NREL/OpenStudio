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

#ifndef RESULTSVIEWER_TABDROPDOCK_HPP
#define RESULTSVIEWER_TABDROPDOCK_HPP

#include <QTabWidget>
#include <QDropEvent>


namespace resultsviewer{

/** TabDropDock is a ui tab that supports dropping of mimedata
*/
class TabDropDock : public QTabWidget
{
  Q_OBJECT

public:

  TabDropDock( QWidget* parent=nullptr);


private:
  void dropEvent(QDropEvent *evt);
  void dragEnterEvent(QDragEnterEvent *e);

protected:
  // drag event
 // void mousePressEvent(QMouseEvent *e);
 // void mouseMoveEvent(QMouseEvent *e);
public slots:
  void slotPerformDrag();
 // QPoint m_startDragPos;

signals:

};


}; // resultsviewer namespace

#endif // RESULTSVIEWER_TABDROPDOCK_HPP
