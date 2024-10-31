/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERNALINTERFACESCHEDULE_IMPL_HPP
#define MODEL_EXTERNALINTERFACESCHEDULE_IMPL_HPP

#include "ModelAPI.hpp"
#include "Schedule_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** ExternalInterfaceSchedule_Impl is a ModelObject_Impl that is the implementation class for ExternalInterfaceSchedule.*/
    class MODEL_API ExternalInterfaceSchedule_Impl : public Schedule_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ExternalInterfaceSchedule_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ExternalInterfaceSchedule_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ExternalInterfaceSchedule_Impl(const ExternalInterfaceSchedule_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ExternalInterfaceSchedule_Impl() override = default;

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

      double initialValue() const;

      bool exportToBCVTB() const;

      bool isExportToBCVTBDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      virtual bool setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits) override;

      virtual bool resetScheduleTypeLimits() override;

      bool setInitialValue(double initialValue);

      bool setExportToBCVTB(bool exportToBCVTB);

      void resetExportToBCVTB();

      // ensure that this object does not contain the date 2/29
      virtual void ensureNoLeapDays() override;

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ExternalInterfaceSchedule");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERNALINTERFACESCHEDULE_IMPL_HPP
