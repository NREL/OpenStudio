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

#ifndef MODEL_MIXER_IMPL_HPP
#define MODEL_MIXER_IMPL_HPP

#include "HVACComponent_Impl.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class MODEL_API Mixer_Impl : public HVACComponent_Impl
    {
     public:
      Mixer_Impl(IddObjectType type, Model_Impl* model);

      Mixer_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      Mixer_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      Mixer_Impl(const Mixer_Impl& other, Model_Impl* model, bool keepHandles);

      virtual ~Mixer_Impl() = default;

      virtual unsigned outletPort() const = 0;

      virtual unsigned inletPort(unsigned branchIndex) const = 0;

      virtual unsigned nextInletPort() const = 0;

      virtual boost::optional<ModelObject> outletModelObject() const;

      virtual boost::optional<ModelObject> inletModelObject(unsigned branchIndex) const;

      virtual boost::optional<ModelObject> lastInletModelObject() const;

      virtual std::vector<ModelObject> inletModelObjects() const;

      virtual unsigned newInletPortAfterBranch(unsigned branchIndex);

      virtual unsigned branchIndexForInletModelObject(ModelObject modelObject) const;

      virtual unsigned nextBranchIndex() const;

      virtual void removePortForBranch(unsigned branchIndex);

      virtual std::vector<HVACComponent> edges(const boost::optional<HVACComponent>& prev) override;

      bool isRemovable() const override;

     private:
      REGISTER_LOGGER("openstudio.model.Mixer");
    };

  }  // namespace detail

}  // namespace model

}  // namespace openstudio

#endif
