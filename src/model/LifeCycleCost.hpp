/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_LIFECYCLECOST_HPP
#define MODEL_LIFECYCLECOST_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "ModelExtensibleGroup.hpp"

#include "../utilities/core/Optional.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class LifeCycleCost_Impl;
  }

  /** LifeCycleCost derives from ModelObject and is an interface to the OpenStudio IDD object named "OS:LifeCycleCost".
 *
 *  LifeCycleCost objects are used to add costs related to building construction and operation.
 *  The LifeCycleCost's lineItemType will be set based on the type of ModelObject.
 */
  class MODEL_API LifeCycleCost : public ModelObject
  {

   public:
    /** @name Constructors and Destructors */
    //@{

    /// Constructs a new LifeCycleCost object in the model.
    /// This will throw if the object type is not known
    explicit LifeCycleCost(const ModelObject& modelObject);

    virtual ~LifeCycleCost() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    LifeCycleCost(const LifeCycleCost& other) = default;
    LifeCycleCost(LifeCycleCost&& other) = default;
    LifeCycleCost& operator=(const LifeCycleCost&) = default;
    LifeCycleCost& operator=(LifeCycleCost&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    static std::vector<std::string> validCategoryValues();

    static std::vector<std::string> validItemTypeValues();

    static std::vector<std::string> validStartOfCostsValues();

    /// Create a LifeCycleCost for the given ModelObject with cost, costUnits, category, and optional repeatPeriodYear.
    /// Will return an empty result if ModelObject type is unknown or if costUnits or category are not valid
    static boost::optional<LifeCycleCost> createLifeCycleCost(const std::string& name, const ModelObject& modelObject, double cost,
                                                              const std::string& costUnits, const std::string& category, int repeatPeriodYears = 0,
                                                              int yearsFromStart = 0);

    //@}
    /** @name Getters */
    //@{

    /// Returns the category
    std::string category() const;

    /// Returns the type of ModelObject this LifeCycleCost applies to
    std::string itemType() const;

    /// Returns the ModelObject this LifeCycleCost applies to
    ModelObject item() const;

    /// Returns the cost
    double cost() const;

    /// Returns a list of valid cost units based on lineItemType.
    std::vector<std::string> validCostUnitsValues() const;

    /// Returns the cost units
    std::string costUnits() const;

    /// The First Year of Cost is based on the number of years past the Start of Costs. For most maintenance costs the Start of Costs should be Service Period.
    std::string startOfCosts() const;
    bool isStartOfCostsDefaulted() const;

    /// Years from start is added to months from start to give the time at which the costs start to occur.
    int yearsFromStart() const;
    bool isYearsFromStartDefaulted() const;
    int monthsFromStart() const;
    bool isMonthsFromStartDefaulted() const;

    /// Repeat period years is added to repeat period months to give the interval at which this cost reccurs.
    /// If both repeat period years and repeat period months are zero (default), this cost does not recurr.
    int repeatPeriodYears() const;
    bool isRepeatPeriodYearsDefaulted() const;
    int repeatPeriodMonths() const;
    bool isRepeatPeriodMonthsDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setCategory(const std::string& category);

    bool setCost(double cost);

    bool setCostUnits(const std::string& costUnits);

    bool setStartOfCosts(const std::string& startOfCosts);
    void resetStartOfCosts();

    bool setYearsFromStart(int yearsFromStart);
    void resetYearsFromStart();
    bool setMonthsFromStart(int monthsFromStart);
    void resetMonthsFromStart();

    bool setRepeatPeriodYears(int repeatPeriodYears);
    void resetRepeatPeriodYears();
    bool setRepeatPeriodMonths(int repeatPeriodMonths);
    void resetRepeatPeriodMonths();

    //@}

    /// Returns the total cost based on number of units.
    double totalCost() const;

    /// Attemps to convert cost units to "CostPerEach" and set cost to totalCost
    bool convertToCostPerEach();

    /// Compute the number of objects that would be used for CostPerEach.
    /// Includes space and instance multipliers.
    boost::optional<int> costedQuantity() const;

    /// Compute the area that would be used for CostPerArea.
    /// Includes space and instance multipliers.
    boost::optional<double> costedArea() const;

    /// Compute the number of ThermalZone objects that would be used for CostPerThermalZone.
    /// Includes space and instance multipliers.
    boost::optional<int> costedThermalZones() const;

   protected:
    /// @cond
    using ImplType = detail::LifeCycleCost_Impl;

    friend class Model;
    friend class IdfObject;

    // constructor
    explicit LifeCycleCost(std::shared_ptr<detail::LifeCycleCost_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.LifeCycleCost");

    /// @endcond
  };

  /** \relates LifeCycleCost */
  using OptionalLifeCycleCost = boost::optional<LifeCycleCost>;

  /** \relates LifeCycleCost */
  using LifeCycleCostVector = std::vector<LifeCycleCost>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_LIFECYCLECOST_HPP
