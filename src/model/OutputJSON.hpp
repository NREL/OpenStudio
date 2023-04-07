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

#ifndef MODEL_OUTPUTJSON_HPP
#define MODEL_OUTPUTJSON_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class OutputJSON_Impl;

  }  // namespace detail

  /** OutputJSON is a ModelObject that wraps the OpenStudio IDD object 'OS:Output:JSON'. */
  class MODEL_API OutputJSON : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~OutputJSON() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    OutputJSON(const OutputJSON& other) = default;
    OutputJSON(OutputJSON&& other) = default;
    OutputJSON& operator=(const OutputJSON&) = default;
    OutputJSON& operator=(OutputJSON&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> optionTypeValues();

    /** @name Getters */
    //@{

    std::string optionType() const;

    bool outputJSON() const;

    bool outputCBOR() const;

    bool outputMessagePack() const;

    //@}
    /** @name Setters */
    //@{

    bool setOptionType(const std::string& optionType);

    bool setOutputJSON(bool outputJSON);

    bool setOutputCBOR(bool outputCBOR);

    bool setOutputMessagePack(bool outputMessagePack);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::OutputJSON_Impl;

    explicit OutputJSON(std::shared_ptr<detail::OutputJSON_Impl> impl);

    friend class detail::OutputJSON_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    explicit OutputJSON(Model& model);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.OutputJSON");
  };

  /** \relates OutputJSON*/
  using OptionalOutputJSON = boost::optional<OutputJSON>;

  /** \relates OutputJSON*/
  using OutputJSONVector = std::vector<OutputJSON>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OUTPUTJSON_HPP
