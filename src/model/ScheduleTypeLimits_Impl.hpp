/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SCHEDULETYPELIMITS_IMPL_HPP
#define MODEL_SCHEDULETYPELIMITS_IMPL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** ScheduleTypeLimits_Impl is a ResourceObject_Impl that is the implementation class for ScheduleTypeLimits.*/
    class MODEL_API ScheduleTypeLimits_Impl : public ResourceObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      ScheduleTypeLimits_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ScheduleTypeLimits_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ScheduleTypeLimits_Impl(const ScheduleTypeLimits_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ScheduleTypeLimits_Impl() override = default;

      //@}

      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<double> lowerLimitValue() const;

      boost::optional<double> upperLimitValue() const;

      boost::optional<std::string> numericType() const;

      std::string unitType() const;

      bool isUnitTypeDefaulted() const;

      boost::optional<Unit> units(bool returnIP = false) const;

      //@}
      /** @name Setters */
      //@{

      bool setLowerLimitValue(double lowerLimitValue);

      void resetLowerLimitValue();

      bool setUpperLimitValue(double upperLimitValue);

      void resetUpperLimitValue();

      bool setNumericType(const std::string& numericType);

      void resetNumericType();

      bool setUnitType(const std::string& unitType);

      void resetUnitType();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ScheduleTypeLimits");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULETYPELIMITS_IMPL_HPP
