/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_LIFECYCLECOST_IMPL_HPP
#define MODEL_LIFECYCLECOST_IMPL_HPP

#include "ParentObject_Impl.hpp"
#include "LifeCycleCost.hpp"

#include "../utilities/core/Optional.hpp"

namespace openstudio {
namespace model {

  class SpaceLoadDefinition;

  namespace detail {

    class MODEL_API LifeCycleCost_Impl : public ModelObject_Impl
    {

     public:
      // constructor
      LifeCycleCost_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      LifeCycleCost_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // clone copy constructor
      LifeCycleCost_Impl(const LifeCycleCost_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~LifeCycleCost_Impl() override = default;

      virtual IddObjectType iddObjectType() const override {
        return LifeCycleCost::iddObjectType();
      }

      virtual const std::vector<std::string>& outputVariableNames() const override;

      /** @name Getters */
      //@{

      std::string category() const;

      std::string itemType() const;

      ModelObject item() const;

      double cost() const;

      std::vector<std::string> validCostUnitsValues() const;

      std::string costUnits() const;

      std::string startOfCosts() const;
      bool isStartOfCostsDefaulted() const;

      int yearsFromStart() const;
      bool isYearsFromStartDefaulted() const;
      int monthsFromStart() const;
      bool isMonthsFromStartDefaulted() const;

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

      double totalCost() const;

      bool convertToCostPerEach();

      boost::optional<int> costedQuantity() const;

      boost::optional<double> costedArea() const;

      boost::optional<int> costedThermalZones() const;

     private:
      REGISTER_LOGGER("openstudio.model.LifeCycleCost");

      //double getArea(const SpaceLoadInstance& spaceLoadInstance) const;
      //double getArea(const SpaceLoadDefinition& spaceLoadDefinition) const;
    };

  }  // namespace detail
}  // namespace model
}  // namespace openstudio

#endif  // MODEL_LIFECYCLECOST_IMPL_HPP
