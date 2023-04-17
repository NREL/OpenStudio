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

#ifndef MODEL_INTERNALMASS_HPP
#define MODEL_INTERNALMASS_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance.hpp"

namespace openstudio {
namespace model {

  class InternalMassDefinition;
  class SurfacePropertyConvectionCoefficients;

  namespace detail {

    class InternalMass_Impl;

  }  // namespace detail

  /** InternalMass is a SpaceLoadInstance that wraps the OpenStudio IDD object 'OS_InternalMass'.
 *  Its fields are derived from the EnergyPlus IDD object 'InternalMass'.
 *  \sa InternalMassDefinition
 */
  class MODEL_API InternalMass : public SpaceLoadInstance
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit InternalMass(const InternalMassDefinition& internalMassDefinition);

    virtual ~InternalMass() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    InternalMass(const InternalMass& other) = default;
    InternalMass(InternalMass&& other) = default;
    InternalMass& operator=(const InternalMass&) = default;
    InternalMass& operator=(InternalMass&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    /** Gets the InternalMassDefinition object. */
    InternalMassDefinition internalMassDefinition() const;

    /** Returns the SurfacePropertyConvectionCoefficients, if it exists. */
    boost::optional<SurfacePropertyConvectionCoefficients> surfacePropertyConvectionCoefficients() const;

    //@}
    /** @name Setters */
    //@{

    /** Sets the InternalMassDefinition object. */
    bool setInternalMassDefinition(const InternalMassDefinition& definition);

    bool setMultiplier(double multiplier);

    void resetMultiplier();

    //@}
    /** @name Other */
    //@{

    /** Returns the surfaceArea if possible based on the underlying data of
   *  internalMassDefinition(). */
    boost::optional<double> surfaceArea() const;

    /** Returns the surfaceAreaPerFloorArea if possible based on the underlying data of
   *  internalMassDefinition(). */
    boost::optional<double> surfaceAreaPerFloorArea() const;

    /** Returns the surfaceAreaPerPerson if possible based on the underlying data of
   *  internalMassDefinition(). */
    boost::optional<double> surfaceAreaPerPerson() const;

    /** Returns the construction surface area represented by this instance, assuming floorArea (m^2)
   *  and numPeople. */
    double getSurfaceArea(double floorArea, double numPeople) const;

    /** Returns the m^2/m^2 of construction surface area per floor area represented by this
   *  instance, assuming floorArea (m^2) and numPeople. */
    double getSurfaceAreaPerFloorArea(double floorArea, double numPeople) const;

    /** Returns the m^2/person of construction surface area represented by this instance, assuming
   *  floorArea (m^2) and numPeople. */
    double getSurfaceAreaPerPerson(double floorArea, double numPeople) const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::InternalMass_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit InternalMass(std::shared_ptr<detail::InternalMass_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.InternalMass");
  };

  /** \relates InternalMass*/
  using OptionalInternalMass = boost::optional<InternalMass>;

  /** \relates InternalMass*/
  using InternalMassVector = std::vector<InternalMass>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_INTERNALMASS_HPP
