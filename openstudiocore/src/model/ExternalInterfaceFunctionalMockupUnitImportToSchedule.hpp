/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITIMPORTTOSCHEDULE_HPP
#define MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITIMPORTTOSCHEDULE_HPP

#include <model/ModelAPI.hpp>
#include "Schedule.hpp"

#include "ExternalInterfaceFunctionalMockupUnitImport.hpp"
#include "ExternalInterfaceFunctionalMockupUnitImport_Impl.hpp"

namespace openstudio {

namespace model {

namespace detail {

  class ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl;

} // detail

/** ExternalInterfaceFunctionalMockupUnitImportToSchedule is a ModelObject that wraps the OpenStudio IDD object 'OS:ExternalInterface:FunctionalMockupUnitImport:To:Schedule'. */
class MODEL_API ExternalInterfaceFunctionalMockupUnitImportToSchedule : public Schedule {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ExternalInterfaceFunctionalMockupUnitImportToSchedule(const Model& model,
                                                                 const ExternalInterfaceFunctionalMockupUnitImport& fMUFile,
                                                                 const std::string& fMUInstanceName,
                                                                 const std::string& fMUVariableName,
                                                                 double initialValue);

  virtual ~ExternalInterfaceFunctionalMockupUnitImportToSchedule() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  ExternalInterfaceFunctionalMockupUnitImport fMUFile() const;

  std::string fMUInstanceName() const;

  std::string fMUVariableName() const;

  double initialValue() const;

  //@}
  /** @name Setters */
  //@{

  bool setFMUFile(const ExternalInterfaceFunctionalMockupUnitImport& fMUFile);

  bool setFMUInstanceName(const std::string& fMUInstanceName);

  bool setFMUVariableName(const std::string& fMUVariableName);

  bool setInitialValue(double initialValue);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl ImplType;

  explicit ExternalInterfaceFunctionalMockupUnitImportToSchedule(std::shared_ptr<detail::ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl> impl);

  friend class detail::ExternalInterfaceFunctionalMockupUnitImportToSchedule_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ExternalInterfaceFunctionalMockupUnitImportToSchedule");
};

/** \relates ExternalInterfaceFunctionalMockupUnitImportToSchedule*/
typedef boost::optional<ExternalInterfaceFunctionalMockupUnitImportToSchedule> OptionalExternalInterfaceFunctionalMockupUnitImportToSchedule;

/** \relates ExternalInterfaceFunctionalMockupUnitImportToSchedule*/
typedef std::vector<ExternalInterfaceFunctionalMockupUnitImportToSchedule> ExternalInterfaceFunctionalMockupUnitImportToScheduleVector;

} // model
} // openstudio

#endif // MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITIMPORTTOSCHEDULE_HPP
