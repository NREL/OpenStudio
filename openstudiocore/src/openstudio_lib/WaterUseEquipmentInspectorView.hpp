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

#ifndef OPENSTUDIO_WATERUSEEQUIPMENTINSPECTORVIEW_HPP
#define OPENSTUDIO_WATERUSEEQUIPMENTINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"
#include "ModelObjectVectorController.hpp"

namespace openstudio {

namespace model {

class WaterUseEquipmentDefinition;

}

class OSLineEdit;

class OSQuantityEdit;

class OSDropZone;

class TargetTemperatureScheduleVC : public ModelObjectVectorController
{
  Q_OBJECT

  public:

  virtual ~TargetTemperatureScheduleVC() {}

  protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);
  virtual std::vector<OSItemId> makeVector();
  virtual void onRemoveItem(OSItem* item);
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);
  virtual void onDrop(const OSItemId& itemId);

  boost::optional<model::WaterUseEquipmentDefinition> waterUseEquipmentDefinition() const;
};

class SensibleFractionScheduleVC : public ModelObjectVectorController
{
  Q_OBJECT

  public:

  virtual ~SensibleFractionScheduleVC() {}

  protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);
  virtual std::vector<OSItemId> makeVector();
  virtual void onRemoveItem(OSItem* item);
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);
  virtual void onDrop(const OSItemId& itemId);

  boost::optional<model::WaterUseEquipmentDefinition> waterUseEquipmentDefinition() const;
};

class LatentFractionScheduleVC : public ModelObjectVectorController
{
  Q_OBJECT

  public:

  virtual ~LatentFractionScheduleVC() {}

  protected:

  virtual void onChangeRelationship(const model::ModelObject& modelObject, int index, Handle newHandle, Handle oldHandle);
  virtual std::vector<OSItemId> makeVector();
  virtual void onRemoveItem(OSItem* item);
  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);
  virtual void onDrop(const OSItemId& itemId);

  boost::optional<model::WaterUseEquipmentDefinition> waterUseEquipmentDefinition() const;
};

class WaterUseEquipmentDefinitionInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    WaterUseEquipmentDefinitionInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent = 0 );

    virtual ~WaterUseEquipmentDefinitionInspectorView() {}

  protected:

    virtual void onClearSelection();

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);

    virtual void onUpdate();

  private:

    void attach(openstudio::model::WaterUseEquipmentDefinition & waterUseEquipmentDefinition);

    void detach();

    void refresh();

    OSLineEdit * m_nameEdit;

    OSLineEdit * m_endUseSubcategoryEdit;

    OSQuantityEdit * m_peakFlowRateEdit;

    TargetTemperatureScheduleVC * m_targetTemperatureScheduleVC;
    SensibleFractionScheduleVC * m_sensibleFractionScheduleVC;
    LatentFractionScheduleVC * m_latentFractionScheduleVC;

    OSDropZone * m_targetTemperatureScheduleDZ;
    OSDropZone * m_sensibleFractionScheduleDZ;
    OSDropZone * m_latentFractionScheduleDZ;

    bool m_isIP;

  public slots:

    void toggleUnits(bool displayIP);
};

} // openstudio

#endif // OPENSTUDIO_WATERUSEEQUIPMENTINSPECTORVIEW_HPP

