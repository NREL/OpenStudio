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

#include "ScheduleSetInspectorView.hpp"

#include "ModelObjectItem.hpp"
#include "ModelObjectTypeListView.hpp"
#include "OSDropZone.hpp"
#include "../shared_gui_components/OSLineEdit.hpp"

#include "../model/DefaultScheduleSet_Impl.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/Schedule.hpp"
#include "../model/Schedule_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include <utilities/idd/OS_DefaultScheduleSet_FieldEnums.hxx>

#include <QGridLayout>
#include <QLabel>
#include <QStackedWidget>
#include <QVBoxLayout>

#include <sstream>

namespace openstudio {

// HoursOfOperationVC

void HoursOfOperationVC::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_DefaultScheduleSetFields::HoursofOperationScheduleName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> HoursOfOperationVC::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::DefaultScheduleSet defaultScheduleSet = m_modelObject->cast<model::DefaultScheduleSet>();
    boost::optional<model::Schedule> schedule = defaultScheduleSet.hoursofOperationSchedule();
    if (schedule){
      result.push_back(modelObjectToItemId(*schedule, false));
    }
  }
  return result;
}

void HoursOfOperationVC::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::DefaultScheduleSet defaultScheduleSet = m_modelObject->cast<model::DefaultScheduleSet>();
    defaultScheduleSet.resetHoursofOperationSchedule();
  }
}

void HoursOfOperationVC::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void HoursOfOperationVC::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::DefaultScheduleSet defaultScheduleSet = m_modelObject->cast<model::DefaultScheduleSet>();
    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::Schedule>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        model::Schedule schedule = modelObject->cast<model::Schedule>();
        defaultScheduleSet.setHoursofOperationSchedule(schedule);
      }
    }
  }
}

// NumberOfPeopleVC

void NumberOfPeopleVC::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_DefaultScheduleSetFields::NumberofPeopleScheduleName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> NumberOfPeopleVC::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::DefaultScheduleSet defaultScheduleSet = m_modelObject->cast<model::DefaultScheduleSet>();
    boost::optional<model::Schedule> schedule = defaultScheduleSet.numberofPeopleSchedule();
    if (schedule){
      result.push_back(modelObjectToItemId(*schedule, false));
    }
  }
  return result;
}

void NumberOfPeopleVC::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::DefaultScheduleSet defaultScheduleSet = m_modelObject->cast<model::DefaultScheduleSet>();
    defaultScheduleSet.resetNumberofPeopleSchedule();
  }
}

void NumberOfPeopleVC::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void NumberOfPeopleVC::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::DefaultScheduleSet defaultScheduleSet = m_modelObject->cast<model::DefaultScheduleSet>();
    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::Schedule>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        model::Schedule schedule = modelObject->cast<model::Schedule>();
        defaultScheduleSet.setNumberofPeopleSchedule(schedule);
      }
    }
  }
}

// PeopleActivityScheduleVC

void PeopleActivityScheduleVC::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_DefaultScheduleSetFields::PeopleActivityLevelScheduleName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> PeopleActivityScheduleVC::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::DefaultScheduleSet defaultScheduleSet = m_modelObject->cast<model::DefaultScheduleSet>();
    boost::optional<model::Schedule> schedule = defaultScheduleSet.peopleActivityLevelSchedule();
    if (schedule){
      result.push_back(modelObjectToItemId(*schedule, false));
    }
  }
  return result;
}

void PeopleActivityScheduleVC::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::DefaultScheduleSet defaultScheduleSet = m_modelObject->cast<model::DefaultScheduleSet>();
    defaultScheduleSet.resetPeopleActivityLevelSchedule();
  }
}

void PeopleActivityScheduleVC::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void PeopleActivityScheduleVC::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::DefaultScheduleSet defaultScheduleSet = m_modelObject->cast<model::DefaultScheduleSet>();
    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::Schedule>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        model::Schedule schedule = modelObject->cast<model::Schedule>();
        defaultScheduleSet.setPeopleActivityLevelSchedule(schedule);
      }
    }
  }
}

