/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PIPEADIABATIC_HPP
#define MODEL_PIPEADIABATIC_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "StraightComponent.hpp"
#include "Connection.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class PipeAdiabatic_Impl;
  }  // namespace detail

  /** PipeAdiabatic is an interface to the IDD object named "OS:Pipe:Adiabatic"
 *
 *  The purpose of this class is to simplify the construction and manipulation
 *  Pipe:Adiabatic objects in energyplus.
 */
  class MODEL_API PipeAdiabatic : public StraightComponent
  {

   public:
    /** Constructs a new PipeAdiabatic object and places it inside the
   *  model.  The object is fully initialized with all companion objects.
   */
    explicit PipeAdiabatic(const Model& model);

    virtual ~PipeAdiabatic() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    PipeAdiabatic(const PipeAdiabatic& other) = default;
    PipeAdiabatic(PipeAdiabatic&& other) = default;
    PipeAdiabatic& operator=(const PipeAdiabatic&) = default;
    PipeAdiabatic& operator=(PipeAdiabatic&&) = default;

    unsigned inletPort() const;

    unsigned outletPort() const;

    static IddObjectType iddObjectType();

   protected:
    friend class Model;

    friend class openstudio::IdfObject;

    /// @cond

    using ImplType = detail::PipeAdiabatic_Impl;

    explicit PipeAdiabatic(std::shared_ptr<detail::PipeAdiabatic_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.PipeAdiabatic");

    /// @endcond
  };

  /** \relates PipeAdiabatic */
  using OptionalPipeAdiabatic = boost::optional<PipeAdiabatic>;

  /** \relates PipeAdiabatic */
  using PipeAdiabaticVector = std::vector<PipeAdiabatic>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PIPEADIABATIC_HPP
