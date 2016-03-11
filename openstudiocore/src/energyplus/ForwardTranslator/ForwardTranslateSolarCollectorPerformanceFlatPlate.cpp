/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "../../model/SolarCollectorPerformanceFlatPlate.hpp"

#include <utilities/idd/SolarCollectorPerformance_FlatPlate_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSolarCollectorPerformanceFlatPlate(model::SolarCollectorPerformanceFlatPlate & modelObject)
{
  IdfObject idfObject(openstudio::IddObjectType::SolarCollectorPerformance_FlatPlate);

  m_idfObjects.push_back(idfObject);

  boost::optional<double> d;

  idfObject.setString(SolarCollectorPerformance_FlatPlateFields::Name, modelObject.name().get());

  idfObject.setDouble(SolarCollectorPerformance_FlatPlateFields::GrossArea, modelObject.grossArea());

  idfObject.setString(SolarCollectorPerformance_FlatPlateFields::TestFluid, modelObject.testFluid());

  idfObject.setDouble(SolarCollectorPerformance_FlatPlateFields::TestFlowRate, modelObject.testFlowRate());

  idfObject.setString(SolarCollectorPerformance_FlatPlateFields::TestCorrelationType, modelObject.testCorrelationType());

  idfObject.setDouble(SolarCollectorPerformance_FlatPlateFields::Coefficient1ofEfficiencyEquation, modelObject.coefficient1ofEfficiencyEquation());

  idfObject.setDouble(SolarCollectorPerformance_FlatPlateFields::Coefficient2ofEfficiencyEquation, modelObject.coefficient2ofEfficiencyEquation());

  d = modelObject.coefficient3ofEfficiencyEquation();
  if (d){
    idfObject.setDouble(SolarCollectorPerformance_FlatPlateFields::Coefficient3ofEfficiencyEquation, *d);
  }

  d = modelObject.coefficient2ofIncidentAngleModifier();
  if (d){
    idfObject.setDouble(SolarCollectorPerformance_FlatPlateFields::Coefficient2ofIncidentAngleModifier, *d);
  }

  d = modelObject.coefficient3ofIncidentAngleModifier();
  if (d){
    idfObject.setDouble(SolarCollectorPerformance_FlatPlateFields::Coefficient3ofIncidentAngleModifier, *d);
  }

  return idfObject;
}

} // energyplus

} // openstudio

