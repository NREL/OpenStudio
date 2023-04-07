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

#ifndef MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITEXPORTTOSCHEDULE_HPP
#define MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITEXPORTTOSCHEDULE_HPP

#include "ModelAPI.hpp"
#include "Schedule.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl;

  }  // namespace detail

  /** ExternalInterfaceFunctionalMockupUnitExportToSchedule is a ModelObject that wraps the OpenStudio IDD object 'OS:ExternalInterface:FunctionalMockupUnitExport:To:Schedule'. */
  class MODEL_API ExternalInterfaceFunctionalMockupUnitExportToSchedule : public Schedule
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ExternalInterfaceFunctionalMockupUnitExportToSchedule(const Model& model, const std::string& fMUVariableName, double initialValue);

    virtual ~ExternalInterfaceFunctionalMockupUnitExportToSchedule() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ExternalInterfaceFunctionalMockupUnitExportToSchedule(const ExternalInterfaceFunctionalMockupUnitExportToSchedule& other) = default;
    ExternalInterfaceFunctionalMockupUnitExportToSchedule(ExternalInterfaceFunctionalMockupUnitExportToSchedule&& other) = default;
    ExternalInterfaceFunctionalMockupUnitExportToSchedule& operator=(const ExternalInterfaceFunctionalMockupUnitExportToSchedule&) = default;
    ExternalInterfaceFunctionalMockupUnitExportToSchedule& operator=(ExternalInterfaceFunctionalMockupUnitExportToSchedule&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    std::string fMUVariableName() const;

    double initialValue() const;

    //@}
    /** @name Setters */
    //@{

    bool setFMUVariableName(const std::string& fMUVariableName);

    bool setInitialValue(double initialValue);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl;

    explicit ExternalInterfaceFunctionalMockupUnitExportToSchedule(
      std::shared_ptr<detail::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl> impl);

    friend class detail::ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ExternalInterfaceFunctionalMockupUnitExportToSchedule");
  };

  /** \relates ExternalInterfaceFunctionalMockupUnitExportToSchedule*/
  using OptionalExternalInterfaceFunctionalMockupUnitExportToSchedule = boost::optional<ExternalInterfaceFunctionalMockupUnitExportToSchedule>;

  /** \relates ExternalInterfaceFunctionalMockupUnitExportToSchedule*/
  using ExternalInterfaceFunctionalMockupUnitExportToScheduleVector = std::vector<ExternalInterfaceFunctionalMockupUnitExportToSchedule>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITEXPORTTOSCHEDULE_HPP
