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

#include "../../model/SiteGroundTemperatureDeep.hpp"
#include "../../model/SiteGroundTemperatureDeep_Impl.hpp"

#include <utilities/idd/Site_GroundTemperature_Deep_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSiteGroundTemperatureDeep( SiteGroundTemperatureDeep& modelObject )
{
  IdfObject idfObject( openstudio::IddObjectType::Site_GroundTemperature_Deep );

  m_idfObjects.push_back(idfObject);

  idfObject.setDouble(Site_GroundTemperature_DeepFields::JanuaryDeepGroundTemperature, modelObject.januaryDeepGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_DeepFields::FebruaryDeepGroundTemperature, modelObject.februaryDeepGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_DeepFields::MarchDeepGroundTemperature, modelObject.marchDeepGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_DeepFields::AprilDeepGroundTemperature, modelObject.aprilDeepGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_DeepFields::MayDeepGroundTemperature, modelObject.mayDeepGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_DeepFields::JuneDeepGroundTemperature, modelObject.juneDeepGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_DeepFields::JulyDeepGroundTemperature, modelObject.julyDeepGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_DeepFields::AugustDeepGroundTemperature, modelObject.augustDeepGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_DeepFields::SeptemberDeepGroundTemperature, modelObject.septemberDeepGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_DeepFields::OctoberGroundTemperature, modelObject.octoberDeepGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_DeepFields::NovemberDeepGroundTemperature, modelObject.novemberDeepGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_DeepFields::DecemberDeepGroundTemperature, modelObject.decemberDeepGroundTemperature());

  return idfObject;
}

} // energyplus

} // openstudio
