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

#include "MainRightColumnController.hpp"
#include "HorizontalTabWidget.hpp"
#include "../shared_gui_components/LocalLibraryController.hpp"
#include "../shared_gui_components/EditController.hpp"
#include "../shared_gui_components/OSViewSwitcher.hpp"
#include "../shared_gui_components/OSListView.hpp"
#include "../shared_gui_components/LocalLibraryView.hpp"
#include "../shared_gui_components/EditView.hpp"
#include "PatApp.hpp"

#include "../utilities/core/Assert.hpp"

#include <QLayout>
#include <QStackedWidget>

namespace openstudio{
  
namespace pat {

MainRightColumnController::MainRightColumnController()
  : QObject()
{
  m_localLibraryController = QSharedPointer<LocalLibraryController>( new LocalLibraryController(PatApp::instance()) );

  m_editController = QSharedPointer<EditController>( new EditController() );

  horizontalTabWidget = new HorizontalTabWidget();

  horizontalTabWidget->addTab(MEASURE_LIBRARY,"Measure Library");

  horizontalTabWidget->addTab(EDIT,"Edit");

  bool isConnected = connect(horizontalTabWidget,SIGNAL(tabSelected(int)),this,SLOT(showHorizontalTab(int)));
  OS_ASSERT(isConnected);

  showHorizontalTab(MEASURE_LIBRARY);
}

MainRightColumnController::~MainRightColumnController()
{
  if( horizontalTabWidget ) { delete horizontalTabWidget; }
}

QSharedPointer<LocalLibraryController> MainRightColumnController::localLibraryController() const
{
  return m_localLibraryController;
}

QSharedPointer<EditController> MainRightColumnController::editController() const
{
  return m_editController;
}

void MainRightColumnController::showHorizontalTab(int id)
{
  switch( id )
  {
    case MEASURE_LIBRARY:
      horizontalTabWidget->mainViewSwitcher->setView(m_localLibraryController->localLibraryView);

      break;
    case EDIT:
      horizontalTabWidget->mainViewSwitcher->setView(m_editController->editView);

      break;
    default:
      break;
  }
}

void MainRightColumnController::setCurrentId(int id)
{
  horizontalTabWidget->setCurrentId(id);
}

} // pat

} // openstudio

