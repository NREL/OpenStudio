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

#include "PatMainMenu.hpp"

#include "PatApp.hpp"
#include "CloudMonitor.hpp"

#include "../utilities/core/Assert.hpp"

#include <QMenu>

namespace openstudio{
  
namespace pat {

PatMainMenu::PatMainMenu(QWidget *parent) : 
  QMenuBar(parent)
{
  // File menu
  m_fileMenu = new QMenu(tr("&File"),this);
  addMenu(m_fileMenu);

  m_newAction = new QAction(tr("&New Project"), this);
  m_loadFileAction = new QAction(tr("&Open Project"), this);
  m_saveFileAction = new QAction(tr("&Save Project"), this);
  m_saveAsFileAction = new QAction(tr("Save Project &As"), this);
  m_clearAllResultsAction = new QAction(tr("&Clear All Results"), this);
  m_exportXmlAction = new QAction(tr("&Export XML Report"), this);
  m_exportSpreadsheetAction = new QAction(tr("&Export Analysis Spreadsheet"), this);
  m_scanForToolsAction = new QAction(tr("Scan for &Tools"), this);
  m_showToolsAction = new QAction(tr("Show Too&ls"), this);
  m_exitAction = new QAction(tr("E&xit"), this);

  m_exitAction->setShortcuts(QKeySequence::Quit);
  m_newAction->setShortcut(QKeySequence(QKeySequence::New));
  m_loadFileAction->setShortcut(QKeySequence(QKeySequence::Open));
  m_saveFileAction->setShortcut(QKeySequence(QKeySequence::Save));

  // DLM: actions which result in this menu being deleted should be queued

  connect(m_newAction, &QAction::triggered, this, &PatMainMenu::newClicked, Qt::QueuedConnection);

  connect(m_loadFileAction, &QAction::triggered, this, &PatMainMenu::loadFileClicked, Qt::QueuedConnection);

  connect(m_saveFileAction, &QAction::triggered, this, &PatMainMenu::saveFileClicked);

  connect(m_saveAsFileAction, &QAction::triggered, this, &PatMainMenu::saveAsFileClicked);

  connect(m_clearAllResultsAction, &QAction::triggered, this, &PatMainMenu::clearAllResultsClicked);

  connect(m_exportXmlAction, &QAction::triggered, this, &PatMainMenu::exportXmlClicked);

  connect(m_exportSpreadsheetAction, &QAction::triggered, this, &PatMainMenu::exportSpreadsheetClicked);

  connect(m_scanForToolsAction, &QAction::triggered, this, &PatMainMenu::scanForToolsClicked);

  connect(m_showToolsAction, &QAction::triggered, this, &PatMainMenu::showToolsClicked);

  connect(m_exitAction, &QAction::triggered, this, &PatMainMenu::exitClicked, Qt::QueuedConnection);

  m_fileMenu->addAction(m_newAction);
  m_fileMenu->addAction(m_loadFileAction);
  /*************************/
  m_fileMenu->addSeparator();
  m_fileMenu->addAction(m_saveFileAction);
  m_fileMenu->addAction(m_saveAsFileAction);
  /*************************/
  m_fileMenu->addSeparator();
  m_fileMenu->addAction(m_clearAllResultsAction);
  m_fileMenu->addAction(m_exportXmlAction);
  m_fileMenu->addAction(m_exportSpreadsheetAction);
  /*************************/
  m_fileMenu->addSeparator();
  m_fileMenu->addAction(m_exitAction);

  // Preferences menu
  m_preferencesMenu = new QMenu(tr("&Preferences"),this);
  addMenu(m_preferencesMenu);

  m_changeMeasuresDir = new QAction(tr("&Change My Measures Directory"),this);

  connect(m_changeMeasuresDir, &QAction::triggered, this, &PatMainMenu::changeMeasuresClicked);

  m_preferencesMenu->addAction(m_changeMeasuresDir);
  
  m_configureProxy = new QAction(tr("&Configure Internet Proxy"),this);

  connect(m_configureProxy, &QAction::triggered, this, &PatMainMenu::configureProxyClicked);

  m_preferencesMenu->addAction(m_scanForToolsAction);
  m_preferencesMenu->addAction(m_showToolsAction);
  
  m_preferencesMenu->addAction(m_configureProxy);

   // Window menu
  m_windowMenu = new QMenu(tr("&Measures"),this);
  addMenu(m_windowMenu);

  m_openBclDlgAction = new QAction(tr("Find &Measures"),this);

  connect(m_openBclDlgAction, &QAction::triggered, this, &PatMainMenu::openBclDlgClicked);

  m_windowMenu->addAction(m_openBclDlgAction);
  
  // Cloud menu
  m_cloudMenu = new QMenu(tr("&Cloud"),this);
  addMenu(m_cloudMenu);

  m_openCloudDlgAction = new QAction(tr("&Cloud Settings"),this);
  m_openCloudDlgAction->setEnabled(false);

  connect(m_openCloudDlgAction, &QAction::triggered, this, &PatMainMenu::openCloudDlgClicked);

  m_cloudMenu->addAction(m_openCloudDlgAction);

  m_openMonitorUseDlgAction = new QAction(tr("&Monitor Use"),this);
  m_openMonitorUseDlgAction->setEnabled(false);

  connect(m_openMonitorUseDlgAction, &QAction::triggered, this, &PatMainMenu::openMonitorUseDlgClicked);

  m_cloudMenu->addAction(m_openMonitorUseDlgAction);
  
  // Help menu
  m_helpMenu = new QMenu(tr("&Help"),this);
  addMenu(m_helpMenu);

  m_helpAction = new QAction(tr("PAT &Help"),this);
  m_aboutAction = new QAction(tr("&About"),this);

  connect(m_helpAction, &QAction::triggered, this, &PatMainMenu::helpClicked);

  connect(m_aboutAction, &QAction::triggered, this, &PatMainMenu::aboutClicked);

  m_helpMenu->addAction(m_helpAction);
  m_helpMenu->addAction(m_aboutAction);
}

PatMainMenu::~PatMainMenu()
{
}

void PatMainMenu::configure(bool haveCurrentProject)
{
  if (haveCurrentProject){

    m_saveFileAction->setEnabled(true);
    m_saveAsFileAction->setEnabled(true);
    m_clearAllResultsAction->setEnabled(true);
    m_exportXmlAction->setEnabled(true);
    m_exportSpreadsheetAction->setEnabled(true);

  }else{

    m_saveFileAction->setEnabled(false);
    m_saveAsFileAction->setEnabled(false);
    m_clearAllResultsAction->setEnabled(false);
    m_exportXmlAction->setEnabled(false);
    m_exportSpreadsheetAction->setEnabled(false);

  }
}

}

} // openstudio
