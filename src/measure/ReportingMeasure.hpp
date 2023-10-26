/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MEASURE_REPORTINGMEASURE_HPP
#define MEASURE_REPORTINGMEASURE_HPP

#include "MeasureAPI.hpp"
#include "OSMeasure.hpp"

#include "../utilities/core/Logger.hpp"

namespace openstudio {

class IdfObject;

namespace model {
  class Model;
}

namespace measure {

  /** ReportingMeasure is an abstract base class for UserScripts that generate reports. */
  class MEASURE_API ReportingMeasure : public OSMeasure
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~ReportingMeasure() = default;

    //@}
    /** @name Getters */
    //@{

    /** Returns the arguments for this script. In interactive applications, an OSRunner presents
   *  these arguments to the user to produce an OSArgumentMap of user_arguments that it then passes
   *  to this script's run method. The same basic steps should happen in applications with non-
   *  interactive scripts, but in that case an entity other than an OSRunner may be in charge of
   *  collecting user arguments. The base class implementation returns an empty vector. */
    virtual std::vector<OSArgument> arguments(const openstudio::model::Model& model) const;

    /** Returns the outputs for this script. The base class implementation returns an empty vector. */
    virtual std::vector<OSOutput> outputs() const;

    //@}
    /** @name Actions */
    //@{

    /** Run the script with the given runner and user_arguments. The base class implementation calls
   *  runner.prepareForMeasureRun(*this) and should be called at the beginning of derived class
   *  implementations of this method. (In C++, that call looks like
   *  ReportingMeasure::run(runner, user_arguments). In Ruby that call looks like
   *  super(runner, user_arguments). */
    virtual bool run(OSRunner& runner, const std::map<std::string, OSArgument>& user_arguments) const;

    /** This method is called on all reporting measures immediately before the E+
   *  simulation. The code that injects these objects into the IDF checks that
   *  only objects of allowed types are added to prevent changes that impact
   *  energy use. The injector code also prevents duplicate report requests.
   *  The default implementation of this method returns an empty vector.
   *  The runner allows the user to load the last osm/idf and to log messages.
   * The argument map is available with the arguments for the reporting measure. */
    virtual std::vector<IdfObject> energyPlusOutputRequests(OSRunner& runner, const std::map<std::string, OSArgument>& user_arguments) const;
    //@}
   protected:
    ReportingMeasure() = default;

   private:
    REGISTER_LOGGER("openstudio.measure.ReportingMeasure");
  };

}  // namespace measure
}  // namespace openstudio

#endif  // MEASURE_REPORTINGMEASURE_HPP
