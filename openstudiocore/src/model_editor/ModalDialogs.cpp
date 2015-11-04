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

#include "ModalDialogs.hpp"

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"
#include "../model/Space.hpp"
#include "../model/Space_Impl.hpp"
#include "../model/ThermalZone.hpp"
#include "../model/ThermalZone_Impl.hpp"
#include "../model/SpaceLoadInstance.hpp"
#include "../model/SpaceLoadInstance_Impl.hpp"
#include "../model/SpaceLoadDefinition.hpp"
#include "../model/SpaceLoadDefinition_Impl.hpp"
#include "../model/ElectricEquipment.hpp"
#include "../model/ElectricEquipmentDefinition.hpp"
#include "../model/GasEquipment.hpp"
#include "../model/GasEquipmentDefinition.hpp"
#include "../model/HotWaterEquipment.hpp"
#include "../model/HotWaterEquipmentDefinition.hpp"
#include "../model/InternalMass.hpp"
#include "../model/InternalMassDefinition.hpp"
#include "../model/Lights.hpp"
#include "../model/LightsDefinition.hpp"
#include "../model/Luminaire.hpp"
#include "../model/LuminaireDefinition.hpp"
#include "../model/People.hpp"
#include "../model/PeopleDefinition.hpp"

#include "../utilities/core/Application.hpp"
#include "../utilities/core/System.hpp"
#include "../utilities/core/Assert.hpp"

#include <utilities/idd/OS_ElectricEquipment_FieldEnums.hxx>
#include <utilities/idd/OS_GasEquipment_FieldEnums.hxx>
#include <utilities/idd/OS_HotWaterEquipment_FieldEnums.hxx>
#include <utilities/idd/OS_InternalMass_FieldEnums.hxx>
#include <utilities/idd/OS_Lights_FieldEnums.hxx>
#include <utilities/idd/OS_Luminaire_FieldEnums.hxx>
#include <utilities/idd/OS_People_FieldEnums.hxx>

#include "../utilities/idd/IddFile.hpp"
#include "../utilities/idd/IddObject.hpp"
#include <utilities/idd/IddFactory.hxx>

#include "../utilities/core/Compare.hpp"

#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIcon>

#include <algorithm>

using namespace openstudio;
using namespace openstudio::model;

ModelObjectSelectorDialog::ModelObjectSelectorDialog(const openstudio::IddObjectType& typeToDisplay,
                                                     const openstudio::model::Model& model,
                                                     QWidget * parent)
  : QDialog(parent), m_model(model)
{
  m_typesToDisplay.push_back(typeToDisplay);

  init();
}

ModelObjectSelectorDialog::ModelObjectSelectorDialog(const std::vector<openstudio::IddObjectType>& typesToDisplay,
                                                     const openstudio::model::Model& model,
                                                     QWidget * parent)
  : QDialog(parent), m_typesToDisplay(typesToDisplay), m_model(model)
{
  init();
}

ModelObjectSelectorDialog::~ModelObjectSelectorDialog()
{
}

boost::optional<openstudio::model::ModelObject> ModelObjectSelectorDialog::selectedModelObject() const
{
  boost::optional<openstudio::model::ModelObject> result;
  int currentIndex = m_comboBox->currentIndex();
  if (currentIndex >= 0){
    QVariant itemData = m_comboBox->itemData(currentIndex);
    OS_ASSERT(itemData.isValid());
    Handle handle(itemData.toString());
    result = m_model.getModelObject<ModelObject>(handle);
  }
  return result;
}

void ModelObjectSelectorDialog::setWindowTitle(const std::string& text)
{
  QDialog::setWindowTitle(toQString(text));
  QDialog::setWindowIconText(toQString(text));
}

void ModelObjectSelectorDialog::setUserText(const std::string& text)
{
  m_userTextLabel->setText(toQString(text));
}

void ModelObjectSelectorDialog::setOKButtonText(const std::string& text)
{
  m_okButton->setText(toQString(text));
  m_okButton->setToolTip(toQString(text));
}

void ModelObjectSelectorDialog::setCancelButtonText(const std::string& text)
{
  m_cancelButton->setText(toQString(text));
  m_cancelButton->setToolTip(toQString(text));
}