// LightingScheduleVC

void LightingScheduleVC::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_DefaultScheduleSetFields::LightingScheduleName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> LightingScheduleVC::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::DefaultScheduleSet defaultScheduleSet = m_modelObject->cast<model::DefaultScheduleSet>();
    boost::optional<model::Schedule> schedule = defaultScheduleSet.lightingSchedule();
    if (schedule){
      result.push_back(modelObjectToItemId(*schedule, false));
    }
  }
  return result;
}

void LightingScheduleVC::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::DefaultScheduleSet defaultScheduleSet = m_modelObject->cast<model::DefaultScheduleSet>();
    defaultScheduleSet.resetLightingSchedule();
  }
}

void LightingScheduleVC::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void LightingScheduleVC::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::DefaultScheduleSet defaultScheduleSet = m_modelObject->cast<model::DefaultScheduleSet>();
    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::Schedule>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        model::Schedule schedule = modelObject->cast<model::Schedule>();
        defaultScheduleSet.setLightingSchedule(schedule);
      }
    }
  }
}

// ElectricEquipmentScheduleVC

void ElectricEquipmentScheduleVC::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_DefaultScheduleSetFields::ElectricEquipmentScheduleName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> ElectricEquipmentScheduleVC::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::DefaultScheduleSet defaultScheduleSet = m_modelObject->cast<model::DefaultScheduleSet>();
    boost::optional<model::Schedule> schedule = defaultScheduleSet.electricEquipmentSchedule();
    if (schedule){
      result.push_back(modelObjectToItemId(*schedule, false));
    }
  }
  return result;
}

void ElectricEquipmentScheduleVC::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::DefaultScheduleSet defaultScheduleSet = m_modelObject->cast<model::DefaultScheduleSet>();
    defaultScheduleSet.resetElectricEquipmentSchedule();
  }
}

void ElectricEquipmentScheduleVC::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void ElectricEquipmentScheduleVC::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::DefaultScheduleSet defaultScheduleSet = m_modelObject->cast<model::DefaultScheduleSet>();
    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::Schedule>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        model::Schedule schedule = modelObject->cast<model::Schedule>();
        defaultScheduleSet.setElectricEquipmentSchedule(schedule);
      }
    }
  }
}

// GasEquipmentScheduleVC

void GasEquipmentScheduleVC::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_DefaultScheduleSetFields::GasEquipmentScheduleName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> GasEquipmentScheduleVC::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::DefaultScheduleSet defaultScheduleSet = m_modelObject->cast<model::DefaultScheduleSet>();
    boost::optional<model::Schedule> schedule = defaultScheduleSet.gasEquipmentSchedule();
    if (schedule){
      result.push_back(modelObjectToItemId(*schedule, false));
    }
  }
  return result;
}

void GasEquipmentScheduleVC::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::DefaultScheduleSet defaultScheduleSet = m_modelObject->cast<model::DefaultScheduleSet>();
    defaultScheduleSet.resetGasEquipmentSchedule();
  }
}

void GasEquipmentScheduleVC::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void GasEquipmentScheduleVC::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::DefaultScheduleSet defaultScheduleSet = m_modelObject->cast<model::DefaultScheduleSet>();
    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::Schedule>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        model::Schedule schedule = modelObject->cast<model::Schedule>();
        defaultScheduleSet.setGasEquipmentSchedule(schedule);
      }
    }
  }
}

// HotWaterEquipmentScheduleVC

void HotWaterEquipmentScheduleVC::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_DefaultScheduleSetFields::HotWaterEquipmentScheduleName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> HotWaterEquipmentScheduleVC::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::DefaultScheduleSet defaultScheduleSet = m_modelObject->cast<model::DefaultScheduleSet>();
    boost::optional<model::Schedule> schedule = defaultScheduleSet.hotWaterEquipmentSchedule();
    if (schedule){
      result.push_back(modelObjectToItemId(*schedule, false));
    }
  }
  return result;
}

