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

#ifndef MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITEXPORTTOACTUATOR_HPP
#define MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITEXPORTTOACTUATOR_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl;

  }  // namespace detail

  /** ExternalInterfaceFunctionalMockupUnitExportToActuator is a ModelObject that wraps the OpenStudio IDD object 'OS:ExternalInterface:FunctionalMockupUnitExport:To:Actuator'. */
  class MODEL_API ExternalInterfaceFunctionalMockupUnitExportToActuator : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ExternalInterfaceFunctionalMockupUnitExportToActuator(const ModelObject& modelObject, const std::string& actuatedComponentType,
                                                                   const std::string& actuatedComponentControlType,
                                                                   const std::string& fMUVariableName, double initialValue);

    virtual ~ExternalInterfaceFunctionalMockupUnitExportToActuator() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ExternalInterfaceFunctionalMockupUnitExportToActuator(const ExternalInterfaceFunctionalMockupUnitExportToActuator& other) = default;
    ExternalInterfaceFunctionalMockupUnitExportToActuator(ExternalInterfaceFunctionalMockupUnitExportToActuator&& other) = default;
    ExternalInterfaceFunctionalMockupUnitExportToActuator& operator=(const ExternalInterfaceFunctionalMockupUnitExportToActuator&) = default;
    ExternalInterfaceFunctionalMockupUnitExportToActuator& operator=(ExternalInterfaceFunctionalMockupUnitExportToActuator&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    ModelObject actuatedComponentUnique() const;

    std::string actuatedComponentType() const;

    std::string actuatedComponentControlType() const;

    std::string fMUVariableName() const;

    double initialValue() const;

    //@}
    /** @name Setters */
    //@{

    bool setActuatedComponentUnique(const ModelObject& modelObject);

    bool setActuatedComponentType(const std::string& actuatedComponentType);

    bool setActuatedComponentControlType(const std::string& actuatedComponentControlType);

    bool setFMUVariableName(const std::string& fMUVariableName);

    bool setInitialValue(double initialValue);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl;

    explicit ExternalInterfaceFunctionalMockupUnitExportToActuator(
      std::shared_ptr<detail::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl> impl);

    friend class detail::ExternalInterfaceFunctionalMockupUnitExportToActuator_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ExternalInterfaceFunctionalMockupUnitExportToActuator");
  };

  /** \relates ExternalInterfaceFunctionalMockupUnitExportToActuator*/
  using OptionalExternalInterfaceFunctionalMockupUnitExportToActuator = boost::optional<ExternalInterfaceFunctionalMockupUnitExportToActuator>;

  /** \relates ExternalInterfaceFunctionalMockupUnitExportToActuator*/
  using ExternalInterfaceFunctionalMockupUnitExportToActuatorVector = std::vector<ExternalInterfaceFunctionalMockupUnitExportToActuator>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITEXPORTTOACTUATOR_HPP
