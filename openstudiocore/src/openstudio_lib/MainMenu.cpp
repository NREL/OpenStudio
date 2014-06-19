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

#include "MainMenu.hpp"

#include "FileOperations.hpp"
#include "OSAppBase.hpp"
#include "OSDocument.hpp"

#include "../osversion/VersionTranslator.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Path.hpp"

#include <QMenu>

namespace openstudio {

MainMenu::MainMenu(bool isIP, bool isPlugin, QWidget *parent) : 
  QMenuBar(parent), m_isPlugin(isPlugin)
{
  m_isIP = isIP;

  QAction * action = 0;

  bool isConnected = 0;

  // File menu
  m_fileMenu = new QMenu(tr("&File"),this);
  addMenu(m_fileMenu);

  // DLM: actions which result in this menu being deleted should be queued
  action = new QAction(tr("&New"), this);
  action->setShortcut(QKeySequence(QKeySequence::New));
  m_fileMenu->addAction(action);
  isConnected = connect(action, SIGNAL(triggered()), this, SIGNAL(newClicked()), Qt::QueuedConnection);
  OS_ASSERT(isConnected);

  // DLM: actions which result in this menu being deleted should be queued
  action = new QAction(tr("&Open"), this);
  action->setShortcut(QKeySequence(QKeySequence::Open));
  m_fileMenu->addAction(action);
  isConnected = connect(action, SIGNAL(triggered()), this, SIGNAL(loadFileClicked()), Qt::QueuedConnection);
  OS_ASSERT(isConnected);

  m_fileMenu->addSeparator();
  
  m_revertToSavedAction = new QAction(tr("Revert to Saved"), this);
  m_revertToSavedAction->setDisabled(true);
  m_fileMenu->addAction(m_revertToSavedAction);
  isConnected = connect(m_revertToSavedAction, SIGNAL(triggered()), this, SIGNAL(revertFileClicked()), Qt::QueuedConnection);
  OS_ASSERT(isConnected);

  action = new QAction(tr("&Save"), this);
  action->setShortcut(QKeySequence(QKeySequence::Save)); 
  m_fileMenu->addAction(action);
  isConnected = connect(action, SIGNAL(triggered()), this, SIGNAL(saveFileClicked()));
  OS_ASSERT(isConnected);

  action = new QAction(tr("Save &As"), this);
  m_fileMenu->addAction(action);
  isConnected = connect(action, SIGNAL(triggered()), this, SIGNAL(saveAsFileClicked()));
  OS_ASSERT(isConnected); 

  m_fileMenu->addSeparator();

   //formatMenu = editMenu->addMenu(tr("&Format"))
  QMenu * importMenu = m_fileMenu->addMenu(tr("Import"));

  action = new QAction(tr("IDF"), this);
  importMenu->addAction(action);
  isConnected = connect(action, SIGNAL(triggered()), this, SIGNAL(importClicked()), Qt::QueuedConnection);
  OS_ASSERT(isConnected);

  action = new QAction(tr("gbXML"), this); 
  importMenu->addAction(action);
  isConnected = connect(action, SIGNAL(triggered()), this, SIGNAL(importgbXMLClicked()), Qt::QueuedConnection);
  OS_ASSERT(isConnected);

  action = new QAction(tr("SDD"), this); 
  importMenu->addAction(action);
  isConnected = connect(action, SIGNAL(triggered()), this, SIGNAL(importSDDClicked()), Qt::QueuedConnection);
  OS_ASSERT(isConnected);

  QMenu * exportMenu = m_fileMenu->addMenu(tr("Export"));

  action = new QAction(tr("IDF"), this);
  exportMenu->addAction(action);
  isConnected = connect(action, SIGNAL(triggered()), this, SIGNAL(exportClicked()));
  OS_ASSERT(isConnected);

  action = new QAction(tr("gbXML"), this);
  exportMenu->addAction(action);
  isConnected = connect(action, SIGNAL(triggered()), this, SIGNAL(exportgbXMLClicked()));
  OS_ASSERT(isConnected);

  action = new QAction(tr("SDD"), this);
  exportMenu->addAction(action);
  isConnected = connect(action, SIGNAL(triggered()), this, SIGNAL(exportSDDClicked()));
  OS_ASSERT(isConnected);

  action = new QAction(tr("&Load Library"), this);
  m_fileMenu->addAction(action);
  isConnected = connect(action, SIGNAL(triggered()), this, SIGNAL(loadLibraryClicked()));
  OS_ASSERT(isConnected);

  if (!m_isPlugin){

    m_fileMenu->addSeparator();

    //m_fileMenu->addAction(action);
    //isConnected = connect(action, SIGNAL(triggered()), this, SIGNAL(showRubyConsoleClicked()));
    //OS_ASSERT(isConnected);

    m_fileMenu->addSeparator();

    action = new QAction(tr("E&xit"), this);
    action->setShortcuts(QKeySequence::Quit);
    m_fileMenu->addAction(action);
    isConnected = connect(action, SIGNAL(triggered()),this,SIGNAL(exitClicked()), Qt::QueuedConnection);
    OS_ASSERT(isConnected);

  }

  // Preferences menu
  m_preferencesMenu = new QMenu(tr("&Preferences"),this);
  addMenu(m_preferencesMenu);

  QMenu * unitsMenu = m_preferencesMenu->addMenu(tr("Units"));

  m_displaySIUnitsAction = new QAction(tr("Metric (&SI)"),this);
  unitsMenu->addAction(m_displaySIUnitsAction);
  isConnected = connect(m_displaySIUnitsAction, SIGNAL(triggered()),this,SLOT(displaySIUnitsClicked()));
  OS_ASSERT(isConnected);

  m_displayIPUnitsAction = new QAction(tr("English (&I-P)"),this);
  unitsMenu->addAction(m_displayIPUnitsAction);
  isConnected = connect(m_displayIPUnitsAction, SIGNAL(triggered()),this,SLOT(displayIPUnitsClicked()));
  OS_ASSERT(isConnected);

  action = new QAction(tr("Change My Measures Directory"),this);
  m_preferencesMenu->addAction(action);
  isConnected = connect(action, SIGNAL(triggered()),this,SIGNAL(changeMyMeasuresDir()));
  OS_ASSERT(isConnected);

  action = new QAction(tr("Scan for Tools"),this);
  m_preferencesMenu->addAction(action);
  isConnected = connect(action, SIGNAL(triggered()),this,SIGNAL(scanForToolsClicked()));
  OS_ASSERT(isConnected);

  action = new QAction(tr("Show Tools"),this);
  m_preferencesMenu->addAction(action);
  isConnected = connect(action, SIGNAL(triggered()),this,SIGNAL(showRunManagerPreferencesClicked()));
  OS_ASSERT(isConnected);

  action = new QAction(tr("Change BCL Login Information"),this);
  //m_preferencesMenu->addAction(action);
  isConnected = connect(action, SIGNAL(triggered()),this,SIGNAL(changeBclLogin()));
  OS_ASSERT(isConnected);

  action = new QAction(tr("&Configure Internet Proxy"),this);
  m_preferencesMenu->addAction(action);
  isConnected = connect(action, SIGNAL(triggered()),this,SIGNAL(configureProxyClicked()));
  OS_ASSERT(isConnected);

  if(m_isIP){
    m_displayIPUnitsAction->trigger();
  }
  else{
    m_displaySIUnitsAction->trigger();
  }

  // Measure menu
  m_measureMenu = new QMenu(tr("Components && Measures"),this);
  addMenu(m_measureMenu);

  action = new QAction(tr("Apply Measure Now"),this);
  action->setShortcut(QKeySequence(QKeySequence(tr("Ctrl+M"))));
  m_measureMenu->addAction(action);
  isConnected = connect(action, SIGNAL(triggered()),this,SIGNAL(applyMeasureClicked()));
  OS_ASSERT(isConnected);

  action = new QAction(tr("Find Measures"),this);
  m_measureMenu->addAction(action);
  isConnected = connect(action, SIGNAL(triggered()),this,SIGNAL(downloadMeasuresClicked()));
  OS_ASSERT(isConnected);

  action = new QAction(tr("Find Components"),this);
  m_measureMenu->addAction(action); 
  isConnected = connect(action, SIGNAL(triggered()),this,SIGNAL(downloadComponentsClicked()));
  OS_ASSERT(isConnected);

  // Help menu
  m_helpMenu = new QMenu(tr("&Help"),this);
  addMenu(m_helpMenu);

  action = new QAction(tr("OpenStudio &Help"),this);
  m_helpMenu->addAction(action);
  isConnected = connect(action, SIGNAL(triggered()),this,SIGNAL(helpClicked()));
  OS_ASSERT(isConnected);

  action = new QAction(tr("&About"),this);
  m_helpMenu->addAction(action);
  isConnected = connect(action, SIGNAL(triggered()),this,SIGNAL(aboutClicked()));
  OS_ASSERT(isConnected);

}

MainMenu::~MainMenu()
{
}

void MainMenu::displaySIUnitsClicked()
{
  m_displaySIUnitsAction->setIcon(QIcon(":/images/check.png"));
  m_displayIPUnitsAction->setIcon(QIcon());
  emit toggleUnitsClicked(false);
}

void MainMenu::displayIPUnitsClicked()
{
  m_displayIPUnitsAction->setIcon(QIcon(":/images/check.png"));
  m_displaySIUnitsAction->setIcon(QIcon());
  emit toggleUnitsClicked(true);
}

void MainMenu::enableRevertToSavedAction(bool enable)
{
  m_revertToSavedAction->setEnabled(enable);
}

} // openstudio