void HotWaterEquipmentScheduleVC::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::DefaultScheduleSet defaultScheduleSet = m_modelObject->cast<model::DefaultScheduleSet>();
    defaultScheduleSet.resetHotWaterEquipmentSchedule();
  }
}

void HotWaterEquipmentScheduleVC::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void HotWaterEquipmentScheduleVC::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::DefaultScheduleSet defaultScheduleSet = m_modelObject->cast<model::DefaultScheduleSet>();
    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::Schedule>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        model::Schedule schedule = modelObject->cast<model::Schedule>();
        defaultScheduleSet.setHotWaterEquipmentSchedule(schedule);
      }
    }
  }
}

// InfiltrationScheduleVC

void InfiltrationScheduleVC::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_DefaultScheduleSetFields::InfiltrationScheduleName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> InfiltrationScheduleVC::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::DefaultScheduleSet defaultScheduleSet = m_modelObject->cast<model::DefaultScheduleSet>();
    boost::optional<model::Schedule> schedule = defaultScheduleSet.infiltrationSchedule();
    if (schedule){
      result.push_back(modelObjectToItemId(*schedule, false));
    }
  }
  return result;
}

void InfiltrationScheduleVC::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::DefaultScheduleSet defaultScheduleSet = m_modelObject->cast<model::DefaultScheduleSet>();
    defaultScheduleSet.resetInfiltrationSchedule();
  }
}

void InfiltrationScheduleVC::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void InfiltrationScheduleVC::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::DefaultScheduleSet defaultScheduleSet = m_modelObject->cast<model::DefaultScheduleSet>();
    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::Schedule>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        model::Schedule schedule = modelObject->cast<model::Schedule>();
        defaultScheduleSet.setInfiltrationSchedule(schedule);
      }
    }
  }
}

// SteamEquipmentScheduleVC

void SteamEquipmentScheduleVC::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_DefaultScheduleSetFields::SteamEquipmentScheduleName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> SteamEquipmentScheduleVC::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::DefaultScheduleSet defaultScheduleSet = m_modelObject->cast<model::DefaultScheduleSet>();
    boost::optional<model::Schedule> schedule = defaultScheduleSet.steamEquipmentSchedule();
    if (schedule){
      result.push_back(modelObjectToItemId(*schedule, false));
    }
  }
  return result;
}

void SteamEquipmentScheduleVC::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::DefaultScheduleSet defaultScheduleSet = m_modelObject->cast<model::DefaultScheduleSet>();
    defaultScheduleSet.resetSteamEquipmentSchedule();
  }
}

void SteamEquipmentScheduleVC::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void SteamEquipmentScheduleVC::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::DefaultScheduleSet defaultScheduleSet = m_modelObject->cast<model::DefaultScheduleSet>();
    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::Schedule>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        model::Schedule schedule = modelObject->cast<model::Schedule>();
        defaultScheduleSet.setSteamEquipmentSchedule(schedule);
      }
    }
  }
}

// OtherEquipmentScheduleVC

void OtherEquipmentScheduleVC::onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle)
{
  if (index == OS_DefaultScheduleSetFields::OtherEquipmentScheduleName){
    emit itemIds(makeVector());
  }
}

std::vector<OSItemId> OtherEquipmentScheduleVC::makeVector()
{
  std::vector<OSItemId> result;
  if (m_modelObject){
    model::DefaultScheduleSet defaultScheduleSet = m_modelObject->cast<model::DefaultScheduleSet>();
    boost::optional<model::Schedule> schedule = defaultScheduleSet.otherEquipmentSchedule();
    if (schedule){
      result.push_back(modelObjectToItemId(*schedule, false));
    }
  }
  return result;
}

void OtherEquipmentScheduleVC::onRemoveItem(OSItem* item)
{
  if (m_modelObject){
    model::DefaultScheduleSet defaultScheduleSet = m_modelObject->cast<model::DefaultScheduleSet>();
    defaultScheduleSet.resetOtherEquipmentSchedule();
  }
}

void OtherEquipmentScheduleVC::onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId)
{
  onDrop(replacementItemId);
}

