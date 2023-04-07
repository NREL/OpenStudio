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

#ifndef MODEL_OUTPUTDIAGNOSTICS_HPP
#define MODEL_OUTPUTDIAGNOSTICS_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class OutputDiagnostics_Impl;

  }  // namespace detail

  /** OutputDiagnostics is a ModelObject that wraps the OpenStudio IDD object 'OS:Output:Diagnostics'. */
  class MODEL_API OutputDiagnostics : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~OutputDiagnostics() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    OutputDiagnostics(const OutputDiagnostics& other) = default;
    OutputDiagnostics(OutputDiagnostics&& other) = default;
    OutputDiagnostics& operator=(const OutputDiagnostics&) = default;
    OutputDiagnostics& operator=(OutputDiagnostics&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> keyValues();
    static std::vector<std::string> validKeyValues();

    /** @name Getters */
    //@{

    std::vector<std::string> keys() const;

    //@}
    /** @name Setters */
    //@{

    // Return false if key isn't valid. If is already present, not added twice (ensures unicity) and logs an info in that case too
    bool addKey(const std::string& key);

    // Calls clearKeys, then for each k, addKey(k)
    bool setKeys(const std::vector<std::string>& keys);

    // Just a convenience function to add 'DisplayExtraWarnings' as a key since it's so common
    bool enableDisplayExtraWarnings();

    // Clears out every key
    void clearKeys();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::OutputDiagnostics_Impl;

    explicit OutputDiagnostics(std::shared_ptr<detail::OutputDiagnostics_Impl> impl);

    friend class detail::OutputDiagnostics_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    explicit OutputDiagnostics(Model& model);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.OutputDiagnostics");
  };

  /** \relates OutputDiagnostics*/
  using OptionalOutputDiagnostics = boost::optional<OutputDiagnostics>;

  /** \relates OutputDiagnostics*/
  using OutputDiagnosticsVector = std::vector<OutputDiagnostics>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OUTPUTDIAGNOSTICS_HPP
