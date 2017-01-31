/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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
  auto hiddenWidget = new QWidget();
  int index = this->stackedWidget()->addWidget(hiddenWidget);
  OS_ASSERT(index == 0);

  // index of the default is 1
  auto defaultInspectorView = new DefaultInspectorView(model, parent);
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
