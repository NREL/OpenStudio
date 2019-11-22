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
#include "../../model/GeneratorFuelCellStackCooler.hpp"
#include "../../model/GeneratorFuelCellStackCooler_Impl.hpp"

#include "../../model/Connection.hpp"
#include "../../model/Connection_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Generator_FuelCell_StackCooler_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateGeneratorFuelCellStackCooler(GeneratorFuelCellStackCooler & modelObject)
{
  boost::optional<std::string> s;
  boost::optional<double> d;
  boost::optional<Connection> connection;

  IdfObject pcm = createAndRegisterIdfObject(openstudio::IddObjectType::Generator_FuelCell_StackCooler, modelObject);
  //Name
  s = modelObject.name();
  if (s) {
    pcm.setName(*s);
  }

  // HeatRecoveryWaterInletNodeName

  if (boost::optional<ModelObject> mo = modelObject.inletModelObject()) {
    if (boost::optional<Node> node = mo->optionalCast<Node>()) {
      pcm.setString(Generator_FuelCell_StackCoolerFields::HeatRecoveryWaterInletNodeName, node->name().get());
    }
  }

  // HeatRecoveryWaterOutletNodeName

  if (boost::optional<ModelObject> mo = modelObject.outletModelObject()) {
    if (boost::optional<Node> node = mo->optionalCast<Node>()) {
      pcm.setString(Generator_FuelCell_StackCoolerFields::HeatRecoveryWaterOutletNodeName, node->name().get());
    }
  }
  /*
  //HeatRecoveryWaterInletNodeName
  connection = modelObject.heatRecoveryWaterInletNode();
  if (connection) {
    pcm.setString(Generator_FuelCell_StackCoolerFields::HeatRecoveryWaterInletNodeName, connection.get().nameString());
  }

  //HeatRecoveryWaterOutletNodeName
  connection = modelObject.heatRecoveryWaterOutletNode();
  if (connection) {
    pcm.setString(Generator_FuelCell_StackCoolerFields::HeatRecoveryWaterOutletNodeName, connection.get().nameString());
  }
  */
  //NominalStackTemperature
  d = modelObject.nominalStackTemperature();
  if (d) {
    pcm.setDouble(Generator_FuelCell_StackCoolerFields::NominalStackTemperature, d.get());
  }

  //ActualStackTemperature
  d = modelObject.actualStackTemperature();
  if (d) {
    pcm.setDouble(Generator_FuelCell_StackCoolerFields::ActualStackTemperature, d.get());
  }

  //Coefficientr0
  d = modelObject.coefficientr0();
  if (d) {
    pcm.setDouble(Generator_FuelCell_StackCoolerFields::Coefficientr0, d.get());
  }

  //Coefficientr1
  d = modelObject.coefficientr1();
  if (d) {
    pcm.setDouble(Generator_FuelCell_StackCoolerFields::Coefficientr1, d.get());
  }

  //Coefficientr2
  d = modelObject.coefficientr2();
  if (d) {
    pcm.setDouble(Generator_FuelCell_StackCoolerFields::Coefficientr2, d.get());
  }

  //Coefficientr3
  d = modelObject.coefficientr3();
  if (d) {
    pcm.setDouble(Generator_FuelCell_StackCoolerFields::Coefficientr3, d.get());
  }

  //StackCoolantFlowRate
  d = modelObject.stackCoolantFlowRate();
  if (d) {
    pcm.setDouble(Generator_FuelCell_StackCoolerFields::StackCoolantFlowRate, d.get());
  }

  //StackCoolerUFactorTimesAreaValue
  d = modelObject.stackCoolerUFactorTimesAreaValue();
  if (d) {
    pcm.setDouble(Generator_FuelCell_StackCoolerFields::StackCoolerUFactorTimesAreaValue, d.get());
  }

  //FscogenAdjustmentFactor
  d = modelObject.fscogenAdjustmentFactor();
  if (d) {
    pcm.setDouble(Generator_FuelCell_StackCoolerFields::FscogenAdjustmentFactor, d.get());
  }

  //StackCogenerationExchangerArea
  d = modelObject.stackCogenerationExchangerArea();
  if (d) {
    pcm.setDouble(Generator_FuelCell_StackCoolerFields::StackCogenerationExchangerArea, d.get());
  }

  //StackCogenerationExchangerNominalFlowRate
  d = modelObject.stackCogenerationExchangerNominalFlowRate();
  if (d) {
    pcm.setDouble(Generator_FuelCell_StackCoolerFields::StackCogenerationExchangerNominalFlowRate, d.get());
  }

  //StackCogenerationExchangerNominalHeatTransferCoefficient
  d = modelObject.stackCogenerationExchangerNominalHeatTransferCoefficient();
  if (d) {
    pcm.setDouble(Generator_FuelCell_StackCoolerFields::StackCogenerationExchangerNominalHeatTransferCoefficient, d.get());
  }

  //StackCogenerationExchangerNominalHeatTransferCoefficientExponent
  d = modelObject.stackCogenerationExchangerNominalHeatTransferCoefficientExponent();
  if (d) {
    pcm.setDouble(Generator_FuelCell_StackCoolerFields::StackCogenerationExchangerNominalHeatTransferCoefficientExponent, d.get());
  }

  //StackCoolerPumpPower
  d = modelObject.stackCoolerPumpPower();
  if (d) {
    pcm.setDouble(Generator_FuelCell_StackCoolerFields::StackCoolerPumpPower, d.get());
  }

  //StackCoolerPumpHeatLossFraction
  d = modelObject.stackCoolerPumpHeatLossFraction();
  if (d) {
    pcm.setDouble(Generator_FuelCell_StackCoolerFields::StackCoolerPumpHeatLossFraction, d.get());
  }

  //StackAirCoolerFanCoefficientf0
  d = modelObject.stackAirCoolerFanCoefficientf0();
  if (d) {
    pcm.setDouble(Generator_FuelCell_StackCoolerFields::StackAirCoolerFanCoefficientf0, d.get());
  }

  //StackAirCoolerFanCoefficientf1
  d = modelObject.stackAirCoolerFanCoefficientf1();
  if (d) {
    pcm.setDouble(Generator_FuelCell_StackCoolerFields::StackAirCoolerFanCoefficientf1, d.get());
  }

  //StackAirCoolerFanCoefficientf2
  d = modelObject.stackAirCoolerFanCoefficientf2();
  if (d) {
    pcm.setDouble(Generator_FuelCell_StackCoolerFields::StackAirCoolerFanCoefficientf2, d.get());
  }

  return pcm;

}

} // energyplus

} // openstudio

