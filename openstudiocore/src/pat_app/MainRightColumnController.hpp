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

#ifndef PATAPP_MAINRIGHTCOLUMNCONTROLLER_HPP
#define PATAPP_MAINRIGHTCOLUMNCONTROLLER_HPP

#include <QObject>
#include <QPointer>
#include <QSharedPointer> 

namespace openstudio{

class LocalLibraryController;
class EditController;
  
namespace pat {

class HorizontalTabWidget;

class MainRightColumnController : public QObject
{
  Q_OBJECT

  public:

  QPointer<HorizontalTabWidget> horizontalTabWidget; 

  MainRightColumnController();

  virtual ~MainRightColumnController();

  QSharedPointer<LocalLibraryController> localLibraryController() const;

  QSharedPointer<EditController> editController() const;

  enum RightColumnTabID
  {
    MEASURE_LIBRARY,
    EDIT
  };

  public slots:

  // Activate the tab button corresponding to id
  // Show the associated view
  void setCurrentId(int id);

  private slots:

  // Show the view corresponding to id
  // This method does not change the state of the tab buttons
  void showHorizontalTab(int id); 

  private:

  QSharedPointer<LocalLibraryController> m_localLibraryController;
  QSharedPointer<EditController> m_editController;
};

}

} // openstudio

#endif // PAT_APP_MAINRIGHTCOLUMNCONTROLLER_HPP

