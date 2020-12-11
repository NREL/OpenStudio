/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "OutputJSON.hpp"
#include "OutputJSON_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_Output_JSON_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    OutputJSON_Impl::OutputJSON_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == OutputJSON::iddObjectType());
    }

    OutputJSON_Impl::OutputJSON_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == OutputJSON::iddObjectType());
    }

    OutputJSON_Impl::OutputJSON_Impl(const OutputJSON_Impl& other, Model_Impl* model, bool keepHandle) : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& OutputJSON_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType OutputJSON_Impl::iddObjectType() const {
      return OutputJSON::iddObjectType();
    }

    std::string OutputJSON_Impl::optionType() const {
      boost::optional<std::string> value = getString(OS_Output_JSONFields::OptionType, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool OutputJSON_Impl::outputJSON() const {
      return getBooleanFieldValue(OS_Output_JSONFields::OutputJSON);
    }

    bool OutputJSON_Impl::outputCBOR() const {
      return getBooleanFieldValue(OS_Output_JSONFields::OutputCBOR);
    }

    bool OutputJSON_Impl::outputMessagePack() const {
      return getBooleanFieldValue(OS_Output_JSONFields::OutputMessagePack);
    }

    bool OutputJSON_Impl::setOptionType(const std::string& optionType) {
      bool result = setString(OS_Output_JSONFields::OptionType, optionType);
      return result;
    }

    bool OutputJSON_Impl::setOutputJSON(bool outputJSON) {
      return setBooleanFieldValue(OS_Output_JSONFields::OutputJSON, outputJSON);
    }

    bool OutputJSON_Impl::setOutputCBOR(bool outputCBOR) {
      return setBooleanFieldValue(OS_Output_JSONFields::OutputCBOR, outputCBOR);
    }

    bool OutputJSON_Impl::setOutputMessagePack(bool outputMessagePack) {
      return setBooleanFieldValue(OS_Output_JSONFields::OutputMessagePack, outputMessagePack);
    }

  }  // namespace detail

  IddObjectType OutputJSON::iddObjectType() {
    return IddObjectType(IddObjectType::OS_Output_JSON);
  }

  std::vector<std::string> OutputJSON::optionTypeValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_Output_JSONFields::OptionType);
  }

  std::string OutputJSON::optionType() const {
    return getImpl<detail::OutputJSON_Impl>()->optionType();
  }

  bool OutputJSON::outputJSON() const {
    return getImpl<detail::OutputJSON_Impl>()->outputJSON();
  }

  bool OutputJSON::outputCBOR() const {
    return getImpl<detail::OutputJSON_Impl>()->outputCBOR();
  }

  bool OutputJSON::outputMessagePack() const {
    return getImpl<detail::OutputJSON_Impl>()->outputMessagePack();
  }

  bool OutputJSON::setOptionType(const std::string& optionType) {
    return getImpl<detail::OutputJSON_Impl>()->setOptionType(optionType);
  }

  bool OutputJSON::setOutputJSON(bool outputJSON) {
    return getImpl<detail::OutputJSON_Impl>()->setOutputJSON(outputJSON);
  }

  bool OutputJSON::setOutputCBOR(bool outputCBOR) {
    return getImpl<detail::OutputJSON_Impl>()->setOutputCBOR(outputCBOR);
  }

  bool OutputJSON::setOutputMessagePack(bool outputMessagePack) {
    return getImpl<detail::OutputJSON_Impl>()->setOutputMessagePack(outputMessagePack);
  }

  /// @cond
  OutputJSON::OutputJSON(std::shared_ptr<detail::OutputJSON_Impl> impl) : ModelObject(impl) {}
  OutputJSON::OutputJSON(Model& model) : ModelObject(OutputJSON::iddObjectType(), model) {
    // This is a required-field, so pick one...
    // might as well default to more (TimeSeriesAndTabular) than less (TimeSeries)
    setOptionType("TimeSeriesAndTabular");
    // Same defaults as EnergyPlus IDD
    setOutputJSON(true);
    setOutputCBOR(false);
    setOutputMessagePack(false);
  }

  /// @endcond

}  // namespace model
}  // namespace openstudio
