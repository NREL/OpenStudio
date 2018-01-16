/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITIMPORTFROMVARIABLE_HPP
#define MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITIMPORTFROMVARIABLE_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"
#include "ExternalInterfaceFunctionalMockupUnitImport.hpp"
#include "ExternalInterfaceFunctionalMockupUnitImport_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl;

} // detail

/** ExternalInterfaceFunctionalMockupUnitImportFromVariable is a ModelObject that wraps the OpenStudio IDD object 'OS:ExternalInterface:FunctionalMockupUnitImport:From:Variable'. */
class MODEL_API ExternalInterfaceFunctionalMockupUnitImportFromVariable : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

   explicit ExternalInterfaceFunctionalMockupUnitImportFromVariable(const Model& model,
                                                                    const std::string& outputVariableIndexKeyName,
                                                                    const std::string& outputVariableName,
                                                                    const ExternalInterfaceFunctionalMockupUnitImport& fMUFile,
                                                                    const std::string& fMUInstanceName,
                                                                    const std::string& fMUVariableName);

  virtual ~ExternalInterfaceFunctionalMockupUnitImportFromVariable() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  std::string outputVariableIndexKeyName() const;

  std::string outputVariableName() const;

  ExternalInterfaceFunctionalMockupUnitImport fMUFile() const;

  std::string fMUInstanceName() const;

  std::string fMUVariableName() const;

  //@}
  /** @name Setters */
  //@{

  bool setOutputVariableIndexKeyName(const std::string& outputVariableIndexKeyName);

  bool setOutputVariableName(const std::string& outputVariableName);

  bool setFMUFile(const ExternalInterfaceFunctionalMockupUnitImport& fMUFile);

  bool setFMUInstanceName(const std::string& fMUInstanceName);

  bool setFMUVariableName(const std::string& fMUVariableName);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl ImplType;

  explicit ExternalInterfaceFunctionalMockupUnitImportFromVariable(std::shared_ptr<detail::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl> impl);

  friend class detail::ExternalInterfaceFunctionalMockupUnitImportFromVariable_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ExternalInterfaceFunctionalMockupUnitImportFromVariable");
};

/** \relates ExternalInterfaceFunctionalMockupUnitImportFromVariable*/
typedef boost::optional<ExternalInterfaceFunctionalMockupUnitImportFromVariable> OptionalExternalInterfaceFunctionalMockupUnitImportFromVariable;

/** \relates ExternalInterfaceFunctionalMockupUnitImportFromVariable*/
typedef std::vector<ExternalInterfaceFunctionalMockupUnitImportFromVariable> ExternalInterfaceFunctionalMockupUnitImportFromVariableVector;

} // model
} // openstudio

#endif // MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITIMPORTFROMVARIABLE_HPP
