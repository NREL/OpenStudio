/**********************************************************************
*  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#include "ScheduleSetsView.hpp"
#include "ModelObjectListView.hpp"
#include "OSItem.hpp"
#include "ScheduleSetInspectorView.hpp"

#include "../model/Model_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QStyleOption>
#include <QPainter>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStackedWidget>
#include <QScrollArea>
#include <sstream>

#include <utilities/idd/IddEnums.hxx>
namespace openstudio {


ScheduleSetsView::ScheduleSetsView(const openstudio::model::Model& model,
                                   QWidget * parent)
                                   : ModelSubTabView(new ModelObjectListView(IddObjectType::OS_DefaultScheduleSet, model, true, parent),
                                   new ScheduleSetInspectorView(model, parent),
                                   false,
                                   parent)
{
}

std::vector<std::pair<IddObjectType, std::string> > ScheduleSetsView::modelObjectTypesAndNames()
{
  std::vector<std::pair<IddObjectType, std::string> > result;
  result.push_back(std::make_pair<IddObjectType, std::string>(IddObjectType::OS_DefaultScheduleSet, "ScheduleSets"));
  return result;
}

ScheduleSetsInspectorView::ScheduleSetsInspectorView(const model::Model& model,
                                                       QWidget * parent )
  : ModelObjectInspectorView(model, false, parent)
{
  // index of hidden widget is 0
  QWidget* hiddenWidget = new QWidget();
  int index = this->stackedWidget()->addWidget(hiddenWidget);
  OS_ASSERT(index == 0);

  // index of the default is 1
  DefaultInspectorView* defaultInspectorView = new DefaultInspectorView(model, parent);
  index = this->stackedWidget()->addWidget(defaultInspectorView);
  OS_ASSERT(index == 1);

  //ScheduleSetsInspectorView* scheduleSetsInspectorView = new ScheduleSetsInspectorView(model, parent);
  //index = this->stackedWidget()->addWidget(scheduleSetsInspectorView);
  //m_inspectorIndexMap[IddObjectType::OS_ScheduleSets] = index;
}

void ScheduleSetsInspectorView::onClearSelection()
{
  ModelObjectInspectorView::onClearSelection(); // call parent implementation
  QWidget* widget = this->stackedWidget()->currentWidget();
  ModelObjectInspectorView* modelObjectInspectorView = qobject_cast<ModelObjectInspectorView*>(widget);
  OS_ASSERT(modelObjectInspectorView);
  modelObjectInspectorView->clearSelection();

  this->stackedWidget()->setCurrentIndex(0);
}

void ScheduleSetsInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  int index = 1;
  std::map<openstudio::IddObjectType, int>::const_iterator it = m_inspectorIndexMap.find(modelObject.iddObjectType());
  if (it != m_inspectorIndexMap.end()){
    index = it->second;
  }

  QWidget* widget = this->stackedWidget()->widget(index);
  ModelObjectInspectorView* modelObjectInspectorView = qobject_cast<ModelObjectInspectorView*>(widget);
  OS_ASSERT(modelObjectInspectorView);
  modelObjectInspectorView->selectModelObject(modelObject);
  this->stackedWidget()->setCurrentIndex(index);
}

void ScheduleSetsInspectorView::onUpdate()
{
}

} // openstudio
