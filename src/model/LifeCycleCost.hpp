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

    virtual ~LifeCycleCost() = default;
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
