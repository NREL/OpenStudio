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

#ifndef MODEL_INTERNALMASSDEFINITION_HPP
#define MODEL_INTERNALMASSDEFINITION_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadDefinition.hpp"

namespace openstudio {
namespace model {

  class ConstructionBase;

  namespace detail {

    class InternalMassDefinition_Impl;

  }  // namespace detail

  /** InternalMassDefinition is a SpaceLoadDefinition that wraps the OpenStudio IDD object
 *  'OS_InternalMass_Definition'. Its fields are derived from the EnergyPlus IDD object
 *  'InternalMass'. \sa InternalMass
 */
  class MODEL_API InternalMassDefinition : public SpaceLoadDefinition
  {

   public:
    /** @name Constructors and Destructors */
    //@{

    explicit InternalMassDefinition(const Model& model);

    virtual ~InternalMassDefinition() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    InternalMassDefinition(const InternalMassDefinition& other) = default;
    InternalMassDefinition(InternalMassDefinition&& other) = default;
    InternalMassDefinition& operator=(const InternalMassDefinition&) = default;
    InternalMassDefinition& operator=(InternalMassDefinition&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> validDesignLevelCalculationMethodValues();

    /** @name Getters */
    //@{

    /// Returns the construction object, if this object does not directly specify construction the hierarchy is searched.
    boost::optional<ConstructionBase> construction() const;

    /// Returns true if the construction is not directly referenced by this surface .
    bool isConstructionDefaulted() const;

    std::string designLevelCalculationMethod() const;
    boost::optional<double> surfaceArea() const;
    boost::optional<double> surfaceAreaperSpaceFloorArea() const;
    boost::optional<double> surfaceAreaperPerson() const;

    //@}
    /** @name Setters */
    //@{

    /// Sets the construction object.
    bool setConstruction(const ConstructionBase& construction);

    /// Resets the construction object.
    void resetConstruction();

    /// Sets designLevelCalculationMethod to 'SurfaceArea', clears surfaceAreaperSpaceFloorArea
    /// and surfaceAreaperPerson, and sets surfaceArea.
    bool setSurfaceArea(double surfaceArea);

    /// Sets designLevelCalculationMethod to 'SurfaceArea/Area', clears surfaceArea
    /// and surfaceAreaperPerson, and sets surfaceAreaperSpaceFloorArea.
    bool setSurfaceAreaperSpaceFloorArea(double surfaceAreaperSpaceFloorArea);

    /// Sets designLevelCalculationMethod to 'SurfaceArea/Person', clears surfaceArea
    /// and surfaceAreaperSpaceFloorArea, and sets surfaceAreaperPerson.
    bool setSurfaceAreaperPerson(double surfaceAreaperPerson);

    //@}
    /** @name Other */
    //@{

    /** Returns the construction surface area represented by this definition, assuming floorArea (m^2)
   *  and numPeople. */
    double getSurfaceArea(double floorArea, double numPeople) const;

    /** Returns the m^2/m^2 of construction surface area per floor area represented by this
   *  definition, assuming floorArea (m^2) and numPeople. */
    double getSurfaceAreaPerFloorArea(double floorArea, double numPeople) const;

    /** Returns the m^2/person of construction surface area represented by this definition, assuming
   *  floorArea (m^2) and numPeople. */
    double getSurfaceAreaPerPerson(double floorArea, double numPeople) const;

    /** If method is a \link validDesignLevelCalculationMethodValues valid design level calculation
   *  method \endlink, changes this definition to an equivalent construction surface area, under the
   *  assumptions of floorArea (m^2) and numPeople. */
    bool setDesignLevelCalculationMethod(const std::string& method, double floorArea, double numPeople);

    //@}
   protected:
    /// @cond
    using ImplType = detail::InternalMassDefinition_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit InternalMassDefinition(std::shared_ptr<detail::InternalMassDefinition_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.InternalMassDefinition");
  };

  /** \relates InternalMassDefinition*/
  using OptionalInternalMassDefinition = boost::optional<InternalMassDefinition>;

  /** \relates InternalMassDefinition*/
  using InternalMassDefinitionVector = std::vector<InternalMassDefinition>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_INTERNALMASSDEFINITION_HPP
