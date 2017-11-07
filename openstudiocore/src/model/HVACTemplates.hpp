/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_HVACTEMPLATES_HPP
#define MODEL_HVACTEMPLATES_HPP

#include "ModelAPI.hpp"
#include <vector>

namespace openstudio {

namespace model {

class Loop;

class Model;

class ThermalZone;

MODEL_API void addSystemType1(Model & model, std::vector<ThermalZone> zones);

MODEL_API void addSystemType2(Model & model, std::vector<ThermalZone> zones);

MODEL_API Loop addSystemType3(Model & model);

MODEL_API Loop addSystemType4(Model & model);

MODEL_API Loop addSystemType5(Model & model);

MODEL_API Loop addSystemType6(Model & model);

MODEL_API Loop addSystemType7(Model & model);

MODEL_API Loop addSystemType8(Model & model);

MODEL_API Loop addSystemType9(Model & model);

MODEL_API Loop addSystemType10(Model & model);

MODEL_API Loop airLoopHVACTHAI(Model & model);

/*Split type/DX Type*/
MODEL_API Loop addSplitTypeHVACTHAIType0(Model & model);

/*Split type/DX Type (Inverter)*/
MODEL_API Loop addSplitTypeHVACTHAIType1(Model & model);

/*Split type/DX Type (Inverter) with ERV*/
MODEL_API Loop addSplitTypeHVACTHAIType2(Model & model);

/*Central System (Air Loop and Water Loop)*/
MODEL_API Loop addCentralSystemHVACTHAI(Model & model);

/*Central System (Thai Central A/C Air Cooled System)*/
MODEL_API Loop addCentralSystemWithAirCooled(Model & model);

/*Central System (Thai  Central  A/C Water Cooled System)*/
MODEL_API Loop addCentralSystemWithWaterCooled(Model & model);

/*Thai Air Loop*/
MODEL_API Loop addThaiAirLoop(Model & model);

/*Thai Air Loop with ERV*/
MODEL_API Loop addThaizAirLoopWithERV(Model & model);


} // model

} // openstudio

#endif // MODEL_HVACTEMPLATES_HPP
