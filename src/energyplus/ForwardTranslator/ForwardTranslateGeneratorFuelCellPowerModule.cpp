/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/GeneratorFuelCellPowerModule.hpp"
#include "../../model/GeneratorFuelCellPowerModule_Impl.hpp"

#include "../../model/CurveQuadratic.hpp"
#include "../../model/CurveQuadratic_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Generator_FuelCell_PowerModule_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateGeneratorFuelCellPowerModule(GeneratorFuelCellPowerModule & modelObject)
{
  boost::optional<std::string> s;
  boost::optional<double> d;
  boost::optional<Node> node;
  boost::optional<CurveQuadratic> curve;
  boost::optional<ThermalZone> tz;

  IdfObject pcm = createAndRegisterIdfObject(openstudio::IddObjectType::Generator_FuelCell_PowerModule, modelObject);
  //Name
  s = modelObject.name();
  if (s) {
    pcm.setName(*s);
  }

  //EfficiencyCurveMode
  s = modelObject.efficiencyCurveMode();
  if (s) {
    pcm.setString(Generator_FuelCell_PowerModuleFields::EfficiencyCurveMode, s.get());
  }

  //EfficiencyCurveName
  curve = modelObject.efficiencyCurve();
  if (curve) {
    pcm.setString(Generator_FuelCell_PowerModuleFields::EfficiencyCurveName, curve.get().nameString());
  }

  //NominalEfficiency
  d = modelObject.nominalEfficiency();
  if (d) {
    pcm.setDouble(Generator_FuelCell_PowerModuleFields::NominalEfficiency, d.get());
  }

  //NominalElectricalPower
  d = modelObject.nominalElectricalPower();
  if (d) {
    pcm.setDouble(Generator_FuelCell_PowerModuleFields::NominalElectricalPower, d.get());
  }

  //NumberofStopsatStartofSimulation
  d = modelObject.numberofStopsatStartofSimulation();
  if (d) {
    pcm.setDouble(Generator_FuelCell_PowerModuleFields::NumberofStopsatStartofSimulation, d.get());
  }

  //CyclingPerformanceDegradationCoefficient
  d = modelObject.cyclingPerformanceDegradationCoefficient();
  if (d) {
    pcm.setDouble(Generator_FuelCell_PowerModuleFields::CyclingPerformanceDegradationCoefficient, d.get());
  }

  //NumberofRunHoursatBeginningofSimulation
  d = modelObject.numberofRunHoursatBeginningofSimulation();
  if (d) {
    pcm.setDouble(Generator_FuelCell_PowerModuleFields::NumberofRunHoursatBeginningofSimulation, d.get());
  }

  //AccumulatedRunTimeDegradationCoefficient
  d = modelObject.accumulatedRunTimeDegradationCoefficient();
  if (d) {
    pcm.setDouble(Generator_FuelCell_PowerModuleFields::AccumulatedRunTimeDegradationCoefficient, d.get());
  }

  //RunTimeDegradationInitiationTimeThreshold
  d = modelObject.runTimeDegradationInitiationTimeThreshold();
  if (d) {
    pcm.setDouble(Generator_FuelCell_PowerModuleFields::RunTimeDegradationInitiationTimeThreshold, d.get());
  }

  //PowerUpTransientLimit
  d = modelObject.powerUpTransientLimit();
  if (d) {
    pcm.setDouble(Generator_FuelCell_PowerModuleFields::PowerUpTransientLimit, d.get());
  }

  //PowerDownTransientLimit
  d = modelObject.powerDownTransientLimit();
  if (d) {
    pcm.setDouble(Generator_FuelCell_PowerModuleFields::PowerDownTransientLimit, d.get());
  }

  //StartUpTime
  d = modelObject.startUpTime();
  if (d) {
    pcm.setDouble(Generator_FuelCell_PowerModuleFields::StartUpTime, d.get());
  }

  //StartUpFuel
  d = modelObject.startUpFuel();
  if (d) {
    pcm.setDouble(Generator_FuelCell_PowerModuleFields::StartUpFuel, d.get());
  }

  //StartUpElectricityConsumption
  d = modelObject.startUpElectricityConsumption();
  if (d) {
    pcm.setDouble(Generator_FuelCell_PowerModuleFields::StartUpElectricityConsumption, d.get());
  }

  //StartUpElectricityProduced
  d = modelObject.startUpElectricityProduced();
  if (d) {
    pcm.setDouble(Generator_FuelCell_PowerModuleFields::StartUpElectricityProduced, d.get());
  }

  //ShutDownTime
  d = modelObject.shutDownTime();
  if (d) {
    pcm.setDouble(Generator_FuelCell_PowerModuleFields::ShutDownTime, d.get());
  }

  //ShutDownFuel
  d = modelObject.shutDownFuel();
  if (d) {
    pcm.setDouble(Generator_FuelCell_PowerModuleFields::ShutDownFuel, d.get());
  }

  //ShutDownElectricityConsumption
  d = modelObject.shutDownElectricityConsumption();
  if (d) {
    pcm.setDouble(Generator_FuelCell_PowerModuleFields::ShutDownElectricityConsumption, d.get());
  }

  //AncillaryElectricityConstantTerm
  d = modelObject.ancillaryElectricityConstantTerm();
  if (d) {
    pcm.setDouble(Generator_FuelCell_PowerModuleFields::AncillaryElectricityConstantTerm, d.get());
  }

  //AncillaryElectricityLinearTerm
  d = modelObject.ancillaryElectricityLinearTerm();
  if (d) {
    pcm.setDouble(Generator_FuelCell_PowerModuleFields::AncillaryElectricityLinearTerm, d.get());
  }

  //SkinLossCalculationMode
  s = modelObject.skinLossCalculationMode();
  if (s) {
    pcm.setString(Generator_FuelCell_PowerModuleFields::SkinLossCalculationMode, s.get());
  }

  //ZoneName
  tz = modelObject.zone();
  if (tz) {
    pcm.setString(Generator_FuelCell_PowerModuleFields::ZoneName, tz.get().nameString());
  }

  //skinLossRadiativeFraction
  d = modelObject.skinLossRadiativeFraction();
  if (d) {
    pcm.setDouble(Generator_FuelCell_PowerModuleFields::SkinLossRadiativeFraction, d.get());
  }

  //ConstantSkinLossRate
  d = modelObject.constantSkinLossRate();
  if (d) {
    pcm.setDouble(Generator_FuelCell_PowerModuleFields::ConstantSkinLossRate, d.get());
  }

  //SkinLossUFactorTimesAreaTerm
  d = modelObject.skinLossUFactorTimesAreaTerm();
  if (d) {
    pcm.setDouble(Generator_FuelCell_PowerModuleFields::SkinLossUFactorTimesAreaTerm, d.get());
  }

  //SkinLossQuadraticCurveName
  curve = modelObject.skinLossQuadraticCurve();
  if (curve) {
    pcm.setString(Generator_FuelCell_PowerModuleFields::SkinLossQuadraticCurveName, curve.get().nameString());
  }

  //DilutionAirFlowRate
  d = modelObject.dilutionAirFlowRate();
  if (d) {
    pcm.setDouble(Generator_FuelCell_PowerModuleFields::DilutionAirFlowRate, d.get());
  }

  //StackHeatlosstoDilutionAir
  d = modelObject.stackHeatlosstoDilutionAir();
  if (d) {
    pcm.setDouble(Generator_FuelCell_PowerModuleFields::StackHeatlosstoDilutionAir, d.get());
  }

  //DilutionInletAirNodeName
  node = modelObject.dilutionInletAirNode();
  if (node) {
    pcm.setString(Generator_FuelCell_PowerModuleFields::DilutionInletAirNodeName, node.get().nameString());
  //} else if( tz ) {
  //  pcm.setString(Generator_FuelCell_PowerModuleFields::DilutionInletAirNodeName, tz->zoneAirNode().nameString());
  } else {
    auto name = modelObject.nameString() + " OA Node";
    IdfObject oaNodeListIdf(openstudio::IddObjectType::OutdoorAir_NodeList);
    oaNodeListIdf.setString(0, name);
    m_idfObjects.push_back(oaNodeListIdf);

    pcm.setString(Generator_FuelCell_PowerModuleFields::DilutionInletAirNodeName,name);
  }

  //DilutionOutletAirNodeName
  node = modelObject.dilutionOutletAirNode();
  if (node) {
    pcm.setString(Generator_FuelCell_PowerModuleFields::DilutionOutletAirNodeName, node.get().nameString());
  } else {
    pcm.setString(Generator_FuelCell_PowerModuleFields::DilutionOutletAirNodeName, modelObject.nameString() + " Dilution Outlet Air Node");
  }

  //MinimumOperatingPoint
  d = modelObject.minimumOperatingPoint();
  if (d) {
    pcm.setDouble(Generator_FuelCell_PowerModuleFields::MinimumOperatingPoint, d.get());
  }

  //MaximumOperatingPoint
  d = modelObject.maximumOperatingPoint();
  if (d) {
    pcm.setDouble(Generator_FuelCell_PowerModuleFields::MaximumOperatingPoint, d.get());
  }

  return pcm;

}

} // energyplus

} // openstudio

