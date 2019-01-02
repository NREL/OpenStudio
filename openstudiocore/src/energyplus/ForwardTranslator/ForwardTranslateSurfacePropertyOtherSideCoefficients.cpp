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
#include "../../model/SurfacePropertyOtherSideCoefficients.hpp"
#include "../../model/SurfacePropertyOtherSideCoefficients_Impl.hpp"
#include "../../model/Schedule.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/SurfaceProperty_OtherSideCoefficients_FieldEnums.hxx>

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSurfacePropertyOtherSideCoefficients(model::SurfacePropertyOtherSideCoefficients & modelObject)
{
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SurfaceProperty_OtherSideCoefficients,
                                                       modelObject);

  boost::optional<double> combinedConvectiveRadiativeFilmCoefficient = modelObject.combinedConvectiveRadiativeFilmCoefficient();

  double constantTemperature = modelObject.constantTemperature();
  double constantTemperatureCoefficient = modelObject.constantTemperatureCoefficient();
  double externalDryBulbTemperatureCoefficient = modelObject.externalDryBulbTemperatureCoefficient();
  double groundTemperatureCoefficient = modelObject.groundTemperatureCoefficient();
  double windSpeedCoefficient = modelObject.windSpeedCoefficient();
  double zoneAirTemperatureCoefficient = modelObject.zoneAirTemperatureCoefficient();
  boost::optional<Schedule> constantTemperatureSchedule = modelObject.constantTemperatureSchedule();
  bool sinusoidalVariationofConstantTemperatureCoefficient = modelObject.sinusoidalVariationofConstantTemperatureCoefficient();
  double periodofSinusoidalVariation = modelObject.periodofSinusoidalVariation();
  double previousOtherSideTemperatureCoefficient = modelObject.previousOtherSideTemperatureCoefficient();
  boost::optional<double> minimumOtherSideTemperatureLimit = modelObject.minimumOtherSideTemperatureLimit();
  boost::optional<double> maximumOtherSideTemperatureLimit = modelObject.maximumOtherSideTemperatureLimit();

  if (combinedConvectiveRadiativeFilmCoefficient){
    idfObject.setDouble(SurfaceProperty_OtherSideCoefficientsFields::CombinedConvective_RadiativeFilmCoefficient, *combinedConvectiveRadiativeFilmCoefficient);
  } else{
    // required in E+, this indicates that the coefficient should be computed as a function of the other variables
    idfObject.setDouble(SurfaceProperty_OtherSideCoefficientsFields::CombinedConvective_RadiativeFilmCoefficient, 0.0);
  }

  bool isConstantTemperatureScheduled = false;
  if (constantTemperatureSchedule){
    boost::optional<IdfObject> sch = translateAndMapModelObject(*constantTemperatureSchedule);
    if (sch && sch->name()){
      isConstantTemperatureScheduled = idfObject.setString(SurfaceProperty_OtherSideCoefficientsFields::ConstantTemperatureScheduleName, sch->name().get());
    }
  }

  if (!isConstantTemperatureScheduled){
    idfObject.setDouble(SurfaceProperty_OtherSideCoefficientsFields::ConstantTemperature, constantTemperature);
  }

  idfObject.setDouble(SurfaceProperty_OtherSideCoefficientsFields::ConstantTemperatureCoefficient, constantTemperatureCoefficient);
  idfObject.setDouble(SurfaceProperty_OtherSideCoefficientsFields::ExternalDryBulbTemperatureCoefficient, externalDryBulbTemperatureCoefficient);
  idfObject.setDouble(SurfaceProperty_OtherSideCoefficientsFields::GroundTemperatureCoefficient, groundTemperatureCoefficient);
  idfObject.setDouble(SurfaceProperty_OtherSideCoefficientsFields::WindSpeedCoefficient, windSpeedCoefficient);
  idfObject.setDouble(SurfaceProperty_OtherSideCoefficientsFields::ZoneAirTemperatureCoefficient, zoneAirTemperatureCoefficient);

  if (!isConstantTemperatureScheduled){
    if (sinusoidalVariationofConstantTemperatureCoefficient){
      idfObject.setString(SurfaceProperty_OtherSideCoefficientsFields::SinusoidalVariationofConstantTemperatureCoefficient, "Yes");
    } else{
      idfObject.setString(SurfaceProperty_OtherSideCoefficientsFields::SinusoidalVariationofConstantTemperatureCoefficient, "No");
    }
    idfObject.setDouble(SurfaceProperty_OtherSideCoefficientsFields::PeriodofSinusoidalVariation, periodofSinusoidalVariation);
  }

  idfObject.setDouble(SurfaceProperty_OtherSideCoefficientsFields::PreviousOtherSideTemperatureCoefficient, previousOtherSideTemperatureCoefficient);

  if (minimumOtherSideTemperatureLimit){
    idfObject.setDouble(SurfaceProperty_OtherSideCoefficientsFields::MinimumOtherSideTemperatureLimit, *minimumOtherSideTemperatureLimit);
  }

  if (maximumOtherSideTemperatureLimit){
    idfObject.setDouble(SurfaceProperty_OtherSideCoefficientsFields::MaximumOtherSideTemperatureLimit, *maximumOtherSideTemperatureLimit);
  }

  return idfObject;
}

} // energyplus

} // openstudio

