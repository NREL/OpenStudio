/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_OUTPUTDEBUGGINGDATA_IMPL_HPP
#define MODEL_OUTPUTDEBUGGINGDATA_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** OutputDebuggingData_Impl is a ModelObject_Impl that is the implementation class for OutputDebuggingData.*/
    class MODEL_API OutputDebuggingData_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      OutputDebuggingData_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      OutputDebuggingData_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      OutputDebuggingData_Impl(const OutputDebuggingData_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~OutputDebuggingData_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      bool reportDebuggingData() const;

      bool reportDuringWarmup() const;

      //@}
      /** @name Setters */
      //@{

      bool setReportDebuggingData(bool reportDebuggingData);

      bool setReportDuringWarmup(bool reportDuringWarmup);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.OutputDebuggingData");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OUTPUTDEBUGGINGDATA_IMPL_HPP