void ModelObjectSelectorDialog::onPushButtonOK(bool)
{
  boost::optional<ModelObject> modelObject = selectedModelObject();
  emit closed(modelObject);
}

void ModelObjectSelectorDialog::onPushButtonCancel(bool)
{
  boost::optional<ModelObject> modelObject;
  emit closed(modelObject);
}

void ModelObjectSelectorDialog::onAddWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> impl)
{
  std::vector<openstudio::IddObjectType>::const_iterator it = std::find(m_typesToDisplay.begin(), m_typesToDisplay.end(), impl->iddObject().type());
  if (it != m_typesToDisplay.end()){
    loadComboBoxData();
  }
}

void ModelObjectSelectorDialog::onRemoveWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> impl)
{
  std::vector<openstudio::IddObjectType>::const_iterator it = std::find(m_typesToDisplay.begin(), m_typesToDisplay.end(), impl->iddObject().type());
  if (it != m_typesToDisplay.end()){
    loadComboBoxData();
  }
}

void ModelObjectSelectorDialog::init()
{
  Qt::WindowFlags flags = Qt::Dialog | Qt::WindowStaysOnTopHint;
  this->setWindowFlags(flags);

  setWindowIcon(QIcon(":/images/me_16.png"));
  QDialog::setWindowIconText(tr("Select Model Object"));
  QDialog::setWindowTitle(tr("Select Model Object"));
  createWidgets();
  loadStyleSheet();
  connectSignalsAndSlots();
  loadComboBoxData();
}

void ModelObjectSelectorDialog::createWidgets()
{
  QFont labelFont;
  labelFont.setPointSize(12);
  //labelFont.setBold(true);

  QFont subLabelFont;
  //subLabelFont.setPointSize(12);
  subLabelFont.setBold(true);

  m_userTextLabel = new QLabel(this);
  m_userTextLabel->setObjectName("userTextLabel");
  m_userTextLabel->setText("Select Model Object");
  m_userTextLabel->setWordWrap(true);

  m_comboBox = new QComboBox(this);

  m_okButton = new QPushButton(this);
  m_okButton->setObjectName("okButton");
  m_okButton->setText(tr("OK"));
  m_okButton->setToolTip(tr("OK"));

  m_cancelButton = new QPushButton(this);
  m_cancelButton->setObjectName("cancelButton)");
  m_cancelButton->setText(tr("Cancel"));
  m_cancelButton->setToolTip(tr("Cancel"));

  auto buttonLayout = new QHBoxLayout;
  buttonLayout->addSpacing(5);
  buttonLayout->addWidget(m_okButton);
  buttonLayout->addSpacing(5);
  buttonLayout->addWidget(m_cancelButton);
  buttonLayout->addStretch(0);

  auto buttonGroup = new QWidget(this);
  buttonGroup->setLayout(buttonLayout);

  auto mainLayout = new QVBoxLayout;
  mainLayout->addWidget(m_userTextLabel);
  mainLayout->addWidget(m_comboBox);
  mainLayout->addWidget(buttonGroup);

  this->setLayout(mainLayout);
}

void ModelObjectSelectorDialog::connectSignalsAndSlots()
{
  connect(m_okButton, &QPushButton::clicked, this, &ModelObjectSelectorDialog::onPushButtonOK);

  connect(m_cancelButton, &QPushButton::clicked, this, &ModelObjectSelectorDialog::onPushButtonCancel);

  connect(m_model.getImpl<model::detail::Model_Impl>().get(),
    static_cast<void (model::detail::Model_Impl::*)(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const IddObjectType &, const UUID &) const>(&model::detail::Model_Impl::addWorkspaceObject),
    this,
    &ModelObjectSelectorDialog::onAddWorkspaceObject);

  connect(m_model.getImpl<model::detail::Model_Impl>().get(),
    static_cast<void (model::detail::Model_Impl::*)(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const IddObjectType &, const UUID &) const>(&model::detail::Model_Impl::removeWorkspaceObject),
    this,
    &ModelObjectSelectorDialog::onRemoveWorkspaceObject);
}

void ModelObjectSelectorDialog::loadStyleSheet()
{
  QFile data(":/ModalDialogs.qss");
  QString style;
  if(data.open(QFile::ReadOnly))
  {
    QTextStream styleIn(&data);
    style = styleIn.readAll();
    data.close();
    setStyleSheet(style);
  }
}

