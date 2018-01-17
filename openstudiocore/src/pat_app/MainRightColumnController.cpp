/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

  connect(horizontalTabWidget.data(), &HorizontalTabWidget::tabSelected, this, &MainRightColumnController::showHorizontalTab);

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
    // Evan note: It's bad to play with null pointers
      if (!m_localLibraryController->localLibraryView) {
        //return;
      }
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

