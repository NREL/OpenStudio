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

#ifndef MODEL_EXTERNALINTERFACEACTUATOR_HPP
#define MODEL_EXTERNALINTERFACEACTUATOR_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class ExternalInterfaceActuator_Impl;

  }  // namespace detail

  /** ExternalInterfaceActuator is a ModelObject that wraps the OpenStudio IDD object 'OS:ExternalInterface:Actuator'. */
  class MODEL_API ExternalInterfaceActuator : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ExternalInterfaceActuator(const ModelObject& modelObject);

    explicit ExternalInterfaceActuator(const ModelObject& modelObject, const std::string actuatedComponentType,
                                       const std::string actuatedComponentControlType);

    virtual ~ExternalInterfaceActuator() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ExternalInterfaceActuator(const ExternalInterfaceActuator& other) = default;
    ExternalInterfaceActuator(ExternalInterfaceActuator&& other) = default;
    ExternalInterfaceActuator& operator=(const ExternalInterfaceActuator&) = default;
    ExternalInterfaceActuator& operator=(ExternalInterfaceActuator&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    ModelObject actuatedComponentUnique() const;

    std::string actuatedComponentType() const;

    std::string actuatedComponentControlType() const;

    boost::optional<double> optionalInitialValue() const;

    bool exportToBCVTB() const;

    bool isExportToBCVTBDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setActuatedComponentUnique(const ModelObject& modelObject);

    bool setActuatedComponentType(const std::string& actuatedComponentType);

    bool setActuatedComponentControlType(const std::string& actuatedComponentControlType);

    bool setOptionalInitialValue(double optionalInitialValue);

    void resetOptionalInitialValue();

    bool setExportToBCVTB(bool exportToBCVTB);

    void resetExportToBCVTB();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ExternalInterfaceActuator_Impl;

    explicit ExternalInterfaceActuator(std::shared_ptr<detail::ExternalInterfaceActuator_Impl> impl);

    friend class detail::ExternalInterfaceActuator_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ExternalInterfaceActuator");
  };

  /** \relates ExternalInterfaceActuator*/
  using OptionalExternalInterfaceActuator = boost::optional<ExternalInterfaceActuator>;

  /** \relates ExternalInterfaceActuator*/
  using ExternalInterfaceActuatorVector = std::vector<ExternalInterfaceActuator>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERNALINTERFACEACTUATOR_HPP
