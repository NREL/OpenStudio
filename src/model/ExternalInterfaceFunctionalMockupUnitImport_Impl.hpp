/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITIMPORT_IMPL_HPP
#define MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITIMPORT_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** ExternalInterfaceFunctionalMockupUnitImport_Impl is a ModelObject_Impl that is the implementation class for ExternalInterfaceFunctionalMockupUnitImport.*/
    class MODEL_API ExternalInterfaceFunctionalMockupUnitImport_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ExternalInterfaceFunctionalMockupUnitImport_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ExternalInterfaceFunctionalMockupUnitImport_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ExternalInterfaceFunctionalMockupUnitImport_Impl(const ExternalInterfaceFunctionalMockupUnitImport_Impl& other, Model_Impl* model,
                                                       bool keepHandle);

      virtual ~ExternalInterfaceFunctionalMockupUnitImport_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      double fMUTimeout() const;

      bool isFMUTimeoutDefaulted() const;

      int fMULoggingOn() const;

      bool isFMULoggingOnDefaulted() const;

      std::string fMUFileName() const;

      //@}
      /** @name Setters */
      //@{

      bool setFMUTimeout(double fMUTimeout);

      void resetFMUTimeout();

      bool setFMULoggingOn(int fMULoggingOn);

      void resetFMULoggingOn();

      bool setFMUFileName(const std::string& fMUName);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ExternalInterfaceFunctionalMockupUnitImport");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_EXTERNALINTERFACEFUNCTIONALMOCKUPUNITIMPORT_IMPL_HPP
