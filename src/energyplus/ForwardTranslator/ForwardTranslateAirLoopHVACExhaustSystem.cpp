/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/Model.hpp"

#include "../../model/AirLoopHVACExhaustSystem.hpp"

// TODO: Check the following class names against object getters and setters.
#include "../../model/ZoneMixers.hpp"
#include "../../model/ZoneMixers_Impl.hpp"

#include "../../model/FansSystemModel.hpp"
#include "../../model/FansSystemModel_Impl.hpp"

#include <utilities/idd/AirLoopHVAC_ExhaustSystem_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateAirLoopHVACExhaustSystem(model::AirLoopHVACExhaustSystem& modelObject) {

    // Instantiate an IdfObject of the class to store the values
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::AirLoopHVAC_ExhaustSystem, modelObject);
    // If it doesn't have a name, or if you aren't sure you are going to want to return it
    // IdfObject idfObject(openstudio::IddObjectType::AirLoopHVAC_ExhaustSystem);
    // m_idfObjects.push_back(idfObject);

    // TODO: Note JM 2018-10-17
    // You are responsible for implementing any additional logic based on choice fields, etc.
    // The ForwardTranslator generator script is meant to facilitate your work, not get you 100% of the way

    // TODO: If you keep createRegisterAndNameIdfObject above, you don't need this.
    // But in some cases, you'll want to handle failure without pushing to the map
    // Name
    idfObject.setName(modelObject.nameString());

    // Zone Mixer Name: Required Object
    ZoneMixers zoneMixer = modelObject.zoneMixer();
    if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(zoneMixer)) {
      idfObject.setString(AirLoopHVAC_ExhaustSystemFields::ZoneMixerName, wo_->nameString());
    }

    // Fan Object Type: Required String
    const std::string fanObjectType = modelObject.fanObjectType();
    idfObject.setString(AirLoopHVAC_ExhaustSystemFields::FanObjectType, fanObjectType);

    // Fan Name: Required Object
    FansSystemModel fan = modelObject.fan();
    if (boost::optional<IdfObject> wo_ = translateAndMapModelObject(fan)) {
      idfObject.setString(AirLoopHVAC_ExhaustSystemFields::FanName, wo_->nameString());
    }

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio