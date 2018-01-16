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

#include "InspectorGadget.hpp"
#include "InspectorDialog.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../model/ResourceObject.hpp"
#include "../model/ResourceObject_Impl.hpp"
#include "../model/AccessPolicyStore.hpp"

#include "../utilities/core/Application.hpp"
#include "../utilities/core/Assert.hpp"

#include "../utilities/idd/IddFile.hpp"
#include "../utilities/idd/IddObject.hpp"
#include <utilities/idd/IddFactory.hxx>

#include <QLabel>
#include <QIcon>
#include <QListWidget>
#include <QTimer>
#include <QStackedWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QSplitter>
#include <QSpacerItem>
#include <QShowEvent>
#include <QCloseEvent>
#include <QSettings>

using namespace openstudio;
using namespace openstudio::model;

InspectorDialog::InspectorDialog(InspectorDialogClient client, QWidget * parent)
  : QMainWindow(parent),
    m_inspectorGadget(nullptr),
    m_workspaceChanged(false),
    m_workspaceObjectAdded(false),
    m_workspaceObjectRemoved(false)
{
  init(client);
}

InspectorDialog::InspectorDialog(openstudio::model::Model& model,
                                 InspectorDialogClient client,
                                 QWidget * parent)
  : QMainWindow(parent),
    m_inspectorGadget(nullptr),
    m_model(model),
    m_workspaceChanged(false),
    m_workspaceObjectAdded(false),
    m_workspaceObjectRemoved(false)
{
  init(client);
}

InspectorDialog::~InspectorDialog()
{
}

openstudio::IddObjectType InspectorDialog::iddObjectType() const
{
  return m_iddObjectType;
}

bool InspectorDialog::setIddObjectType(const openstudio::IddObjectType& iddObjectType, bool force)
{
  if ((iddObjectType == m_iddObjectType) && !force){
    return true;
  }

  if (m_typesToDisplay.find(iddObjectType) == m_typesToDisplay.end()){
    return false;
  }

  m_iddObjectType = iddObjectType;

  // ensure that list widget has iddObjectType selected
  m_listWidget->setUpdatesEnabled(false);
  m_listWidget->blockSignals(true);

  m_listWidget->setSortingEnabled(false);
  m_listWidget->clearSelection();

  for (int i = 0; i < m_listWidget->count(); ++i){

    QVariant data = m_listWidget->item(i)->data(Qt::UserRole);
    if (!data.isValid()){
      continue;
    }

    if (data.toInt() == iddObjectType.value()){
      //select this row
      m_listWidget->setCurrentRow(i);
      break;
    }
  }

  m_listWidget->blockSignals(false);
  m_listWidget->setUpdatesEnabled(true);

  // update the object list in the table view
  m_selectionLabel->setText(iddObjectType.valueDescription().c_str());
  loadTableWidgetData();

  // set the selection
  std::vector<Handle> selectedObjectHandles;
  if (!m_objectHandles.empty()){
    selectedObjectHandles.push_back(m_objectHandles[0]);
  }
  setSelectedObjectHandles(selectedObjectHandles, true);

  // determine if the selection widget should be hidden
  bool disabledType = true;
  if (m_disableSelectionTypes.find(iddObjectType) == m_disableSelectionTypes.end()){
    disabledType = false;
  }

  hideSelectionWidget(disabledType);

  emit onIddObjectTypeChanged(m_iddObjectType);

  return true;
}

std::vector<openstudio::Handle> InspectorDialog::selectedObjectHandles() const
{
  return m_selectedObjectHandles;
}

bool InspectorDialog::setSelectedObjectHandles(const std::vector<openstudio::Handle>& selectedObjectHandles, bool force)
{
  // temporary disable multi select
  if (selectedObjectHandles.size() > 1){
    return false;
  }

  // see if same selection
  if (m_selectedObjectHandles.size() == selectedObjectHandles.size()){
    bool allMatch = true;
    for (unsigned i = 0; i < m_selectedObjectHandles.size(); ++i){
      if (m_selectedObjectHandles[i] != selectedObjectHandles[i]){
        allMatch = false;
        break;
      }
    }
    if (allMatch && !force){
      return true;
    }
  }

  // check if selection is ok
  for (Handle handle : selectedObjectHandles){
    boost::optional<WorkspaceObject> object = m_model.getObject(handle);
    if (!object || (object->iddObject().type() != m_iddObjectType)){
      return false;
    }
  }

  m_selectedObjectHandles = selectedObjectHandles;

  // update table widget
  m_tableWidget->setUpdatesEnabled(false);
  m_tableWidget->blockSignals(true);

  m_tableWidget->setSortingEnabled(false);
  m_tableWidget->clearSelection();

  for (int i = 0; i < m_tableWidget->rowCount(); ++i){

    QString handleString = m_tableWidget->item(i,0)->data(Qt::UserRole).toString();
    //std::string temp = toString(handleString);
    Handle handle(toUUID(handleString));

    auto it = std::find(m_selectedObjectHandles.begin(),
                        m_selectedObjectHandles.end(),
                        handle);
    if (it != m_selectedObjectHandles.end()){
      //select this row
      m_tableWidget->selectRow(i);
    }
  }

  m_tableWidget->setSortingEnabled(true);

  m_tableWidget->blockSignals(false);
  m_tableWidget->setUpdatesEnabled(true);

  // update inspector gadget
  if (m_selectedObjectHandles.empty()){
    m_inspectorGadget->clear(true);
  }else if (m_selectedObjectHandles.size() == 1){
    boost::optional<WorkspaceObject> workspaceObject = m_model.getObject(m_selectedObjectHandles[0]);
    OS_ASSERT(workspaceObject);
    m_inspectorGadget->layoutModelObj(*workspaceObject,false,false,false,true);
    m_inspectorGadget->createAllFields();
  }else{
    // temporary do not allow multi select
    OS_ASSERT(false);
  }

  bool enableAdd = (m_disableAddTypes.find(m_iddObjectType) == m_disableAddTypes.end());
  bool enableCopy = (m_disableCopyTypes.find(m_iddObjectType) == m_disableCopyTypes.end());
  bool enableRemove = (m_disableRemoveTypes.find(m_iddObjectType) == m_disableRemoveTypes.end());
  bool enablePurge = (m_disablePurgeTypes.find(m_iddObjectType) == m_disablePurgeTypes.end());

  boost::optional<IddObject> iddObject = IddFactory::instance().getObject(m_iddObjectType);
  OS_ASSERT(iddObject);

  if (iddObject->properties().unique){
    enableCopy = false;
    enablePurge = false;
    if (m_objectHandles.empty()){
      enableRemove = false;
    }else{
      enableAdd = false;
      if (iddObject->properties().required){
        enableRemove = false;
      }
    }
  }else{
    if (m_selectedObjectHandles.empty()){
      enableCopy = false;
      enableRemove = false;
    }else if (m_selectedObjectHandles.size() == 1){
      // no-op
    }else{
      enableCopy = false;
    }
  }

  std::vector<WorkspaceObject> objects = m_model.getObjectsByType(m_iddObjectType);
  if (objects.empty()){
    enablePurge = false;
  }else if(!objects[0].optionalCast<ResourceObject>()){
    enablePurge = false;
  }

  // update push buttons
  m_pushButtonNew->setEnabled(enableAdd);
  m_pushButtonCopy->setEnabled(enableCopy);
  m_pushButtonDelete->setEnabled(enableRemove);
  m_pushButtonPurge->setEnabled(enablePurge);

  emit selectedObjectHandlesChanged(m_selectedObjectHandles);

  return true;
}

