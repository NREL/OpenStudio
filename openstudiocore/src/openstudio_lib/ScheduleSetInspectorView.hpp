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

#ifndef OPENSTUDIO_SCHEDULESETINSPECTORVIEW_HPP
#define OPENSTUDIO_SCHEDULESETINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"
#include "ModelObjectVectorController.hpp"

#include "../model/DefaultScheduleSet.hpp"
#include "../model/Model.hpp"

namespace openstudio {

class OSDropZone;
class OSLineEdit2;

class HoursOfOperationVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~HoursOfOperationVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle) override;
  virtual std::vector<OSItemId> makeVector() override;
  virtual void onRemoveItem(OSItem* item) override;
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;
  virtual void onDrop(const OSItemId& itemId) override;
};

class NumberOfPeopleVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~NumberOfPeopleVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle) override;
  virtual std::vector<OSItemId> makeVector() override;
  virtual void onRemoveItem(OSItem* item) override;
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;
  virtual void onDrop(const OSItemId& itemId) override;
};

class PeopleActivityScheduleVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~PeopleActivityScheduleVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle) override;
  virtual std::vector<OSItemId> makeVector() override;
  virtual void onRemoveItem(OSItem* item) override;
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;
  virtual void onDrop(const OSItemId& itemId) override;
};

class LightingScheduleVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~LightingScheduleVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle) override;
  virtual std::vector<OSItemId> makeVector() override;
  virtual void onRemoveItem(OSItem* item) override;
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;
  virtual void onDrop(const OSItemId& itemId) override;
};

class ElectricEquipmentScheduleVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~ElectricEquipmentScheduleVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle) override;
  virtual std::vector<OSItemId> makeVector() override;
  virtual void onRemoveItem(OSItem* item) override;
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;
  virtual void onDrop(const OSItemId& itemId) override;
};

class GasEquipmentScheduleVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~GasEquipmentScheduleVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle) override;
  virtual std::vector<OSItemId> makeVector() override;
  virtual void onRemoveItem(OSItem* item) override;
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;
  virtual void onDrop(const OSItemId& itemId) override;
};

class HotWaterEquipmentScheduleVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~HotWaterEquipmentScheduleVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle) override;
  virtual std::vector<OSItemId> makeVector() override;
  virtual void onRemoveItem(OSItem* item) override;
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;
  virtual void onDrop(const OSItemId& itemId) override;
};

class InfiltrationScheduleVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~InfiltrationScheduleVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle) override;
  virtual std::vector<OSItemId> makeVector() override;
  virtual void onRemoveItem(OSItem* item) override;
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;
  virtual void onDrop(const OSItemId& itemId) override;
};

class SteamEquipmentScheduleVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~SteamEquipmentScheduleVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle) override;
  virtual std::vector<OSItemId> makeVector() override;
  virtual void onRemoveItem(OSItem* item) override;
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;
  virtual void onDrop(const OSItemId& itemId) override;
};

class OtherEquipmentScheduleVC : public ModelObjectVectorController
{
  Q_OBJECT

public:
  virtual ~OtherEquipmentScheduleVC() {}

protected:
  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle) override;
  virtual std::vector<OSItemId> makeVector() override;
  virtual void onRemoveItem(OSItem* item) override;
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId) override;
  virtual void onDrop(const OSItemId& itemId) override;
};

class ScheduleSetInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:
    ScheduleSetInspectorView(const model::Model& model,
      QWidget * parent = nullptr);
    virtual ~ScheduleSetInspectorView() {}

  protected:
    virtual void onClearSelection() override;
    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject) override;
    virtual void onUpdate() override;

  private:
    void attach(openstudio::model::DefaultScheduleSet& scheduleSet);
    void detach();
    void configDropZones();

    OSLineEdit2 * m_nameEdit;

    OSDropZone * m_hoursOfOperationDZ;
    OSDropZone * m_numberOfPeopleDZ;
    OSDropZone * m_peopleActivityScheduleDZ;
    OSDropZone * m_lightingScheduleDZ;
    OSDropZone * m_electricEquipmentScheduleDZ;
    OSDropZone * m_gasEquipmentScheduleDZ;
    OSDropZone * m_hotWaterEquipmentScheduleDZ;
    OSDropZone * m_infiltrationScheduleDZ;
    OSDropZone * m_ventilationPerPersonScheduleDZ;
    OSDropZone * m_ventilationPerFloorAreaScheduleDZ;

    HoursOfOperationVC * m_hoursOfOperationVC;
    NumberOfPeopleVC * m_numberOfPeopleVC;
    PeopleActivityScheduleVC * m_peopleActivityScheduleVC;
    LightingScheduleVC * m_lightingScheduleVC;
    ElectricEquipmentScheduleVC * m_electricEquipmentScheduleVC;
    GasEquipmentScheduleVC * m_gasEquipmentScheduleVC;
    HotWaterEquipmentScheduleVC * m_hotWaterEquipmentScheduleVC;
    InfiltrationScheduleVC * m_infiltrationScheduleVC;
    SteamEquipmentScheduleVC * m_steamEquipmentScheduleVC;
    OtherEquipmentScheduleVC * m_otherEquipmentScheduleVC;

    std::vector<ModelObjectVectorController *> m_vectorControllers;
    std::vector<OSDropZone *> m_dropZones;

    boost::optional<model::DefaultScheduleSet> m_defaultScheduleSet;
};

} // openstudio

#endif // OPENSTUDIO_SCHEDULESETINSPECTORVIEW_HPP
