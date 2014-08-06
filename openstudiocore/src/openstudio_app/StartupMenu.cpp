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

#include "StartupMenu.hpp"
#include "OpenStudioApp.hpp"
#include <QMenu>

namespace openstudio {

StartupMenu::StartupMenu(QWidget *parent) : 
  QMenuBar(parent)
{

  // File menu
  m_fileMenu = new QMenu(tr("&File"),this);
  QAction * newAction = new QAction(tr("&New"), this);
  QAction * loadFileAction = new QAction(tr("&Open"), this);
  QAction * importAction = new QAction(tr("&Import IDF"), this);
  //QAction * importSDDAction = new QAction(tr("&Import SDD"), this);
  QAction * exitAction = new QAction(tr("E&xit"), this);
  exitAction->setShortcuts(QKeySequence::Quit);

  connect(importAction, &QAction::triggered, this, &StartupMenu::importClicked);
  //connect(importSDDAction, &QAction::triggered, this, &StartupMenu::importSDDClicked);
  connect(loadFileAction, &QAction::triggered, this, &StartupMenu::loadFileClicked);
  connect(newAction, &QAction::triggered, this, &StartupMenu::newClicked);
  connect(exitAction, &QAction::triggered, this, &StartupMenu::exitClicked);
  m_fileMenu->addAction(newAction);
  m_fileMenu->addAction(loadFileAction);
  m_fileMenu->addSeparator();
  m_fileMenu->addAction(importAction);
  //m_fileMenu->addAction(importSDDAction);
  m_fileMenu->addSeparator();
  m_fileMenu->addAction(exitAction);

  addMenu(m_fileMenu);

  // Help menu
  m_helpMenu = new QMenu(tr("&Help"),this);

  addMenu(m_helpMenu);

  QAction * helpAction = new QAction(tr("OpenStudio &Help"),this);

  connect(helpAction, &QAction::triggered, this, &StartupMenu::helpClicked);
  m_helpMenu->addAction(helpAction);
}

} // openstudio

