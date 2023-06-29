/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Construction.hpp"
#include "../../model/Construction_Impl.hpp"
#include "../../model/PipeIndoor.hpp"
#include "../../model/PipeIndoor_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/Pipe_Indoor_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translatePipeIndoor(PipeIndoor& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::Pipe_Indoor);

    m_idfObjects.push_back(idfObject);

    if (auto value = modelObject.name()) {
      idfObject.setName(value.get());
    }

    if (auto node = modelObject.inletModelObject()) {
      idfObject.setString(openstudio::Pipe_IndoorFields::FluidInletNodeName, node->name().get());
    }

    if (auto node = modelObject.outletModelObject()) {
      idfObject.setString(openstudio::Pipe_IndoorFields::FluidOutletNodeName, node->name().get());
    }

    if (auto construction = modelObject.construction()) {
      idfObject.setString(openstudio::Pipe_IndoorFields::ConstructionName, construction->name().get());
    }

    idfObject.setString(openstudio::Pipe_IndoorFields::EnvironmentType, modelObject.environmentType());

    if (auto zone = modelObject.ambientTemperatureZone()) {
      idfObject.setString(openstudio::Pipe_IndoorFields::AmbientTemperatureZoneName, zone->name().get());
    }

    if (auto schedule = modelObject.ambientTemperatureSchedule()) {
      idfObject.setString(openstudio::Pipe_IndoorFields::AmbientTemperatureScheduleName, schedule->name().get());
    }

    if (auto schedule = modelObject.ambientAirVelocitySchedule()) {
      idfObject.setString(openstudio::Pipe_IndoorFields::AmbientAirVelocityScheduleName, schedule->name().get());
    }

    idfObject.setDouble(openstudio::Pipe_IndoorFields::PipeInsideDiameter, modelObject.pipeInsideDiameter());

    idfObject.setDouble(openstudio::Pipe_IndoorFields::PipeLength, modelObject.pipeLength());

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
