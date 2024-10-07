/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_LIFECYCLECOSTUSEPRICEESCALATION_HPP
#define MODEL_LIFECYCLECOSTUSEPRICEESCALATION_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"
#include "../utilities/core/Optional.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class LifeCycleCostUsePriceEscalation_Impl;
  }

  /** LifeCycleCostUsePriceEscalation derives from ParentObject and is an interface to the OpenStudio IDD object named "OS:LifeCycleCost:UsePriceEscalation".
 *
 *  LifeCycleCostUsePriceEscalation specifies escalation about energy and water costs separately from general inflation assumptions.
 */
  class MODEL_API LifeCycleCostUsePriceEscalation : public ParentObject
  {

   public:
    /** @name Constructors and Destructors */
    //@{
    virtual ~LifeCycleCostUsePriceEscalation() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    LifeCycleCostUsePriceEscalation(const LifeCycleCostUsePriceEscalation& other) = default;
    LifeCycleCostUsePriceEscalation(LifeCycleCostUsePriceEscalation&& other) = default;
    LifeCycleCostUsePriceEscalation& operator=(const LifeCycleCostUsePriceEscalation&) = default;
    LifeCycleCostUsePriceEscalation& operator=(LifeCycleCostUsePriceEscalation&&) = default;

    /// Constructs a new LifeCycleCostUsePriceEscalation object in the model.
    explicit LifeCycleCostUsePriceEscalation(const Model& model);

    //@}
    /** @name Getters */
    //@{

    boost::optional<std::string> resource() const;

    boost::optional<std::string> escalationStartMonth() const;

    boost::optional<int> escalationStartYear() const;

    /** Get the escalation for the year at index. Indexing starts at 0. */
    boost::optional<double> yearEscalation(unsigned index) const;

    //@}
    /** @name Setters */
    //@{

    bool setResource(const std::string& str);

    bool setEscalationStartMonth(const std::string& str);

    /** Set the escalation for the year at index. Indexing starts at 0. */
    bool setEscalationStartYear(int num);

    bool setYearEscalation(unsigned index, double num);

    //@}

    /// Returns the IddObjectType.
    static IddObjectType iddObjectType();

    /// Returns the number of years for which escalation data is specified, e.g. the number of extensible fields.
    unsigned numYears() const;

   protected:
    /// @cond

    using ImplType = detail::LifeCycleCostUsePriceEscalation_Impl;

    friend class Model;
    friend class IdfObject;

    // constructor
    explicit LifeCycleCostUsePriceEscalation(std::shared_ptr<detail::LifeCycleCostUsePriceEscalation_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.LifeCycleCostUsePriceEscalation");

    /// @endcond
  };

  /** \relates LifeCycleCostUsePriceEscalation */
  using OptionalLifeCycleCostUsePriceEscalation = boost::optional<LifeCycleCostUsePriceEscalation>;

  /** \relates LifeCycleCostUsePriceEscalation */
  using LifeCycleCostUsePriceEscalationVector = std::vector<LifeCycleCostUsePriceEscalation>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_LIFECYCLECOSTUSEPRICEESCALATION_HPP
