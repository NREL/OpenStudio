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
#include "../../model/ConvergenceLimits.hpp"
#include <utilities/idd/ConvergenceLimits_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateConvergenceLimits( ConvergenceLimits & modelObject )
{
  IdfObject idfObject( openstudio::IddObjectType::ConvergenceLimits);

  if (OptionalInt i = modelObject.minimumSystemTimestep()) {
    idfObject.setInt(ConvergenceLimitsFields::MinimumSystemTimestep,*i);
  }else{
    // EnergyPlus IDD has a note saying 1 is the default but does not actually list it as the default
    // A severe error message is issued if this field is blank
    idfObject.setInt(ConvergenceLimitsFields::MinimumSystemTimestep,1);
  }

  if (!modelObject.isMaximumHVACIterationsDefaulted()) {
    idfObject.setInt(ConvergenceLimitsFields::MaximumHVACIterations,
                     modelObject.maximumHVACIterations());
  }

  if (!modelObject.isMinimumPlantIterationsDefaulted()) {
    idfObject.setInt(ConvergenceLimitsFields::MinimumPlantIterations,
                     modelObject.minimumPlantIterations());
  }

  if (!modelObject.isMaximumPlantIterationsDefaulted()) {
    idfObject.setInt(ConvergenceLimitsFields::MaximumPlantIterations,
                     modelObject.maximumPlantIterations());
  }

  m_idfObjects.push_back(idfObject);

  return idfObject;
}

} // energyplus
} // openstudio
