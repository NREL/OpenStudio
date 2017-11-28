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

#include "MainRightColumnController.hpp"

#include "ConstructionsTabController.hpp"
#include "GeometryTabController.hpp"
#include "HorizontalTabWidget.hpp"
#include "InspectorController.hpp"
#include "InspectorView.hpp"
#include "LibraryTabWidget.hpp"
#include "LocationTabController.hpp"
#include "ModelObjectTypeListView.hpp"
#include "OSAppBase.hpp"
#include "OSCollapsibleItem.hpp"
#include "OSCollapsibleItemHeader.hpp"
#include "OSDocument.hpp"
#include "OSItem.hpp"
#include "OSItemList.hpp"
#include "SchedulesTabController.hpp"
#include "SpaceTypeInspectorView.hpp"
#include "ThermalZonesView.hpp"

#include "../shared_gui_components/EditController.hpp"
#include "../shared_gui_components/EditView.hpp"
#include "../shared_gui_components/LocalLibraryController.hpp"
#include "../shared_gui_components/LocalLibraryView.hpp"
#include "../shared_gui_components/MeasureManager.hpp"
#include "../shared_gui_components/OSViewSwitcher.hpp"

#include "../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <QStackedWidget>
#include <QLayout>
#include <QTimer>

namespace openstudio {

MainRightColumnController::MainRightColumnController(const model::Model & model,
    const openstudio::path &resourcesPath)
  : OSQObjectController(),
    m_model(model),
    m_resourcesPath(resourcesPath),
    m_measureLibraryController(new LocalLibraryController(OSAppBase::instance())),
    m_measureEditController(new EditController()),
    m_myModelTabIsHidden(false)
{
  m_measureLibraryController->localLibraryView->setStyleSheet("QStackedWidget { border-top: 0px; }");
  OSAppBase::instance()->measureManager().setLibraryController(m_measureLibraryController);
  m_horizontalTabWidget = new HorizontalTabWidget();
  addQObject(m_horizontalTabWidget);

  // My Model

  m_myModelView = new QStackedWidget();
  m_myModelView->setStyleSheet("QStackedWidget { border-top: 1px solid black; }");
  m_horizontalTabWidget->addTab(m_myModelView,MY_MODEL,"My Model");

  // Library

  m_libraryView = new QStackedWidget();
  m_libraryView->setStyleSheet("QStackedWidget { border-top: 1px solid black; }");
  m_horizontalTabWidget->addTab(m_libraryView,LIBRARY,"Library");


  // Editor
  m_editView = new QStackedWidget();
  m_editView->setStyleSheet("QStackedWidget { border-top: 1px solid black; }");
  m_horizontalTabWidget->addTab(m_editView,EDIT,"Edit");

  // Inspector, we're keeping it around to be able to follow the units toggled
  m_inspectorController = std::shared_ptr<InspectorController>( new InspectorController() );
  connect(this, &MainRightColumnController::toggleUnitsClicked, m_inspectorController.get(), &InspectorController::toggleUnitsClicked);
  connect(m_inspectorController.get(), &InspectorController::removeButtonClicked, this, &MainRightColumnController::onRemoveButtonClicked);
  connect(m_inspectorController.get(), &InspectorController::workspaceObjectRemoved, this, &MainRightColumnController::onWorkspaceObjectRemoved);

  auto isConnected = connect(m_inspectorController.get(), SIGNAL(itemRemoveClicked(OSItem *)), this, SLOT(onItemRemoveClicked(OSItem *)));
  OS_ASSERT(isConnected);
}

void MainRightColumnController::onItemRemoveClicked(OSItem *)
{
  setEditView(nullptr);
}

void MainRightColumnController::emitItemRemoveClicked(OSItem * item)
{
  emit itemRemoveClicked(item);
}

void MainRightColumnController::registerSystemItem(const Handle & systemHandle, SystemItem * systemItem)
{
  m_systemItemMap[systemHandle] = systemItem;
}

void MainRightColumnController::unregisterSystemItem(const Handle & systemHandle)
{
  auto it = m_systemItemMap.find(systemHandle);
  if( it != m_systemItemMap.end() )
  {
    m_systemItemMap.erase(it);
  }
}

SystemItem * MainRightColumnController::systemItem(const Handle & systemHandle) const
{
  auto it = m_systemItemMap.find(systemHandle);
  if( it != m_systemItemMap.end() )
  {
    return it->second;
  }

  return nullptr;
}

void MainRightColumnController::inspectModelObject(model::OptionalModelObject & modelObject, bool readOnly)
{
  if( modelObject )
  {
    m_horizontalTabWidget->setCurrentId(EDIT);
    setEditView(m_inspectorController->inspectorView());
    m_inspectorController->layoutModelObject(modelObject, readOnly);
  }
  else
  {
    m_inspectorController->layoutModelObject(modelObject, readOnly);
  }
  m_inspectorController->inspectorView()->currentView()->m_libraryTabWidget->hideRemoveButton();
}

void MainRightColumnController::inspectModelObjectByItem(OSItem * item, bool readOnly)
{
  m_item = item;
  if (m_item)
  {
    boost::optional<model::ModelObject> modelObject;
    std::shared_ptr<OSDocument> currentDocument = OSAppBase::instance()->currentDocument();
    if (currentDocument){
      modelObject = currentDocument->getModelObject(item->itemId());
    }

    if (modelObject)
    {
      m_horizontalTabWidget->setCurrentId(EDIT);
      setEditView(m_inspectorController->inspectorView());
      m_inspectorController->layoutModelObject(modelObject, readOnly);
    }
    else
    {
      m_inspectorController->layoutModelObject(modelObject, readOnly);
    }

    if (m_item->itemId().isDefaulted()) {
      m_inspectorController->inspectorView()->currentView()->m_libraryTabWidget->hideRemoveButton();
    }
    else {
      m_inspectorController->inspectorView()->currentView()->m_libraryTabWidget->showRemoveButton();
    }
  }
  else {
    setEditView(nullptr);
  }
}

void MainRightColumnController::onRemoveButtonClicked(bool checked)
{
  if (m_item) {
    m_item->onRemoveClicked();
    setEditView(nullptr);
  }
}

void MainRightColumnController::onWorkspaceObjectRemoved()
{
  setEditView(nullptr);
}

HorizontalTabWidget * MainRightColumnController::mainRightColumnView() const
{
  return m_horizontalTabWidget;
}

QSharedPointer<LocalLibraryController> MainRightColumnController::measureLibraryController()
{
  return m_measureLibraryController;
}

void MainRightColumnController::setEditView(QWidget *widget)
{
  if( QWidget * oldwidget = m_editView->currentWidget() )
  {
    LOG(Debug, "Removing old edit widget: " << oldwidget);
    m_editView->removeWidget(oldwidget);

    if (oldwidget != m_inspectorController->inspectorView()
        && oldwidget != m_measureEditController->editView.data())
    {
      LOG(Debug, "Deleting old edit widget: " << oldwidget);
      delete oldwidget;
    } else {
      boost::optional<model::ModelObject> nomodelobject;
      m_inspectorController->layoutModelObject(nomodelobject, false);
    }
  }

  if( widget )
  {
    LOG(Debug, "Setting new edit widget: " << widget);
    m_editView->addWidget(widget);
  }
}

void MainRightColumnController::setMyModelView(QWidget * widget)
{
  if( QWidget * oldwidget = m_myModelView->currentWidget() )
  {
    m_myModelView->removeWidget(oldwidget);

    delete oldwidget;
  }

  if( widget )
  {
    m_myModelView->addWidget(widget);
  }
}

void MainRightColumnController::setLibraryView(QWidget * widget)
{
  if( QWidget * oldwidget = m_libraryView->currentWidget() )
  {
    m_libraryView->removeWidget(oldwidget);

    if (oldwidget != m_measureLibraryController->localLibraryView)
    {
      delete oldwidget;
    }
  }

  if( widget )
  {
    m_libraryView->addWidget(widget);
  }
}

void MainRightColumnController::configureForSiteSubTab(int subTabID)
{
  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

  setLibraryView(nullptr);
  setMyModelView(nullptr);
  setEditView(nullptr);

  if (subTabID == 0) {
    doc->closeSidebar();
  }
  else {
    doc->openSidebar();
  }
}

void MainRightColumnController::configureForSchedulesSubTab(int subTabID)
{
  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

  setLibraryView(nullptr);
  setMyModelView(nullptr);
  setEditView(nullptr);

  switch( subTabID )
  {
    //case SchedulesTabController::YEAR_SETTINGS:
    //{
    //}
    case SchedulesTabController::SCHEDULE_SETS:
    {
      model::Model lib = doc->componentLibrary();

      // my model
      auto myModelList = new ModelObjectTypeListView(m_model, true, OSItemType::CollapsibleListHeader);
      myModelList->setItemsType(OSItemType::LibraryItem);
      myModelList->setItemsDraggable(true);
      myModelList->setItemsRemoveable(false);
      myModelList->setShowFilterLayout(true);

      myModelList->addModelObjectType(IddObjectType::OS_Schedule_VariableInterval, "Variable Interval Schedules");
      myModelList->addModelObjectType(IddObjectType::OS_Schedule_FixedInterval, "Fixed Interval Schedules");
      myModelList->addModelObjectType(IddObjectType::OS_Schedule_Year, "Year Schedules");
      myModelList->addModelObjectType(IddObjectType::OS_Schedule_Constant, "Constant Schedules");
      myModelList->addModelObjectType(IddObjectType::OS_Schedule_Compact, "Compact Schedules");
      myModelList->addModelObjectType(IddObjectType::OS_Schedule_Ruleset, "Ruleset Schedules");

      setMyModelView(myModelList);

      // my library
      auto myLibraryList = new ModelObjectTypeListView(lib,true,OSItemType::CollapsibleListHeader);
      myLibraryList->setItemsDraggable(true);
      myLibraryList->setItemsRemoveable(false);
      myLibraryList->setItemsType(OSItemType::LibraryItem);

      myLibraryList->addModelObjectType(IddObjectType::OS_Schedule_VariableInterval, "Variable Interval Schedules");
      myLibraryList->addModelObjectType(IddObjectType::OS_Schedule_FixedInterval, "Fixed Interval Schedules");
      myLibraryList->addModelObjectType(IddObjectType::OS_Schedule_Year, "Year Schedules");
      myLibraryList->addModelObjectType(IddObjectType::OS_Schedule_Constant, "Constant Schedules");
      myLibraryList->addModelObjectType(IddObjectType::OS_Schedule_Compact, "Compact Schedules");
      myLibraryList->addModelObjectType(IddObjectType::OS_Schedule_Ruleset, "Ruleset Schedules");
      myLibraryList->addModelObjectType(IddObjectType::OS_DefaultScheduleSet, "Schedule Sets");

      setLibraryView(myLibraryList);
      doc->openSidebar();
      //doc->closeSidebar();

      break;
    }
    case SchedulesTabController::SCHEDULES:
    {
      model::Model lib = doc->componentLibrary();

      // my library
      auto myLibraryList = new ModelObjectTypeListView(lib,true,OSItemType::CollapsibleListHeader);
      myLibraryList->setItemsDraggable(true);
      myLibraryList->setItemsRemoveable(false);
      myLibraryList->setItemsType(OSItemType::LibraryItem);
      myLibraryList->setShowFilterLayout(true);

      myLibraryList->addModelObjectType(IddObjectType::OS_Schedule_Ruleset, "Schedule Rulesets");

      setLibraryView(myLibraryList);
      doc->openSidebar();
      //doc->closeSidebar();

      break;
    }
    default:
      break;
  }
}

void MainRightColumnController::configureForConstructionsSubTab(int subTabID)
{
  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

  setLibraryView(nullptr);
  setMyModelView(nullptr);
  setEditView(nullptr);

  switch( subTabID )
  {
    case ConstructionsTabController::DEFAULT_CONSTRUCTIONS:
    {
      model::Model lib = doc->componentLibrary();

      // my model
      auto myModelList = new ModelObjectTypeListView(m_model, true, OSItemType::CollapsibleListHeader);
      myModelList->setItemsDraggable(true);
      myModelList->setItemsRemoveable(false);
      myModelList->setItemsType(OSItemType::LibraryItem);
      myModelList->setShowFilterLayout(true);

      myModelList->addModelObjectType(IddObjectType::OS_Construction_WindowDataFile, "Window Data File Constructions");
      myModelList->addModelObjectType(IddObjectType::OS_Construction_FfactorGroundFloor, "F-factor Ground Floor Constructions");
      myModelList->addModelObjectType(IddObjectType::OS_Construction_CfactorUndergroundWall, "C-factor Underground Wall Constructions");
      myModelList->addModelObjectType(IddObjectType::OS_Construction_InternalSource, "Internal Source Constructions");
      myModelList->addModelObjectType(IddObjectType::OS_Construction, "Constructions");

      setMyModelView(myModelList);

      // my library
      auto myLibraryList = new ModelObjectTypeListView(lib,true,OSItemType::CollapsibleListHeader,true);
      myLibraryList->setItemsDraggable(true);
      myLibraryList->setItemsRemoveable(false);
      myLibraryList->setItemsType(OSItemType::LibraryItem);
      myLibraryList->setShowFilterLayout(true);

      myLibraryList->addModelObjectType(IddObjectType::OS_Construction_WindowDataFile, "Window Data File Constructions");
      myLibraryList->addModelObjectType(IddObjectType::OS_Construction_FfactorGroundFloor, "F-factor Ground Floor Constructions");
      myLibraryList->addModelObjectType(IddObjectType::OS_Construction_CfactorUndergroundWall, "C-factor Underground Wall Constructions");
      myLibraryList->addModelObjectType(IddObjectType::OS_Construction_InternalSource, "Internal Source Constructions");
      myLibraryList->addModelObjectType(IddObjectType::OS_Construction, "Constructions");
      myLibraryList->addModelObjectType(IddObjectType::OS_DefaultConstructionSet, "Construction Sets");

      setLibraryView(myLibraryList);

      doc->openSidebar();
      break;
    }
    case ConstructionsTabController::CONSTRUCTIONS:
    {
      model::Model lib = doc->componentLibrary();

      // my model
      auto myModelList = new ModelObjectTypeListView(m_model, true, OSItemType::CollapsibleListHeader);
      myModelList->setItemsDraggable(true);
      myModelList->setItemsRemoveable(false);
      myModelList->setItemsType(OSItemType::LibraryItem);
      myModelList->setShowFilterLayout(true);

      myModelList->addModelObjectType(IddObjectType::OS_WindowMaterial_GlazingGroup_Thermochromic, "Glazing Group Thermochromic Window Materials");
      myModelList->addModelObjectType(IddObjectType::OS_WindowMaterial_Glazing_RefractionExtinctionMethod, "Refraction Extinction Method Glazing Window Materials");
      myModelList->addModelObjectType(IddObjectType::OS_WindowMaterial_Shade, "Shade Window Materials");
      myModelList->addModelObjectType(IddObjectType::OS_WindowMaterial_Screen, "Screen Window Materials");
      myModelList->addModelObjectType(IddObjectType::OS_WindowMaterial_Blind, "Blind Window Materials");
      myModelList->addModelObjectType(IddObjectType::OS_WindowMaterial_DaylightRedirectionDevice, "Daylight Redirection Device Window Materials");
      myModelList->addModelObjectType(IddObjectType::OS_WindowMaterial_GasMixture, "Gas Mixture Window Materials");
      myModelList->addModelObjectType(IddObjectType::OS_WindowMaterial_Gas, "Gas Window Materials");
      myModelList->addModelObjectType(IddObjectType::OS_WindowMaterial_Glazing, "Glazing Window Materials");
      myModelList->addModelObjectType(IddObjectType::OS_WindowMaterial_SimpleGlazingSystem, "Simple Glazing System Window Materials");

      myModelList->addModelObjectType(IddObjectType::OS_Material_RoofVegetation, "Roof Vegetation Materials");
      myModelList->addModelObjectType(IddObjectType::OS_Material_InfraredTransparent, "Infrared Transparent Materials");
      myModelList->addModelObjectType(IddObjectType::OS_Material_AirWall, "Air Wall Materials");
      myModelList->addModelObjectType(IddObjectType::OS_Material_AirGap, "Air Gap Materials");
      myModelList->addModelObjectType(IddObjectType::OS_Material_NoMass, "No Mass Materials");
      myModelList->addModelObjectType(IddObjectType::OS_Material, "Materials");

      setMyModelView(myModelList);

      // my library
      auto myLibraryList = new ModelObjectTypeListView(lib,true,OSItemType::CollapsibleListHeader, true);
      myLibraryList->setItemsDraggable(true);
      myLibraryList->setItemsRemoveable(false);
      myLibraryList->setItemsType(OSItemType::LibraryItem);
      myLibraryList->setShowFilterLayout(true);

      myLibraryList->addModelObjectType(IddObjectType::OS_WindowMaterial_GlazingGroup_Thermochromic, "Glazing Group Thermochromic Window Materials");
      myLibraryList->addModelObjectType(IddObjectType::OS_WindowMaterial_Glazing_RefractionExtinctionMethod, "Refraction Extinction Method Glazing Window Materials");
      myLibraryList->addModelObjectType(IddObjectType::OS_WindowMaterial_Shade, "Shade Window Materials");
      myLibraryList->addModelObjectType(IddObjectType::OS_WindowMaterial_Screen, "Screen Window Materials");
      myLibraryList->addModelObjectType(IddObjectType::OS_WindowMaterial_Blind, "Blind Window Materials");
      myLibraryList->addModelObjectType(IddObjectType::OS_WindowMaterial_DaylightRedirectionDevice, "Daylight Redirection Device Window Materials");
      myLibraryList->addModelObjectType(IddObjectType::OS_WindowMaterial_GasMixture, "Gas Mixture Window Materials");
      myLibraryList->addModelObjectType(IddObjectType::OS_WindowMaterial_Gas, "Gas Window Materials");
      myLibraryList->addModelObjectType(IddObjectType::OS_WindowMaterial_Glazing, "Glazing Window Materials");
      myLibraryList->addModelObjectType(IddObjectType::OS_WindowMaterial_SimpleGlazingSystem, "Simple Glazing System Window Materials");

      myLibraryList->addModelObjectType(IddObjectType::OS_Material_RoofVegetation, "Roof Vegetation Materials");
      myLibraryList->addModelObjectType(IddObjectType::OS_Material_InfraredTransparent, "Infrared Transparent Materials");
      myLibraryList->addModelObjectType(IddObjectType::OS_Material_AirWall, "Air Wall Materials");
      myLibraryList->addModelObjectType(IddObjectType::OS_Material_AirGap, "Air Gap Materials");
      myLibraryList->addModelObjectType(IddObjectType::OS_Material_NoMass, "No Mass Materials");
      myLibraryList->addModelObjectType(IddObjectType::OS_Material, "Materials");

      myLibraryList->addModelObjectType(IddObjectType::OS_Construction_WindowDataFile, "Window Data File Constructions");
      myLibraryList->addModelObjectType(IddObjectType::OS_Construction_FfactorGroundFloor, "F-factor Ground Floor Constructions");
      myLibraryList->addModelObjectType(IddObjectType::OS_Construction_CfactorUndergroundWall, "C-factor Underground Wall Constructions");
      myLibraryList->addModelObjectType(IddObjectType::OS_Construction_InternalSource, "Internal Source Constructions");
      myLibraryList->addModelObjectType(IddObjectType::OS_Construction, "Constructions");

      setLibraryView(myLibraryList);

      doc->openSidebar();
      break;
    }
    case ConstructionsTabController::MATERIALS:
    {
      model::Model lib = doc->componentLibrary();

      // my model
      auto myModelList = new ModelObjectTypeListView(m_model, true, OSItemType::CollapsibleListHeader);
      myModelList->setItemsDraggable(true);
      myModelList->setItemsRemoveable(false);
      myModelList->setItemsType(OSItemType::LibraryItem);
      myModelList->setShowFilterLayout(true);

      setMyModelView(myModelList);

      // my library
      auto myLibraryList = new ModelObjectTypeListView(lib,true,OSItemType::CollapsibleListHeader,true);
      myLibraryList->setItemsDraggable(true);
      myLibraryList->setItemsRemoveable(false);
      myLibraryList->setItemsType(OSItemType::LibraryItem);
      myLibraryList->setShowFilterLayout(true);

      myLibraryList->addModelObjectType(IddObjectType::OS_WindowMaterial_GlazingGroup_Thermochromic, "Glazing Group Thermochromic Window Materials");
      myLibraryList->addModelObjectType(IddObjectType::OS_WindowMaterial_Glazing_RefractionExtinctionMethod, "Refraction Extinction Method Glazing Window Materials");
      myLibraryList->addModelObjectType(IddObjectType::OS_WindowMaterial_Shade, "Shade Window Materials");
      myLibraryList->addModelObjectType(IddObjectType::OS_WindowMaterial_Screen, "Screen Window Materials");
      myLibraryList->addModelObjectType(IddObjectType::OS_WindowMaterial_Blind, "Blind Window Materials");
      myLibraryList->addModelObjectType(IddObjectType::OS_WindowMaterial_DaylightRedirectionDevice, "Daylight Redirection Device Window Materials");
      myLibraryList->addModelObjectType(IddObjectType::OS_WindowMaterial_GasMixture, "Gas Mixture Window Materials");
      myLibraryList->addModelObjectType(IddObjectType::OS_WindowMaterial_Gas, "Gas Window Materials");
      myLibraryList->addModelObjectType(IddObjectType::OS_WindowMaterial_Glazing, "Glazing Window Materials");
      myLibraryList->addModelObjectType(IddObjectType::OS_WindowMaterial_SimpleGlazingSystem, "Simple Glazing System Window Materials");

      myLibraryList->addModelObjectType(IddObjectType::OS_Material_RoofVegetation, "Roof Vegetation Materials");
      myLibraryList->addModelObjectType(IddObjectType::OS_Material_InfraredTransparent, "Infrared Transparent Materials");
      myLibraryList->addModelObjectType(IddObjectType::OS_Material_AirWall, "Air Wall Materials");
      myLibraryList->addModelObjectType(IddObjectType::OS_Material_AirGap, "Air Gap Materials");
      myLibraryList->addModelObjectType(IddObjectType::OS_Material_NoMass, "No Mass Materials");
      myLibraryList->addModelObjectType(IddObjectType::OS_Material, "Materials");

      setLibraryView(myLibraryList);

      doc->openSidebar();
      break;
    }
    default:
      break;
  }
}


void MainRightColumnController::configureForGeometrySubTab(int subTabID)
{
  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

  setLibraryView(nullptr);
  setMyModelView(nullptr);
  setEditView(nullptr);

  switch( subTabID )
  {
    case GeometryTabController::PREVIEW:
    {
      doc->closeSidebar();
      break;
    }
    case GeometryTabController::EDITOR:
    {
      doc->closeSidebar();
      break;
    }
    default:
      break;
  }
}

void MainRightColumnController::configureForLoadsSubTab(int subTabID)
{
  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

  model::Model lib = doc->componentLibrary();


  setEditView(nullptr);

  // my model

  auto myModelList = new ModelObjectTypeListView(m_model, true, OSItemType::CollapsibleListHeader);
  myModelList->setItemsDraggable(true);
  myModelList->setItemsRemoveable(false);
  myModelList->setItemsType(OSItemType::LibraryItem);
  myModelList->setShowFilterLayout(true);

  myModelList->addModelObjectType(IddObjectType::OS_Construction_WindowDataFile, "Window Data File Constructions");
  myModelList->addModelObjectType(IddObjectType::OS_Construction_FfactorGroundFloor, "F-factor Ground Floor Constructions");
  myModelList->addModelObjectType(IddObjectType::OS_Construction_CfactorUndergroundWall, "C-factor Underground Wall Constructions");
  myModelList->addModelObjectType(IddObjectType::OS_Construction_InternalSource, "Internal Source Constructions");
  myModelList->addModelObjectType(IddObjectType::OS_Construction, "Constructions");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_VariableInterval, "Variable Interval Schedules");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_FixedInterval, "Fixed Interval Schedules");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_Year, "Year Schedules");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_Constant, "Constant Schedules");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_Compact, "Compact Schedules");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_Ruleset, "Ruleset Schedules");

  setMyModelView(myModelList);

  // my library

  auto myLibraryList = new ModelObjectTypeListView(lib,true,OSItemType::CollapsibleListHeader, true);
  myLibraryList->setItemsDraggable(true);
  myLibraryList->setItemsRemoveable(false);
  myLibraryList->setItemsType(OSItemType::LibraryItem);
  myLibraryList->setShowFilterLayout(true);

  myLibraryList->addModelObjectType(IddObjectType::OS_Construction_WindowDataFile, "Window Data File Constructions");
  myLibraryList->addModelObjectType(IddObjectType::OS_Construction_FfactorGroundFloor, "F-factor Ground Floor Constructions");
  myLibraryList->addModelObjectType(IddObjectType::OS_Construction_CfactorUndergroundWall, "C-factor Underground Wall Constructions");
  myLibraryList->addModelObjectType(IddObjectType::OS_Construction_InternalSource, "Internal Source Constructions");
  myLibraryList->addModelObjectType(IddObjectType::OS_Construction, "Constructions");
  myLibraryList->addModelObjectType(IddObjectType::OS_InternalMass_Definition, "Internal Mass Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_OtherEquipment_Definition, "Other Equipment Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_SteamEquipment_Definition, "Steam Equipment Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_HotWaterEquipment_Definition, "Hot Water Equipment Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_WaterUse_Equipment_Definition, "Water Use Equipment Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_GasEquipment_Definition, "Gas Equipment Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_ElectricEquipment_Definition, "Electric Equipment Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_Luminaire_Definition, "Luminaire Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_Lights_Definition, "Lights Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_People_Definition, "People Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_Schedule_VariableInterval, "Variable Interval Schedules");
  myLibraryList->addModelObjectType(IddObjectType::OS_Schedule_FixedInterval, "Fixed Interval Schedules");
  myLibraryList->addModelObjectType(IddObjectType::OS_Schedule_Year, "Year Schedules");
  myLibraryList->addModelObjectType(IddObjectType::OS_Schedule_Constant, "Constant Schedules");
  myLibraryList->addModelObjectType(IddObjectType::OS_Schedule_Compact, "Compact Schedules");
  myLibraryList->addModelObjectType(IddObjectType::OS_Schedule_Ruleset, "Ruleset Schedules");

  setLibraryView(myLibraryList);

  doc->openSidebar();
}

