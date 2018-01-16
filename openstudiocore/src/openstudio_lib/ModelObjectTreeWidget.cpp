/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "ModelObjectTreeWidget.hpp"
#include "ModelObjectTreeItems.hpp"
#include "OSAppBase.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include <QVBoxLayout>
#include <QTreeWidget>
#include <QPainter>
#include <QTimer>

namespace openstudio {

ModelObjectTreeWidget::ModelObjectTreeWidget(const model::Model& model, QWidget* parent )
  : OSItemSelector(parent), m_model(model)
{
  m_vLayout = new QVBoxLayout();
  m_vLayout->setContentsMargins(0,7,0,0);
  m_vLayout->setSpacing(7);
  setLayout(m_vLayout);

  m_treeWidget = new QTreeWidget(parent);
  m_treeWidget->setStyleSheet("QTreeWidget { border: none; border-top: 1px solid black; }");
  m_treeWidget->setAttribute(Qt::WA_MacShowFocusRect,0);

  m_vLayout->addWidget(m_treeWidget);

  // model.getImpl<model::detail::Model_Impl>().get()->addWorkspaceObjectPtr.connect<ModelObjectTreeWidget, &ModelObjectTreeWidget::objectAdded>(this);
  connect(OSAppBase::instance(), &OSAppBase::workspaceObjectAddedPtr, this, &ModelObjectTreeWidget::objectAdded, Qt::QueuedConnection);

  //model.getImpl<model::detail::Model_Impl>().get()->removeWorkspaceObjectPtr.connect<ModelObjectTreeWidget, &ModelObjectTreeWidget::objectRemoved>(this);
  connect(OSAppBase::instance(), &OSAppBase::workspaceObjectRemovedPtr, this, &ModelObjectTreeWidget::objectRemoved, Qt::QueuedConnection);
}

OSItem* ModelObjectTreeWidget::selectedItem() const
{
  // todo: something
  return nullptr;
}

QTreeWidget* ModelObjectTreeWidget::treeWidget() const
{
  return m_treeWidget;
}

QVBoxLayout* ModelObjectTreeWidget::vLayout() const
{
  return m_vLayout;
}

openstudio::model::Model ModelObjectTreeWidget::model() const
{
  return m_model;
}

void ModelObjectTreeWidget::objectAdded(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> impl, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle)
{
  onObjectAdded(impl->getObject<model::ModelObject>(), iddObjectType, handle);
}

void ModelObjectTreeWidget::objectRemoved(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> impl, const openstudio::IddObjectType& iddObjectType, const openstudio::UUID& handle)
{
  onObjectRemoved(impl->getObject<model::ModelObject>(), iddObjectType, handle);
}

void ModelObjectTreeWidget::refresh()
{
  int N = m_treeWidget->topLevelItemCount();
  for (int i = 0; i < N; ++i){
    QTreeWidgetItem* treeItem = m_treeWidget->topLevelItem(i);
    ModelObjectTreeItem* modelObjectTreeItem = dynamic_cast<ModelObjectTreeItem*>(treeItem);
    if(modelObjectTreeItem){
      if (!modelObjectTreeItem->isDirty()){
        modelObjectTreeItem->makeDirty();
        QTimer::singleShot(0, modelObjectTreeItem, SLOT(refresh()));
      }
    }
  }
}

} // openstudio

