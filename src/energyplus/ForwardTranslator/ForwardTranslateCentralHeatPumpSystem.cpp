/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/CentralHeatPumpSystem.hpp"
#include "../../model/CentralHeatPumpSystem_Impl.hpp"
#include "../../model/CentralHeatPumpSystemModule.hpp"
#include "../../model/ChillerHeaterPerformanceElectricEIR.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/CentralHeatPumpSystem_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateCentralHeatPumpSystem(CentralHeatPumpSystem& modelObject) {
    OptionalString s;
    OptionalDouble d;
    OptionalModelObject temp;

    auto const& modules = modelObject.getImpl<model::detail::CentralHeatPumpSystem_Impl>()->modules();

    // If the CentralHeatPumpSystem doesn't have at least one CentralHeatPumpSystemModule, then it shouldn't be translated
    if (modules.empty()) {
      LOG(Warn, "CentralHeatPumpSystem " << modelObject.name().get() << " has no CentralHeatPumpSystemModules, it will not be translated");
      return boost::none;
    }

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::CentralHeatPumpSystem, modelObject);

    // ControlMethod
    if ((s = modelObject.controlMethod())) {
      idfObject.setString(CentralHeatPumpSystemFields::ControlMethod, s.get());
    }

    // AncillaryPower
    if ((d = modelObject.ancillaryPower())) {
      idfObject.setDouble(CentralHeatPumpSystemFields::AncillaryPower, d.get());
    }

    // AncillaryOperationScheduleName
    {
      if (auto schedule = modelObject.ancillaryOperationSchedule()) {
        if (auto _schedule = translateAndMapModelObject(schedule.get())) {
          idfObject.setString(CentralHeatPumpSystemFields::AncillaryOperationScheduleName, _schedule->name().get());
        }
      }
    }

    // supply = Cooling Loop, demand=Source Loop, tertiary = Heating Loop

    // CoolingLoopInletNodeName
    if (auto mo = modelObject.supplyInletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        idfObject.setString(CentralHeatPumpSystemFields::CoolingLoopInletNodeName, node->name().get());
      }
    }

    // CoolingLoopOutletNodeName
    if (auto mo = modelObject.supplyOutletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        idfObject.setString(CentralHeatPumpSystemFields::CoolingLoopOutletNodeName, node->name().get());
      }
    }

    // SourceLoopInletNodeName
    if (auto mo = modelObject.demandInletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        idfObject.setString(CentralHeatPumpSystemFields::SourceLoopInletNodeName, node->name().get());
      }
    }

    // SourceLoopOutletNodeName
    if (auto mo = modelObject.demandOutletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        idfObject.setString(CentralHeatPumpSystemFields::SourceLoopOutletNodeName, node->name().get());
      }
    }

    // HeatingLoopInletNodeName
    if (auto mo = modelObject.tertiaryInletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        idfObject.setString(CentralHeatPumpSystemFields::HeatingLoopInletNodeName, node->name().get());
      }
    }
    // HeatingLoopOutletNodeName
    if (auto mo = modelObject.tertiaryOutletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        idfObject.setString(CentralHeatPumpSystemFields::HeatingLoopOutletNodeName, node->name().get());
      }
    }

    // ChillerHeaterModulesPerformanceComponentObjectType1
    // ChillerHeaterModulesPerformanceComponentName1
    // ChillerHeaterModulesControlScheduleName1
    // NumberofChillerHeaterModules1

    for (auto const& module : modules) {
      IdfExtensibleGroup group = idfObject.pushExtensibleGroup();

      auto performanceComponent = module.chillerHeaterModulesPerformanceComponent();
      if (auto _performanceComponent = translateAndMapModelObject(performanceComponent)) {
        group.setString(CentralHeatPumpSystemExtensibleFields::ChillerHeaterModulesPerformanceComponentObjectType,
                        _performanceComponent->iddObject().name());
        group.setString(CentralHeatPumpSystemExtensibleFields::ChillerHeaterModulesPerformanceComponentName, _performanceComponent->name().get());
      }
      {
        auto schedule = module.chillerHeaterModulesControlSchedule();
        if (auto _schedule = translateAndMapModelObject(schedule)) {
          group.setString(CentralHeatPumpSystemExtensibleFields::ChillerHeaterModulesControlScheduleName, _schedule->name().get());
        }
      }
      group.setInt(CentralHeatPumpSystemExtensibleFields::NumberofChillerHeaterModules, module.numberofChillerHeaterModules());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