void MainRightColumnController::configureForSpaceTypesSubTab(int subTabID)
{
  // no sub tabs
  OS_ASSERT(subTabID == 0);

  setEditView(nullptr);

  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

  // my model
  auto myModelList = new ModelObjectTypeListView(m_model, true, OSItemType::CollapsibleListHeader);
  myModelList->setItemsDraggable(true);
  myModelList->setItemsRemoveable(false);
  myModelList->setItemsType(OSItemType::LibraryItem);
  myModelList->setShowFilterLayout(true);

  myModelList->addModelObjectType(IddObjectType::OS_Schedule_VariableInterval, "Variable Interval Schedules");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_FixedInterval, "Fixed Interval Schedules");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_Year, "Year Schedules");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_Constant, "Constant Schedules");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_Compact, "Compact Schedules");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_Ruleset, "Ruleset Schedules");

  myModelList->addModelObjectType(IddObjectType::OS_InternalMass_Definition, "Internal Mass Definitions");
  myModelList->addModelObjectType(IddObjectType::OS_OtherEquipment_Definition, "Other Equipment Definitions");
  myModelList->addModelObjectType(IddObjectType::OS_SteamEquipment_Definition, "Steam Equipment Definitions");
  myModelList->addModelObjectType(IddObjectType::OS_HotWaterEquipment_Definition, "Hot Water Equipment Definitions");
  myModelList->addModelObjectType(IddObjectType::OS_WaterUse_Equipment_Definition, "Water Use Equipment Definitions");
  myModelList->addModelObjectType(IddObjectType::OS_GasEquipment_Definition, "Gas Equipment Definitions");
  myModelList->addModelObjectType(IddObjectType::OS_ElectricEquipment_Definition, "Electric Equipment Definitions");
  myModelList->addModelObjectType(IddObjectType::OS_Luminaire_Definition, "Luminaire Definitions");
  myModelList->addModelObjectType(IddObjectType::OS_Lights_Definition, "Lights Definitions");
  myModelList->addModelObjectType(IddObjectType::OS_People_Definition, "People Definitions");

  //OSCollapsibleItemHeader* unassignedSpacesCollapsibleHeader = new OSCollapsibleItemHeader("Unassigned Spaces", OSItemId("",""), OSItemType::CollapsibleListHeader);
  //unassignedSpacesCollapsibleHeader->setRemoveable(false);
  //SpaceTypeUnassignedSpacesVectorController* unassignedSpacesVectorController = new SpaceTypeUnassignedSpacesVectorController();
  //unassignedSpacesVectorController->attachModel(m_model);
  //OSItemList* unassignedSpacesList = new OSItemList(unassignedSpacesVectorController, false);
  //OSCollapsibleItem* unassignedSpacesCollapsibleItem = new OSCollapsibleItem(unassignedSpacesCollapsibleHeader, unassignedSpacesList);
  //myModelList->addCollapsibleItem(unassignedSpacesCollapsibleItem);

  //myModelList->addModelObjectType(IddObjectType::OS_SpaceInfiltration_DesignFlowRate, "Space Infiltration Design Flow Rates"); // do not show in my model because these are not shareable
  myModelList->addModelObjectType(IddObjectType::OS_DesignSpecification_OutdoorAir, "Design Specification Outdoor Air");
  myModelList->addModelObjectType(IddObjectType::OS_DefaultScheduleSet, "Schedule Sets");
  myModelList->addModelObjectType(IddObjectType::OS_DefaultConstructionSet, "Construction Sets");

  setMyModelView(myModelList);

  // my library
  model::Model lib = doc->componentLibrary();

  auto myLibraryList = new ModelObjectTypeListView(lib,true,OSItemType::CollapsibleListHeader,true);
  myLibraryList->setItemsDraggable(true);
  myLibraryList->setItemsRemoveable(false);
  myLibraryList->setItemsType(OSItemType::LibraryItem);
  myLibraryList->setShowFilterLayout(true);

  myLibraryList->addModelObjectType(IddObjectType::OS_Schedule_VariableInterval, "Variable Interval Schedules");
  myLibraryList->addModelObjectType(IddObjectType::OS_Schedule_FixedInterval, "Fixed Interval Schedules");
  myLibraryList->addModelObjectType(IddObjectType::OS_Schedule_Year, "Year Schedules");
  myLibraryList->addModelObjectType(IddObjectType::OS_Schedule_Constant, "Constant Schedules");
  myLibraryList->addModelObjectType(IddObjectType::OS_Schedule_Compact, "Compact Schedules");
  myLibraryList->addModelObjectType(IddObjectType::OS_Schedule_Ruleset, "Ruleset Schedules");

  myLibraryList->addModelObjectType(IddObjectType::OS_InternalMass_Definition, "Internal Mass Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_OtherEquipment_Definition, "Other Equipment Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_SteamEquipment_Definition, "Steam Equipment Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_HotWaterEquipment_Definition, "Hot Water Equipment Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_WaterUse_Equipment_Definition, "Water Use Equipment Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_GasEquipment_Definition, "Gas Equipment Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_ElectricEquipment_Definition, "Electric Equipment Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_Luminaire_Definition, "Luminaire Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_Lights_Definition, "Lights Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_People_Definition, "People Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_SpaceInfiltration_DesignFlowRate, "Space Infiltration Design Flow Rates");
  myLibraryList->addModelObjectType(IddObjectType::OS_SpaceInfiltration_EffectiveLeakageArea, "Space Infiltration Effective Leakage Areas");
  myLibraryList->addModelObjectType(IddObjectType::OS_DesignSpecification_OutdoorAir, "Design Specification Outdoor Air");
  myLibraryList->addModelObjectType(IddObjectType::OS_DefaultScheduleSet, "Schedule Sets");
  myLibraryList->addModelObjectType(IddObjectType::OS_DefaultConstructionSet, "Construction Sets");
  myLibraryList->addModelObjectType(IddObjectType::OS_SpaceType, "Space Types");
  setLibraryView(myLibraryList);

  doc->openSidebar();
}

