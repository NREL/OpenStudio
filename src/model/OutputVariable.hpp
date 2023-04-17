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

#ifndef MODEL_OUTPUTVARIABLE_HPP
#define MODEL_OUTPUTVARIABLE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

class ReportingFrequency;

namespace model {

  class Schedule;

  namespace detail {
    class OutputVariable_Impl;
  }  // namespace detail

  /** OutputVariable derives from ModelObject and is an interface to the OpenStudio
 *  IDD object named "OS:Output:Variable".
 *
 *  OutputVariable is used to request output data from EnergyPlus simulations.  Each ModelObject defines
 *  a list of available output variable names which can be used to create OutputVariable requests.  An
 *  OutputVariable request can apply to all objects (e.g. if the 'Key Value' field is set to '*') or only
 *  a particular object (e.g. if the 'Key Value' field is set to an object's name).  If an OutputVariable object
 *  refers to a particular ModelObject then it is parented by that ModelObject, otherwise it is unparented.
 *  If an OutputVariable object is present when a Model is simulated in EnergyPlus, the OutputVariable can be
 *  used to access simulation results for its parent ModelObject.
 */
  class MODEL_API OutputVariable : public ModelObject
  {
   public:
    virtual ~OutputVariable() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    OutputVariable(const OutputVariable& other) = default;
    OutputVariable(OutputVariable&& other) = default;
    OutputVariable& operator=(const OutputVariable&) = default;
    OutputVariable& operator=(OutputVariable&&) = default;

    /** @name Constructors and Destructors */
    //@{

    /// Constructs a new OutputVariable object in the model.
    explicit OutputVariable(const std::string& variableName, const Model& model);

    virtual std::vector<openstudio::IdfObject> remove();

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> reportingFrequencyValues();

    /** @name Getters */
    //@{

    std::string keyValue() const;

    bool isKeyValueDefaulted() const;

    std::string variableName() const;

    std::string reportingFrequency() const;

    bool isReportingFrequencyDefaulted() const;

    boost::optional<Schedule> schedule() const;

    bool exportToBCVTB() const;

    bool isExportToBCVTBDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setKeyValue(const std::string& keyValue);

    void resetKeyValue();

    bool setVariableName(const std::string& variableName);

    bool setReportingFrequency(const std::string& reportingFrequency);

    void resetReportingFrequency();

    bool setSchedule(Schedule& schedule);

    void resetSchedule();

    bool setExportToBCVTB(bool exportToBCVTB);

    void resetExportToBCVTB();

    //@}
   protected:
    /// @cond

    using ImplType = detail::OutputVariable_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    // constructor
    explicit OutputVariable(std::shared_ptr<detail::OutputVariable_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.OutputVariable");

    /// @endcond
  };

  /** \relates OutputVariable */
  using OptionalOutputVariable = boost::optional<OutputVariable>;

  /** \relates OutputVariable */
  using OutputVariableVector = std::vector<OutputVariable>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_VARIABLE_HPP
