/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

