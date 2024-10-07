/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
    virtual ~CurrencyType() override = default;
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
