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

#include <openstudio_lib/MainMenu.hpp>
#include <QMenu>
#include <utilities/core/Assert.hpp>


namespace openstudio {

MainMenu::MainMenu(bool isIP, bool isPlugin, QWidget *parent) : 
  QMenuBar(parent), m_isPlugin(isPlugin)
{
  m_isIP = isIP;

  // File menu
  m_fileMenu = new QMenu(tr("&File"),this);
  QAction * newAction = new QAction(tr("&New"), this);
  QAction * loadFileAction = new QAction(tr("&Open"), this);
  QAction * loadLibraryAction = new QAction(tr("&Load Library"), this);
  QAction * exportAction = new QAction(tr("&Export IDF"), this);
  QAction * importAction = new QAction(tr("&Import IDF"), this);
  //QAction * importSDDAction = new QAction(tr("&Import SDD"), this);
  QAction * saveAsFileAction = new QAction(tr("Save &As"), this);
  QAction * saveFileAction = new QAction(tr("&Save"), this);
  QAction * scanForTools = new QAction(tr("Scan for &Tools"), this);
  QAction * showRunManagerPreferences = new QAction(tr("Show Tools"), this);

  //QAction * showRubyConsoleAction;
  QAction * exitAction = 0;
  if (!m_isPlugin){
    //showRubyConsoleAction = new QAction(tr("Show &Ruby Console"), this);
    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcuts(QKeySequence::Quit);
  }

  newAction->setShortcut(QKeySequence(QKeySequence::New));
  loadFileAction->setShortcut(QKeySequence(QKeySequence::Open));
  saveFileAction->setShortcut(QKeySequence(QKeySequence::Save));

  // DLM: actions which result in this menu being deleted should be queued
  connect(exportAction, SIGNAL(triggered()), this, SIGNAL(exportClicked()));
  connect(importAction, SIGNAL(triggered()), this, SIGNAL(importClicked()));
  //connect(importSDDAction, SIGNAL(triggered()), this, SIGNAL(importSDDClicked()));
  connect(loadFileAction, SIGNAL(triggered()), this, SIGNAL(loadFileClicked()), Qt::QueuedConnection);
  connect(loadLibraryAction, SIGNAL(triggered()), this, SIGNAL(loadLibraryClicked()));
  connect(saveAsFileAction, SIGNAL(triggered()), this, SIGNAL(saveAsFileClicked()));
  connect(saveFileAction, SIGNAL(triggered()), this, SIGNAL(saveFileClicked()));
  connect(scanForTools, SIGNAL(triggered()), this, SIGNAL(scanForToolsClicked()));
  connect(showRunManagerPreferences, SIGNAL(triggered()), this, SIGNAL(showRunManagerPreferencesClicked()));
  connect(newAction, SIGNAL(triggered()), this, SIGNAL(newClicked()), Qt::QueuedConnection);
  if (!m_isPlugin){
    //connect(showRubyConsoleAction, SIGNAL(triggered()), this, SIGNAL(showRubyConsoleClicked()));
    connect(exitAction, SIGNAL(triggered()),this,SIGNAL(exitClicked()), Qt::QueuedConnection);
  }

  m_fileMenu->addAction(newAction);
  m_fileMenu->addAction(loadFileAction);
  m_fileMenu->addAction(loadLibraryAction);
  m_fileMenu->addSeparator();
  m_fileMenu->addAction(saveFileAction);
  m_fileMenu->addAction(saveAsFileAction);
  m_fileMenu->addSeparator();
  m_fileMenu->addAction(exportAction);
  m_fileMenu->addAction(importAction);
  //m_fileMenu->addAction(importSDDAction);
  if (!m_isPlugin){
    m_fileMenu->addSeparator();
    //m_fileMenu->addAction(showRubyConsoleAction);
    m_fileMenu->addAction(scanForTools);
    m_fileMenu->addAction(showRunManagerPreferences);
    m_fileMenu->addSeparator();
    m_fileMenu->addAction(exitAction);
  }

  addMenu(m_fileMenu);

  // Preferences menu
  m_preferencesMenu = new QMenu(tr("&Preferences"),this);
  addMenu(m_preferencesMenu);

  m_displaySIUnitsAction = new QAction(tr("Metric (&SI)"),this);
  connect(m_displaySIUnitsAction, SIGNAL(triggered()),this,SLOT(displaySIUnitsClicked()));
  m_preferencesMenu->addAction(m_displaySIUnitsAction);

  m_displayIPUnitsAction = new QAction(tr("English (&I-P)"),this);
  connect(m_displayIPUnitsAction, SIGNAL(triggered()),this,SLOT(displayIPUnitsClicked()));
  m_preferencesMenu->addAction(m_displayIPUnitsAction);


  m_configureProxy = new QAction(tr("&Configure Internet Proxy"),this);

  bool isConnected = connect(m_configureProxy, SIGNAL(triggered()),this,SIGNAL(configureProxyClicked()));
  OS_ASSERT(isConnected);

  m_preferencesMenu->addAction(m_configureProxy);


  // Online BCL menu
  m_windowMenu = new QMenu(tr("&Online BCL"),this);
  addMenu(m_windowMenu);

  m_openBclDlgAction = new QAction(tr("Find &Components"),this);
  connect(m_openBclDlgAction, SIGNAL(triggered()),this,SIGNAL(openBclDlgClicked()));
  m_windowMenu->addAction(m_openBclDlgAction);

  //m_openLibDlgAction = new QAction(tr("Local Library"),this);
  //connect(m_openLibDlgAction, SIGNAL(triggered()),this,SIGNAL(openLibDlgClicked()));
  //m_windowMenu->addAction(m_openLibDlgAction);

  if(m_isIP){
    m_displayIPUnitsAction->trigger();
  }
  else{
    m_displaySIUnitsAction->trigger();
  }
  
  // Help menu
  m_helpMenu = new QMenu(tr("&Help"),this);

  addMenu(m_helpMenu);

  QAction * helpAction = new QAction(tr("OpenStudio &Help"),this);
  QAction * aboutAction = new QAction(tr("&About"),this);

  connect(helpAction, SIGNAL(triggered()),this,SIGNAL(helpClicked()));
  connect(aboutAction, SIGNAL(triggered()),this,SIGNAL(aboutClicked()));

  m_helpMenu->addAction(helpAction);
  m_helpMenu->addAction(aboutAction);
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

} // openstudio

