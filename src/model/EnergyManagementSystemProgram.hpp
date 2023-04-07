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

#ifndef MODEL_ENERGYMANAGEMENTSYSTEMPROGRAM_HPP
#define MODEL_ENERGYMANAGEMENTSYSTEMPROGRAM_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class EnergyManagementSystemProgram_Impl;

  }  // namespace detail

  /** EnergyManagementSystemProgram is a ModelObject that wraps the OpenStudio IDD object 'OS:EnergyManagementSystem:Program'. */
  class MODEL_API EnergyManagementSystemProgram : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit EnergyManagementSystemProgram(const Model& model);

    virtual ~EnergyManagementSystemProgram() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    EnergyManagementSystemProgram(const EnergyManagementSystemProgram& other) = default;
    EnergyManagementSystemProgram(EnergyManagementSystemProgram&& other) = default;
    EnergyManagementSystemProgram& operator=(const EnergyManagementSystemProgram&) = default;
    EnergyManagementSystemProgram& operator=(EnergyManagementSystemProgram&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    std::string body() const;

    std::vector<std::string> lines() const;

    std::vector<ModelObject> referencedObjects() const;

    std::vector<std::string> invalidReferencedObjects() const;

    //@}
    /** @name Setters */
    //@{

    bool setBody(const std::string& body);

    bool resetBody();

    bool addLine(const std::string& body);

    bool setLines(const std::vector<std::string>& body);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::EnergyManagementSystemProgram_Impl;

    explicit EnergyManagementSystemProgram(std::shared_ptr<detail::EnergyManagementSystemProgram_Impl> impl);

    friend class detail::EnergyManagementSystemProgram_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.EnergyManagementSystemProgram");
  };

  /** \relates EnergyManagementSystemProgram*/
  using OptionalEnergyManagementSystemProgram = boost::optional<EnergyManagementSystemProgram>;

  /** \relates EnergyManagementSystemProgram*/
  using EnergyManagementSystemProgramVector = std::vector<EnergyManagementSystemProgram>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ENERGYMANAGEMENTSYSTEMPROGRAM_HPP
