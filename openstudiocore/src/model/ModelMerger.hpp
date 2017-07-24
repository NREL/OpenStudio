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

#ifndef MODEL_MODELMERGER_HPP
#define MODEL_MODELMERGER_HPP

#include "ModelAPI.hpp"

#include "Model.hpp"

#include <map>

namespace openstudio
{
  namespace model
  {
    
    class Space;
    class ThermalZone;

    MODEL_API class ModelMerger
    {
    public:
      ModelMerger();

      /// Merges changes from newModel into currentModel
      /// Handle mapping is mapping of handles in currentModel (keys) to handles in newModel (values)
      void mergeModels(Model& currentModel, const Model& newModel, const std::map<UUID, UUID>& handleMapping);

    private:

      void mergeSpace(Space& currentSpace, const Space& newSpace);
      void mergeThermalZone(ThermalZone& currentThermalZone, const ThermalZone& newThermalZone);

      boost::optional<UUID> getNewModelHandle(const UUID& currentHandle);
      boost::optional<UUID> getCurrentModelHandle(const UUID& newHandle);

      Model m_currentModel;
      Model m_newModel;
      std::set<UUID> m_newMergedHandles;
      std::map<UUID, UUID> m_currentToNewHandleMapping;
      std::map<UUID, UUID> m_newToCurrentHandleMapping;
    };

  }
}
#endif //MODEL_MODELMERGER_HPP
