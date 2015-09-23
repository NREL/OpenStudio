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

#include "GridViewSubTab.hpp"

#include "OSAppBase.hpp"
#include "OSDocument.hpp"
#include "OSItemSelectorButtons.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"

#include "../utilities/core/Assert.hpp"

#include "../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include <QBoxLayout>
#include <QScrollArea>

namespace openstudio {
  
  GridViewSubTab::GridViewSubTab(bool isIP, const model::Model & model, QWidget * parent)
    : QWidget(parent),
    m_model(model),
    m_isIP(isIP)
  {

    // ***** Main Layout *****
    auto mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);
    setLayout(mainLayout);

    // ***** Scroll *****
    m_scrollLayout = new QVBoxLayout();
    m_scrollLayout->setContentsMargins(0, 0, 0, 0);
    m_scrollLayout->setSpacing(0);

    auto scrollWidget = new QWidget();
    scrollWidget->setObjectName("ScrollWidget");
    scrollWidget->setStyleSheet("QWidget#ScrollWidget { background: transparent; }");
    scrollWidget->setLayout(m_scrollLayout);

    auto scrollArea = new QScrollArea();
    scrollArea->setContentsMargins(0, 0, 0, 0);
    scrollArea->setFrameStyle(QFrame::NoFrame);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOn);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setWidget(scrollWidget);
    scrollArea->setWidgetResizable(true);
    scrollArea->setBackgroundRole(QPalette::NoRole);
    mainLayout->addWidget(scrollArea);

    // Item Selector Buttons
    m_itemSelectorButtons = new OSItemSelectorButtons();
    m_itemSelectorButtons->hideDropZone();
    mainLayout->addWidget(m_itemSelectorButtons, 0, Qt::AlignBottom);

    connect(m_itemSelectorButtons, &OSItemSelectorButtons::addClicked, this, &GridViewSubTab::onAddClicked);
    connect(m_itemSelectorButtons, &OSItemSelectorButtons::copyClicked, this, &GridViewSubTab::onCopyClicked);
    connect(m_itemSelectorButtons, &OSItemSelectorButtons::removeClicked, this, &GridViewSubTab::onRemoveClicked);
    connect(m_itemSelectorButtons, &OSItemSelectorButtons::purgeClicked, this, &GridViewSubTab::onPurgeClicked);

    connect(this, &GridViewSubTab::toggleUnitsClicked, this, &GridViewSubTab::toggleUnits);
  }

  void GridViewSubTab::setGridView(OSGridView * gridView)
  {
    m_scrollLayout->addWidget(gridView, 0, Qt::AlignTop);

    connect(gridView, &OSGridView::dropZoneItemClicked, this, &GridViewSubTab::onDropZoneItemClicked);

    connect(this, &GridViewSubTab::selectionCleared, gridView, &OSGridView::onSelectionCleared);

    connect(gridView, &OSGridView::gridRowSelectionChanged, this, &GridViewSubTab::gridRowSelectionChanged);

    connect(this, &GridViewSubTab::toggleUnitsClicked, m_gridController, &OSGridController::toggleUnitsClicked);

    connect(this, &GridViewSubTab::toggleUnitsClicked, m_gridController, &OSGridController::toggleUnits);
  }

  void GridViewSubTab::setGridController(OSGridController * gridController)
  {
    connect(this, &GridViewSubTab::toggleUnitsClicked, gridController, &OSGridController::toggleUnits);
  }

  void GridViewSubTab::onAddClicked()
  {
    if (m_supportsMultipleObjectSelection || m_gridController->selectedObjects().size() == 1)
    {
      // Always make at least one
      if (!m_gridController->selectedObjects().size()) {
        addObject(m_gridController->m_iddObjectType);
      }
      else {
        for (auto &obj : m_gridController->selectedObjects())
        {
          addObject(obj);
        }
      }
    }
  }

  void GridViewSubTab::onCopyClicked()
  {
    if (m_supportsMultipleObjectSelection || m_gridController->selectedObjects().size() == 1)
    {
      for (auto &obj : m_gridController->selectedObjects())
      {
        if (!obj.handle().isNull()){
          copyObject(obj);
        }
      }
    }
  }

  void GridViewSubTab::onRemoveClicked()
  {
    if (m_supportsMultipleObjectSelection || m_gridController->selectedObjects().size() == 1)
    {
      for (auto &obj : m_gridController->selectedObjects())
      {
        removeObject(obj);
      }
    }
    // No selected rows will remain after a delete, so clear selection
    onClearSelection();
  }

  void GridViewSubTab::onPurgeClicked()
  {
    purgeObjects(m_iddObjectType);

    // No selected rows will remain after a delete, so clear selection
    onClearSelection();
  }

  void GridViewSubTab::addObject(const model::ModelObject& modelObject)
  {
    addObject(modelObject.iddObjectType());
  }

  void GridViewSubTab::copyObject(const openstudio::model::ModelObject& modelObject)
  {
    modelObject.clone(this->m_model);
  }

  void GridViewSubTab::removeObject(openstudio::model::ModelObject modelObject)
  {
    modelObject.remove();
  }

  std::vector<model::ModelObject> GridViewSubTab::selectedObjects() const
  {
    return m_gridController->selectedObjects();
  }

  void GridViewSubTab::onDropZoneItemClicked(OSItem* item)
  {
    std::shared_ptr<OSDocument> currentDocument = OSAppBase::instance()->currentDocument();
    if (currentDocument){
      if (!item) {
        emit dropZoneItemSelected(item, false);
      }
      else {
        // Note: perhaps passing this here offers more flexibility in the future when determining readOnly
        emit dropZoneItemSelected(item, item->itemId().isDefaulted());
      }
    }
  }

  void GridViewSubTab::toggleUnits(bool isIP)
  {
    m_isIP = isIP;
  }

  void GridViewSubTab::gridRowSelectionChanged(int checkState)
  {
    if (this->m_gridController->getObjectSelector()->m_selectedObjects.size() == 0) {
      onClearSelection();
    }
    else {
      onSelectItem();
    }
  }

} // openstudio
