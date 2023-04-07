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

#ifndef MODEL_GENERATOR_HPP
#define MODEL_GENERATOR_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {
namespace model {

  class ElectricLoadCenterDistribution;

  namespace detail {
    class Generator_Impl;
  }

  /** Generator is the base class for generators.  The ratedElectricPowerOutput, availabilitySchedule, and
 *  ratedThermaltoElectricalPowerRatio fields are mapped to fields in the ElectricLoadCenter:Generators object
 *  in EnergyPlus.  The ElectricLoadCenter:Generators object does not exist in OpenStudio.
 */
  class MODEL_API Generator : public ParentObject
  {

   public:
    Generator(IddObjectType type, const Model& model);

    virtual ~Generator() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    Generator(const Generator& other) = default;
    Generator(Generator&& other) = default;
    Generator& operator=(const Generator&) = default;
    Generator& operator=(Generator&&) = default;

    std::string generatorObjectType() const;

    boost::optional<double> ratedElectricPowerOutput() const;

    boost::optional<Schedule> availabilitySchedule() const;

    boost::optional<double> ratedThermaltoElectricalPowerRatio() const;

    boost::optional<ElectricLoadCenterDistribution> electricLoadCenterDistribution() const;

   protected:
    friend class Model;
    friend class openstudio::IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    friend class detail::Generator_Impl;

    /// @cond

    using ImplType = detail::Generator_Impl;

    explicit Generator(std::shared_ptr<detail::Generator_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.Generator");

    /// @endcond
  };

  using OptionalGenerator = boost::optional<Generator>;

  using GeneratorVector = std::vector<Generator>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GENERATOR_HPP
