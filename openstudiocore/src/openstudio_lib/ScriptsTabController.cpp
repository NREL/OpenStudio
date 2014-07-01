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

#include "ScriptsTabController.hpp"

#include "OSAppBase.hpp"
#include "ScriptFolderListView.hpp"
#include "ScriptsTabView.hpp"

#include "../shared_gui_components/VariableList.hpp"
#include "../shared_gui_components/VariableView.hpp"

#include "../model/Model.hpp"

namespace openstudio {

ScriptsTabController::ScriptsTabController()
  : MainTabController(new ScriptsTabView(0)), scriptsTabView(dynamic_cast<ScriptsTabView *>(mainContentWidget()))
{

  m_variableGroupListController = QSharedPointer<openstudio::measuretab::VariableGroupListController>(new openstudio::measuretab::VariableGroupListController(false, OSAppBase::instance()));
  m_variableGroupItemDelegate = QSharedPointer<openstudio::measuretab::VariableGroupItemDelegate>(new openstudio::measuretab::VariableGroupItemDelegate(true));

  //m_groups[MeasureType::ModelMeasure] = QString("OpenStudio Measures");
  ////m_groups[MeasureType::EnergyPlusMeasure] = QString("EnergyPlus Measures");

  //for( std::map<MeasureType,QString>::const_iterator it = m_groups.begin();
  //     it != m_groups.end();
  //     it++ )
  //{
  //  QSharedPointer<VariableGroupItem> variableGroupItem;
  //  variableGroupItem = QSharedPointer<VariableGroupItem>(new VariableGroupItem(it->first,it->second));
  //  m_variableGroupListController->addItem(variableGroupItem);
  //}

  scriptsTabView->variableGroupListView->setListController(m_variableGroupListController);
  scriptsTabView->variableGroupListView->setDelegate(m_variableGroupItemDelegate);

}

ScriptsTabController::~ScriptsTabController()
{
  if( scriptsTabView ) { delete scriptsTabView; }
}

} // openstudio
