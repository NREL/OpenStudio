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
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/CoilCoolingWaterToAirHeatPumpEquationFit.hpp"
#include "../../model/CoilCoolingWaterToAirHeatPumpEquationFit_Impl.hpp"
#include "../../model/CurveQuadLinear.hpp"
#include "../../model/CurveQuintLinear.hpp"

#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"

#include <utilities/idd/Coil_Cooling_WaterToAirHeatPump_EquationFit_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

//using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateCoilCoolingWaterToAirHeatPumpEquationFit(CoilCoolingWaterToAirHeatPumpEquationFit& modelObject) {
    //setup boost optionals to use to store get method returns
    boost::optional<std::string> s;
    boost::optional<double> value;
    boost::optional<Node> node;

    // Make sure the modelObject gets ut into the map, and the new idfObject gets put into the final file.
    // Also sets the idfObjects name

    IdfObject idfObject = createRegisterAndNameIdfObject(IddObjectType::Coil_Cooling_WaterToAirHeatPump_EquationFit, modelObject);

    // Object Name
    //std::string baseName = idfObject.name().get();

    //  A3 ,Field Water Inlet Node Name

    if (boost::optional<ModelObject> mo = modelObject.waterInletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::WaterInletNodeName, node->name().get());
      }
    }

    // A4, Field Water Outlet Node Name

    if (boost::optional<ModelObject> mo = modelObject.waterOutletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::WaterOutletNodeName, node->name().get());
      }
    }

    // A5, Field Air Inlet Node Name

    if (boost::optional<ModelObject> mo = modelObject.airInletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::AirInletNodeName, node->name().get());
      }
    }

    // A6 , \field Air Outlet Node Name

    if (boost::optional<ModelObject> mo = modelObject.airOutletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        idfObject.setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::AirOutletNodeName, node->name().get());
      }
    }

    // N1 Field Rated Air Flow Rate

    value = modelObject.ratedAirFlowRate();

    if (value) {
      idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate, *value);
    }

    else {
      idfObject.setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedAirFlowRate, "Autosize");
    }

    //  N2 Rated Water Flow Rate

    value = modelObject.ratedWaterFlowRate();

    if (value) {
      idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate, *value);
    }

    else {
      idfObject.setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatedWaterFlowRate, "Autosize");
    }

    // N3,  Field Rated Total Cooling Capacity

    value = modelObject.ratedTotalCoolingCapacity();

    if (value) {
      idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::GrossRatedTotalCoolingCapacity, *value);
    }

    else {
      idfObject.setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::GrossRatedTotalCoolingCapacity, "Autosize");
    }

    // N4,  Field Rated Sensible Cooling Capacity

    value = modelObject.ratedSensibleCoolingCapacity();

    if (value) {
      idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::GrossRatedSensibleCoolingCapacity, *value);
    }

    else {
      idfObject.setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::GrossRatedSensibleCoolingCapacity, "Autosize");
    }

    //   N5, Field Rated Cooling Coefficient of Performance

    value = modelObject.ratedCoolingCoefficientofPerformance();

    if (value) {
      idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::GrossRatedCoolingCOP, *value);
    }

    // Total Cooling Capacity Curve Name
    {
      auto curve = modelObject.totalCoolingCapacityCurve();
      if (auto _curve = translateAndMapModelObject(curve)) {
        idfObject.setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::TotalCoolingCapacityCurveName, _curve->nameString());
      }
    }

    // Sensible Cooling Capacity Curve Name
    {
      auto curve = modelObject.sensibleCoolingCapacityCurve();
      if (auto _curve = translateAndMapModelObject(curve)) {
        idfObject.setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::SensibleCoolingCapacityCurveName, _curve->nameString());
      }
    }

    // Cooling Power Consumption Curve Name
    {
      auto curve = modelObject.coolingPowerConsumptionCurve();
      if (auto _curve = translateAndMapModelObject(curve)) {
        idfObject.setString(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::CoolingPowerConsumptionCurveName, _curve->nameString());
      }
    }

    // N22, Field Nominal Time for Condensate Removal to Begin

    value = modelObject.nominalTimeforCondensateRemovaltoBegin();

    if (value) {
      idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::NominalTimeforCondensateRemovaltoBegin, *value);
    }

    //  N23, Field Ratio of Initial Moisture Evaporation Rate and Steady State Latent

    value = modelObject.ratioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity();

    if (value) {
      idfObject.setDouble(Coil_Cooling_WaterToAirHeatPump_EquationFitFields::RatioofInitialMoistureEvaporationRateandSteadyStateLatentCapacity,
                          *value);
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
