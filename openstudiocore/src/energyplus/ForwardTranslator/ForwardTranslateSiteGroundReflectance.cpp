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

#include "../../model/SiteGroundReflectance.hpp"
#include "../../model/SiteGroundReflectance_Impl.hpp"

#include <utilities/idd/Site_GroundReflectance_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSiteGroundReflectance( SiteGroundReflectance& modelObject )
{
  IdfObject idfObject( openstudio::IddObjectType::Site_GroundReflectance );

  m_idfObjects.push_back(idfObject);

  if (!modelObject.isJanuaryGroundReflectanceDefaulted()){
    idfObject.setDouble(Site_GroundReflectanceFields::JanuaryGroundReflectance, modelObject.januaryGroundReflectance()); 
  }

  if (!modelObject.isFebruaryGroundReflectanceDefaulted()){
    idfObject.setDouble(Site_GroundReflectanceFields::FebruaryGroundReflectance, modelObject.februaryGroundReflectance()); 
  }

  if (!modelObject.isMarchGroundReflectanceDefaulted()){
    idfObject.setDouble(Site_GroundReflectanceFields::MarchGroundReflectance, modelObject.marchGroundReflectance()); 
  }

  if (!modelObject.isAprilGroundReflectanceDefaulted()){
    idfObject.setDouble(Site_GroundReflectanceFields::AprilGroundReflectance, modelObject.aprilGroundReflectance()); 
  }

  if (!modelObject.isMayGroundReflectanceDefaulted()){
    idfObject.setDouble(Site_GroundReflectanceFields::MayGroundReflectance, modelObject.mayGroundReflectance()); 
  }

  if (!modelObject.isJuneGroundReflectanceDefaulted()){
    idfObject.setDouble(Site_GroundReflectanceFields::JuneGroundReflectance, modelObject.juneGroundReflectance()); 
  }

  if (!modelObject.isJulyGroundReflectanceDefaulted()){
    idfObject.setDouble(Site_GroundReflectanceFields::JulyGroundReflectance, modelObject.julyGroundReflectance()); 
  }

  if (!modelObject.isAugustGroundReflectanceDefaulted()){
    idfObject.setDouble(Site_GroundReflectanceFields::AugustGroundReflectance, modelObject.augustGroundReflectance()); 
  }

  if (!modelObject.isSeptemberGroundReflectanceDefaulted()){
    idfObject.setDouble(Site_GroundReflectanceFields::SeptemberGroundReflectance, modelObject.septemberGroundReflectance()); 
  }

  if (!modelObject.isOctoberGroundReflectanceDefaulted()){
    idfObject.setDouble(Site_GroundReflectanceFields::OctoberGroundReflectance, modelObject.octoberGroundReflectance()); 
  }

  if (!modelObject.isNovemberGroundReflectanceDefaulted()){
    idfObject.setDouble(Site_GroundReflectanceFields::NovemberGroundReflectance, modelObject.novemberGroundReflectance()); 
  }

  if (!modelObject.isDecemberGroundReflectanceDefaulted()){
    idfObject.setDouble(Site_GroundReflectanceFields::DecemberGroundReflectance, modelObject.decemberGroundReflectance()); 
  }

  return idfObject;
}

} // energyplus

} // openstudio

