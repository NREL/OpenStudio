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

    // Toggle Units
    connect(this, &GridViewSubTab::toggleUnitsClicked, this, &GridViewSubTab::toggleUnits);

    //connect(thermalZoneView, &ThermalZoneView::modelObjectSelected, this, &ThermalZonesController::modelObjectSelected);

    //connect(thermalZoneView, &ThermalZoneView::dropZoneItemSelected, this, &ThermalZonesController::dropZoneItemSelected);

  }

  void GridViewSubTab::setGridView(OSGridView * gridView)
  {
    m_scrollLayout->addWidget(gridView, 0, Qt::AlignTop);

    connect(gridView, &OSGridView::dropZoneItemClicked, this, &GridViewSubTab::dropZoneItemClicked);

    connect(this, &GridViewSubTab::itemSelected, gridView, &OSGridView::itemSelected);

    connect(this, &GridViewSubTab::selectionCleared, gridView, &OSGridView::onSelectionCleared);

    connect(gridView, &OSGridView::gridRowSelected, this, &GridViewSubTab::gridRowSelected);
  }

  void GridViewSubTab::setGridController(OSGridController * gridController)
  {
    connect(this, &GridViewSubTab::toggleUnitsClicked, gridController, &OSGridController::toggleUnitsClicked);
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
  }
  
  void GridViewSubTab::onPurgeClicked()
  {
    purgeObjects(m_iddObjectType);
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

  //void GridViewSubTab::onSelectItem(OSItem* item)
  //{
  //  //selectItem(item);
  //  //m_itemSelectorButtons->enableAddButton();
  //  m_itemSelectorButtons->enableCopyButton();
  //  m_itemSelectorButtons->enableRemoveButton();
  //  m_itemSelectorButtons->enablePurgeButton();
  //}

  //void GridViewSubTab::onClearSelection()
  //{
  //  //clearSelection()  
  //  //m_itemSelectorButtons->disableAddButton();
  //  m_itemSelectorButtons->disableCopyButton();
  //  m_itemSelectorButtons->disableRemoveButton();
  //  m_itemSelectorButtons->disablePurgeButton();
  //}

  void GridViewSubTab::toggleUnits(bool isIP)
  {
    m_isIP = isIP;
  }

} // openstudio
