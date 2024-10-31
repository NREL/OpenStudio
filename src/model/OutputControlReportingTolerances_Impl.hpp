/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_OUTPUTCONTROLREPORTINGTOLERANCES_IMPL_HPP
#define MODEL_OUTPUTCONTROLREPORTINGTOLERANCES_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** OutputControlReportingTolerances_Impl is a ModelObject_Impl that is the implementation class for OutputControlReportingTolerances.*/
    class MODEL_API OutputControlReportingTolerances_Impl : public ModelObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      OutputControlReportingTolerances_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      OutputControlReportingTolerances_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      OutputControlReportingTolerances_Impl(const OutputControlReportingTolerances_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~OutputControlReportingTolerances_Impl() override = default;

      //@}

      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      double toleranceforTimeHeatingSetpointNotMet() const;

      bool isToleranceforTimeHeatingSetpointNotMetDefaulted() const;

      double toleranceforTimeCoolingSetpointNotMet() const;

      bool isToleranceforTimeCoolingSetpointNotMetDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setToleranceforTimeHeatingSetpointNotMet(double toleranceforTimeHeatingSetpointNotMet);

      void resetToleranceforTimeHeatingSetpointNotMet();

      bool setToleranceforTimeCoolingSetpointNotMet(double toleranceforTimeCoolingSetpointNotMet);

      void resetToleranceforTimeCoolingSetpointNotMet();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.OutputControlReportingTolerances");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OUTPUTCONTROLREPORTINGTOLERANCES_IMPL_HPP
