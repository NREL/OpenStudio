/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_TIMESTEP_HPP
#define MODEL_TIMESTEP_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class Timestep_Impl;
  }  // namespace detail

  /** Timestep derives from ModelObject and is an interface to the OpenStudio IDD object named "Timestep".
 *
 *  Timestep allows the user to set the zone timestep for EnergyPlus simulations.
 *  Timestep does not have a public constructor because it is a unique ModelObject.
 *  To get the Timestep object for a Model or create one if it does not yet exist use model.getUniqueObject<Timestep>().
 *  To get the Timestep object for a Model but not create one if it does not yet exist use model.getOptionalUniqueObject<Timestep>().
 */
  class MODEL_API Timestep : public ModelObject
  {
   public:
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    virtual ~Timestep() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    Timestep(const Timestep& other) = default;
    Timestep(Timestep&& other) = default;
    Timestep& operator=(const Timestep&) = default;
    Timestep& operator=(Timestep&&) = default;

    //@}
    /** @name Getters */
    //@{

    int numberOfTimestepsPerHour() const;

    bool isNumberOfTimestepsPerHourDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setNumberOfTimestepsPerHour(int numberOfTimestepsPerHour);

    void resetNumberOfTimestepsPerHour();

    //@}

   protected:
    /** @name Constructors and Destructors */
    //@{

    /// Constructs a new Timestep object in the model.
    explicit Timestep(const Model& model);

    //@}

    /// @cond
    using ImplType = detail::Timestep_Impl;

    friend class Model;

    friend class openstudio::IdfObject;

    // constructor
    explicit Timestep(std::shared_ptr<detail::Timestep_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.Timestep");

    /// @endcond
  };

  /** \relates Timestep */
  using OptionalTimestep = boost::optional<Timestep>;

}  // namespace model

}  // namespace openstudio

#endif  // MODEL_TIMESTEP_HPP
