/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MEASURE_ENERGYPLUSMEASURE_HPP
#define MEASURE_ENERGYPLUSMEASURE_HPP

#include "MeasureAPI.hpp"
#include "OSMeasure.hpp"

#include "../utilities/core/Logger.hpp"

namespace openstudio {

class Workspace;

namespace measure {

  /** EnergyPlusMeasure is an abstract base class for OSMeasures that operate on EnergyPlus
 *  Workspaces. */
  class MEASURE_API EnergyPlusMeasure : public OSMeasure
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~EnergyPlusMeasure() = default;

    //@}
    /** @name Getters */
    //@{

    /** Returns the arguments for this script. In interactive applications, an OSRunner presents
   *  these arguments to the user to produce an OSArgumentMap of user_arguments that it then passes
   *  to this script's run method. The same basic steps should happen in applications with non-
   *  interactive scripts, but in that case an entity other than an OSRunner may be in charge of
   *  collecting user arguments. The base class implementation returns an empty vector. */
    virtual std::vector<OSArgument> arguments(const openstudio::Workspace& workspace) const;

    /** Returns the outputs for this script. The base class implementation returns an empty vector. */
    virtual std::vector<OSOutput> outputs() const;

    //@}
    /** @name Actions */
    //@{

    /** Run the script on the given workspace with the given runner and user_arguments. The base
   *  class implementation calls runner.prepareForMeasureRun(*this) and should be called at the
   *  beginning of derived class implementations of this method. (In C++, that call looks like
   *  EnergyPlusMeasure::run(workspace, runner, user_arguments). In Ruby that call looks like
   *  super(workspace, runner, user_arguments). */
    virtual bool run(openstudio::Workspace& workspace, OSRunner& runner, const std::map<std::string, OSArgument>& user_arguments) const;

    //@}

   protected:
    EnergyPlusMeasure();

   private:
    REGISTER_LOGGER("openstudio.measure.EnergyPlusMeasure");
  };

}  // namespace measure
}  // namespace openstudio

#endif  // MEASURE_ENERGYPLUSMEASURE_HPP
