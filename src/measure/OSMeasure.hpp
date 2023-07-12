/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MEASURE_OSMEASURE_HPP
#define MEASURE_OSMEASURE_HPP

#include "MeasureAPI.hpp"

#include "../utilities/core/Logger.hpp"
#include "../utilities/core/Deprecated.hpp"

namespace openstudio {
namespace measure {

  class OSArgument;
  class OSOutput;
  class OSRunner;

  /** OSMeasure is an abstract base class for a user script. There are derived types that operate
 *  on OpenStudio Models and EnergyPlus Workspaces. */
  class MEASURE_API OSMeasure
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~OSMeasure() = default;

    //@}
    /** @name Getters */
    //@{

    /// user facing name of this measure, defaults to empty
    virtual std::string name() const;

    /// user facing description of this measure, defaults to empty
    virtual std::string description() const;

    /// user facing BCL taxonomy of this measure, defaults to empty
    virtual std::string taxonomy() const;

    /// user facing technical description of this measure, defaults to empty
    virtual std::string modeler_description() const;

    //@}
    /** @name Actions */
    //@{

    /** Register this script with SketchUp. Deprecated in favor of registerWithApplication().
   *  Base class implementation does nothing and returns false. \deprecated */
    OS_DEPRECATED(1, 13, 0) virtual bool registerWithSketchUp() const;

    /** Register this script with an Application. Base class implementation does nothing and
   *  returns false. \deprecated */
    OS_DEPRECATED(1, 13, 0) virtual bool registerWithApplication() const;

   protected:
    OSMeasure() = default;

    //@}
   private:
    REGISTER_LOGGER("openstudio.measure.OSMeasure");
  };

  /** \relates OSMeasure */
  using OptionalOSMeasure = boost::optional<OSMeasure>;

  /** \relates OSMeasure */
  using OSMeasureVector = std::vector<OSMeasure>;

}  // namespace measure
}  // namespace openstudio

#endif  // MEASURE_OSMEASURE_HPP