void MainRightColumnController::configureForFacilitySubTab(int subTabID)
{
  setEditView(nullptr);

  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

  // my model
  auto myModelList = new ModelObjectTypeListView(m_model, true, OSItemType::CollapsibleListHeader);
  myModelList->setItemsDraggable(true);
  myModelList->setItemsRemoveable(false);
  myModelList->setItemsType(OSItemType::LibraryItem);
  myModelList->setShowFilterLayout(true);

  myModelList->addModelObjectType(IddObjectType::OS_SubSurface, "Sub Surfaces");
  myModelList->addModelObjectType(IddObjectType::OS_Surface, "Surfaces");
  myModelList->addModelObjectType(IddObjectType::OS_Construction_WindowDataFile, "Window Data File Constructions");
  myModelList->addModelObjectType(IddObjectType::OS_Construction_FfactorGroundFloor, "F-factor Ground Floor Constructions");
  myModelList->addModelObjectType(IddObjectType::OS_Construction_CfactorUndergroundWall, "C-factor Underground Wall Constructions");
  myModelList->addModelObjectType(IddObjectType::OS_Construction_InternalSource, "Internal Source Constructions");
  myModelList->addModelObjectType(IddObjectType::OS_Construction, "Constructions");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_VariableInterval, "Variable Interval Schedules");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_FixedInterval, "Fixed Interval Schedules");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_Year, "Year Schedules");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_Constant, "Constant Schedules");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_Compact, "Compact Schedules");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_Ruleset, "Ruleset Schedules");
  myModelList->addModelObjectType(IddObjectType::OS_InternalMass_Definition, "Internal Mass Definitions");
  myModelList->addModelObjectType(IddObjectType::OS_OtherEquipment_Definition, "Other Equipment Definitions");
  myModelList->addModelObjectType(IddObjectType::OS_SteamEquipment_Definition, "Steam Equipment Definitions");
  myModelList->addModelObjectType(IddObjectType::OS_HotWaterEquipment_Definition, "Hot Water Equipment Definitions");
  myModelList->addModelObjectType(IddObjectType::OS_WaterUse_Equipment_Definition, "Water Use Equipment Definitions");
  myModelList->addModelObjectType(IddObjectType::OS_GasEquipment_Definition, "Gas Equipment Definitions");
  myModelList->addModelObjectType(IddObjectType::OS_ElectricEquipment_Definition, "Electric Equipment Definitions");
  myModelList->addModelObjectType(IddObjectType::OS_Luminaire_Definition, "Luminaire Definitions");
  myModelList->addModelObjectType(IddObjectType::OS_Lights_Definition, "Lights Definitions");
  myModelList->addModelObjectType(IddObjectType::OS_People_Definition, "People Definitions");
  //myModelList->addModelObjectType(IddObjectType::OS_SpaceInfiltration_DesignFlowRate, "Space Infiltration Design Flow Rates"); // do not show in my model because these are not shareable
  myModelList->addModelObjectType(IddObjectType::OS_DesignSpecification_OutdoorAir, "Design Specification Outdoor Air");
  myModelList->addModelObjectType(IddObjectType::OS_DefaultScheduleSet, "Schedule Sets");
  myModelList->addModelObjectType(IddObjectType::OS_DefaultConstructionSet, "Construction Sets");
  myModelList->addModelObjectType(IddObjectType::OS_SpaceType, "Space Types");
  myModelList->addModelObjectType(IddObjectType::OS_ThermalZone, "Thermal Zones");
  myModelList->addModelObjectType(IddObjectType::OS_BuildingStory, "Building Stories");

  setMyModelView(myModelList);

  // Library
  model::Model lib = doc->combinedComponentLibrary();

  auto myLibraryList = new ModelObjectTypeListView(lib,true,OSItemType::CollapsibleListHeader);
  myLibraryList->setItemsDraggable(true);
  myLibraryList->setItemsRemoveable(false);
  myLibraryList->setItemsType(OSItemType::LibraryItem);
  myLibraryList->setShowFilterLayout(true);

  myLibraryList->addModelObjectType(IddObjectType::OS_Fan_ZoneExhaust,"Fan Zone Exhaust");
  myLibraryList->addModelObjectType(IddObjectType::OS_ZoneHVAC_PackagedTerminalHeatPump,"PTHP");
  myLibraryList->addModelObjectType(IddObjectType::OS_ZoneHVAC_PackagedTerminalAirConditioner,"PTAC");
  myLibraryList->addModelObjectType(IddObjectType::OS_ZoneHVAC_WaterToAirHeatPump,"Water To Air HP");
  myLibraryList->addModelObjectType(IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlow,"Low Temp Radiant Constant Flow");
  myLibraryList->addModelObjectType(IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_VariableFlow,"Low Temp Radiant Variable Flow");
  myLibraryList->addModelObjectType(IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_Electric,"Low Temp Radiant Electric");
  myLibraryList->addModelObjectType(IddObjectType::OS_ZoneHVAC_HighTemperatureRadiant,"High Temp Radiant");
  myLibraryList->addModelObjectType(IddObjectType::OS_Construction_WindowDataFile, "Window Data File Constructions");
  myLibraryList->addModelObjectType(IddObjectType::OS_Construction_FfactorGroundFloor, "F-factor Ground Floor Constructions");
  myLibraryList->addModelObjectType(IddObjectType::OS_Construction_CfactorUndergroundWall, "C-factor Underground Wall Constructions");
  myLibraryList->addModelObjectType(IddObjectType::OS_Construction_InternalSource, "Internal Source Constructions");
  myLibraryList->addModelObjectType(IddObjectType::OS_Construction, "Constructions");
  myLibraryList->addModelObjectType(IddObjectType::OS_Schedule_VariableInterval, "Variable Interval Schedules");
  myLibraryList->addModelObjectType(IddObjectType::OS_Schedule_FixedInterval, "Fixed Interval Schedules");
  myLibraryList->addModelObjectType(IddObjectType::OS_Schedule_Year, "Year Schedules");
  myLibraryList->addModelObjectType(IddObjectType::OS_Schedule_Constant, "Constant Schedules");
  myLibraryList->addModelObjectType(IddObjectType::OS_Schedule_Compact, "Compact Schedules");
  myLibraryList->addModelObjectType(IddObjectType::OS_Schedule_Ruleset, "Ruleset Schedules");
  myLibraryList->addModelObjectType(IddObjectType::OS_InternalMass_Definition, "Internal Mass Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_OtherEquipment_Definition, "Other Equipment Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_SteamEquipment_Definition, "Steam Equipment Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_HotWaterEquipment_Definition, "Hot Water Equipment Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_WaterUse_Equipment_Definition, "Water Use Equipment Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_GasEquipment_Definition, "Gas Equipment Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_ElectricEquipment_Definition, "Electric Equipment Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_Luminaire_Definition, "Luminaire Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_Lights_Definition, "Lights Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_People_Definition, "People Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_SpaceInfiltration_DesignFlowRate, "Space Infiltration Design Flow Rates");
  myLibraryList->addModelObjectType(IddObjectType::OS_SpaceInfiltration_EffectiveLeakageArea, "Space Infiltration Effective Leakage Areas");
  myLibraryList->addModelObjectType(IddObjectType::OS_DesignSpecification_OutdoorAir, "Design Specification Outdoor Air");
  myLibraryList->addModelObjectType(IddObjectType::OS_DefaultScheduleSet, "Schedule Sets");
  myLibraryList->addModelObjectType(IddObjectType::OS_DefaultConstructionSet, "Construction Sets");
  myLibraryList->addModelObjectType(IddObjectType::OS_SpaceType, "Space Types");

  setLibraryView(myLibraryList);

  doc->openSidebar();
  //doc->closeSidebar();

}

