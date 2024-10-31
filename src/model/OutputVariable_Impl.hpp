/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_OUTPUTVARIABLE_IMPL_HPP
#define MODEL_OUTPUTVARIABLE_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {

class ReportingFrequency;

namespace model {

  class Schedule;

  namespace detail {

    class MODEL_API OutputVariable_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      // constructor
      OutputVariable_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      OutputVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // copy constructor
      OutputVariable_Impl(const OutputVariable_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~OutputVariable_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      // Get all output variable names that could be associated with this object.
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual std::vector<openstudio::IdfObject> remove() override;

      //@}
      /** @name Getters */
      //@{

      std::string keyValue() const;

      bool isKeyValueDefaulted() const;

      std::string variableName() const;

      std::string reportingFrequency() const;

      bool isReportingFrequencyDefaulted() const;

      std::vector<std::string> reportingFrequencyValues() const;

      boost::optional<Schedule> schedule() const;

      bool exportToBCVTB() const;

      bool isExportToBCVTBDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setKeyValue(const std::string& keyValue);

      void resetKeyValue();

      bool setVariableName(const std::string& variableName);

      bool setReportingFrequency(const std::string& reportingFrequency);

      void resetReportingFrequency();

      bool setSchedule(Schedule& schedule);

      void resetSchedule();

      bool setExportToBCVTB(bool exportToBCVTB);

      void resetExportToBCVTB();

      //@}
     private:
      REGISTER_LOGGER("openstudio.model.OutputVariable");

      boost::optional<ModelObject> scheduleAsModelObject() const;

      bool setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OUTPUTVARIABLE_IMPL_HPP
