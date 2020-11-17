/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/Construction.hpp"
#include "../../model/Construction_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/PipeOutdoor.hpp"
#include "../../model/PipeOutdoor_Impl.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/Pipe_Outdoor_FieldEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translatePipeOutdoor(PipeOutdoor& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::Pipe_Outdoor);

    m_idfObjects.push_back(idfObject);

    if (auto node = modelObject.inletModelObject()) {
      idfObject.setString(openstudio::Pipe_OutdoorFields::FluidInletNodeName, node->name().get());
    }

    if (auto node = modelObject.outletModelObject()) {
      idfObject.setString(openstudio::Pipe_OutdoorFields::FluidOutletNodeName, node->name().get());
    }

    if (auto value = modelObject.name()) {
      idfObject.setName(value.get());
    }

    if (auto construction = modelObject.construction()) {
      idfObject.setString(openstudio::Pipe_OutdoorFields::ConstructionName, construction->name().get());
    }

    if (auto node = modelObject.ambientTemperatureOutdoorAirNode()) {
      idfObject.setString(openstudio::Pipe_OutdoorFields::AmbientTemperatureOutdoorAirNodeName, node->name().get());
    } else {
      auto name = modelObject.name().get() + " OA Node";
      IdfObject oaNodeListIdf(openstudio::IddObjectType::OutdoorAir_NodeList);
      oaNodeListIdf.setString(0, name);
      m_idfObjects.push_back(oaNodeListIdf);

      idfObject.setString(openstudio::Pipe_OutdoorFields::AmbientTemperatureOutdoorAirNodeName, name);
    }

    idfObject.setDouble(openstudio::Pipe_OutdoorFields::PipeInsideDiameter, modelObject.pipeInsideDiameter());

    idfObject.setDouble(openstudio::Pipe_OutdoorFields::PipeLength, modelObject.pipeLength());

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
