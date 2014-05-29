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

#include <QVBoxLayout>

#include "ScriptsTabView.hpp"
#include "../analysisdriver/SimpleProject.hpp"

#include "OSAppBase.hpp"
#include "../shared_gui_components/OSListView.hpp"
#include "../shared_gui_components/MeasureManager.hpp"

namespace openstudio {

ScriptsTabView::ScriptsTabView(QWidget * parent)
  : MainTabView("Measures",false,parent)
{
  //setTitle("Organize and Edit Measures for Project");

  // Main Content

  mainContent = new QWidget();

  QVBoxLayout * mainContentVLayout = new QVBoxLayout();
  mainContentVLayout->setContentsMargins(0,0,0,0);
  mainContentVLayout->setSpacing(0);
  mainContentVLayout->setAlignment(Qt::AlignTop);
  mainContent->setLayout(mainContentVLayout);

  addTabWidget(mainContent);

  variableGroupListView = new OSListView(true);
  variableGroupListView->setContentsMargins(0,0,0,0);
  variableGroupListView->setSpacing(0);
  mainContentVLayout->addWidget(variableGroupListView);
}

void ScriptsTabView::showEvent(QShowEvent *e)
{
  MainTabView::showEvent(e);

  boost::optional<openstudio::analysisdriver::SimpleProject> project = OSAppBase::instance()->project();
  if (project)
  {
    OSAppBase::instance()->measureManager().updateMeasures(*project, project->measures(), false);
  }
  variableGroupListView->refreshAllViews();
}


} // openstudio

