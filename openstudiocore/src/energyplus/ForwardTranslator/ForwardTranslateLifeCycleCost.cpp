/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/LifeCycleCost.hpp"
#include "../../model/LifeCycleCostParameters.hpp"
#include "../../model/LifeCycleCostParameters_Impl.hpp"

#include <utilities/idd/LifeCycleCost_NonrecurringCost_FieldEnums.hxx>
#include <utilities/idd/LifeCycleCost_RecurringCosts_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateLifeCycleCost( LifeCycleCost& modelObject )
{
  boost::optional<LifeCycleCostParameters> lifeCycleCostParameters = modelObject.model().lifeCycleCostParameters();
  if (!lifeCycleCostParameters){
    lifeCycleCostParameters = modelObject.model().getUniqueModelObject<LifeCycleCostParameters>();
  }

  int lengthOfStudyPeriodInMonths = 12*lifeCycleCostParameters->lengthOfStudyPeriodInYears();

  boost::optional<IdfObject> result;

  ModelObject item = modelObject.item();
  std::string comment = item.iddObject().name() + ", " + item.name().get();

  std::string name = modelObject.name().get();
  std::string category = modelObject.category();
  double totalCost = modelObject.totalCost();
  int yearsFromStart = modelObject.yearsFromStart();
  int monthsFromStart = modelObject.monthsFromStart();
  int repeatPeriodYears = modelObject.repeatPeriodYears();
  int repeatPeriodMonths = modelObject.repeatPeriodMonths();

  if (totalCost == 0){
    return boost::none;
  }

  bool hasRepeatingCosts = (repeatPeriodYears > 0 || repeatPeriodMonths > 0);

  // check for field types of LifeCycleCost:NonrecurringCost {Construction, Salvage, OtherCapital}
  if (istringEqual("Construction", category) ||
      istringEqual("Salvage", category) ||
      istringEqual("OtherCapital", category)){

    if (lengthOfStudyPeriodInMonths >= 12*yearsFromStart + monthsFromStart){

      // convert to LifeCycleCost::NonrecurringCosts
      IdfObject idfObject(openstudio::IddObjectType::LifeCycleCost_NonrecurringCost);
      idfObject.setComment(comment);
      m_idfObjects.push_back(idfObject);

      idfObject.setString(LifeCycleCost_NonrecurringCostFields::Name, name);
      idfObject.setString(LifeCycleCost_NonrecurringCostFields::Category, category);
      idfObject.setDouble(LifeCycleCost_NonrecurringCostFields::Cost, totalCost);
      idfObject.setString(LifeCycleCost_NonrecurringCostFields::StartofCosts, "ServicePeriod");

      if (yearsFromStart > 0){
        idfObject.setInt(LifeCycleCost_NonrecurringCostFields::YearsfromStart, yearsFromStart);
      }

      if (monthsFromStart > 0){
        idfObject.setInt(LifeCycleCost_NonrecurringCostFields::MonthsfromStart, monthsFromStart);
      }

      result = idfObject;

      // may create a recurring cost object to handle replacement costs, shift first replacement costs back one period
      if (istringEqual("Construction", category)){
        name += " Replacement";
      }else {
        name += " Recurring";
      }
      category = "Replacement";
      yearsFromStart += repeatPeriodYears;
      monthsFromStart += repeatPeriodMonths;

    }else{
      //LOG(Warn, "LifeCycleCost '" << name << "' occurs outside the study period, will not be translated");
    }
  }

  if (hasRepeatingCosts) {

    if (lengthOfStudyPeriodInMonths >= 12*yearsFromStart + monthsFromStart){

      // also create a LifeCycleCost::RecurringCosts to capture replacement costs
      IdfObject idfObject(openstudio::IddObjectType::LifeCycleCost_RecurringCosts);
      idfObject.setComment(comment);
      m_idfObjects.push_back(idfObject);

      idfObject.setString(LifeCycleCost_RecurringCostsFields::Name, name);
      idfObject.setString(LifeCycleCost_RecurringCostsFields::Category, category);
      idfObject.setDouble(LifeCycleCost_RecurringCostsFields::Cost, totalCost);
      idfObject.setString(LifeCycleCost_RecurringCostsFields::StartofCosts, "ServicePeriod");

      if (yearsFromStart > 0){
        idfObject.setInt(LifeCycleCost_RecurringCostsFields::YearsfromStart, yearsFromStart);
      }

      if (monthsFromStart > 0){
        idfObject.setInt(LifeCycleCost_RecurringCostsFields::MonthsfromStart, monthsFromStart);
      }

      if (repeatPeriodYears > 0){
        idfObject.setInt(LifeCycleCost_RecurringCostsFields::RepeatPeriodYears, repeatPeriodYears);
      }

      if (repeatPeriodMonths > 0){
        idfObject.setInt(LifeCycleCost_RecurringCostsFields::RepeatPeriodMonths, repeatPeriodMonths);
      }

      if (!result){
        result = idfObject;
      }
    }else{
      //LOG(Warn, "LifeCycleCost '" << name << "' occurs outside the study period, will not be translated");
    }
  }

  return result;
}

} // energyplus

} // openstudio

