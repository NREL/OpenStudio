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

#include "ModelObjectTreeWidget.hpp"
#include "ModelObjectTreeItems.hpp"

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

  bool isConnected = false;
  isConnected = connect(model.getImpl<model::detail::Model_Impl>().get(), 
                        SIGNAL(addWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, const openstudio::UUID&)),
                        this,
                        SLOT(objectAdded(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, const openstudio::UUID&)),
                        Qt::QueuedConnection);
  OS_ASSERT(isConnected);

  isConnected = connect(model.getImpl<model::detail::Model_Impl>().get(), 
                        SIGNAL(removeWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, const openstudio::UUID&)),
                        this,
                        SLOT(objectRemoved(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, const openstudio::UUID&)),
                        Qt::QueuedConnection);
  OS_ASSERT(isConnected);
}

OSItem* ModelObjectTreeWidget::selectedItem() const
{
  // todo: something
  return NULL;
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