void MainRightColumnController::configureForSpacesSubTab(int subTabID)
{
  setEditView(nullptr);

  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

  // my model
  auto myModelList = new ModelObjectTypeListView(m_model, true, OSItemType::CollapsibleListHeader);
  myModelList->setItemsDraggable(true);
  myModelList->setItemsRemoveable(false);
  myModelList->setItemsType(OSItemType::LibraryItem);
  myModelList->setShowFilterLayout(true);

  myModelList->addModelObjectType(IddObjectType::OS_InteriorPartitionSurface, "Interior Partition Surface");
  myModelList->addModelObjectType(IddObjectType::OS_ShadingSurface, "Shading Surface");
  myModelList->addModelObjectType(IddObjectType::OS_ShadingControl, "ShadingControl");
  myModelList->addModelObjectType(IddObjectType::OS_WindowProperty_FrameAndDivider, "Frame And Divider Window Property");
  myModelList->addModelObjectType(IddObjectType::OS_DaylightingDevice_Shelf, "DaylightingDevice Shelf");
  myModelList->addModelObjectType(IddObjectType::OS_SubSurface, "Sub Surfaces");
  myModelList->addModelObjectType(IddObjectType::OS_Surface, "Surfaces");
  myModelList->addModelObjectType(IddObjectType::OS_Construction_WindowDataFile, "Window Data File Constructions");
  myModelList->addModelObjectType(IddObjectType::OS_Construction_FfactorGroundFloor, "F-factor Ground Floor Constructions");
  myModelList->addModelObjectType(IddObjectType::OS_Construction_CfactorUndergroundWall, "C-factor Underground Wall Constructions");
  myModelList->addModelObjectType(IddObjectType::OS_Construction_InternalSource, "Internal Source Constructions");
  myModelList->addModelObjectType(IddObjectType::OS_Construction, "Constructions");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_VariableInterval, "Variable Interval Schedules");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_FixedInterval, "Fixed Interval Schedules");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_Year, "Year Schedules");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_Constant, "Constant Schedules");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_Compact, "Compact Schedules");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_Ruleset, "Ruleset Schedules");
  myModelList->addModelObjectType(IddObjectType::OS_InternalMass_Definition, "Internal Mass Definitions");
  myModelList->addModelObjectType(IddObjectType::OS_OtherEquipment_Definition, "Other Equipment Definitions");
  myModelList->addModelObjectType(IddObjectType::OS_SteamEquipment_Definition, "Steam Equipment Definitions");
  myModelList->addModelObjectType(IddObjectType::OS_HotWaterEquipment_Definition, "Hot Water Equipment Definitions");
  myModelList->addModelObjectType(IddObjectType::OS_WaterHeater_HeatPump,"Water Heater - Heat Pump");
  myModelList->addModelObjectType(IddObjectType::OS_WaterHeater_HeatPump_WrappedCondenser,"Water Heater - Heat Pump - Wrapped Condenser");
  myModelList->addModelObjectType(IddObjectType::OS_WaterUse_Equipment_Definition, "Water Use Equipment Definitions");
  myModelList->addModelObjectType(IddObjectType::OS_GasEquipment_Definition, "Gas Equipment Definitions");
  myModelList->addModelObjectType(IddObjectType::OS_ElectricEquipment_Definition, "Electric Equipment Definitions");
  myModelList->addModelObjectType(IddObjectType::OS_Luminaire_Definition, "Luminaire Definitions");
  myModelList->addModelObjectType(IddObjectType::OS_Lights_Definition, "Lights Definitions");
  myModelList->addModelObjectType(IddObjectType::OS_People_Definition, "People Definitions");
  //myModelList->addModelObjectType(IddObjectType::OS_SpaceInfiltration_DesignFlowRate, "Space Infiltration Design Flow Rates"); // do not show in my model because these are not shareable
  myModelList->addModelObjectType(IddObjectType::OS_DesignSpecification_OutdoorAir, "Design Specification Outdoor Air");
  myModelList->addModelObjectType(IddObjectType::OS_DefaultScheduleSet, "Schedule Sets");
  myModelList->addModelObjectType(IddObjectType::OS_DefaultConstructionSet, "Construction Sets");
  myModelList->addModelObjectType(IddObjectType::OS_SpaceType, "Space Types");
  myModelList->addModelObjectType(IddObjectType::OS_ThermalZone, "Thermal Zones");
  myModelList->addModelObjectType(IddObjectType::OS_BuildingStory, "Building Stories");
  myModelList->addModelObjectType(IddObjectType::OS_InteriorPartitionSurface, "Interior Partition Surface");

  setMyModelView(myModelList);

  // Library
  model::Model lib = doc->combinedComponentLibrary();

  auto myLibraryList = new ModelObjectTypeListView(lib,true,OSItemType::CollapsibleListHeader,true);
  myLibraryList->setItemsDraggable(true);
  myLibraryList->setItemsRemoveable(false);
  myLibraryList->setItemsType(OSItemType::LibraryItem);
  myLibraryList->setShowFilterLayout(true);

  myLibraryList->addModelObjectType(IddObjectType::OS_ShadingControl, "ShadingControl");
  myLibraryList->addModelObjectType(IddObjectType::OS_WindowProperty_FrameAndDivider, "Frame And Divider Window Property");
  myLibraryList->addModelObjectType(IddObjectType::OS_DaylightingDevice_Shelf, "DaylightingDevice Shelf");
  myLibraryList->addModelObjectType(IddObjectType::OS_Fan_ZoneExhaust, "Fan Zone Exhaust");
  myLibraryList->addModelObjectType(IddObjectType::OS_ZoneHVAC_PackagedTerminalHeatPump, "PTHP");
  myLibraryList->addModelObjectType(IddObjectType::OS_ZoneHVAC_PackagedTerminalAirConditioner, "PTAC");
  myLibraryList->addModelObjectType(IddObjectType::OS_ZoneHVAC_WaterToAirHeatPump, "Water To Air HP");
  myLibraryList->addModelObjectType(IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlow, "Low Temp Radiant Constant Flow");
  myLibraryList->addModelObjectType(IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_VariableFlow, "Low Temp Radiant Variable Flow");
  myLibraryList->addModelObjectType(IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_Electric, "Low Temp Radiant Electric");
  myLibraryList->addModelObjectType(IddObjectType::OS_ZoneHVAC_HighTemperatureRadiant, "High Temp Radiant");
  myLibraryList->addModelObjectType(IddObjectType::OS_Construction_WindowDataFile, "Window Data File Constructions");
  myLibraryList->addModelObjectType(IddObjectType::OS_Construction_FfactorGroundFloor, "F-factor Ground Floor Constructions");
  myLibraryList->addModelObjectType(IddObjectType::OS_Construction_CfactorUndergroundWall, "C-factor Underground Wall Constructions");
  myLibraryList->addModelObjectType(IddObjectType::OS_Construction_InternalSource, "Internal Source Constructions");
  myLibraryList->addModelObjectType(IddObjectType::OS_Construction, "Constructions");
  myLibraryList->addModelObjectType(IddObjectType::OS_Schedule_VariableInterval, "Variable Interval Schedules");
  myLibraryList->addModelObjectType(IddObjectType::OS_Schedule_FixedInterval, "Fixed Interval Schedules");
  myLibraryList->addModelObjectType(IddObjectType::OS_Schedule_Year, "Year Schedules");
  myLibraryList->addModelObjectType(IddObjectType::OS_Schedule_Constant, "Constant Schedules");
  myLibraryList->addModelObjectType(IddObjectType::OS_Schedule_Compact, "Compact Schedules");
  myLibraryList->addModelObjectType(IddObjectType::OS_Schedule_Ruleset, "Ruleset Schedules");
  myLibraryList->addModelObjectType(IddObjectType::OS_InternalMass_Definition, "Internal Mass Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_OtherEquipment_Definition, "Other Equipment Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_SteamEquipment_Definition, "Steam Equipment Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_HotWaterEquipment_Definition, "Hot Water Equipment Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_WaterUse_Equipment_Definition, "Water Use Equipment Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_WaterHeater_HeatPump,"Water Heater - Heat Pump");
  myLibraryList->addModelObjectType(IddObjectType::OS_WaterHeater_HeatPump_WrappedCondenser,"Water Heater - Heat Pump - Wrapped Condenser");
  myLibraryList->addModelObjectType(IddObjectType::OS_GasEquipment_Definition, "Gas Equipment Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_ElectricEquipment_Definition, "Electric Equipment Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_Luminaire_Definition, "Luminaire Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_Lights_Definition, "Lights Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_People_Definition, "People Definitions");
  myLibraryList->addModelObjectType(IddObjectType::OS_SpaceInfiltration_DesignFlowRate, "Space Infiltration Design Flow Rates");
  myLibraryList->addModelObjectType(IddObjectType::OS_SpaceInfiltration_EffectiveLeakageArea, "Space Infiltration Effective Leakage Areas");
  myLibraryList->addModelObjectType(IddObjectType::OS_DesignSpecification_OutdoorAir, "Design Specification Outdoor Air");
  myLibraryList->addModelObjectType(IddObjectType::OS_DefaultScheduleSet, "Schedule Sets");
  myLibraryList->addModelObjectType(IddObjectType::OS_DefaultConstructionSet, "Construction Sets");
  myLibraryList->addModelObjectType(IddObjectType::OS_SpaceType, "Space Types");

  setLibraryView(myLibraryList);

  doc->openSidebar();
  //doc->closeSidebar();

}

