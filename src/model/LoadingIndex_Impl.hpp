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

#ifndef MODEL_LOADINGINDEX_IMPL_HPP
#define MODEL_LOADINGINDEX_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

  class Curve;

  namespace detail {

    /** LoadingIndex_Impl is a ParentObject_Impl that is the implementation class for LoadingIndex.*/
    class MODEL_API LoadingIndex_Impl : public ParentObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      LoadingIndex_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      LoadingIndex_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      LoadingIndex_Impl(const LoadingIndex_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~LoadingIndex_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      // Overriding clone and children here because we want to try to remove the Curves (if they aren't used by something else)
      // So we list them as children. But ParentObject_Impl::clone would also clone them, so we override clone to call ModelObject_Impl::clone
      virtual ModelObject clone(Model model) const override;

      virtual std::vector<ModelObject> children() const override;

      // No need to override, it bears only ResourceObjects
      // virtual std::vector<IddObjectType> allowableChildTypes() const override;

      //@}
      /** @name Getters */
      //@{

      double compressorSpeed() const;

      Curve evaporativeCapacityMultiplierFunctionofTemperatureCurve() const;

      Curve compressorPowerMultiplierFunctionofTemperatureCurve() const;

      //@}
      /** @name Setters */
      //@{

      bool setCompressorSpeed(double compressorSpeed);

      bool setEvaporativeCapacityMultiplierFunctionofTemperatureCurve(const Curve& curve);

      bool setCompressorPowerMultiplierFunctionofTemperatureCurve(const Curve& curve);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.LoadingIndex");

      boost::optional<Curve> optionalEvaporativeCapacityMultiplierFunctionofTemperatureCurve() const;
      boost::optional<Curve> optionalCompressorPowerMultiplierFunctionofTemperatureCurve() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_LOADINGINDEX_IMPL_HPP
