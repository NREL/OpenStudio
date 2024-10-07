/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERNALINTERFACEACTUATOR_IMPL_HPP
#define MODEL_EXTERNALINTERFACEACTUATOR_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** ExternalInterfaceActuator_Impl is a ModelObject_Impl that is the implementation class for ExternalInterfaceActuator.*/
    class MODEL_API ExternalInterfaceActuator_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ExternalInterfaceActuator_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ExternalInterfaceActuator_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ExternalInterfaceActuator_Impl(const ExternalInterfaceActuator_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ExternalInterfaceActuator_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      ModelObject actuatedComponentUnique() const;

      std::string actuatedComponentType() const;

      std::string actuatedComponentControlType() const;

      boost::optional<double> optionalInitialValue() const;

      bool exportToBCVTB() const;

      bool isExportToBCVTBDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setActuatedComponentUnique(const ModelObject& allObjects);

      bool setActuatedComponentType(const std::string& actuatedComponentType);

      bool setActuatedComponentControlType(const std::string& actuatedComponentControlType);

      bool setOptionalInitialValue(double optionalInitialValue);

      void resetOptionalInitialValue();

      bool setExportToBCVTB(bool exportToBCVTB);

      void resetExportToBCVTB();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ExternalInterfaceActuator");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERNALINTERFACEACTUATOR_IMPL_HPP
