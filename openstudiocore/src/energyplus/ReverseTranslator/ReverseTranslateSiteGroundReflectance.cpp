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

#include "../ReverseTranslator.hpp"
#include "../../model/SiteGroundReflectance.hpp"
#include "../../model/SiteGroundReflectance_Impl.hpp"
#include <utilities/idd/Site_GroundReflectance_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateSiteGroundReflectance( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::Site_GroundReflectance )
  {
     LOG(Error, "WorkspaceObject is not IddObjectType: Site_GroundReflectance");
     return boost::none;
  }

  SiteGroundReflectance mo = m_model.getUniqueModelObject<SiteGroundReflectance>();

  boost::optional<double> value = workspaceObject.getDouble(Site_GroundReflectanceFields::JanuaryGroundReflectance);
  if( value )
  {
    mo.setJanuaryGroundReflectance(value.get());
  }

  value = workspaceObject.getDouble(Site_GroundReflectanceFields::FebruaryGroundReflectance);
  if( value )
  {
    mo.setFebruaryGroundReflectance(value.get());
  }

  value = workspaceObject.getDouble(Site_GroundReflectanceFields::MarchGroundReflectance);
  if( value )
  {
    mo.setMarchGroundReflectance(value.get());
  }

  value = workspaceObject.getDouble(Site_GroundReflectanceFields::AprilGroundReflectance);
  if( value )
  {
    mo.setAprilGroundReflectance(value.get());
  }

  value = workspaceObject.getDouble(Site_GroundReflectanceFields::MayGroundReflectance);
  if( value )
  {
    mo.setMayGroundReflectance(value.get());
  }

  value = workspaceObject.getDouble(Site_GroundReflectanceFields::JuneGroundReflectance);
  if( value )
  {
    mo.setJuneGroundReflectance(value.get());
  }

  value = workspaceObject.getDouble(Site_GroundReflectanceFields::JulyGroundReflectance);
  if( value )
  {
    mo.setJulyGroundReflectance(value.get());
  }

  value = workspaceObject.getDouble(Site_GroundReflectanceFields::AugustGroundReflectance);
  if( value )
  {
    mo.setAugustGroundReflectance(value.get());
  }

  value = workspaceObject.getDouble(Site_GroundReflectanceFields::SeptemberGroundReflectance);
  if( value )
  {
    mo.setSeptemberGroundReflectance(value.get());
  }

  value = workspaceObject.getDouble(Site_GroundReflectanceFields::OctoberGroundReflectance);
  if( value )
  {
    mo.setOctoberGroundReflectance(value.get());
  }

  value = workspaceObject.getDouble(Site_GroundReflectanceFields::NovemberGroundReflectance);
  if( value )
  {
    mo.setNovemberGroundReflectance(value.get());
  }

  value = workspaceObject.getDouble(Site_GroundReflectanceFields::DecemberGroundReflectance);
  if( value )
  {
    mo.setDecemberGroundReflectance(value.get());
  }

  return mo;

}

} // energyplus

} // openstudio

