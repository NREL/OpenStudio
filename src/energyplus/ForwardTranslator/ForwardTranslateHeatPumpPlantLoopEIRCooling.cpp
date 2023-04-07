/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/HeatPumpPlantLoopEIRCooling.hpp"
#include "../../model/HeatPumpPlantLoopEIRCooling_Impl.hpp"
#include "../../model/HeatPumpPlantLoopEIRHeating.hpp"
#include "../../model/HeatPumpPlantLoopEIRHeating_Impl.hpp"
#include "../../model/CurveBiquadratic.hpp"
#include "../../model/CurveQuadratic.hpp"

#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/HeatPump_PlantLoop_EIR_Cooling_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateHeatPumpPlantLoopEIRCooling(HeatPumpPlantLoopEIRCooling& modelObject) {
    // createRegisterAndNameIdfObject will add it to m_map, m_idfObjects, and name it
    // The fact that it is added to m_map will avoid a recursion issue when it has a companion heating coil (both FT methods call each other)
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::HeatPump_PlantLoop_EIR_Cooling, modelObject);

    boost::optional<double> optvalue;

    if (auto value = modelObject.supplyOutletModelObject()) {
      idfObject.setString(HeatPump_PlantLoop_EIR_CoolingFields::LoadSideOutletNodeName, value->name().get());
    }

    if (auto value = modelObject.supplyInletModelObject()) {
      idfObject.setString(HeatPump_PlantLoop_EIR_CoolingFields::LoadSideInletNodeName, value->name().get());
    }

    // CondenserType
    // The "smart" logic is handled in model itself
    // (eg: if you connect the HP to a secondaryPlantLoop, it switches automatically to "WaterSource")
    std::string condenserType = modelObject.condenserType();
    idfObject.setString(HeatPump_PlantLoop_EIR_CoolingFields::CondenserType, condenserType);

    if (openstudio::istringEqual(condenserType, "WaterSource")) {

      if (auto value = modelObject.demandOutletModelObject()) {
        idfObject.setString(HeatPump_PlantLoop_EIR_CoolingFields::SourceSideOutletNodeName, value->name().get());
      }

      if (auto value = modelObject.demandInletModelObject()) {
        idfObject.setString(HeatPump_PlantLoop_EIR_CoolingFields::SourceSideInletNodeName, value->name().get());
      }

    } else {
      // AirSource
      {
        // Name the source side outlet node
        auto name = modelObject.nameString() + " Outlet Node For Source Side";
        idfObject.setString(HeatPump_PlantLoop_EIR_CoolingFields::SourceSideOutletNodeName, name);
      }

      {
        // Create an OutdoorAir:NodeList for the source side inlet conditions to be set directly from weather file
        IdfObject oaNodeListIdf(openstudio::IddObjectType::OutdoorAir_NodeList);
        auto name = modelObject.nameString() + " Inlet Node For Source Side";
        oaNodeListIdf.setString(0, name);
        m_idfObjects.push_back(oaNodeListIdf);
        idfObject.setString(HeatPump_PlantLoop_EIR_CoolingFields::SourceSideInletNodeName, name);
      }
    }

    boost::optional<HeatPumpPlantLoopEIRHeating> companion = modelObject.companionHeatingHeatPump();
    if (companion) {
      boost::optional<IdfObject> _companion = translateAndMapModelObject(companion.get());
      if (_companion) {
        idfObject.setString(HeatPump_PlantLoop_EIR_CoolingFields::CompanionHeatPumpName, _companion->name().get());
      }
    }

    if (modelObject.isLoadSideReferenceFlowRateAutosized()) {
      idfObject.setString(HeatPump_PlantLoop_EIR_CoolingFields::LoadSideReferenceFlowRate, "Autosize");
    } else if ((optvalue = modelObject.loadSideReferenceFlowRate())) {
      idfObject.setDouble(HeatPump_PlantLoop_EIR_CoolingFields::LoadSideReferenceFlowRate, optvalue.get());
    }

    if (modelObject.isSourceSideReferenceFlowRateAutosized()) {
      idfObject.setString(HeatPump_PlantLoop_EIR_CoolingFields::SourceSideReferenceFlowRate, "Autosize");
    } else if ((optvalue = modelObject.sourceSideReferenceFlowRate())) {
      idfObject.setDouble(HeatPump_PlantLoop_EIR_CoolingFields::SourceSideReferenceFlowRate, optvalue.get());
    }

    if (modelObject.isReferenceCapacityAutosized()) {
      idfObject.setString(HeatPump_PlantLoop_EIR_CoolingFields::ReferenceCapacity, "Autosize");
    } else if ((optvalue = modelObject.referenceCapacity())) {
      idfObject.setDouble(HeatPump_PlantLoop_EIR_CoolingFields::ReferenceCapacity, optvalue.get());
    }

    {
      auto value = modelObject.referenceCoefficientofPerformance();
      idfObject.setDouble(HeatPump_PlantLoop_EIR_CoolingFields::ReferenceCoefficientofPerformance, value);
    }

    {
      auto value = modelObject.sizingFactor();
      idfObject.setDouble(HeatPump_PlantLoop_EIR_CoolingFields::SizingFactor, value);
    }

    {
      auto curve = modelObject.capacityModifierFunctionofTemperatureCurve();
      if (auto _curve = translateAndMapModelObject(curve)) {
        idfObject.setString(HeatPump_PlantLoop_EIR_CoolingFields::CapacityModifierFunctionofTemperatureCurveName, _curve->nameString());
      }
    }

    {
      auto curve = modelObject.electricInputtoOutputRatioModifierFunctionofTemperatureCurve();
      if (auto _curve = translateAndMapModelObject(curve)) {
        idfObject.setString(HeatPump_PlantLoop_EIR_CoolingFields::ElectricInputtoOutputRatioModifierFunctionofTemperatureCurveName,
                            _curve->nameString());
      }
    }

    {
      auto curve = modelObject.electricInputtoOutputRatioModifierFunctionofPartLoadRatioCurve();
      if (auto _curve = translateAndMapModelObject(curve)) {
        idfObject.setString(HeatPump_PlantLoop_EIR_CoolingFields::ElectricInputtoOutputRatioModifierFunctionofPartLoadRatioCurveName,
                            _curve->nameString());
      }
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