openstudio::model::Model InspectorDialog::model() const
{
  return m_model;
}

void InspectorDialog::setModel(openstudio::model::Model& model, bool force)
{
  if ((model == m_model) && !force){
    return;
  }

  // change model
  m_model = model;

  // connect signals to the new model
  this->connectModelSignalsAndSlots();

  setIddObjectType(m_iddObjectType, true);

  updateListWidgetData();

  emit modelChanged(m_model);

  /// \todo For some reason, after InspectorDialog::setModel is called the keyboard focus
  ///       gets "lost" on MacOS After several hours of trial and error, I found that setting the focus
  ///       programmatically in this out-of-band call consistently resolves the problem.
  ///       The "todo" is to reevaluate with a later version of Qt.
  QTimer::singleShot(0, this, SLOT(setFocus()));
}

void InspectorDialog::rebuildInspectorGadget(bool recursive)
{
  m_inspectorGadget->rebuild(recursive);
}

void InspectorDialog::saveState()
{
  QString organizationName("OpenStudio");
  QString applicationName("InspectorDialog");
  QSettings settings(organizationName, applicationName);
  settings.setValue("Geometry", QMainWindow::saveGeometry());
  settings.setValue("State", QMainWindow::saveState());
}

void InspectorDialog::restoreState()
{
  QString organizationName("OpenStudio");
  QString applicationName("InspectorDialog");
  QSettings settings(organizationName, applicationName);
  QMainWindow::restoreGeometry(settings.value("Geometry").toByteArray());
  QMainWindow::restoreState(settings.value("State").toByteArray());
}

void InspectorDialog::onIddObjectTypeChanged(const openstudio::IddObjectType& iddObjectType)
{
}

void InspectorDialog::onSelectedObjectHandlesChanged(const std::vector<openstudio::Handle>& selectedObjectHandles)
{
}

void InspectorDialog::onModelChanged(Model&)
{
}

void InspectorDialog::showEvent(QShowEvent* event)
{
  //restoreState();
  QWidget::showEvent(event);
}

void InspectorDialog::closeEvent(QCloseEvent* event)
{
  saveState();
  QWidget::closeEvent(event);
}

void InspectorDialog::onPushButtonNew(bool)
{
  boost::optional<WorkspaceObject> object = m_model.addObject(IdfObject(m_iddObjectType));

  if (object){
    m_selectedObjectHandles.clear();
    m_selectedObjectHandles.push_back(object->handle());
    //setSelectedObjectHandles(m_selectedObjectHandles, true);
  }
}

void InspectorDialog::onPushButtonCopy(bool)
{
  if (m_selectedObjectHandles.size() == 1){
    boost::optional<WorkspaceObject> workspaceObject = m_model.getObject(m_selectedObjectHandles[0]);
    if (workspaceObject){
      ModelObject object = workspaceObject->cast<ModelObject>().clone(m_model);
      m_selectedObjectHandles.clear();
      m_selectedObjectHandles.push_back(object.handle());
      //setSelectedObjectHandles(m_selectedObjectHandles, true);
    }
  }
}

void InspectorDialog::onPushButtonDelete(bool)
{
  std::vector<Handle> handles = m_selectedObjectHandles;
  for (Handle handle : handles){
    boost::optional<WorkspaceObject> object = m_model.getObject(handle);
    if (object){
      // calls model object remove
      object->remove();
    }
  }
}

void InspectorDialog::onPushButtonPurge(bool)
{
  m_model.purgeUnusedResourceObjects(m_iddObjectType);
}

/*
void InspectorDialog::onCheckBox(bool checked)
{
  std::vector<openstudio::WorkspaceObject> selectedObjects;
  std::vector<openstudio::Handle> selectedHandles;
  getTableWidgetSelected(selectedObjects,selectedHandles);
  if(!selectedObjects.size()) return;
  emit tableWidgetSelectionChanged(selectedHandles);
  m_inspectorGadget->layoutModelObj(selectedObjects[0]);
  // TODO use this call when IG multi-select completed
  //m_inspectorGadget->layoutModelObjs(selectedObjects);
}
*/

