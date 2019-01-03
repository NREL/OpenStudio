/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

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