void OtherEquipmentScheduleVC::onDrop(const OSItemId& itemId)
{
  if (m_modelObject){
    model::DefaultScheduleSet defaultScheduleSet = m_modelObject->cast<model::DefaultScheduleSet>();
    boost::optional<model::ModelObject> modelObject = this->getModelObject(itemId);
    if (modelObject){
      if (modelObject->optionalCast<model::Schedule>()){
        if (this->fromComponentLibrary(itemId)){
          modelObject = modelObject->clone(m_modelObject->model());
        }
        model::Schedule schedule = modelObject->cast<model::Schedule>();
        defaultScheduleSet.setOtherEquipmentSchedule(schedule);
      }
    }
  }
}

/////////////////////////////////////////////////////////////////////////////

ScheduleSetInspectorView::ScheduleSetInspectorView(const model::Model& model,
  QWidget * parent)
  : ModelObjectInspectorView(model, true, parent),
  m_nameEdit(nullptr),

  m_hoursOfOperationDZ(nullptr),
  m_numberOfPeopleDZ(nullptr),
  m_peopleActivityScheduleDZ(nullptr),
  m_lightingScheduleDZ(nullptr),
  m_electricEquipmentScheduleDZ(nullptr),
  m_gasEquipmentScheduleDZ(nullptr),
  m_hotWaterEquipmentScheduleDZ(nullptr),
  m_infiltrationScheduleDZ(nullptr),
  m_ventilationPerPersonScheduleDZ(nullptr),
  m_ventilationPerFloorAreaScheduleDZ(nullptr),

  m_hoursOfOperationVC(nullptr),
  m_numberOfPeopleVC(nullptr),
  m_peopleActivityScheduleVC(nullptr),
  m_lightingScheduleVC(nullptr),
  m_electricEquipmentScheduleVC(nullptr),
  m_gasEquipmentScheduleVC(nullptr),
  m_hotWaterEquipmentScheduleVC(nullptr),
  m_infiltrationScheduleVC(nullptr),
  m_steamEquipmentScheduleVC(nullptr),
  m_otherEquipmentScheduleVC(nullptr),

  m_vectorControllers(std::vector<ModelObjectVectorController *>()),
  m_dropZones(std::vector<OSDropZone *>())
{
  QLabel * label = nullptr;

  label = new QLabel();
  label->setObjectName("H2");
  label->setWordWrap(true);
  label->setText("Name");

  m_nameEdit = new OSLineEdit2();

  auto gridLayout = new QGridLayout();
  gridLayout->setContentsMargins(10,10,10,10);
  gridLayout->setSpacing(10);

  int leftCol = 0;
  int rightCol = 1;
  int row = 0;

  gridLayout->addWidget(label,row,0);

  row++;

  gridLayout->addWidget(m_nameEdit,row,0,1,2);

  row++;

  QFrame * line;

  line = new QFrame();
  line->setFrameShape(QFrame::HLine);
  line->setFrameShadow(QFrame::Sunken);
  gridLayout->addWidget(line,row,0,1,-1);
  row++;

  label = new QLabel();
  label->setText("Default Schedules");
  label->setObjectName("H2");
  //label->setContentsMargins(padding,0,padding,0);
  gridLayout->addWidget(label,row,0,1,2);
  row++;

  label = new QLabel();
  label->setObjectName("H2");
  label->setText("Hours of Operation");
  m_hoursOfOperationVC = new HoursOfOperationVC();
  m_vectorControllers.push_back(m_hoursOfOperationVC);
  m_hoursOfOperationDZ = new OSDropZone(m_hoursOfOperationVC);
  m_dropZones.push_back(m_hoursOfOperationDZ);
  gridLayout->addWidget(label,row,leftCol);
  gridLayout->addWidget(m_hoursOfOperationDZ,row+1,leftCol);

  label = new QLabel();
  label->setObjectName("H2");
  label->setText("Number of People");
  m_numberOfPeopleVC = new NumberOfPeopleVC();
  m_vectorControllers.push_back(m_numberOfPeopleVC);
  m_numberOfPeopleDZ = new OSDropZone(m_numberOfPeopleVC);
  m_dropZones.push_back(m_numberOfPeopleDZ);
  gridLayout->addWidget(label,row,rightCol);
  gridLayout->addWidget(m_numberOfPeopleDZ,row+1,rightCol);

  row += 2;

  label = new QLabel();
  label->setObjectName("H2");
  label->setText("People Activity");
  m_peopleActivityScheduleVC = new PeopleActivityScheduleVC();
  m_vectorControllers.push_back(m_peopleActivityScheduleVC);
  m_peopleActivityScheduleDZ = new OSDropZone(m_peopleActivityScheduleVC);
  m_dropZones.push_back(m_peopleActivityScheduleDZ);
  gridLayout->addWidget(label,row,leftCol);
  gridLayout->addWidget(m_peopleActivityScheduleDZ,row+1,leftCol);

  label = new QLabel();
  label->setObjectName("H2");
  label->setText("Lighting");
  m_lightingScheduleVC = new LightingScheduleVC();
  m_vectorControllers.push_back(m_lightingScheduleVC);
  m_lightingScheduleDZ = new OSDropZone(m_lightingScheduleVC);
  m_dropZones.push_back(m_lightingScheduleDZ);
  gridLayout->addWidget(label,row,rightCol);
  gridLayout->addWidget(m_lightingScheduleDZ,row+1,rightCol);

  row += 2;

  label = new QLabel();
  label->setObjectName("H2");
  label->setText("Electric Equipment");
  m_electricEquipmentScheduleVC = new ElectricEquipmentScheduleVC();
  m_vectorControllers.push_back(m_electricEquipmentScheduleVC);
  m_electricEquipmentScheduleDZ = new OSDropZone(m_electricEquipmentScheduleVC);
  m_dropZones.push_back(m_electricEquipmentScheduleDZ);
  gridLayout->addWidget(label,row,leftCol);
  gridLayout->addWidget(m_electricEquipmentScheduleDZ,row+1,leftCol);

  label = new QLabel();
  label->setObjectName("H2");
  label->setText("Gas Equipment");
  m_gasEquipmentScheduleVC = new GasEquipmentScheduleVC();
  m_vectorControllers.push_back(m_gasEquipmentScheduleVC);
  m_gasEquipmentScheduleDZ = new OSDropZone(m_gasEquipmentScheduleVC);
  m_dropZones.push_back(m_gasEquipmentScheduleDZ);
  gridLayout->addWidget(label,row,rightCol);
  gridLayout->addWidget(m_gasEquipmentScheduleDZ,row+1,rightCol);

  row += 2;

  label = new QLabel();
  label->setObjectName("H2");
  label->setText("Hot Water Equipment");
  m_hotWaterEquipmentScheduleVC = new HotWaterEquipmentScheduleVC();
  m_vectorControllers.push_back(m_hotWaterEquipmentScheduleVC);
  m_hotWaterEquipmentScheduleDZ = new OSDropZone(m_hotWaterEquipmentScheduleVC);
  m_dropZones.push_back(m_hotWaterEquipmentScheduleDZ);
  gridLayout->addWidget(label,row,leftCol);
  gridLayout->addWidget(m_hotWaterEquipmentScheduleDZ,row+1,leftCol);

  label = new QLabel();
  label->setObjectName("H2");
  label->setText("Steam Equipment");
  m_steamEquipmentScheduleVC = new SteamEquipmentScheduleVC();
  m_vectorControllers.push_back(m_steamEquipmentScheduleVC);
  m_ventilationPerPersonScheduleDZ = new OSDropZone(m_steamEquipmentScheduleVC);
  m_dropZones.push_back(m_ventilationPerPersonScheduleDZ);
  gridLayout->addWidget(label,row,rightCol);
  gridLayout->addWidget(m_ventilationPerPersonScheduleDZ,row+1,rightCol);

  row += 2;

  label = new QLabel();
  label->setObjectName("H2");
  label->setText("Other Equipment");
  m_otherEquipmentScheduleVC = new OtherEquipmentScheduleVC();
  m_vectorControllers.push_back(m_otherEquipmentScheduleVC);
  m_ventilationPerFloorAreaScheduleDZ = new OSDropZone(m_otherEquipmentScheduleVC);
  m_dropZones.push_back(m_ventilationPerFloorAreaScheduleDZ);
  gridLayout->addWidget(label,row,leftCol);
  gridLayout->addWidget(m_ventilationPerFloorAreaScheduleDZ,row+1,leftCol);

  label = new QLabel();
  label->setObjectName("H2");
  label->setText("Infiltration");
  m_infiltrationScheduleVC = new InfiltrationScheduleVC();
  m_vectorControllers.push_back(m_infiltrationScheduleVC);
  m_infiltrationScheduleDZ = new OSDropZone(m_infiltrationScheduleVC);
  m_dropZones.push_back(m_infiltrationScheduleDZ);
  gridLayout->addWidget(label,row,rightCol);
  gridLayout->addWidget(m_infiltrationScheduleDZ,row+1,rightCol);

  row += 2;

  OS_ASSERT(m_vectorControllers.size() == 10);

  configDropZones();

  //QVBoxLayout * mainVLayout = new QVBoxLayout();
  //mainVLayout->setContentsMargins(padding,padding,padding,padding);
  //mainVLayout->setSpacing(0);

  gridLayout->setColumnStretch(2,100);
  gridLayout->setRowStretch(row,100);

  //mainVLayout->addLayout(hLayout);
  //mainVLayout->addLayout(gridLayout);

  auto widget = new QWidget();
  //widget->setLayout(mainVLayout);
  widget->setLayout(gridLayout);

  // ETH@20130222 - Commented out lines below, replacing two with lines immediately below them,
  // to get rid of warning.
  // int index;
  auto hiddenWidget = new QWidget();
  // index = this->stackedWidget()->addWidget(hiddenWidget);
  this->stackedWidget()->addWidget(hiddenWidget);
  // index = this->stackedWidget()->addWidget(widget);
  this->stackedWidget()->addWidget(widget);
}

