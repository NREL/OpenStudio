/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <energyplus/ForwardTranslator.hpp>

#include <model/CurveFunctionalPressureDrop.hpp>
#include <model/CurveFunctionalPressureDrop_Impl.hpp>

#include <utilities/core/Assert.hpp>

#include <utilities/idd/Curve_Functional_PressureDrop_FieldEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {
namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateCurveFunctionalPressureDrop(CurveFunctionalPressureDrop& modelObject)
{
  IdfObject idfObject(IddObjectType::Curve_Functional_PressureDrop);

  m_idfObjects.push_back(idfObject);

  OptionalString s;
  OptionalDouble d;

  if((s = modelObject.name())) {
    idfObject.setName(*s);
  }

  idfObject.setDouble(Curve_Functional_PressureDropFields::Diameter,modelObject.diameter());
  if ((d = modelObject.minorLossCoefficient())) {
    idfObject.setDouble(Curve_Functional_PressureDropFields::MinorLossCoefficient,*d);
  }
  if ((d = modelObject.length())) {
    idfObject.setDouble(Curve_Functional_PressureDropFields::Length,*d);
  }
  if ((d = modelObject.roughness())) {
    idfObject.setDouble(Curve_Functional_PressureDropFields::Roughness,*d);
  }
  if ((d = modelObject.fixedFrictionFactor())) {
    idfObject.setDouble(Curve_Functional_PressureDropFields::FixedFrictionFactor,*d);
  }

  return idfObject;
}

} // energyplus
} // openstudio

