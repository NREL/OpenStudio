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

#ifndef MODEL_METERCUSTOMDECREMENT_HPP
#define MODEL_METERCUSTOMDECREMENT_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

//#include <memory>

namespace openstudio {
namespace model {

  namespace detail {

    class MeterCustomDecrement_Impl;

  }  // namespace detail

  /** MeterCustomDecrement is a ModelObject that wraps the OpenStudio IDD object 'OS:Meter:Custom'. */
  class MODEL_API MeterCustomDecrement : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    // Constructs a new MeterCustomDecrement object in the model.
    explicit MeterCustomDecrement(const Model& model, const std::string& sourceMeterName);

    virtual ~MeterCustomDecrement() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    MeterCustomDecrement(const MeterCustomDecrement& other) = default;
    MeterCustomDecrement(MeterCustomDecrement&& other) = default;
    MeterCustomDecrement& operator=(const MeterCustomDecrement&) = default;
    MeterCustomDecrement& operator=(MeterCustomDecrement&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> fuelTypeValues();

    /** @name Getters */
    //@{

    boost::optional<std::string> fuelType() const;

    std::string sourceMeterName() const;

    // Return a vector of (Key, Var) pairs
    std::vector<std::pair<std::string, std::string>> keyVarGroups();

    // Return the number of (KeyName, OutputVariableorMeterName) groups
    unsigned numKeyVarGroups() const;

    // Lower Level functions
    /** Get the Key Name at index. Indexing starts at 0. */
    boost::optional<std::string> keyName(unsigned index) const;

    /** Get the Output Variable of Meter Name at index. Indexing starts at 0. */
    boost::optional<std::string> outputVariableorMeterName(unsigned index) const;

    //@}
    /** @name Setters */
    //@{

    bool setFuelType(const std::string& fuelType);

    void resetFuelType();

    bool setSourceMeterName(const std::string& sourceMeterName);

    // Add a new (Key, Var) group
    bool addKeyVarGroup(const std::string& keyName, const std::string& outputVariableorMeterName);

    // Remove the (Key, Var) group at given index
    bool removeKeyVarGroup(unsigned groupIndex);

    // Remove all the (Key, Var) groups
    void removeAllKeyVarGroups();

    // Lower level functions
    /** Set the Key Name at index. Indexing starts at 0. */
    bool setKeyName(unsigned index, const std::string& str);

    /** Set the Output Variable of Meter Name at index. Indexing starts at 0. */
    bool setOutputVariableorMeterName(unsigned index, const std::string& str);

    //@}
    /** @name Other */
    //@{

    //@}
    /** @name Type Casting */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::MeterCustomDecrement_Impl;

    explicit MeterCustomDecrement(std::shared_ptr<detail::MeterCustomDecrement_Impl> impl);

    friend class detail::MeterCustomDecrement_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    std::shared_ptr<detail::MeterCustomDecrement_Impl> m_impl;

    REGISTER_LOGGER("openstudio.model.MeterCustomDecrement");
  };

  /** \relates MeterCustomDecrement*/
  using OptionalMeterCustomDecrement = boost::optional<MeterCustomDecrement>;

  /** \relates MeterCustomDecrement*/
  using MeterCustomDecrementVector = std::vector<MeterCustomDecrement>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_METERCUSTOMDECREMENT_HPP