// sort by name
struct NameSorter {
  bool operator()(const WorkspaceObject& left, const WorkspaceObject& right) const {
    OS_ASSERT(left.name());
    OS_ASSERT(right.name());
    return (istringLess(left.name().get(), right.name().get()));
  }
};

void ModelObjectSelectorDialog::loadComboBoxData()
{
  m_comboBox->clear();

  std::vector<WorkspaceObject> workspaceObjects;

  // get all objects
  for (IddObjectType iddObjectType : m_typesToDisplay){
    std::vector<WorkspaceObject> temp = m_model.getObjectsByType(iddObjectType);
    workspaceObjects.insert(workspaceObjects.end(), temp.begin(), temp.end());
  }

  NameSorter nameSorter;
  std::sort(workspaceObjects.begin(), workspaceObjects.end(), nameSorter);

  // add to combo box
  for (WorkspaceObject workspaceObject : workspaceObjects){
    OS_ASSERT(workspaceObject.name());
    std::string objectName = workspaceObject.name().get();
    m_comboBox->addItem(toQString(objectName), workspaceObject.handle().toString());
  }

  if (m_comboBox->count() > 0){
    m_comboBox->setCurrentIndex(0);
  }
}


ModelObjectSelectorDialogWatcher::ModelObjectSelectorDialogWatcher(std::shared_ptr<ModelObjectSelectorDialog> modelObjectSelectorDialog)
  : m_modelObjectSelectorDialog(modelObjectSelectorDialog)
{
  OS_ASSERT(modelObjectSelectorDialog);

  connect(modelObjectSelectorDialog.get(), &ModelObjectSelectorDialog::closed, this, &ModelObjectSelectorDialogWatcher::onClose);
}

boost::optional<openstudio::model::ModelObject> ModelObjectSelectorDialogWatcher::selectedModelObject() const
{
  if (m_modelObjectSelectorDialog){
    m_selectedModelObject = m_modelObjectSelectorDialog->selectedModelObject();
  }
  return m_selectedModelObject;
}


bool ModelObjectSelectorDialogWatcher::isSelectionFinal() const
{
  return (!m_modelObjectSelectorDialog);
}

void ModelObjectSelectorDialogWatcher::onClose(const boost::optional<openstudio::model::ModelObject>& selectedModelObject)
{
  m_modelObjectSelectorDialog.reset();
  m_selectedModelObject = selectedModelObject;
}

void ensureThermalZone(openstudio::model::Space& space)
{
  Model model = space.model();

  boost::optional<ThermalZone> thermalZone = space.thermalZone();
  if (thermalZone){
    return;
  }

  IddObjectType thermalZoneType = IddObjectType::OS_ThermalZone;
  boost::optional<ModelObject> selectedModelObject;
  if (!model.getObjectsByType(thermalZoneType).empty()){

    // allow user to select a definition
    std::vector<IddObjectType> typesToDisplay;
    typesToDisplay.push_back(thermalZoneType);

    std::shared_ptr<ModelObjectSelectorDialog> dialog(new ModelObjectSelectorDialog(typesToDisplay, model));
    dialog->setWindowTitle("Select ThermalZone");
    dialog->setUserText("Select an existing ThermalZone or press New to create a new one.");
    dialog->setOKButtonText("Select");
    dialog->setCancelButtonText("New");
    dialog->show();

    ModelObjectSelectorDialogWatcher watcher(dialog);

    while (!watcher.isSelectionFinal()){
      // msleep calls processEvents
      openstudio::System::msleep(1);
    }

    selectedModelObject = watcher.selectedModelObject();
  }

  if (selectedModelObject){
    // user chose an existing thermal zone
    thermalZone = selectedModelObject->optionalCast<ThermalZone>();
    OS_ASSERT(thermalZone);
    bool test = space.setThermalZone(*thermalZone);
    OS_ASSERT(test);
    return;
  }

  // make a new thermal zone
  thermalZone = ThermalZone(model);
  OS_ASSERT(thermalZone);
  bool test = space.setThermalZone(*thermalZone);
  OS_ASSERT(test);

}

