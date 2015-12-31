/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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

