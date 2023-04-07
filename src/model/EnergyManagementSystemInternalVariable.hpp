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

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMINTERNALVARIABLE_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMINTERNALVARIABLE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace energyplus {
  class ReverseTranslator;
}

namespace model {

  namespace detail {

    class EnergyManagementSystemInternalVariable_Impl;

  }  // namespace detail

  /** EnergyManagementSystemInternalVariable is a ModelObject that wraps the OpenStudio IDD object 'OS:EnergyManagementSystem:InternalVariable'. */
  class MODEL_API EnergyManagementSystemInternalVariable : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{
    explicit EnergyManagementSystemInternalVariable(const Model& model, const std::string& internalDataType);

    virtual ~EnergyManagementSystemInternalVariable() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    EnergyManagementSystemInternalVariable(const EnergyManagementSystemInternalVariable& other) = default;
    EnergyManagementSystemInternalVariable(EnergyManagementSystemInternalVariable&& other) = default;
    EnergyManagementSystemInternalVariable& operator=(const EnergyManagementSystemInternalVariable&) = default;
    EnergyManagementSystemInternalVariable& operator=(EnergyManagementSystemInternalVariable&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    std::string internalDataIndexKeyName() const;

    std::string internalDataType() const;

    //@}
    /** @name Setters */
    //@{

    bool setInternalDataIndexKeyName(const std::string& internalDataIndexKeyName);

    void resetInternalDataIndexKeyName();

    bool setInternalDataType(const std::string& internalDataType);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::EnergyManagementSystemInternalVariable_Impl;

    explicit EnergyManagementSystemInternalVariable(std::shared_ptr<detail::EnergyManagementSystemInternalVariable_Impl> impl);

    friend class detail::EnergyManagementSystemInternalVariable_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    friend class energyplus::ReverseTranslator;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.EnergyManagementSystemInternalVariable");
    explicit EnergyManagementSystemInternalVariable(const Model& model);
  };

  /** \relates EnergyManagementSystemInternalVariable*/
  using OptionalEnergyManagementSystemInternalVariable = boost::optional<EnergyManagementSystemInternalVariable>;

  /** \relates EnergyManagementSystemInternalVariable*/
  using EnergyManagementSystemInternalVariableVector = std::vector<EnergyManagementSystemInternalVariable>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ENERGYMANAGEMENTSYSTEMINTERNALVARIABLE_HPP
