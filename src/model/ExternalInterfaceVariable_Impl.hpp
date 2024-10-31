/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERNALINTERFACEVARIABLE_IMPL_HPP
#define MODEL_EXTERNALINTERFACEVARIABLE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** ExternalInterfaceVariable_Impl is a ModelObject_Impl that is the implementation class for ExternalInterfaceVariable.*/
    class MODEL_API ExternalInterfaceVariable_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ExternalInterfaceVariable_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ExternalInterfaceVariable_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ExternalInterfaceVariable_Impl(const ExternalInterfaceVariable_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ExternalInterfaceVariable_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      double initialValue() const;

      bool exportToBCVTB() const;

      bool isExportToBCVTBDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setInitialValue(double initialValue);

      bool setExportToBCVTB(bool exportToBCVTB);

      void resetExportToBCVTB();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ExternalInterfaceVariable");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERNALINTERFACEVARIABLE_IMPL_HPP
