/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/SizingPlant.hpp"
#include "../../model/SizingPlant_Impl.hpp"
#include "../../model/PlantLoop.hpp"
#include "../../model/PlantLoop_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/Sizing_Plant_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  enum class PlantSizingType
  {
    HOTWATER,
    CHILLEDWATER,
    CONDENSER,
    NONE
  };

  PlantSizingType plantSizingType(const ModelObject& component) {
    switch (component.iddObject().type().value()) {
      case openstudio::IddObjectType::OS_Boiler_HotWater: {
        return PlantSizingType::HOTWATER;
      }
      case openstudio::IddObjectType::OS_WaterHeater_Mixed: {
        return PlantSizingType::HOTWATER;
      }
      case openstudio::IddObjectType::OS_WaterHeater_Stratified: {
        return PlantSizingType::HOTWATER;
      }
      case openstudio::IddObjectType::OS_DistrictHeating: {
        return PlantSizingType::HOTWATER;
      }
      case openstudio::IddObjectType::OS_Chiller_Electric_EIR: {
        return PlantSizingType::CHILLEDWATER;
      }
      case openstudio::IddObjectType::OS_Chiller_Absorption_Indirect: {
        return PlantSizingType::CHILLEDWATER;
      }
      case openstudio::IddObjectType::OS_Chiller_Absorption: {
        return PlantSizingType::CHILLEDWATER;
      }
      case openstudio::IddObjectType::OS_ThermalStorage_Ice_Detailed: {
        return PlantSizingType::CHILLEDWATER;
      }
      case openstudio::IddObjectType::OS_DistrictCooling: {
        return PlantSizingType::CHILLEDWATER;
      }
      case openstudio::IddObjectType::OS_CoolingTower_SingleSpeed: {
        return PlantSizingType::CONDENSER;
      }
      case openstudio::IddObjectType::OS_CoolingTower_VariableSpeed: {
        return PlantSizingType::CONDENSER;
      }
      case openstudio::IddObjectType::OS_CoolingTower_TwoSpeed: {
        return PlantSizingType::CONDENSER;
      }
      case openstudio::IddObjectType::OS_GroundHeatExchanger_Vertical: {
        return PlantSizingType::CONDENSER;
      }
      case openstudio::IddObjectType::OS_GroundHeatExchanger_HorizontalTrench: {
        return PlantSizingType::CONDENSER;
      }
      case openstudio::IddObjectType::OS_HeatExchanger_FluidToFluid: {
        return PlantSizingType::CONDENSER;
      }
      default: {
        return PlantSizingType::NONE;
      }
    }
  }

  boost::optional<IdfObject> ForwardTranslator::translateSizingPlant(SizingPlant& modelObject) {
    // These will be used only if reasonable sizing values have not already been provided.
    auto condensorCheck = [](const ModelObject& comp) { return (plantSizingType(comp) == PlantSizingType::CONDENSER); };

    auto chilledWaterCheck = [](const ModelObject& comp) { return (plantSizingType(comp) == PlantSizingType::CHILLEDWATER); };

    if ((modelObject.designLoopExitTemperature() < 0.01) && (modelObject.loopDesignTemperatureDifference() < 0.01)) {
      const auto& components = modelObject.plantLoop().supplyComponents();
      if (std::find_if(components.begin(), components.end(), condensorCheck) != components.end()) {
        modelObject.setLoopType("Condenser");
        modelObject.setDesignLoopExitTemperature(29.4);
        modelObject.setLoopDesignTemperatureDifference(5.6);
      } else if (std::find_if(components.begin(), components.end(), chilledWaterCheck) != components.end()) {
        modelObject.setLoopType("Cooling");
        modelObject.setDesignLoopExitTemperature(7.22);
        modelObject.setLoopDesignTemperatureDifference(6.67);
      } else {
        modelObject.setLoopType("Heating");
        modelObject.setDesignLoopExitTemperature(82.0);
        modelObject.setLoopDesignTemperatureDifference(11.0);
      }
    }

    boost::optional<std::string> s;
    boost::optional<double> value;

    IdfObject idfObject(IddObjectType::Sizing_Plant);

    // PlantorCondenserLoopName

    s = modelObject.plantLoop().name();
    if (s) {
      idfObject.setString(Sizing_PlantFields::PlantorCondenserLoopName, s.get());
    }

    // LoopType

    s = modelObject.loopType();
    if (s) {
      idfObject.setString(Sizing_PlantFields::LoopType, s.get());
    }

    // DesignLoopExitTemperature

    value = modelObject.designLoopExitTemperature();
    if (value) {
      idfObject.setDouble(Sizing_PlantFields::DesignLoopExitTemperature, value.get());
    }

    // LoopDesignTemperatureDifference

    value = modelObject.loopDesignTemperatureDifference();
    if (value) {
      idfObject.setDouble(Sizing_PlantFields::LoopDesignTemperatureDifference, value.get());
    }

    // SizingOption
    s = modelObject.sizingOption();
    if (s) {
      idfObject.setString(Sizing_PlantFields::SizingOption, s.get());
    }

    // ZoneTimestepsinAveragingWindow
    value = modelObject.zoneTimestepsinAveragingWindow();
    if (value) {
      idfObject.setDouble(Sizing_PlantFields::ZoneTimestepsinAveragingWindow, value.get());
    }

    // CoincidentSizingFactorMode
    s = modelObject.coincidentSizingFactorMode();
    if (s) {
      idfObject.setString(Sizing_PlantFields::CoincidentSizingFactorMode, s.get());
    }

    m_idfObjects.push_back(idfObject);

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