void MainRightColumnController::configureForThermalZonesSubTab(int subTabID)
{

  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

  setLibraryView(nullptr);
  setMyModelView(nullptr);
  setEditView(nullptr);

  // My Model

  auto myModelList = new ModelObjectTypeListView(m_model, true, OSItemType::CollapsibleListHeader);
  myModelList->setItemsDraggable(true);
  myModelList->setItemsRemoveable(false);
  myModelList->setItemsType(OSItemType::LibraryItem);
  myModelList->setShowFilterLayout(true);
  myModelList->addModelObjectType(IddObjectType::OS_WaterHeater_HeatPump,"Water Heater - Heat Pump");
  myModelList->addModelObjectType(IddObjectType::OS_WaterHeater_HeatPump_WrappedCondenser,"Water Heater - Heat Pump - Wrapped Condenser");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_VariableInterval, "Variable Interval Schedules");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_FixedInterval, "Fixed Interval Schedules");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_Year, "Year Schedules");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_Constant, "Constant Schedules");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_Compact, "Compact Schedules");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_Ruleset, "Ruleset Schedules");


  setMyModelView(myModelList);

  // Library
  model::Model lib = doc->combinedComponentLibrary();

  auto libraryWidget = new ModelObjectTypeListView(lib,true,OSItemType::CollapsibleListHeader);
  libraryWidget->setItemsDraggable(true);
  libraryWidget->setItemsRemoveable(false);
  libraryWidget->setItemsType(OSItemType::LibraryItem);
  libraryWidget->setShowFilterLayout(true);

  libraryWidget->addModelObjectType(IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Electric,"Baseboard Convective Electric");
  libraryWidget->addModelObjectType(IddObjectType::OS_ZoneHVAC_Baseboard_Convective_Water,"Baseboard Convective Water");
  libraryWidget->addModelObjectType(IddObjectType::OS_ZoneHVAC_Baseboard_RadiantConvective_Electric,"Baseboard Radiant Convective Electric");
  libraryWidget->addModelObjectType(IddObjectType::OS_ZoneHVAC_Baseboard_RadiantConvective_Water,"Baseboard Radiant Convective Water");
  libraryWidget->addModelObjectType(IddObjectType::OS_ZoneHVAC_Dehumidifier_DX,"Dehumidifier - DX");
  libraryWidget->addModelObjectType(IddObjectType::OS_ZoneHVAC_EnergyRecoveryVentilator,"ERV");
  libraryWidget->addModelObjectType(IddObjectType::OS_ZoneHVAC_FourPipeFanCoil,"Four Pipe Fan Coil");
  libraryWidget->addModelObjectType(IddObjectType::OS_Fan_ZoneExhaust,"Fan Zone Exhaust");
  libraryWidget->addModelObjectType(IddObjectType::OS_WaterHeater_HeatPump,"Water Heater - Heat Pump");
  libraryWidget->addModelObjectType(IddObjectType::OS_WaterHeater_HeatPump_WrappedCondenser,"Water Heater - Heat Pump - Wrapped Condenser");
  libraryWidget->addModelObjectType(IddObjectType::OS_ZoneHVAC_PackagedTerminalHeatPump,"PTHP");
  libraryWidget->addModelObjectType(IddObjectType::OS_ZoneHVAC_WaterToAirHeatPump,"Water To Air HP");
  libraryWidget->addModelObjectType(IddObjectType::OS_ZoneHVAC_PackagedTerminalAirConditioner,"PTAC");
  libraryWidget->addModelObjectType(IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_ConstantFlow,"Low Temp Radiant Constant Flow");
  libraryWidget->addModelObjectType(IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_VariableFlow,"Low Temp Radiant Variable Flow");
  libraryWidget->addModelObjectType(IddObjectType::OS_ZoneHVAC_LowTemperatureRadiant_Electric,"Low Temp Radiant Electric");
  libraryWidget->addModelObjectType(IddObjectType::OS_ZoneHVAC_HighTemperatureRadiant,"High Temp Radiant");
  libraryWidget->addModelObjectType(IddObjectType::OS_ZoneHVAC_UnitHeater,"Unit Heater");
  libraryWidget->addModelObjectType(IddObjectType::OS_ZoneHVAC_UnitVentilator,"Unit Ventilator");
  libraryWidget->addModelObjectType(IddObjectType::OS_ZoneVentilation_DesignFlowRate,"Zone Ventilation Design Flow Rate");
  libraryWidget->addModelObjectType(IddObjectType::OS_Schedule_Compact,"Compact Schedules");
  libraryWidget->addModelObjectType(IddObjectType::OS_Schedule_Ruleset,"Schedule Rulesets");

  setLibraryView(libraryWidget);

  m_horizontalTabWidget->setCurrentId(LIBRARY);

  doc->openSidebar();
}