void InspectorDialog::onListWidgetSelectionChanged()
{
  QList<QListWidgetItem *> selectedItems = m_listWidget->selectedItems();

  // One row must be selected
  if(selectedItems.count() == 1){

    //m_tableWidget->setSortingEnabled(false);

    QVariant data = selectedItems.at(0)->data(Qt::UserRole);
    if (data.isValid()){
      IddObjectType iddObjectType(data.toInt());
      setIddObjectType(iddObjectType);
    }

    // do not enable sorting
    //m_tableWidget->setSortingEnabled(true);
  }

}

void InspectorDialog::onTableWidgetSelectionChanged()
{
  std::vector<openstudio::Handle> selectedObjectHandles;
  getTableWidgetSelected(selectedObjectHandles);
  setSelectedObjectHandles(selectedObjectHandles, true);
}

void InspectorDialog::onAddWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> impl, const openstudio::IddObjectType& type, const openstudio::UUID& uuid)
{
  m_workspaceObjectAdded = true;
  m_workspaceChanged = true;

  if (impl->iddObject().type() == m_iddObjectType){

    m_objectHandles.push_back(impl->handle());

    if (m_selectedObjectHandles.empty()){
      m_selectedObjectHandles.push_back(impl->handle());
    }else{
      // do we want to do this or preserve the current selection?
      // this functionality is now in onPushButtonNew
      //m_selectedObjectHandles.clear();
      //m_selectedObjectHandles.push_back(impl->handle());
      //setSelectedObjectHandles(m_selectedObjectHandles, true);
    }
  }

  QTimer::singleShot(0,this,SLOT(onTimeout()));

}

void InspectorDialog::onWorkspaceChange()
{
  m_workspaceChanged = true;

  QTimer::singleShot(0,this,SLOT(onTimeout()));
}

void InspectorDialog::onTimeout()
{
  if (m_workspaceObjectAdded){
    updateListWidgetData();
    m_workspaceObjectAdded = false;
  }

  if (m_workspaceObjectRemoved){
    updateListWidgetData();
    m_workspaceObjectRemoved = false;
  }

  if (m_workspaceChanged){
    loadTableWidgetData();
    setSelectedObjectHandles(m_selectedObjectHandles, true);
    m_workspaceChanged = false;
  }
}

void InspectorDialog::onRemoveWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> impl, const openstudio::IddObjectType& type, const openstudio::UUID& uuid)
{
  m_workspaceObjectRemoved = true;
  m_workspaceChanged = true;

  // if removed object is of current type
  if (impl->iddObject().type() == m_iddObjectType){

    auto it = std::remove(m_objectHandles.begin(),m_objectHandles.end(),impl->handle());
    if (it != m_objectHandles.end()){
      m_objectHandles.erase(it, m_objectHandles.end());
    }

    it = std::remove(m_selectedObjectHandles.begin(),m_selectedObjectHandles.end(),impl->handle());
    if (it != m_selectedObjectHandles.end()){
      m_selectedObjectHandles.erase(it, m_selectedObjectHandles.end());
    }

    if (m_selectedObjectHandles.empty()){
      if (!m_objectHandles.empty()){
        m_selectedObjectHandles.push_back(m_objectHandles[0]);
      }
    }
  }

  QTimer::singleShot(0,this,SLOT(onTimeout()));
}

