/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/ChillerElectricASHRAE205.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"

#include "../../model/ExternalFile.hpp"

#include <utilities/idd/Chiller_Electric_ASHRAE205_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateChillerElectricASHRAE205(const WorkspaceObject& workspaceObject) {

    std::string fileName = workspaceObject.getString(Chiller_Electric_ASHRAE205Fields::RepresentationFileName).get();
    boost::optional<ExternalFile> extfile = ExternalFile::getExternalFile(m_model, fileName);

    if (!extfile) {
      LOG(Error, "Could not translate " << workspaceObject.briefDescription() << ", cannot find Representation File \"" << fileName << "\"");
      return boost::none;
    }

    boost::optional<std::string> ambientTemperatureIndicator_ =
      workspaceObject.getString(Chiller_Electric_ASHRAE205Fields::AmbientTemperatureIndicator);
    if (!ambientTemperatureIndicator_) {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Ambient Temperature Indicator'");
      return boost::none;
    }

    openstudio::model::ChillerElectricASHRAE205 modelObject(extfile.get());

    // Name
    if (boost::optional<std::string> _name = workspaceObject.name()) {
      modelObject.setName(_name.get());
    }

    // Performance Interpolation Method: Optional String
    if (boost::optional<std::string> _performanceInterpolationMethod =
          workspaceObject.getString(Chiller_Electric_ASHRAE205Fields::PerformanceInterpolationMethod)) {
      modelObject.setPerformanceInterpolationMethod(_performanceInterpolationMethod.get());
    }

    // Rated Capacity: Optional Double
    if (boost::optional<double> _ratedCapacity = workspaceObject.getDouble(Chiller_Electric_ASHRAE205Fields::RatedCapacity)) {
      modelObject.setRatedCapacity(_ratedCapacity.get());
    }

    // Sizing Factor: Optional Double
    if (boost::optional<double> _sizingFactor = workspaceObject.getDouble(Chiller_Electric_ASHRAE205Fields::SizingFactor)) {
      modelObject.setSizingFactor(_sizingFactor.get());
    }

    // Ambient Temperature Indicator: Required String
    if (openstudio::istringEqual("Schedule", ambientTemperatureIndicator_.get())) {

      // Ambient Temperature Schedule Name: Optional Object
      if (auto wo_sch_ = workspaceObject.getTarget(Chiller_Electric_ASHRAE205Fields::AmbientTemperatureScheduleName)) {
        if (auto mo_sch_ = translateAndMapWorkspaceObject(wo_sch_.get())) {
          if (boost::optional<Schedule> sch_ = mo_sch_->optionalCast<Schedule>()) {
            modelObject.setAmbientTemperatureSchedule(sch_.get());
          } else {
            LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Ambient Temperature Schedule Name'");
          }
        }
      } else {
        LOG(Warn, workspaceObject.briefDescription()
                    << " has an Ambient Temperature Indicator of type 'Schedule', but no Schedule assigned. Falling back to 'Outdoors'");
      }
    } else if (openstudio::istringEqual("Zone", ambientTemperatureIndicator_.get())) {

      // Ambient Temperature Zone Name: Optional Object
      if (auto wo_z_ = workspaceObject.getTarget(Chiller_Electric_ASHRAE205Fields::AmbientTemperatureZoneName)) {
        if (auto mo_ = translateAndMapWorkspaceObject(wo_z_.get())) {
          // Zone is translated, and a Space is returned instead
          if (boost::optional<Space> space_ = mo_->optionalCast<Space>()) {
            if (auto z_ = space_->thermalZone()) {
              modelObject.setAmbientTemperatureZone(z_.get());
            }
          } else {
            LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Ambient Temperature Zone Name'");
          }
        }
      } else {
        LOG(Warn, workspaceObject.briefDescription()
                    << " has an Ambient Temperature Indicator of type 'Zone', but no Zone assigned. Falling back to 'Outdoors'");
      }
    } else {
      // Ambient Temperature Outdoor Air Node Name: Optional Node
      if (boost::optional<std::string> s_ = workspaceObject.getString(Chiller_Electric_ASHRAE205Fields::AmbientTemperatureOutdoorAirNodeName)) {
        modelObject.setAmbientTemperatureOutdoorAirNodeName(s_.get());
      }
    }

    // Chilled Water Maximum Requested Flow Rate: Optional Double
    if (boost::optional<double> _chilledWaterMaximumRequestedFlowRate =
          workspaceObject.getDouble(Chiller_Electric_ASHRAE205Fields::ChilledWaterMaximumRequestedFlowRate)) {
      modelObject.setChilledWaterMaximumRequestedFlowRate(_chilledWaterMaximumRequestedFlowRate.get());
    }

    // Condenser Maximum Requested Flow Rate: Optional Double
    if (boost::optional<double> _condenserMaximumRequestedFlowRate =
          workspaceObject.getDouble(Chiller_Electric_ASHRAE205Fields::CondenserMaximumRequestedFlowRate)) {
      modelObject.setCondenserMaximumRequestedFlowRate(_condenserMaximumRequestedFlowRate.get());
    }

    // Chiller Flow Mode: Optional String
    if (boost::optional<std::string> _chillerFlowMode = workspaceObject.getString(Chiller_Electric_ASHRAE205Fields::ChillerFlowMode)) {
      modelObject.setChillerFlowMode(_chillerFlowMode.get());
    }

    if (boost::optional<double> _oilCoolerDesignFlowRate = workspaceObject.getDouble(Chiller_Electric_ASHRAE205Fields::OilCoolerDesignFlowRate)) {
      modelObject.setOilCoolerDesignFlowRate(_oilCoolerDesignFlowRate.get());
    }

    // Auxiliary Cooling Design Flow Rate: Optional Double
    if (boost::optional<double> _auxiliaryCoolingDesignFlowRate =
          workspaceObject.getDouble(Chiller_Electric_ASHRAE205Fields::AuxiliaryCoolingDesignFlowRate)) {
      modelObject.setAuxiliaryCoolingDesignFlowRate(_auxiliaryCoolingDesignFlowRate.get());
    }

    // End-Use Subcategory: Optional String
    if (boost::optional<std::string> _endUseSubcategory = workspaceObject.getString(Chiller_Electric_ASHRAE205Fields::EndUseSubcategory)) {
      modelObject.setEndUseSubcategory(_endUseSubcategory.get());
    }

    if (workspaceObject.getTarget(Chiller_Electric_ASHRAE205Fields::ChilledWaterInletNodeName)
        || workspaceObject.getTarget(Chiller_Electric_ASHRAE205Fields::ChilledWaterOutletNodeName)
        || workspaceObject.getTarget(Chiller_Electric_ASHRAE205Fields::CondenserInletNodeName)
        || workspaceObject.getTarget(Chiller_Electric_ASHRAE205Fields::CondenserOutletNodeName)
        || workspaceObject.getTarget(Chiller_Electric_ASHRAE205Fields::HeatRecoveryInletNodeName)
        || workspaceObject.getTarget(Chiller_Electric_ASHRAE205Fields::HeatRecoveryOutletNodeName)
        || workspaceObject.getTarget(Chiller_Electric_ASHRAE205Fields::OilCoolerInletNodeName)
        || workspaceObject.getTarget(Chiller_Electric_ASHRAE205Fields::OilCoolerOutletNodeName)
        || workspaceObject.getTarget(Chiller_Electric_ASHRAE205Fields::AuxiliaryInletNodeName)
        || workspaceObject.getTarget(Chiller_Electric_ASHRAE205Fields::AuxiliaryOutletNodeName)) {
      LOG(Warn, "For " << workspaceObject.briefDescription() << " Loop Connections are NOT ReverseTranslated.");
    }

    return modelObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