void MainRightColumnController::configureForHVACSystemsSubTab(int subTabID)
{

  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

  setLibraryView(nullptr);
  setMyModelView(nullptr);
  setEditView(nullptr);

  // my model
  auto myModelList = new ModelObjectTypeListView(m_model, true, OSItemType::CollapsibleListHeader);
  myModelList->setItemsDraggable(true);
  myModelList->setItemsRemoveable(false);
  myModelList->setItemsType(OSItemType::LibraryItem);
  myModelList->setShowFilterLayout(true);

  myModelList->addModelObjectType(IddObjectType::OS_ZoneHVAC_FourPipeFanCoil,"Four Pipe Fan Coil");
  myModelList->addModelObjectType(IddObjectType::OS_ZoneHVAC_PackagedTerminalHeatPump,"PTHP");
  myModelList->addModelObjectType(IddObjectType::OS_ZoneHVAC_WaterToAirHeatPump,"Water To Air HP");
  myModelList->addModelObjectType(IddObjectType::OS_ZoneHVAC_PackagedTerminalAirConditioner,"PTAC");
  myModelList->addModelObjectType(IddObjectType::OS_ZoneHVAC_UnitHeater,"Unit Heater");
  myModelList->addModelObjectType(IddObjectType::OS_ZoneHVAC_UnitVentilator,"Unit Ventilator");
  myModelList->addModelObjectType(IddObjectType::OS_WaterUse_Equipment_Definition,"Water Use Equipment Definition");
  myModelList->addModelObjectType(IddObjectType::OS_WaterUse_Connections,"Water Use Connections");
  // For some reason, this doesn't suffice to pick up the WaterHeater:Mixed linked to a HeatPump:WaterHeater
  myModelList->addModelObjectType(IddObjectType::OS_WaterHeater_Mixed, "Water Heater Mixed");
  myModelList->addModelObjectType(IddObjectType::OS_WaterHeater_Stratified, "Water Heater Stratified");
  myModelList->addModelObjectType(IddObjectType::OS_ThermalZone,"Thermal Zone");
  myModelList->addModelObjectType(IddObjectType::OS_ThermalStorage_ChilledWater_Stratified,"Thermal Storage - Chilled Water");
  myModelList->addModelObjectType(IddObjectType::OS_Refrigeration_System,"Refrigeration System");
  myModelList->addModelObjectType(IddObjectType::OS_Refrigeration_Condenser_WaterCooled,"Refrigeration Condenser Water Cooled");
  myModelList->addModelObjectType(IddObjectType::OS_HeatPump_WaterToWater_EquationFit_Heating,"Heat Pump - Water to Water - Heating");
  myModelList->addModelObjectType(IddObjectType::OS_HeatPump_WaterToWater_EquationFit_Cooling,"Heat Pump - Water to Water - Cooling");
  myModelList->addModelObjectType(IddObjectType::OS_HeatExchanger_FluidToFluid,"Heat Exchanger Fluid To Fluid");
  myModelList->addModelObjectType(IddObjectType::OS_Coil_Heating_Water,"Coil Heating Water");
  myModelList->addModelObjectType(IddObjectType::OS_Coil_Cooling_Water,"Coil Cooling Water");
  myModelList->addModelObjectType(IddObjectType::OS_Chiller_Electric_EIR,"Chiller - Electric EIR");
  myModelList->addModelObjectType(IddObjectType::OS_Chiller_Absorption,"Chiller - Absorption");
  myModelList->addModelObjectType(IddObjectType::OS_Chiller_Absorption_Indirect,"Chiller - Indirect Absorption");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_VariableInterval, "Variable Interval Schedules");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_FixedInterval, "Fixed Interval Schedules");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_Year, "Year Schedules");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_Constant, "Constant Schedules");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_Compact, "Compact Schedules");
  myModelList->addModelObjectType(IddObjectType::OS_Schedule_Ruleset, "Ruleset Schedules");

  setMyModelView(myModelList);

  // Library

  model::Model lib = doc->hvacComponentLibrary();

  auto libraryWidget = new ModelObjectTypeListView(lib,true,OSItemType::CollapsibleListHeader,true);
  libraryWidget->setItemsDraggable(true);
  libraryWidget->setItemsRemoveable(false);
  libraryWidget->setItemsType(OSItemType::LibraryItem);
  libraryWidget->setShowFilterLayout(true);

  libraryWidget->addModelObjectType(IddObjectType::OS_SolarCollector_IntegralCollectorStorage, "Solar Collector Integral Collector Storage");
  libraryWidget->addModelObjectType(IddObjectType::OS_SolarCollector_FlatPlate_Water, "Solar Collector Flat Plate Water");
  //libraryWidget->addModelObjectType(IddObjectType::OS_SolarCollector_FlatPlate_PhotovoltaicThermal, "Solar Collector Flat Plate Photovoltaic Thermal");
  libraryWidget->addModelObjectType(IddObjectType::OS_WaterUse_Equipment,"Water Use Equipment");
  libraryWidget->addModelObjectType(IddObjectType::OS_WaterUse_Connections,"Water Use Connections");
  libraryWidget->addModelObjectType(IddObjectType::OS_WaterHeater_HeatPump,"Water Heater - Heat Pump");
  libraryWidget->addModelObjectType(IddObjectType::OS_WaterHeater_HeatPump_WrappedCondenser,"Water Heater - Heat Pump - Wrapped Condenser");
  libraryWidget->addModelObjectType(IddObjectType::OS_WaterHeater_Mixed,"Water Heater Mixed");
  libraryWidget->addModelObjectType(IddObjectType::OS_WaterHeater_Stratified,"Water Heater Stratified");
  libraryWidget->addModelObjectType(IddObjectType::OS_AirConditioner_VariableRefrigerantFlow,"VRF System");
  libraryWidget->addModelObjectType(IddObjectType::OS_ZoneHVAC_TerminalUnit_VariableRefrigerantFlow,"VRF Terminal");
  libraryWidget->addModelObjectType(IddObjectType::OS_ThermalStorage_Ice_Detailed,"Thermal Storage - Ice Storage");
  libraryWidget->addModelObjectType(IddObjectType::OS_ThermalStorage_ChilledWater_Stratified,"Thermal Storage - Chilled Water");
  libraryWidget->addModelObjectType(IddObjectType::OS_TemperingValve,"Tempering Valve");
  libraryWidget->addModelObjectType(IddObjectType::OS_SetpointManager_Coldest,"Setpoint Manager Coldest");
  libraryWidget->addModelObjectType(IddObjectType::OS_SetpointManager_FollowGroundTemperature,"Setpoint Manager Follow Ground Temperature");
  libraryWidget->addModelObjectType(IddObjectType::OS_SetpointManager_FollowOutdoorAirTemperature,"Setpoint Manager Follow Outdoor Air Temperature");
  libraryWidget->addModelObjectType(IddObjectType::OS_SetpointManager_FollowSystemNodeTemperature,"Setpoint Manager Follow System Node Temperature");
  libraryWidget->addModelObjectType(IddObjectType::OS_SetpointManager_MixedAir,"Setpoint Manager Mixed Air");
  libraryWidget->addModelObjectType(IddObjectType::OS_SetpointManager_MultiZone_Cooling_Average,"Setpoint Manager MultiZone Cooling Average");
  libraryWidget->addModelObjectType(IddObjectType::OS_SetpointManager_MultiZone_Heating_Average,"Setpoint Manager MultiZone Heating Average");
  libraryWidget->addModelObjectType(IddObjectType::OS_SetpointManager_MultiZone_Humidity_Maximum,"Setpoint Manager MultiZone Humidity Maximum");
  libraryWidget->addModelObjectType(IddObjectType::OS_SetpointManager_MultiZone_Humidity_Minimum,"Setpoint Manager MultiZone Humidity Minimum");
  libraryWidget->addModelObjectType(IddObjectType::OS_SetpointManager_MultiZone_MaximumHumidity_Average,"Setpoint Manager MultiZone MaximumHumidity Average");
  libraryWidget->addModelObjectType(IddObjectType::OS_SetpointManager_MultiZone_MinimumHumidity_Average,"Setpoint Manager MultiZone MinimumHumidity Average");
  libraryWidget->addModelObjectType(IddObjectType::OS_SetpointManager_OutdoorAirPretreat,"Setpoint Manager Outdoor Air Pretreat");
  libraryWidget->addModelObjectType(IddObjectType::OS_SetpointManager_OutdoorAirReset,"Setpoint Manager Outdoor Air Reset");
  libraryWidget->addModelObjectType(IddObjectType::OS_SetpointManager_Scheduled,"Setpoint Manager Scheduled");
  libraryWidget->addModelObjectType(IddObjectType::OS_SetpointManager_Scheduled_DualSetpoint,"Setpoint Manager Scheduled Dual Setpoint");
  libraryWidget->addModelObjectType(IddObjectType::OS_SetpointManager_SingleZone_Cooling,"Setpoint Manager Single Zone Cooling");
  libraryWidget->addModelObjectType(IddObjectType::OS_SetpointManager_SingleZone_Heating,"Setpoint Manager Single Zone Heating");
  libraryWidget->addModelObjectType(IddObjectType::OS_SetpointManager_SingleZone_Humidity_Maximum,"Setpoint Manager Humidity Maximum");
  libraryWidget->addModelObjectType(IddObjectType::OS_SetpointManager_SingleZone_Humidity_Minimum,"Setpoint Manager Humidity Minimum");
  libraryWidget->addModelObjectType(IddObjectType::OS_SetpointManager_SingleZone_OneStageCooling,"Setpoint Manager One Stage Cooling");
  libraryWidget->addModelObjectType(IddObjectType::OS_SetpointManager_SingleZone_OneStageHeating,"Setpoint Manager One Stage Heating");
  libraryWidget->addModelObjectType(IddObjectType::OS_SetpointManager_SingleZone_Reheat,"Setpoint Manager Single Zone Reheat");
  libraryWidget->addModelObjectType(IddObjectType::OS_SetpointManager_Warmest,"Setpoint Manager Warmest");
  libraryWidget->addModelObjectType(IddObjectType::OS_SetpointManager_WarmestTemperatureFlow,"Setpoint Manager Warmest Temp and Flow");
  libraryWidget->addModelObjectType(IddObjectType::OS_Refrigeration_WalkIn,"Refrigeration Walkin");
  libraryWidget->addModelObjectType(IddObjectType::OS_Refrigeration_System,"Refrigeration System");
  libraryWidget->addModelObjectType(IddObjectType::OS_Refrigeration_Subcooler_Mechanical,"Refrigeration Subcooler Mechanical");
  libraryWidget->addModelObjectType(IddObjectType::OS_Refrigeration_Subcooler_LiquidSuction,"Refrigeration Subcooler Liquid Suction");
  libraryWidget->addModelObjectType(IddObjectType::OS_Refrigeration_Compressor,"Refrigeration Compressor");
  libraryWidget->addModelObjectType(IddObjectType::OS_Refrigeration_Condenser_Cascade,"Refrigeration Condenser Cascade");
  libraryWidget->addModelObjectType(IddObjectType::OS_Refrigeration_Condenser_WaterCooled,"Refrigeration Condenser Water Cooled");
  libraryWidget->addModelObjectType(IddObjectType::OS_Refrigeration_Condenser_EvaporativeCooled,"Refrigeration Condenser Evaporative Cooled");
  libraryWidget->addModelObjectType(IddObjectType::OS_Refrigeration_Condenser_AirCooled,"Refrigeration Condenser Air Cooled");
  libraryWidget->addModelObjectType(IddObjectType::OS_Refrigeration_Case,"Refrigeration Case");
  libraryWidget->addModelObjectType(IddObjectType::OS_Pump_ConstantSpeed,"Pump Constant Speed");
  libraryWidget->addModelObjectType(IddObjectType::OS_HeaderedPumps_ConstantSpeed,"Pump Constant Speed Headered");
  libraryWidget->addModelObjectType(IddObjectType::OS_Pump_VariableSpeed,"Pump Variable Speed");
  libraryWidget->addModelObjectType(IddObjectType::OS_HeaderedPumps_VariableSpeed,"Pump Variable Speed Headered");
  libraryWidget->addModelObjectType(IddObjectType::OS_PlantComponent_TemperatureSource, "Plant Component - Temp Source");
  libraryWidget->addModelObjectType(IddObjectType::OS_Pipe_Outdoor, "Pipe - Outdoor");
  libraryWidget->addModelObjectType(IddObjectType::OS_Pipe_Indoor, "Pipe - Indoor");
  libraryWidget->addModelObjectType(IddObjectType::OS_Pipe_Adiabatic, "Pipe - Adiabatic");
  libraryWidget->addModelObjectType(IddObjectType::OS_LoadProfile_Plant, "Load Profile - Plant");
  libraryWidget->addModelObjectType(IddObjectType::OS_Humidifier_Steam_Electric,"Humidifier Steam Electric");
  libraryWidget->addModelObjectType(IddObjectType::OS_HeatPump_WaterToWater_EquationFit_Heating,"Heat Pump - Water to Water - Heating");
  libraryWidget->addModelObjectType(IddObjectType::OS_HeatPump_WaterToWater_EquationFit_Cooling,"Heat Pump - Water to Water - Cooling");
  libraryWidget->addModelObjectType(IddObjectType::OS_HeatExchanger_FluidToFluid,"Heat Exchanger Fluid To Fluid");
  libraryWidget->addModelObjectType(IddObjectType::OS_HeatExchanger_AirToAir_SensibleAndLatent,"Heat Exchanger Air To Air Sensible and Latent");
  libraryWidget->addModelObjectType(IddObjectType::OS_Generator_FuelCell_ExhaustGasToWaterHeatExchanger,"Generator FuelCell - ExhaustGasToWaterHeatExchanger");
  libraryWidget->addModelObjectType(IddObjectType::OS_Generator_MicroTurbine_HeatRecovery,"Generator MicroTurbine - Heat Recovery");
  libraryWidget->addModelObjectType(IddObjectType::OS_GroundHeatExchanger_Vertical, "Ground Heat Exchanger - Vertical ");
  libraryWidget->addModelObjectType(IddObjectType::OS_GroundHeatExchanger_HorizontalTrench,"Ground Heat Exchanger - Horizontal");
  libraryWidget->addModelObjectType(IddObjectType::OS_FluidCooler_SingleSpeed,"Fluid Cooler Single Speed");
  libraryWidget->addModelObjectType(IddObjectType::OS_FluidCooler_TwoSpeed,"Fluid Cooler Two Speed");
  libraryWidget->addModelObjectType(IddObjectType::OS_ZoneHVAC_FourPipeFanCoil,"Four Pipe Fan Coil");
  libraryWidget->addModelObjectType(IddObjectType::OS_ZoneHVAC_PackagedTerminalHeatPump,"PTHP");
  libraryWidget->addModelObjectType(IddObjectType::OS_ZoneHVAC_WaterToAirHeatPump,"Water To Air HP");
  libraryWidget->addModelObjectType(IddObjectType::OS_ZoneHVAC_PackagedTerminalAirConditioner,"PTAC");
  libraryWidget->addModelObjectType(IddObjectType::OS_ZoneHVAC_UnitHeater,"Unit Heater");
  libraryWidget->addModelObjectType(IddObjectType::OS_ZoneHVAC_UnitVentilator,"Unit Ventilator");
  libraryWidget->addModelObjectType(IddObjectType::OS_Fan_VariableVolume,"Fan Variable Volume");
  libraryWidget->addModelObjectType(IddObjectType::OS_Fan_ConstantVolume,"Fan Constant Volume");
  libraryWidget->addModelObjectType(IddObjectType::OS_EvaporativeCooler_Direct_ResearchSpecial,"Evaporative Cooler Direct Research Special");
  libraryWidget->addModelObjectType(IddObjectType::OS_EvaporativeCooler_Indirect_ResearchSpecial,"Evaporative Cooler Indirect Research Special");
  libraryWidget->addModelObjectType(IddObjectType::OS_EvaporativeFluidCooler_TwoSpeed,"Evaporative Fluid Cooler Two Speed");
  libraryWidget->addModelObjectType(IddObjectType::OS_EvaporativeFluidCooler_SingleSpeed,"Evaporative Fluid Cooler Single Speed");
  libraryWidget->addModelObjectType(IddObjectType::OS_Duct,"Duct");
  libraryWidget->addModelObjectType(IddObjectType::OS_DistrictCooling,"District Cooling");
  libraryWidget->addModelObjectType(IddObjectType::OS_DistrictHeating,"District Heating");
  libraryWidget->addModelObjectType(IddObjectType::OS_CoolingTower_TwoSpeed, "Cooling Tower Two Speed");
  libraryWidget->addModelObjectType(IddObjectType::OS_CoolingTower_SingleSpeed, "Cooling Tower Single Speed");
  libraryWidget->addModelObjectType(IddObjectType::OS_CoolingTower_VariableSpeed, "Cooling Tower Variable Speed");
  libraryWidget->addModelObjectType(IddObjectType::OS_Chiller_Electric_EIR,"Chiller - Electric EIR");
  libraryWidget->addModelObjectType(IddObjectType::OS_Chiller_Absorption_Indirect,"Chiller - Indirect Absorption");
  libraryWidget->addModelObjectType(IddObjectType::OS_Chiller_Absorption,"Chiller - Absorption");
  libraryWidget->addModelObjectType(IddObjectType::OS_Coil_Heating_Gas,"Coil Heating Gas");
  libraryWidget->addModelObjectType(IddObjectType::OS_Coil_Heating_DX_SingleSpeed,"Coil Heating DX SingleSpeed");
  libraryWidget->addModelObjectType(IddObjectType::OS_Coil_Heating_Electric,"Coil Heating Electric");
  libraryWidget->addModelObjectType(IddObjectType::OS_Coil_Heating_Water,"Coil Heating Water");
  libraryWidget->addModelObjectType(IddObjectType::OS_Coil_Cooling_Water,"Coil Cooling Water");
  libraryWidget->addModelObjectType(IddObjectType::OS_Coil_Cooling_DX_TwoStageWithHumidityControlMode,"Coil Cooling DX TwoStage - Humidity Control");
  libraryWidget->addModelObjectType(IddObjectType::OS_Coil_Cooling_DX_TwoSpeed,"Coil Cooling DX TwoSpeed");
  libraryWidget->addModelObjectType(IddObjectType::OS_Coil_Cooling_DX_SingleSpeed,"Coil Cooling DX SingleSpeed");
  libraryWidget->addModelObjectType(IddObjectType::OS_Boiler_HotWater,"Boiler Hot Water");
  libraryWidget->addModelObjectType(IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_FourPipeInduction, "Air Terminal Four Pipe Induction");
  libraryWidget->addModelObjectType(IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_CooledBeam, "Air Terminal Chilled Beam");
  libraryWidget->addModelObjectType(IddObjectType::OS_AirTerminal_SingleDuct_ConstantVolume_Reheat,"AirTerminal Single Duct Constant Volume Reheat");
  libraryWidget->addModelObjectType(IddObjectType::OS_AirTerminal_SingleDuct_VAV_Reheat,"AirTerminal Single Duct VAV Reheat");
  libraryWidget->addModelObjectType(IddObjectType::OS_AirTerminal_SingleDuct_ParallelPIU_Reheat,"AirTerminal Single Duct Parallel PIU Reheat");
  libraryWidget->addModelObjectType(IddObjectType::OS_AirTerminal_SingleDuct_SeriesPIU_Reheat,"AirTerminal Single Duct Series PIU Reheat");
  libraryWidget->addModelObjectType(IddObjectType::OS_AirTerminal_SingleDuct_InletSideMixer,"AirTerminal Inlet Side Mixer");
  libraryWidget->addModelObjectType(IddObjectType::OS_AirTerminal_SingleDuct_VAV_HeatAndCool_Reheat,"AirTerminal Heat and Cool Reheat");
  libraryWidget->addModelObjectType(IddObjectType::OS_AirTerminal_SingleDuct_VAV_HeatAndCool_NoReheat,"AirTerminal Heat and Cool No Reheat");
  libraryWidget->addModelObjectType(IddObjectType::OS_AirTerminal_SingleDuct_VAV_NoReheat,"AirTerminal Single Duct VAV NoReheat");
  libraryWidget->addModelObjectType(IddObjectType::OS_AirTerminal_SingleDuct_Uncontrolled,"AirTerminal Single Duct Uncontrolled");
  libraryWidget->addModelObjectType(IddObjectType::OS_AirTerminal_DualDuct_VAV, "Air Terminal Dual Duct VAV");
  libraryWidget->addModelObjectType(IddObjectType::OS_AirLoopHVAC_OutdoorAirSystem,"AirLoopHVAC Outdoor Air System");
  libraryWidget->addModelObjectType(IddObjectType::OS_AirLoopHVAC_UnitarySystem, "AirLoopHVAC Unitary System");
  libraryWidget->addModelObjectType(IddObjectType::OS_AirLoopHVAC_UnitaryHeatCool_VAVChangeoverBypass, "AirLoopHVAC Unitary VAV Changeover Bypass");


  setLibraryView(libraryWidget);

  m_horizontalTabWidget->setCurrentId(LIBRARY);

  doc->openSidebar();
}

