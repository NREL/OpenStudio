/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/ChillerElectricASHRAE205.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"

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

    // TODO: Note JM 2018-10-17
    // You are responsible for implementing any additional logic based on choice fields, etc.
    // The ReverseTranslator generator script is meant to facilitate your work, not get you 100% of the way

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
                    << " has an Ambient Temperature Indicator of type Schedule, but no Schedule. Falling back to 'Outdoors'");
      }
    } else if (openstudio::istringEqual("Zone", ambientTemperatureIndicator_.get())) {

      // Ambient Temperature Zone Name: Optional Object
      if (auto wo_z_ = workspaceObject.getTarget(Chiller_Electric_ASHRAE205Fields::AmbientTemperatureZoneName)) {
        if (auto mo_z_ = translateAndMapWorkspaceObject(wo_z_.get())) {
          // TODO: check return types
          if (boost::optional<ThermalZone> z_ = mo_z_->optionalCast<ThermalZone>()) {
            modelObject.setAmbientTemperatureZone(z_.get());
          } else {
            LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Ambient Temperature Zone Name'");
          }
        }
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
