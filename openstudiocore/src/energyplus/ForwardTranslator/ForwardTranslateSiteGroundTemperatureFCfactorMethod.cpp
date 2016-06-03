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

#include "../../model/SiteGroundTemperatureFCfactorMethod.hpp"
#include "../../model/SiteGroundTemperatureFCfactorMethod_Impl.hpp"

#include <utilities/idd/Site_GroundTemperature_FCfactorMethod_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSiteGroundTemperatureFCfactorMethod( SiteGroundTemperatureFCfactorMethod& modelObject )
{
  IdfObject idfObject( openstudio::IddObjectType::Site_GroundTemperature_FCfactorMethod );

  m_idfObjects.push_back(idfObject);

  idfObject.setDouble(Site_GroundTemperature_FCfactorMethodFields::JanuaryGroundTemperature, modelObject.januaryGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_FCfactorMethodFields::FebruaryGroundTemperature, modelObject.februaryGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_FCfactorMethodFields::MarchGroundTemperature, modelObject.marchGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_FCfactorMethodFields::AprilGroundTemperature, modelObject.aprilGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_FCfactorMethodFields::MayGroundTemperature, modelObject.mayGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_FCfactorMethodFields::JuneGroundTemperature, modelObject.juneGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_FCfactorMethodFields::JulyGroundTemperature, modelObject.julyGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_FCfactorMethodFields::AugustGroundTemperature, modelObject.augustGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_FCfactorMethodFields::SeptemberGroundTemperature, modelObject.septemberGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_FCfactorMethodFields::OctoberGroundTemperature, modelObject.octoberGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_FCfactorMethodFields::NovemberGroundTemperature, modelObject.novemberGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_FCfactorMethodFields::DecemberGroundTemperature, modelObject.decemberGroundTemperature());

  return idfObject;
}

} // energyplus

} // openstudio