void MainRightColumnController::configureForBuildingSummarySubTab(int subTabID)
{
  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

  setLibraryView(nullptr);
  setMyModelView(nullptr);
  setEditView(nullptr);

  //doc->openSidebar();
  doc->closeSidebar();
}

void MainRightColumnController::configureForOutputVariablesSubTab(int subTabID)
{
  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

  setLibraryView(nullptr);
  setMyModelView(nullptr);
  setEditView(nullptr);

  //doc->openSidebar();
  doc->closeSidebar();
}

void MainRightColumnController::configureForSimulationSettingsSubTab(int subTabID)
{
  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

  setLibraryView(nullptr);
  setMyModelView(nullptr);
  setEditView(nullptr);

  //doc->openSidebar();
  doc->closeSidebar();
}

void MainRightColumnController::configureForScriptsSubTab(int subTabID)
{
  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

  setLibraryView(m_measureLibraryController->localLibraryView.data());
  setMyModelView(nullptr);
  m_measureEditController->reset();
  setEditView(m_measureEditController->editView.data());

  doc->openSidebar();
}

void MainRightColumnController::configureForRunSimulationSubTab(int subTabID)
{
  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

  setLibraryView(nullptr);
  setMyModelView(nullptr);
  setEditView(nullptr);

  //doc->openSidebar();
  doc->closeSidebar();
}

void MainRightColumnController::configureForResultsSummarySubTab(int subTabID)
{
  std::shared_ptr<OSDocument> doc = OSAppBase::instance()->currentDocument();

  setLibraryView(nullptr);
  setMyModelView(nullptr);
  setEditView(nullptr);

  //doc->openSidebar();
  doc->closeSidebar();
}

void MainRightColumnController::toggleUnits(bool displayIP)
{
}

QSharedPointer<LocalLibraryController> MainRightColumnController::measuresLibraryController()
{
  return m_measureLibraryController;
}

QSharedPointer<EditController> MainRightColumnController::measuresEditController()
{
  return m_measureEditController;
}

void MainRightColumnController::chooseEditTab()
{
  m_horizontalTabWidget->setCurrentId(EDIT);

  OSAppBase::instance()->currentDocument()->openSidebar();
}

void MainRightColumnController::hideMyModelTab(bool hide)
{
  m_myModelTabIsHidden = hide;

  m_horizontalTabWidget->hideTab(m_myModelView,hide);
}

} // openstudio

