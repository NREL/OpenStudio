/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITEXPORTTOSCHEDULE_IMPL_HPP
#define MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITEXPORTTOSCHEDULE_IMPL_HPP

#include "ModelAPI.hpp"
#include "Schedule_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl is a ModelObject_Impl that is the implementation class for ExternalInterfaceFunctionalMockupUnitExportToSchedule.*/
    class MODEL_API ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl : public Schedule_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                 bool keepHandle);

      ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl(const ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl& other,
                                                                 Model_Impl* model, bool keepHandle);

      virtual ~ExternalInterfaceFunctionalMockupUnitExportToSchedule_Impl() override = default;

      //@}
      /** @name Getters */
      //@{

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      // return any children objects in the hierarchy
      virtual std::vector<ModelObject> children() const override;

      // Get all output variable names that could be associated with this object.
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual boost::optional<ScheduleTypeLimits> scheduleTypeLimits() const override;

      virtual std::vector<double> values() const override;

      std::string fMUVariableName() const;

      double initialValue() const;

      //@}
      /** @name Setters */
      //@{

      virtual bool setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) override;

      virtual bool resetScheduleTypeLimits() override;

      bool setFMUVariableName(const std::string& fMUVariableName);

      bool setInitialValue(double initialValue);

      // ensure that this object does not contain the date 2/29
      virtual void ensureNoLeapDays() override;

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ExternalInterfaceFunctionalMockupUnitExportToSchedule");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITEXPORTTOSCHEDULE_IMPL_HPP
