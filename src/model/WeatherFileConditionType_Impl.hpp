/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_WEATHERFILECONDITIONTYPE_IMPL_HPP
#define MODEL_WEATHERFILECONDITIONTYPE_IMPL_HPP

#include "SizingPeriod_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class MODEL_API WeatherFileConditionType_Impl : public SizingPeriod_Impl
    {
     public:
      // constructor
      WeatherFileConditionType_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      WeatherFileConditionType_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // clone copy constructor
      WeatherFileConditionType_Impl(const WeatherFileConditionType_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~WeatherFileConditionType_Impl() override = default;

      // Get all output variable names that could be associated with this object.
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      // ensure that this object does not contain the date 2/29
      virtual void ensureNoLeapDays() override;

     private:
      REGISTER_LOGGER("openstudio.model.WeatherFileConditionType");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_WEATHERFILECONDITIONTYPE_IMPL_HPP
