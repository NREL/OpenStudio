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

    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Pipe_Indoor, modelObject);

    if (auto node_ = modelObject.inletModelObject()) {
      idfObject.setString(openstudio::Pipe_IndoorFields::FluidInletNodeName, node_->nameString());
    }

    if (auto node_ = modelObject.outletModelObject()) {
      idfObject.setString(openstudio::Pipe_IndoorFields::FluidOutletNodeName, node_->nameString());
    }

    if (auto construction_ = modelObject.construction()) {
      idfObject.setString(openstudio::Pipe_IndoorFields::ConstructionName, construction_->nameString());
    }

    idfObject.setString(openstudio::Pipe_IndoorFields::EnvironmentType, modelObject.environmentType());

    if (auto zone_ = modelObject.ambientTemperatureZone()) {
      idfObject.setString(openstudio::Pipe_IndoorFields::AmbientTemperatureZoneName, zone_->nameString());
    }

    if (auto schedule_ = modelObject.ambientTemperatureSchedule()) {
      idfObject.setString(openstudio::Pipe_IndoorFields::AmbientTemperatureScheduleName, schedule_->nameString());
    }

    if (auto schedule_ = modelObject.ambientAirVelocitySchedule()) {
      idfObject.setString(openstudio::Pipe_IndoorFields::AmbientAirVelocityScheduleName, schedule_->nameString());
    }

    idfObject.setDouble(openstudio::Pipe_IndoorFields::PipeInsideDiameter, modelObject.pipeInsideDiameter());

    idfObject.setDouble(openstudio::Pipe_IndoorFields::PipeLength, modelObject.pipeLength());

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
