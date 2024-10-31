/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_RUNPERIOD_IMPL_HPP
#define MODEL_RUNPERIOD_IMPL_HPP

#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class MODEL_API RunPeriod_Impl : public ParentObject_Impl
    {
     public:
      // constructor
      RunPeriod_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      RunPeriod_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // clone copy constructor
      RunPeriod_Impl(const RunPeriod_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~RunPeriod_Impl() override = default;

      int getBeginMonth() const;
      int getBeginDayOfMonth() const;
      int getEndMonth() const;
      int getEndDayOfMonth() const;
      bool getUseWeatherFileHolidays() const;
      bool getUseWeatherFileDaylightSavings() const;
      bool getApplyWeekendHolidayRule() const;
      bool getUseWeatherFileRainInd() const;
      bool getUseWeatherFileSnowInd() const;
      int getNumTimePeriodRepeats() const;

      bool setBeginMonth(int month);
      bool setBeginDayOfMonth(int day);
      bool setEndMonth(int month);
      bool setEndDayOfMonth(int day);
      bool setUseWeatherFileHolidays(bool use);
      bool setUseWeatherFileDaylightSavings(bool use);
      bool setApplyWeekendHolidayRule(bool apply);
      bool setUseWeatherFileRainInd(bool rainInd);
      bool setUseWeatherFileSnowInd(bool snowInd);
      bool setNumTimePeriodRepeats(int numRepeats);

      // ensure that this object does not contain the date 2/29
      void ensureNoLeapDays();

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      // set the parent, child may have to call methods on the parent
      virtual bool setParent(ParentObject& newParent) override;

      // return any children objects in the hierarchy
      virtual std::vector<ModelObject> children() const override;

      /// get a vector of allowable children types
      virtual std::vector<IddObjectType> allowableChildTypes() const override;

      // Get all output variable names that could be associated with this object.
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      /** @name Queries */
      //@{

      bool isAnnual() const;

      bool isPartialYear() const;

      bool isRepeated() const;

      //@}

     private:
      REGISTER_LOGGER("openstudio.model.RunPeriod");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_RUNPERIOD_IMPL_HPP
