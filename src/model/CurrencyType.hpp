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

#ifndef MODEL_CURRENCYTYPE_HPP
#define MODEL_CURRENCYTYPE_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

#include "../utilities/core/Optional.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class CurrencyType_Impl;
  }

  /** CurrencyType derives from ParentObject and is an interface to the OpenStudio IDD object named "OS:CurrencyType".
 *
 *  CurrencyType is a unique object used to specify the currency that cost data are entered and presented in for this Model.
 */
  class MODEL_API CurrencyType : public ParentObject
  {

   public:
    virtual ~CurrencyType() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    CurrencyType(const CurrencyType& other) = default;
    CurrencyType(CurrencyType&& other) = default;
    CurrencyType& operator=(const CurrencyType&) = default;
    CurrencyType& operator=(CurrencyType&&) = default;

    /** @name Getters */
    //@{

    boost::optional<std::string> monetaryUnit() const;

    //@}
    /** @name Setters */
    //@{

    bool setMonetaryUnit(const std::string& monetaryUnit);

    //@}

    /// Returns the IddObjectType.
    static IddObjectType iddObjectType();

   protected:
    /** @name Constructors and Destructors */
    //@{

    /// Constructs a new CurrencyType object in the model.
    explicit CurrencyType(const Model& model);

    //@}

    /// @cond

    using ImplType = detail::CurrencyType_Impl;

    friend class Model;
    friend class IdfObject;

    // constructor
    explicit CurrencyType(std::shared_ptr<detail::CurrencyType_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.CurrencyType");

    /// @endcond
  };

  /** \relates CurrencyType */
  using OptionalCurrencyType = boost::optional<CurrencyType>;

  /** \relates CurrencyType */
  using CurrencyTypeVector = std::vector<CurrencyType>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURRENCYTYPE_HPP
