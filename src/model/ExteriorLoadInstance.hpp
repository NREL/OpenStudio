/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERIORLOADINSTANCE_HPP
#define MODEL_EXTERIORLOADINSTANCE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  class ExteriorLoadDefinition;

  namespace detail {
    class ExteriorLoadInstance_Impl;
  }

  /** ExteriorLoadInstance is an abstract class derived from ModelObject. \link ExteriorLoadInstance
 *  ExteriorLoadInstances \endlink instantiate a given ExteriorLoadDefinition in the facility
 *  by defining a multiplier and a schedule.
 */
  class MODEL_API ExteriorLoadInstance : public ModelObject
  {
   public:
    virtual ~ExteriorLoadInstance() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ExteriorLoadInstance(const ExteriorLoadInstance& other) = default;
    ExteriorLoadInstance(ExteriorLoadInstance&& other) = default;
    ExteriorLoadInstance& operator=(const ExteriorLoadInstance&) = default;
    ExteriorLoadInstance& operator=(ExteriorLoadInstance&&) = default;

    /** Returns the definition of this instance. **/
    ExteriorLoadDefinition definition() const;

    /** Sets the definition of this instance. **/
    bool setDefinition(const ExteriorLoadDefinition& definition);

    /** Ensures that no other instances share this instances definition. **/
    void makeUnique();

    /** Gets the multiplier for this space instance, defaults to 1. **/
    double multiplier() const;

    /** Returns true if the multiplier is defaulted. **/
    bool isMultiplierDefaulted() const;

    /** Returns the number of instances this space load instance represents.
  This just forwards to multiplier() here but is included for consistency with SpaceLoadInstance**/
    int quantity() const;

   protected:
    /** @name Constructors and Destructors */
    //@{

    /// Constructs a new ExteriorLoadInstance object in the model.
    ExteriorLoadInstance(IddObjectType type, const ExteriorLoadDefinition& definition);

    //@}

    /// @cond

    using ImplType = detail::ExteriorLoadInstance_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class detail::ExteriorLoadInstance_Impl;

    explicit ExteriorLoadInstance(std::shared_ptr<detail::ExteriorLoadInstance_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.ExteriorLoadInstance");

    /// @endcond
  };

  /** \relates ExteriorLoadInstance */
  using OptionalExteriorLoadInstance = boost::optional<ExteriorLoadInstance>;

  /** \relates ExteriorLoadInstance */
  using ExteriorLoadInstanceVector = std::vector<ExteriorLoadInstance>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERIORLOADINSTANCE_HPP