void InspectorDialog::init(InspectorDialogClient client)
{

  QFile sketchUpPluginPolicy(":/SketchUpPluginPolicy.xml");

  switch (client.value()){
    case InspectorDialogClient::AllOpenStudio:

      m_iddFile = IddFactory::instance().getIddFile(IddFileType::OpenStudio);

      // everything is allowable
      for (IddObject iddObject : m_iddFile.objects()){
        m_typesToDisplay.insert(iddObject.type());
      }

      m_iddObjectType = IddObjectType::OS_Building;

      break;
    case InspectorDialogClient::SketchUpPlugin:

      openstudio::model::AccessPolicyStore::Instance().loadFile(sketchUpPluginPolicy.readAll());

      m_iddFile = IddFactory::instance().getIddFile(IddFileType::OpenStudio);

      // TYPES TO DISPLAY

      //m_typesToDisplay.insert(IddObjectType::OS_Version);

      //m_typesToDisplay.insert(IddObjectType::OS_ConvergenceLimits);
      //m_typesToDisplay.insert(IddObjectType::OS_HeatBalanceAlgorithm);
      //m_typesToDisplay.insert(IddObjectType::OS_RunPeriod);
      //m_typesToDisplay.insert(IddObjectType::OS_RunPeriodControl_DaylightSavingTime);
      //m_typesToDisplay.insert(IddObjectType::OS_RunPeriodControl_SpecialDays);
      //m_typesToDisplay.insert(IddObjectType::OS_ShadowCalculation);
      //m_typesToDisplay.insert(IddObjectType::OS_SimulationControl);
      //m_typesToDisplay.insert(IddObjectType::OS_Sizing_Parameters);
      //m_typesToDisplay.insert(IddObjectType::OS_SurfaceConvectionAlgorithm_Inside);
      //m_typesToDisplay.insert(IddObjectType::OS_SurfaceConvectionAlgorithm_Outside);
      //m_typesToDisplay.insert(IddObjectType::OS_Timestep);
      //m_typesToDisplay.insert(IddObjectType::OS_ZoneAirContaminantBalance);
      //m_typesToDisplay.insert(IddObjectType::OS_ZoneAirHeatBalanceAlgorithm);
      //m_typesToDisplay.insert(IddObjectType::OS_ZoneCapacitanceMultiplier_ResearchSpecial);

      //m_typesToDisplay.insert(IddObjectType::OS_Site);
      //m_typesToDisplay.insert(IddObjectType::OS_Site_GroundReflectance);
      //m_typesToDisplay.insert(IddObjectType::OS_Site_GroundTemperature_BuildingSurface);
      //m_typesToDisplay.insert(IddObjectType::OS_Site_WaterMainsTemperature);
      //m_typesToDisplay.insert(IddObjectType::OS_SizingPeriod_DesignDay);
      //m_typesToDisplay.insert(IddObjectType::OS_SizingPeriod_WeatherFileConditionType);
      //m_typesToDisplay.insert(IddObjectType::OS_SizingPeriod_WeatherFileDays);
      //m_typesToDisplay.insert(IddObjectType::OS_WeatherFile);
      //m_typesToDisplay.insert(IddObjectType::OS_WeatherProperty_SkyTemperature);

      m_typesToDisplay.insert(IddObjectType::OS_BuildingStory);
      m_typesToDisplay.insert(IddObjectType::OS_DefaultConstructionSet);
      m_typesToDisplay.insert(IddObjectType::OS_DefaultScheduleSet);
      m_typesToDisplay.insert(IddObjectType::OS_DefaultSurfaceConstructions);
      m_typesToDisplay.insert(IddObjectType::OS_DefaultSubSurfaceConstructions);
      m_typesToDisplay.insert(IddObjectType::OS_Rendering_Color);
      //m_typesToDisplay.insert(IddObjectType::OS_DesignSpecification_OutdoorAir);
      m_typesToDisplay.insert(IddObjectType::OS_SpaceType);
      m_typesToDisplay.insert(IddObjectType::OS_ShadingControl);
      m_typesToDisplay.insert(IddObjectType::OS_WindowProperty_FrameAndDivider);

      //m_typesToDisplay.insert(IddObjectType::OS_Material);
      //m_typesToDisplay.insert(IddObjectType::OS_Material_AirGap);
      //m_typesToDisplay.insert(IddObjectType::OS_Material_AirWall);
      //m_typesToDisplay.insert(IddObjectType::OS_Material_InfraredTransparent);
      //m_typesToDisplay.insert(IddObjectType::OS_Material_NoMass);
      //m_typesToDisplay.insert(IddObjectType::OS_Material_RoofVegetation);
      //m_typesToDisplay.insert(IddObjectType::OS_WindowMaterial_Blind);
      //m_typesToDisplay.insert(IddObjectType::OS_WindowMaterial_DaylightRedirectionDevice);
      //m_typesToDisplay.insert(IddObjectType::OS_WindowMaterial_Gas);
      //m_typesToDisplay.insert(IddObjectType::OS_WindowMaterial_GasMixture);
      //m_typesToDisplay.insert(IddObjectType::OS_WindowMaterial_Glazing);
      //m_typesToDisplay.insert(IddObjectType::OS_WindowMaterial_GlazingGroup_Thermochromic);
      //m_typesToDisplay.insert(IddObjectType::OS_WindowMaterial_Glazing_RefractionExtinctionMethod);
      //m_typesToDisplay.insert(IddObjectType::OS_WindowMaterial_Screen);
      //m_typesToDisplay.insert(IddObjectType::OS_WindowMaterial_Shade);
      //m_typesToDisplay.insert(IddObjectType::OS_WindowMaterial_SimpleGlazingSystem);

      //m_typesToDisplay.insert(IddObjectType::OS_Construction);
      //m_typesToDisplay.insert(IddObjectType::OS_Construction_CfactorUndergroundWall);
      //m_typesToDisplay.insert(IddObjectType::OS_Construction_FfactorGroundFloor);
      //m_typesToDisplay.insert(IddObjectType::OS_Construction_InternalSource);
      //m_typesToDisplay.insert(IddObjectType::OS_Construction_WindowDataFile);
      //m_typesToDisplay.insert(IddObjectType::OS_Construction_InternalSource);

      //m_typesToDisplay.insert(IddObjectType::OS_Schedule_Compact);
      //m_typesToDisplay.insert(IddObjectType::OS_Schedule_Day);
      //m_typesToDisplay.insert(IddObjectType::OS_Schedule_Week);
      //m_typesToDisplay.insert(IddObjectType::OS_Schedule_Year);
      //m_typesToDisplay.insert(IddObjectType::OS_Schedule_FixedInterval);
      //m_typesToDisplay.insert(IddObjectType::OS_Schedule_VariableInterval);
      //m_typesToDisplay.insert(IddObjectType::OS_ScheduleTypeLimits);

      //m_typesToDisplay.insert(IddObjectType::OS_ElectricEquipment_Definition);
      //m_typesToDisplay.insert(IddObjectType::OS_GasEquipment_Definition);
      //m_typesToDisplay.insert(IddObjectType::OS_HotWaterEquipment_Definition);
      //m_typesToDisplay.insert(IddObjectType::OS_SteamEquipment_Definition);
      //m_typesToDisplay.insert(IddObjectType::OS_OtherEquipment_Definition);
      //m_typesToDisplay.insert(IddObjectType::OS_Lights_Definition);
      //m_typesToDisplay.insert(IddObjectType::OS_Luminaire_Definition);
      //m_typesToDisplay.insert(IddObjectType::OS_People_Definition);
      //m_typesToDisplay.insert(IddObjectType::OS_InternalMass_Definition);

      m_typesToDisplay.insert(IddObjectType::OS_Building);
      //m_typesToDisplay.insert(IddObjectType::OS_DaylightingDevice_Shelf);
      m_typesToDisplay.insert(IddObjectType::OS_Facility);
      m_typesToDisplay.insert(IddObjectType::OS_InteriorPartitionSurfaceGroup);
      m_typesToDisplay.insert(IddObjectType::OS_InteriorPartitionSurface);
      m_typesToDisplay.insert(IddObjectType::OS_ShadingSurfaceGroup);
      m_typesToDisplay.insert(IddObjectType::OS_ShadingSurface);
      m_typesToDisplay.insert(IddObjectType::OS_Space);
      m_typesToDisplay.insert(IddObjectType::OS_Surface);
      m_typesToDisplay.insert(IddObjectType::OS_SubSurface);

      //m_typesToDisplay.insert(IddObjectType::OS_ElectricEquipment);
      //m_typesToDisplay.insert(IddObjectType::OS_GasEquipment);
      //m_typesToDisplay.insert(IddObjectType::OS_HotWaterEquipment);
      //m_typesToDisplay.insert(IddObjectType::OS_SteamEquipment);
      //m_typesToDisplay.insert(IddObjectType::OS_OtherEquipment);
      //m_typesToDisplay.insert(IddObjectType::OS_Lights);
      //m_typesToDisplay.insert(IddObjectType::OS_Luminaire);
      //m_typesToDisplay.insert(IddObjectType::OS_People);
      //m_typesToDisplay.insert(IddObjectType::OS_SpaceInfiltration_DesignFlowRate);
      //m_typesToDisplay.insert(IddObjectType::OS_SpaceInfiltration_EffectiveLeakageArea);
      //m_typesToDisplay.insert(IddObjectType::OS_InternalMass);

      m_typesToDisplay.insert(IddObjectType::OS_Daylighting_Control);
      m_typesToDisplay.insert(IddObjectType::OS_IlluminanceMap);
      m_typesToDisplay.insert(IddObjectType::OS_Glare_Sensor);
      //m_typesToDisplay.insert(IddObjectType::OS_LightingDesignDay);
      //m_typesToDisplay.insert(IddObjectType::OS_LightingSimulationControl);
      //m_typesToDisplay.insert(IddObjectType::OS_LightingSimulationZone);

      m_typesToDisplay.insert(IddObjectType::OS_ThermalZone);
      //m_typesToDisplay.insert(IddObjectType::OS_ThermostatSetpoint_DualSetpoint);

      //m_typesToDisplay.insert(IddObjectType::OS_Meter);
      //m_typesToDisplay.insert(IddObjectType::OS_Output_Variable);

      // DISABLE ADD

      m_disableAddTypes.insert(IddObjectType::OS_RunPeriod);
      m_disableAddTypes.insert(IddObjectType::OS_Site);
      m_disableAddTypes.insert(IddObjectType::OS_WeatherFile);
      m_disableAddTypes.insert(IddObjectType::OS_ShadingControl);
      m_disableAddTypes.insert(IddObjectType::OS_InteriorPartitionSurface);
      m_disableAddTypes.insert(IddObjectType::OS_InteriorPartitionSurfaceGroup);
      m_disableAddTypes.insert(IddObjectType::OS_ShadingSurface);
      m_disableAddTypes.insert(IddObjectType::OS_ShadingSurfaceGroup);
      m_disableAddTypes.insert(IddObjectType::OS_Space);
      m_disableAddTypes.insert(IddObjectType::OS_Surface);
      m_disableAddTypes.insert(IddObjectType::OS_SubSurface);
      m_disableAddTypes.insert(IddObjectType::OS_Daylighting_Control);
      m_disableAddTypes.insert(IddObjectType::OS_IlluminanceMap);
      m_disableAddTypes.insert(IddObjectType::OS_Luminaire);
      m_disableAddTypes.insert(IddObjectType::OS_Glare_Sensor);
      m_disableAddTypes.insert(IddObjectType::OS_ThermalZone);

      // DISABLE COPY

      m_disableCopyTypes.insert(IddObjectType::OS_RunPeriod);
      m_disableCopyTypes.insert(IddObjectType::OS_Site);
      m_disableCopyTypes.insert(IddObjectType::OS_WeatherFile);
      m_disableCopyTypes.insert(IddObjectType::OS_InteriorPartitionSurface);
      m_disableCopyTypes.insert(IddObjectType::OS_InteriorPartitionSurfaceGroup);
      m_disableCopyTypes.insert(IddObjectType::OS_ShadingSurface);
      m_disableCopyTypes.insert(IddObjectType::OS_ShadingSurfaceGroup);
      m_disableCopyTypes.insert(IddObjectType::OS_Space);
      m_disableCopyTypes.insert(IddObjectType::OS_Surface);
      m_disableCopyTypes.insert(IddObjectType::OS_SubSurface);
      m_disableCopyTypes.insert(IddObjectType::OS_Daylighting_Control);
      m_disableCopyTypes.insert(IddObjectType::OS_IlluminanceMap);
      m_disableCopyTypes.insert(IddObjectType::OS_Luminaire);
      m_disableCopyTypes.insert(IddObjectType::OS_Glare_Sensor);
      m_disableCopyTypes.insert(IddObjectType::OS_ThermalZone);

      // DISABLE REMOVE

      m_disableRemoveTypes.insert(IddObjectType::OS_RunPeriod);
      m_disableRemoveTypes.insert(IddObjectType::OS_SimulationControl);
      m_disableRemoveTypes.insert(IddObjectType::OS_Site);
      m_disableRemoveTypes.insert(IddObjectType::OS_WeatherFile);
      m_disableRemoveTypes.insert(IddObjectType::OS_InteriorPartitionSurface);
      m_disableRemoveTypes.insert(IddObjectType::OS_InteriorPartitionSurfaceGroup);
      m_disableRemoveTypes.insert(IddObjectType::OS_ShadingSurface);
      m_disableRemoveTypes.insert(IddObjectType::OS_ShadingSurfaceGroup);
      m_disableRemoveTypes.insert(IddObjectType::OS_Space);
      m_disableRemoveTypes.insert(IddObjectType::OS_Surface);
      m_disableRemoveTypes.insert(IddObjectType::OS_SubSurface);
      m_disableRemoveTypes.insert(IddObjectType::OS_Daylighting_Control);
      m_disableRemoveTypes.insert(IddObjectType::OS_IlluminanceMap);
      m_disableRemoveTypes.insert(IddObjectType::OS_Luminaire);
      m_disableRemoveTypes.insert(IddObjectType::OS_Glare_Sensor);
      //m_disableRemoveTypes.insert(IddObjectType::OS_ThermalZone); // DLM: continue to allow this for now

      // DISABLE PURGE

      //m_disablePurgeTypes.insert(IddObjectType::OS_RunPeriod);

      // INITIAL SELECTION

      m_iddObjectType = IddObjectType::OS_Building;

      break;
    default:
      break;
  }

  OS_ASSERT(!m_typesToDisplay.empty());

  Qt::WindowFlags flags = Qt::Dialog | Qt::WindowMinMaxButtonsHint | Qt::WindowCloseButtonHint; // | Qt::WindowStaysOnTopHint;
  this->setWindowFlags(flags);

  setWindowIcon(QIcon(":/images/me_16.png"));
  setWindowIconText(tr("OpenStudio Inspector"));
  setWindowTitle(tr("OpenStudio Inspector"));
  createWidgets();
  loadStyleSheet();
  connectSelfSignalsAndSlots();
  loadListWidgetData();
  setModel(m_model, true);
}

