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
  bool isConnected = false;

  isConnected = connect(m_newAction, SIGNAL(triggered()), this, SIGNAL(newClicked()), Qt::QueuedConnection);
  OS_ASSERT(isConnected);

  isConnected = connect(m_loadFileAction, SIGNAL(triggered()), this, SIGNAL(loadFileClicked()), Qt::QueuedConnection);
  OS_ASSERT(isConnected);

  isConnected = connect(m_saveFileAction, SIGNAL(triggered()), this, SIGNAL(saveFileClicked()));
  OS_ASSERT(isConnected);

  isConnected = connect(m_saveAsFileAction, SIGNAL(triggered()), this, SIGNAL(saveAsFileClicked()));
  OS_ASSERT(isConnected);

  isConnected = connect(m_clearAllResultsAction, SIGNAL(triggered()), this, SIGNAL(clearAllResultsClicked()));
  OS_ASSERT(isConnected);

  isConnected = connect(m_exportXmlAction, SIGNAL(triggered()), this, SIGNAL(exportXmlClicked()));
  OS_ASSERT(isConnected);

  isConnected = connect(m_exportSpreadsheetAction, SIGNAL(triggered()), this, SIGNAL(exportSpreadsheetClicked()));
  OS_ASSERT(isConnected);

  isConnected = connect(m_scanForToolsAction, SIGNAL(triggered()), this, SIGNAL(scanForToolsClicked()));
  OS_ASSERT(isConnected);

  isConnected = connect(m_showToolsAction, SIGNAL(triggered()), this, SIGNAL(showToolsClicked()));
  OS_ASSERT(isConnected);

  isConnected = connect(m_exitAction, SIGNAL(triggered()),this,SIGNAL(exitClicked()), Qt::QueuedConnection);
  OS_ASSERT(isConnected);

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

  isConnected = connect(m_changeMeasuresDir, SIGNAL(triggered()),this,SIGNAL(changeMeasuresClicked()));
  OS_ASSERT(isConnected);

  m_preferencesMenu->addAction(m_changeMeasuresDir);
  
  m_configureProxy = new QAction(tr("&Configure Internet Proxy"),this);

  isConnected = connect(m_configureProxy, SIGNAL(triggered()),this,SIGNAL(configureProxyClicked()));
  OS_ASSERT(isConnected);

  m_preferencesMenu->addAction(m_scanForToolsAction);
  m_preferencesMenu->addAction(m_showToolsAction);
  
  m_preferencesMenu->addAction(m_configureProxy);

   // Window menu
  m_windowMenu = new QMenu(tr("&Measures"),this);
  addMenu(m_windowMenu);

  m_openBclDlgAction = new QAction(tr("Find &Measures"),this);

  isConnected = connect(m_openBclDlgAction, SIGNAL(triggered()),this,SIGNAL(openBclDlgClicked()));
  OS_ASSERT(isConnected);

  m_windowMenu->addAction(m_openBclDlgAction);
  
  // Cloud menu
  m_cloudMenu = new QMenu(tr("&Cloud"),this);
  addMenu(m_cloudMenu);

  m_openCloudDlgAction = new QAction(tr("&Cloud Settings"),this);

  isConnected = connect(m_openCloudDlgAction, SIGNAL(triggered()),this,SIGNAL(openCloudDlgClicked()));
  OS_ASSERT(isConnected);

  m_cloudMenu->addAction(m_openCloudDlgAction);

  m_openMonitorUseDlgAction = new QAction(tr("&Monitor Use"),this);

  isConnected = connect(m_openMonitorUseDlgAction, SIGNAL(triggered()),this,SIGNAL(openMonitorUseDlgClicked()));
  OS_ASSERT(isConnected);

  m_cloudMenu->addAction(m_openMonitorUseDlgAction);
  
  // Help menu
  m_helpMenu = new QMenu(tr("&Help"),this);
  addMenu(m_helpMenu);

  m_helpAction = new QAction(tr("PAT &Help"),this);
  m_aboutAction = new QAction(tr("&About"),this);

  isConnected = connect(m_helpAction, SIGNAL(triggered()),this,SIGNAL(helpClicked()));
  OS_ASSERT(isConnected);

  isConnected = connect(m_aboutAction, SIGNAL(triggered()),this,SIGNAL(aboutClicked()));
  OS_ASSERT(isConnected);

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
