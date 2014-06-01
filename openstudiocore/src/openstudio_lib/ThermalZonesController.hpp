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

#ifndef OPENSTUDIO_THERMALZONESCONTROLLER_HPP
#define OPENSTUDIO_THERMALZONESCONTROLLER_HPP

#include "../model/Model.hpp"
#include "../model/Schedule.hpp"
#include <boost/smart_ptr.hpp>
#include "../shared_gui_components/OSQObjectController.hpp"
#include "ModelSubTabController.hpp"

namespace openstudio {

namespace model {
  class ThermalZone;

  class ZoneHVACComponent;
}

class ThermalZonesView;

class ThermalZonesController : public ModelSubTabController
{
  Q_OBJECT

  public:

  ThermalZonesController(bool isIP, const model::Model & model);

  virtual ~ThermalZonesController() {}

  signals:

  void toggleUnitsClicked(bool);

  protected:

  virtual void onSelectItem(OSItem *item);

  virtual void onAddObject(const openstudio::IddObjectType& iddObjectType);

  virtual void onCopyObject(const openstudio::model::ModelObject& modelObject);

  virtual void onRemoveObject(openstudio::model::ModelObject modelObject);

  virtual void onReplaceObject(openstudio::model::ModelObject modelObject, const OSItemId& replacementItemId);

  virtual void onPurgeObjects(const openstudio::IddObjectType& iddObjectType);

  virtual void onDrop(const OSItemId& itemId);

  virtual void onInspectItem(OSItem* item);

  public slots:

  void enableThermostat(model::ThermalZone &,bool);

  void setHeatingSchedule(model::ThermalZone &, model::OptionalSchedule &);

  void setCoolingSchedule(model::ThermalZone &, model::OptionalSchedule &);

  void enableHumidistat(model::ThermalZone &,bool);

  void setHumidifyingSchedule(model::ThermalZone &, model::OptionalSchedule &);

  void setDehumidifyingSchedule(model::ThermalZone &, model::OptionalSchedule &);

  void addComponentToZone(model::ThermalZone &, Handle &);

  void removeZoneHVACComponent( model::ZoneHVACComponent & );

  private slots:

  void onZoneHVACComponentClicked( model::ZoneHVACComponent & );

  private:

  //model::Model m_model;

  //model::Model m_library;
};

} // openstudio

#endif // OPENSTUDIO_THERMALZONESCONTROLLER_HPP 

