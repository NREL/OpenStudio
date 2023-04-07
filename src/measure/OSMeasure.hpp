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
    OS_DEPRECATED virtual bool registerWithSketchUp() const;

    /** Register this script with an Application. Base class implementation does nothing and
   *  returns false. \deprecated */
    OS_DEPRECATED virtual bool registerWithApplication() const;

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
