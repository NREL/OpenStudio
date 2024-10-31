/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

    virtual ~InternalMass() override = default;
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
