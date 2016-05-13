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

#include "../../model/SiteGroundTemperatureShallow.hpp"
#include "../../model/SiteGroundTemperatureShallow_Impl.hpp"

#include <utilities/idd/Site_GroundTemperature_Shallow_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSiteGroundTemperatureShallow( SiteGroundTemperatureShallow& modelObject )
{
  IdfObject idfObject( openstudio::IddObjectType::Site_GroundTemperature_Shallow );

  m_idfObjects.push_back(idfObject);

  idfObject.setDouble(Site_GroundTemperature_ShallowFields::JanuarySurfaceGroundTemperature, modelObject.januarySurfaceGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_ShallowFields::FebruarySurfaceGroundTemperature, modelObject.februarySurfaceGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_ShallowFields::MarchSurfaceGroundTemperature, modelObject.marchSurfaceGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_ShallowFields::AprilSurfaceGroundTemperature, modelObject.aprilSurfaceGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_ShallowFields::MaySurfaceGroundTemperature, modelObject.maySurfaceGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_ShallowFields::JuneSurfaceGroundTemperature, modelObject.juneSurfaceGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_ShallowFields::JulySurfaceGroundTemperature, modelObject.julySurfaceGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_ShallowFields::AugustSurfaceGroundTemperature, modelObject.augustSurfaceGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_ShallowFields::SeptemberSurfaceGroundTemperature, modelObject.septemberSurfaceGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_ShallowFields::OctoberGroundTemperature, modelObject.octoberSurfaceGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_ShallowFields::NovemberSurfaceGroundTemperature, modelObject.novemberSurfaceGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_ShallowFields::DecemberSurfaceGroundTemperature, modelObject.decemberSurfaceGroundTemperature());

  return idfObject;
}

} // energyplus

} // openstudio
