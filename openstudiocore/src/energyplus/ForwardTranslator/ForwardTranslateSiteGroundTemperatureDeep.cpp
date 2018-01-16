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

  idfObject.setDouble(Site_GroundTemperature_DeepFields::OctoberDeepGroundTemperature, modelObject.octoberDeepGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_DeepFields::NovemberDeepGroundTemperature, modelObject.novemberDeepGroundTemperature());

  idfObject.setDouble(Site_GroundTemperature_DeepFields::DecemberDeepGroundTemperature, modelObject.decemberDeepGroundTemperature());

  return idfObject;
}

} // energyplus

} // openstudio
