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

  connect(importAction, SIGNAL(triggered()), this, SIGNAL(importClicked()));
  //connect(importSDDAction, SIGNAL(triggered()), this, SIGNAL(importSDDClicked()));
  connect(loadFileAction, SIGNAL(triggered()), this, SIGNAL(loadFileClicked()));
  connect(newAction, SIGNAL(triggered()), this, SIGNAL(newClicked()));
  connect(exitAction, SIGNAL(triggered()),this,SIGNAL(exitClicked()));

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

  connect(helpAction, SIGNAL(triggered()),this,SIGNAL(helpClicked()));

  m_helpMenu->addAction(helpAction);
}

} // openstudio