void InspectorDialog::createWidgets()
{
  /// The list widget

  QFont labelFont;
  labelFont.setPointSize(12);
  //labelFont.setBold(true);

  QFont subLabelFont;
  //subLabelFont.setPointSize(12);
  subLabelFont.setBold(true);

  auto listLabel = new QLabel(this);
  listLabel->setObjectName("listLabel");
  listLabel->setText("Select Type");
  listLabel->setFont(labelFont);
  listLabel->setMinimumHeight(40);
  listLabel->setMaximumHeight(40);

  m_listWidget = new QListWidget(this);
  m_listWidget->setObjectName("listWidget");
  //m_listWidget->setAlternatingRowColors(true);
  m_listWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_listWidget->setSelectionMode(QAbstractItemView::SingleSelection);
  m_listWidget->setAcceptDrops(false);
  m_listWidget->setDragEnabled(false);

  auto listHolderLayout = new QVBoxLayout;
  listHolderLayout->addWidget(listLabel);
  listHolderLayout->addWidget(m_listWidget);

  auto listHolderWidget = new QWidget(this);
  listHolderWidget->setLayout(listHolderLayout);

  /// The table widget
  auto tableLabel = new QLabel(this);
  tableLabel->setObjectName("tableLabel");
  tableLabel->setText("Select Object");
  tableLabel->setFont(labelFont);
  tableLabel->setMinimumHeight(40);
  tableLabel->setMaximumHeight(40);

  m_selectionLabel = new QLabel(this);
  //m_selectionLabel->setObjectName("selectionLabel");
  m_selectionLabel->setText("");
  m_selectionLabel->setFont(subLabelFont);
  //m_selectionLabel->setMinimumHeight(40);
  //m_selectionLabel->setMaximumHeight(40);

  QStringList headerLabels;
  headerLabels.append("Name");
  headerLabels.append("Comment");

  m_tableWidget = new QTableWidget(this);
  m_tableWidget->setObjectName("tableWidget");
  m_tableWidget->setRowCount(0);
  m_tableWidget->setColumnCount(2);
  m_tableWidget->sortByColumn (0, Qt::AscendingOrder);
  m_tableWidget->setSortingEnabled(false);
  m_tableWidget->setAlternatingRowColors(true);
  m_tableWidget->setShowGrid(false);
  m_tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
  m_tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
  m_tableWidget->verticalHeader()->hide();
  m_tableWidget->setHorizontalHeaderLabels(headerLabels);
  m_tableWidget->horizontalHeader()->setStretchLastSection(true);
  m_tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
  m_tableWidget->setAcceptDrops(false);
  m_tableWidget->setDragEnabled(false);

  m_pushButtonNew = new QPushButton(this);
  m_pushButtonNew->setObjectName("pushButtonNew");
  m_pushButtonNew->setToolTip(tr("Add new object"));

  m_pushButtonCopy = new QPushButton(this);
  m_pushButtonCopy->setObjectName("pushButtonCopy");
  m_pushButtonCopy->setToolTip(tr("Copy selected object"));

  m_pushButtonDelete = new QPushButton(this);
  m_pushButtonDelete->setObjectName("pushButtonDelete");
  m_pushButtonDelete->setToolTip(tr("Remove selected objects"));

  m_pushButtonPurge = new QPushButton(this);
  m_pushButtonPurge->setObjectName("pushButtonPurge");
  m_pushButtonPurge->setToolTip(tr("Purge unused objects"));

  auto buttonLayout = new QHBoxLayout;
  buttonLayout->addSpacing(5);
  buttonLayout->addWidget(m_pushButtonNew);
  buttonLayout->addSpacing(5);
  buttonLayout->addWidget(m_pushButtonCopy);
  buttonLayout->addSpacing(5);
  buttonLayout->addWidget(m_pushButtonDelete);
  buttonLayout->addSpacing(5);
  buttonLayout->addWidget(m_pushButtonPurge);
  buttonLayout->addStretch(0);

  auto buttonGroup = new QWidget(this);
  buttonGroup->setLayout(buttonLayout);

  auto tableVBoxLayout = new QVBoxLayout;
  tableVBoxLayout->addWidget(m_tableWidget);
  tableVBoxLayout->addWidget(buttonGroup);

  auto tableWidgetHolder = new QWidget(this);
  tableWidgetHolder->setLayout(tableVBoxLayout);

  auto noSelectionImage = new QLabel(this);
  noSelectionImage->setPixmap(QPixmap(":/images/alert_image.png"));

  QLabel * noSelectionLabel = new QLabel("Pick your selection in SketchUp.", this);
  noSelectionLabel->setFont(labelFont);
  noSelectionLabel->setMinimumHeight(40);
  noSelectionLabel->setAlignment(Qt::AlignCenter);
  noSelectionLabel->setWordWrap(true);

  auto noSelectionLayout = new QVBoxLayout;
  noSelectionLayout->addStretch();
  noSelectionLayout->addWidget(noSelectionImage,0,Qt::AlignCenter);
  noSelectionLayout->addWidget(noSelectionLabel,0,Qt::AlignCenter);
  noSelectionLayout->addStretch();

  auto noSelectionWidget = new QWidget(this);
  noSelectionWidget->setLayout(noSelectionLayout);

  m_stackedWidget = new QStackedWidget(this);
  m_stackedWidget->addWidget(tableWidgetHolder);
  m_stackedWidget->addWidget(noSelectionWidget);

  auto tableHolderLayout = new QVBoxLayout;
  tableHolderLayout->addWidget(tableLabel);
  tableHolderLayout->addWidget(m_selectionLabel);
  tableHolderLayout->addWidget(m_stackedWidget);

  auto tableHolderWidget = new QWidget(this);
  tableHolderWidget->setLayout(tableHolderLayout);

  // The inspectorGadget widget

  auto inspectorGadgetLabel = new QLabel(this);
  inspectorGadgetLabel->setObjectName("igLabel");
  inspectorGadgetLabel->setText("Edit Object");
  inspectorGadgetLabel->setFont(labelFont);
  inspectorGadgetLabel->setMinimumHeight(40);
  inspectorGadgetLabel->setMaximumHeight(40);

  m_inspectorGadget = new InspectorGadget(this);
  connect(this, &InspectorDialog::toggleUnitsClicked, m_inspectorGadget, &InspectorGadget::toggleUnitsClicked);

  auto inspectorGadgetHolderLayout = new QVBoxLayout;
  inspectorGadgetHolderLayout->addWidget(inspectorGadgetLabel);
  inspectorGadgetHolderLayout->addWidget(m_inspectorGadget);

  auto inspectorGadgetHolderWidget = new QWidget(this);
  inspectorGadgetHolderWidget->setLayout(inspectorGadgetHolderLayout);

  // The left widget

  auto leftSplitter = new QSplitter(this);
  leftSplitter->setOrientation(Qt::Vertical);
  leftSplitter->addWidget(listHolderWidget);
  leftSplitter->addWidget(tableHolderWidget);

  // The right widget
/*
  QSplitter* rightSplitter = new QSplitter(this);
  rightSplitter->setOrientation(Qt::Vertical);
  rightSplitter->addWidget(tableHolderWidget);
  rightSplitter->addWidget(inspectorGadgetHolderWidget);
*/
  // The central widget

  auto centralSplitter = new QSplitter(this);
  centralSplitter->setOrientation(Qt::Horizontal);
  centralSplitter->addWidget(leftSplitter);
  centralSplitter->addWidget(inspectorGadgetHolderWidget);

  this->setCentralWidget(centralSplitter);

  hideSelectionWidget(true);
}

