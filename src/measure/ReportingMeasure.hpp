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
