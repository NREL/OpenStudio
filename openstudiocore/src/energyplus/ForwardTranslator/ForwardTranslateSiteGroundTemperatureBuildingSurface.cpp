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

#include "../../model/SiteGroundTemperatureBuildingSurface.hpp"
#include "../../model/SiteGroundTemperatureBuildingSurface_Impl.hpp"

#include <utilities/idd/Site_GroundTemperature_BuildingSurface_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSiteGroundTemperatureBuildingSurface( SiteGroundTemperatureBuildingSurface& modelObject )
{
  IdfObject idfObject( openstudio::IddObjectType::Site_GroundTemperature_BuildingSurface );

  m_idfObjects.push_back(idfObject);

  if (!modelObject.isJanuaryGroundTemperatureDefaulted()){
    idfObject.setDouble(Site_GroundTemperature_BuildingSurfaceFields::JanuaryGroundTemperature, modelObject.januaryGroundTemperature()); 
  }

  if (!modelObject.isFebruaryGroundTemperatureDefaulted()){
    idfObject.setDouble(Site_GroundTemperature_BuildingSurfaceFields::FebruaryGroundTemperature, modelObject.februaryGroundTemperature()); 
  }

  if (!modelObject.isMarchGroundTemperatureDefaulted()){
    idfObject.setDouble(Site_GroundTemperature_BuildingSurfaceFields::MarchGroundTemperature, modelObject.marchGroundTemperature()); 
  }

  if (!modelObject.isAprilGroundTemperatureDefaulted()){
    idfObject.setDouble(Site_GroundTemperature_BuildingSurfaceFields::AprilGroundTemperature, modelObject.aprilGroundTemperature()); 
  }

  if (!modelObject.isMayGroundTemperatureDefaulted()){
    idfObject.setDouble(Site_GroundTemperature_BuildingSurfaceFields::MayGroundTemperature, modelObject.mayGroundTemperature()); 
  }

  if (!modelObject.isJuneGroundTemperatureDefaulted()){
    idfObject.setDouble(Site_GroundTemperature_BuildingSurfaceFields::JuneGroundTemperature, modelObject.juneGroundTemperature()); 
  }

  if (!modelObject.isJulyGroundTemperatureDefaulted()){
    idfObject.setDouble(Site_GroundTemperature_BuildingSurfaceFields::JulyGroundTemperature, modelObject.julyGroundTemperature()); 
  }

  if (!modelObject.isAugustGroundTemperatureDefaulted()){
    idfObject.setDouble(Site_GroundTemperature_BuildingSurfaceFields::AugustGroundTemperature, modelObject.augustGroundTemperature()); 
  }

  if (!modelObject.isSeptemberGroundTemperatureDefaulted()){
    idfObject.setDouble(Site_GroundTemperature_BuildingSurfaceFields::SeptemberGroundTemperature, modelObject.septemberGroundTemperature()); 
  }

  if (!modelObject.isOctoberGroundTemperatureDefaulted()){
    idfObject.setDouble(Site_GroundTemperature_BuildingSurfaceFields::OctoberGroundTemperature, modelObject.octoberGroundTemperature()); 
  }

  if (!modelObject.isNovemberGroundTemperatureDefaulted()){
    idfObject.setDouble(Site_GroundTemperature_BuildingSurfaceFields::NovemberGroundTemperature, modelObject.novemberGroundTemperature()); 
  }

  if (!modelObject.isDecemberGroundTemperatureDefaulted()){
    idfObject.setDouble(Site_GroundTemperature_BuildingSurfaceFields::DecemberGroundTemperature, modelObject.decemberGroundTemperature()); 
  }

  return idfObject;
}

} // energyplus

} // openstudio