void InspectorDialog::connectSelfSignalsAndSlots()
{
  connect(m_pushButtonNew, &QPushButton::clicked, this, &InspectorDialog::onPushButtonNew);

  connect(m_pushButtonCopy, &QPushButton::clicked, this, &InspectorDialog::onPushButtonCopy);

  connect(m_pushButtonDelete, &QPushButton::clicked, this, &InspectorDialog::onPushButtonDelete);

  connect(m_pushButtonPurge, &QPushButton::clicked, this, &InspectorDialog::onPushButtonPurge);

  connect(m_listWidget, &QListWidget::itemSelectionChanged, this, &InspectorDialog::onListWidgetSelectionChanged);

  connect(m_tableWidget, &QTableWidget::itemSelectionChanged, this, &InspectorDialog::onTableWidgetSelectionChanged);

  connect(this, &InspectorDialog::iddObjectTypeChanged, this, &InspectorDialog::onIddObjectTypeChanged);

  connect(this, &InspectorDialog::selectedObjectHandlesChanged, this, &InspectorDialog::onSelectedObjectHandlesChanged);

  connect(this, &InspectorDialog::modelChanged, this, &InspectorDialog::onModelChanged);
}

void InspectorDialog::connectModelSignalsAndSlots()
{
  m_model.getImpl<model::detail::Model_Impl>().get()->addWorkspaceObjectPtr.connect<InspectorDialog, &InspectorDialog::onAddWorkspaceObject>(this);

  m_model.getImpl<model::detail::Model_Impl>().get()->onChange.connect<InspectorDialog, &InspectorDialog::onWorkspaceChange>(this);

  m_model.getImpl<model::detail::Model_Impl>().get()->removeWorkspaceObjectPtr.connect<InspectorDialog, &InspectorDialog::onRemoveWorkspaceObject>(this);
}