void ScheduleSetInspectorView::configDropZones()
{
  for (openstudio::OSDropZone * dropZone : m_dropZones){
    dropZone->setMinItems(0);
    dropZone->setMaxItems(1);
    dropZone->setItemsAcceptDrops(true);
  }
}

void ScheduleSetInspectorView::onClearSelection()
{
  detach();
}

void ScheduleSetInspectorView::onSelectModelObject(const openstudio::model::ModelObject& modelObject)
{
  detach();
  model::DefaultScheduleSet defaultScheduleSet = modelObject.cast<model::DefaultScheduleSet>();
  attach(defaultScheduleSet);
}

void ScheduleSetInspectorView::onUpdate()
{
}

void ScheduleSetInspectorView::attach(openstudio::model::DefaultScheduleSet& defaultScheduleSet)
{
  for (ModelObjectVectorController * vc : m_vectorControllers){
    vc->attach(defaultScheduleSet);
    vc->reportItems();
  }

  m_defaultScheduleSet = defaultScheduleSet;
  // m_nameEdit->bind(defaultScheduleSet, "name");
  m_nameEdit->bind(
    *m_defaultScheduleSet,
    OptionalStringGetter(std::bind(&model::DefaultScheduleSet::name, m_defaultScheduleSet.get_ptr(),true)),
    boost::optional<StringSetter>(std::bind(&model::DefaultScheduleSet::setName, m_defaultScheduleSet.get_ptr(),std::placeholders::_1))
  );

  this->stackedWidget()->setCurrentIndex(1);
}

void ScheduleSetInspectorView::detach()
{
  this->stackedWidget()->setCurrentIndex(0);

  m_nameEdit->unbind();

  for (ModelObjectVectorController * vc : m_vectorControllers){
    vc->detach();
    vc->reportItems();
  }

  m_defaultScheduleSet = boost::none;
}

} // openstudio

