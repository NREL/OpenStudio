/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/LoadProfilePlant.hpp"
#include "../../model/LoadProfilePlant_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Node.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/LoadProfile_Plant_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateLoadProfilePlant(LoadProfilePlant& modelObject) {
    OptionalString s;
    OptionalDouble value;
    OptionalModelObject temp;

    //Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::LoadProfile_Plant, modelObject);

    // InletNodeName
    if (auto node = modelObject.inletModelObject()) {
      idfObject.setString(LoadProfile_PlantFields::InletNodeName, node->name().get());
    }

    // OutletNodeName
    if (auto node = modelObject.outletModelObject()) {
      idfObject.setString(LoadProfile_PlantFields::OutletNodeName, node->name().get());
    }

    // LoadScheduleName
    {
      auto schedule = modelObject.loadSchedule();
      if (auto _schedule = translateAndMapModelObject(schedule)) {
        idfObject.setString(LoadProfile_PlantFields::LoadScheduleName, _schedule->name().get());
      }
    }

    // PeakFlowRate
    if ((value = modelObject.peakFlowRate())) {
      idfObject.setDouble(LoadProfile_PlantFields::PeakFlowRate, value.get());
    }

    // FlowRateFractionScheduleName
    {
      auto schedule = modelObject.flowRateFractionSchedule();
      if (auto _schedule = translateAndMapModelObject(schedule)) {
        idfObject.setString(LoadProfile_PlantFields::FlowRateFractionScheduleName, _schedule->name().get());
      }
    }

    // PlantLoopFluidType
    idfObject.setString(LoadProfile_PlantFields::PlantLoopFluidType, modelObject.plantLoopFluidType());

    // DegreeofSubCooling
    idfObject.setDouble(LoadProfile_PlantFields::DegreeofSubCooling, modelObject.degreeofSubCooling());

    // DegreeofLoopSubCooling
    idfObject.setDouble(LoadProfile_PlantFields::DegreeofLoopSubCooling, modelObject.degreeofLoopSubCooling());

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