void InspectorDialog::hideSelectionWidget(bool hideSelectionWidget)
{
  /// \todo If the code in InspectorDialog::setModel does not properly reset the focus as desired,
  ///       this is the backup plan. After the user clicks on a new section, this hack makes sure
  ///       the keyboard focus is correct. Todo: reevaluate with a new version of Qt in the future.
  /// \sa InspectorDialog::setModel
  setFocus();

  if(hideSelectionWidget){
    m_stackedWidget->setCurrentIndex(1);
  }
  else{
    m_stackedWidget->setCurrentIndex(0);
  }
}

void InspectorDialog::loadStyleSheet()
{
  QFile data(":/InspectorDialog.qss");
  QString style;
  if(data.open(QFile::ReadOnly))
  {
    QTextStream styleIn(&data);
    style = styleIn.readAll();
    data.close();
    setStyleSheet(style);
  }
}

void InspectorDialog::loadListWidgetData()
{
  QFont groupFont;
  //groupFont.setPointSize(12);
  groupFont.setBold(true);

  //backgroundGradient.setColorAt(0.66, QColor(208,212,215));
  //backgroundGradient.setColorAt(1, QColor(107,116,123));
  QBrush groupBackground(QColor(208,212,215));

  QBrush groupForeground(QColor(0,0,0));

  QBrush itemBackground(QColor(255,255,255));

  QBrush itemAlternateBackground(QColor(238,238,238));

  QListWidgetItem* listItem;
  for (const std::string& group : m_iddFile.groups()){

    // quick check if group is empty
    bool empty = true;
    for (const IddObject& iddObject : m_iddFile.getObjectsInGroup(group)){
      if (m_typesToDisplay.find(iddObject.type()) != m_typesToDisplay.end()){
        empty = false;
        break;
      }
    }

    if (empty){
      continue;
    }

    // add the group item
    listItem = new QListWidgetItem();
    listItem->setText(group.c_str());
    listItem->setFlags(Qt::NoItemFlags);
    listItem->setFont(groupFont);
    listItem->setBackground(groupBackground);
    listItem->setForeground(groupForeground);
    m_listWidget->addItem(listItem);

    // add each object
    bool alternate = false;
    for (const IddObject& iddObject : m_iddFile.getObjectsInGroup(group)){

      IddObjectType type = iddObject.type();
      if (m_typesToDisplay.find(type) == m_typesToDisplay.end()){
        continue;
      }

      unsigned numObjects = m_model.numObjectsOfType(type);
      QString text(type.valueDescription().c_str());
      text += QString(" (") + QString::number(numObjects) + QString(")");

      listItem = new QListWidgetItem();
      listItem->setText(text);
      listItem->setFlags(Qt::NoItemFlags | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
      listItem->setData(Qt::UserRole, type.value());

      if (alternate){
        listItem->setBackground(itemAlternateBackground);

      }else{
        listItem->setBackground(itemBackground);
      }

      m_listWidget->addItem(listItem);

      alternate = !alternate;
    }
  }
}

void InspectorDialog::updateListWidgetData()
{
  for (int i = 0; i < m_listWidget->count(); ++i){

    QVariant data = m_listWidget->item(i)->data(Qt::UserRole);
    if (!data.isValid()){
      continue;
    }

    IddObjectType type(data.toInt());
    unsigned numObjects = m_model.numObjectsOfType(type);

    QString text(type.valueDescription().c_str());
    text += QString(" (") + QString::number(numObjects) + QString(")");
    m_listWidget->item(i)->setText(text);
  }


}


void InspectorDialog::loadTableWidgetData()
{
  m_tableWidget->setUpdatesEnabled(false);
  m_tableWidget->blockSignals(true);

  m_tableWidget->clear();
  m_tableWidget->setRowCount(0);
  m_tableWidget->setSortingEnabled(false);

  // clear removes header labels
  QStringList headerLabels;
  headerLabels.append("Name");
  headerLabels.append("Comment");
  m_tableWidget->setHorizontalHeaderLabels(headerLabels);

  // all object handles
  m_objectHandles.clear();

  int i=0, j=0;
  //bool connected;
  //QCheckBox * checkBox;

  std::vector<WorkspaceObject> objects = m_model.getObjectsByType(m_iddObjectType);

  for (WorkspaceObject object : objects){

    m_objectHandles.push_back(object.handle());

    m_tableWidget->insertRow(m_tableWidget->rowCount());

    QString displayName("(No Name)");
    if(object.name()){
      displayName = object.name().get().c_str();
    }
    unsigned numSources = object.numSources();
    displayName += QString(" (") + QString::number(numSources) + QString(")");

    auto tableItem = new QTableWidgetItem(displayName);
    tableItem->setFlags(Qt::NoItemFlags | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    QString handleString(toQString(object.handle()));
    //std::string temp = toString(handleString);
    tableItem->setData(Qt::UserRole, handleString);
    m_tableWidget->setItem(i,j++,tableItem);

    QString description = object.comment().c_str();
    tableItem = new QTableWidgetItem(description);
    tableItem->setFlags(Qt::NoItemFlags | Qt::ItemIsEnabled | Qt::ItemIsSelectable);
    m_tableWidget->setItem(i,j++,tableItem);

    i++;
    j=0;
  }
  m_tableWidget->setSortingEnabled(true);
  m_tableWidget->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
  m_tableWidget->horizontalHeader()->setStretchLastSection(true);
  m_tableWidget->blockSignals(false);
  m_tableWidget->setUpdatesEnabled(true);

}

void InspectorDialog::getTableWidgetSelected(std::vector<openstudio::Handle>& selectedHandles)
{
  m_tableWidget->setSortingEnabled(false);

  selectedHandles.clear();

  QList<QTableWidgetItem *> selectedItems = m_tableWidget->selectedItems();

  for (int i = 0; i < selectedItems.size(); ++i){
    int column = m_tableWidget->column(selectedItems.at(i));
    if (column == 0){
      QString handleString = selectedItems.at(i)->data(Qt::UserRole).toString();
      //std::string temp = toString(handleString);
      selectedHandles.push_back(Handle(toUUID(handleString)));
    }
  }

  m_tableWidget->setSortingEnabled(true);
}

void InspectorDialog::displayIP(const bool displayIP)
{
  if(m_inspectorGadget) m_inspectorGadget->toggleUnits(displayIP);
}
