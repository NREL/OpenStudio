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
      virtual ~LifeCycleCost_Impl() = default;

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
