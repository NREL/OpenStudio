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

#include "../ForwardTranslator.hpp"

#include "../../model/SiteGroundTemperatureUndisturbedXing.hpp"
#include "../../model/SiteGroundTemperatureUndisturbedXing_Impl.hpp"

#include <utilities/idd/Site_GroundTemperature_Undisturbed_Xing_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateSiteGroundTemperatureUndisturbedXing(SiteGroundTemperatureUndisturbedXing& modelObject) {
    boost::optional<double> value;

    // Name
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Site_GroundTemperature_Undisturbed_Xing, modelObject);

    // SoilThermalConductivity
    idfObject.setDouble(Site_GroundTemperature_Undisturbed_XingFields::SoilThermalConductivity, modelObject.soilThermalConductivity());

    // SoilDensity
    idfObject.setDouble(Site_GroundTemperature_Undisturbed_XingFields::SoilDensity, modelObject.soilDensity());

    // SoilSpecificHeat
    idfObject.setDouble(Site_GroundTemperature_Undisturbed_XingFields::SoilSpecificHeat, modelObject.soilSpecificHeat());

    // AverageSoilSurfaceTemperature
    if ((value = modelObject.averageSoilSurfaceTemperature())) {
      idfObject.setDouble(Site_GroundTemperature_Undisturbed_XingFields::AverageSoilSurfaceTemperature, value.get());
    }

    // SoilSurfaceTemperatureAmplitude1
    if ((value = modelObject.soilSurfaceTemperatureAmplitude1())) {
      idfObject.setDouble(Site_GroundTemperature_Undisturbed_XingFields::SoilSurfaceTemperatureAmplitude1, value.get());
    }

    // SoilSurfaceTemperatureAmplitude2
    if ((value = modelObject.soilSurfaceTemperatureAmplitude2())) {
      idfObject.setDouble(Site_GroundTemperature_Undisturbed_XingFields::SoilSurfaceTemperatureAmplitude2, value.get());
    }

    // PhaseShiftofTemperatureAmplitude1
    if ((value = modelObject.phaseShiftofTemperatureAmplitude1())) {
      idfObject.setDouble(Site_GroundTemperature_Undisturbed_XingFields::PhaseShiftofTemperatureAmplitude1, value.get());
    }

    // PhaseShiftofTemperatureAmplitude2
    if ((value = modelObject.phaseShiftofTemperatureAmplitude2())) {
      idfObject.setDouble(Site_GroundTemperature_Undisturbed_XingFields::PhaseShiftofTemperatureAmplitude2, value.get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