void ensureSpaceLoadDefinition(openstudio::model::SpaceLoadInstance& instance)
{
  Model model = instance.model();

  IddObjectType definitionType;
  boost::optional<WorkspaceObject> currentDefinition;

  switch(instance.iddObjectType().value()){
    case IddObjectType::OS_ElectricEquipment:
      currentDefinition = instance.getTarget(OS_ElectricEquipmentFields::ElectricEquipmentDefinitionName);
      definitionType = IddObjectType::OS_ElectricEquipment_Definition;
      break;
    case IddObjectType::OS_GasEquipment:
      currentDefinition = instance.getTarget(OS_GasEquipmentFields::GasEquipmentDefinitionName);
      definitionType = IddObjectType::OS_GasEquipment_Definition;
      break;
    case IddObjectType::OS_HotWaterEquipment:
      currentDefinition = instance.getTarget(OS_HotWaterEquipmentFields::HotWaterEquipmentDefinitionName);
      definitionType = IddObjectType::OS_HotWaterEquipment_Definition;
      break;
    case IddObjectType::OS_InternalMass:
      currentDefinition = instance.getTarget(OS_InternalMassFields::InternalMassDefinitionName);
      definitionType = IddObjectType::OS_InternalMass_Definition;
      break;
    case IddObjectType::OS_Lights:
      currentDefinition = instance.getTarget(OS_LightsFields::LightsDefinitionName);
      definitionType = IddObjectType::OS_Lights_Definition;
      break;
    case IddObjectType::OS_Luminaire:
      currentDefinition = instance.getTarget(OS_LuminaireFields::LuminaireDefinitionName);
      definitionType = IddObjectType::OS_Luminaire_Definition;
      break;
    case IddObjectType::OS_People:
      currentDefinition = instance.getTarget(OS_PeopleFields::PeopleDefinitionName);
      definitionType = IddObjectType::OS_People_Definition;
      break;
    default:
      LOG_FREE_AND_THROW("openstudio.ensureSpaceLoadDefinition", "Unknown IddObjectType " << instance.iddObjectType().valueName());
  }

  if (currentDefinition){
    return;
  }

  boost::optional<SpaceLoadDefinition> definition;

  if (!model.getObjectsByType(definitionType).empty()){

    // allow user to select a definition
    std::vector<IddObjectType> typesToDisplay;
    typesToDisplay.push_back(definitionType);

    std::shared_ptr<ModelObjectSelectorDialog> dialog(new ModelObjectSelectorDialog(typesToDisplay, model));
    dialog->setWindowTitle("Select " + instance.iddObjectType().valueDescription() + "Definition");
    dialog->setUserText("Select an existing " + instance.iddObjectType().valueDescription() + "Definition or press New to create a new definition.");
    dialog->setOKButtonText("Select");
    dialog->setCancelButtonText("New");
    dialog->show();

    ModelObjectSelectorDialogWatcher watcher(dialog);

    while (!watcher.isSelectionFinal()){
      // msleep calls processEvents
      openstudio::System::msleep(1);
    }

    boost::optional<ModelObject> selectedModelObject = watcher.selectedModelObject();

    if (selectedModelObject){
      // user chose an existing definition
      definition = selectedModelObject->optionalCast<SpaceLoadDefinition>();
      OS_ASSERT(definition);
      bool test = instance.setDefinition(*definition);
      OS_ASSERT(test);
      return;
    }
  }

  // make new definition
  switch(instance.iddObjectType().value()){
    case IddObjectType::OS_ElectricEquipment:
      definition = ElectricEquipmentDefinition(model);
      break;
    case IddObjectType::OS_GasEquipment:
      definition = GasEquipmentDefinition(model);
      break;
    case IddObjectType::OS_HotWaterEquipment:
      definition = HotWaterEquipmentDefinition(model);
      break;
    case IddObjectType::OS_InternalMass:
      definition = InternalMassDefinition(model);
      break;
    case IddObjectType::OS_Lights:
      definition = LightsDefinition(model);
      break;
    case IddObjectType::OS_Luminaire:
      definition = LuminaireDefinition(model);
      break;
    case IddObjectType::OS_People:
      definition = PeopleDefinition(model);
      break;
    default:
      LOG_FREE_AND_THROW("openstudio.ensureSpaceLoadDefinition", "Unknown IddObjectType " << instance.iddObjectType().valueName());
  }

  OS_ASSERT(definition);
  bool test = instance.setDefinition(*definition);
  OS_ASSERT(test);

}
