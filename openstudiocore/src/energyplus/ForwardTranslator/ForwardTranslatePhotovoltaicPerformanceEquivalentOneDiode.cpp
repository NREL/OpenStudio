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
#include "../../model/PhotovoltaicPerformanceEquivalentOneDiode.hpp"
#include "../../model/PhotovoltaicPerformanceEquivalentOneDiode_Impl.hpp"

#include <utilities/idd/PhotovoltaicPerformance_EquivalentOneDiode_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translatePhotovoltaicPerformanceEquivalentOneDiode(model::PhotovoltaicPerformanceEquivalentOneDiode & modelObject)
{
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::PhotovoltaicPerformance_EquivalentOneDiode, modelObject);

  if (modelObject.name()) {
    idfObject.setString(PhotovoltaicPerformance_EquivalentOneDiodeFields::Name, modelObject.name().get());
  }

  idfObject.setString(PhotovoltaicPerformance_EquivalentOneDiodeFields::Celltype, modelObject.celltype());

  idfObject.setInt(PhotovoltaicPerformance_EquivalentOneDiodeFields::NumberofCellsinSeries, modelObject.numberOfCellsInSeries());

  idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::ActiveArea, modelObject.activeArea());

  idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::TransmittanceAbsorptanceProduct, modelObject.transmittanceAbsorptanceProduct());

  idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::SemiconductorBandgap, modelObject.semiconductorBandgap());

  idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::ShuntResistance, modelObject.shuntResistance());

  idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::ShortCircuitCurrent, modelObject.shortCircuitCurrent());

  idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::OpenCircuitVoltage, modelObject.openCircuitVoltage());

  idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::ReferenceTemperature, modelObject.referenceTemperature());

  idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::ReferenceInsolation, modelObject.referenceInsolation());

  idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::ModuleCurrentatMaximumPower, modelObject.moduleCurrentatMaximumPower());

  idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::ModuleVoltageatMaximumPower, modelObject.moduleVoltageatMaximumPower());

  idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::TemperatureCoefficientofShortCircuitCurrent, modelObject.temperatureCoefficientofShortCircuitCurrent());

  idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::TemperatureCoefficientofOpenCircuitVoltage, modelObject.temperatureCoefficientofOpenCircuitVoltage());

  idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::NominalOperatingCellTemperatureTestAmbientTemperature, modelObject.nominalOperatingCellTemperatureTestAmbientTemperature());

  idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::NominalOperatingCellTemperatureTestCellTemperature, modelObject.nominalOperatingCellTemperatureTestCellTemperature());

  idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::NominalOperatingCellTemperatureTestInsolation, modelObject.nominalOperatingCellTemperatureTestInsolation());

  idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::ModuleHeatLossCoefficient, modelObject.moduleHeatLossCoefficient());

  idfObject.setDouble(PhotovoltaicPerformance_EquivalentOneDiodeFields::TotalHeatCapacity, modelObject.totalHeatCapacity());

  return idfObject;
}

} // energyplus

} // openstudio

