/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_MATERIALPROPERTYGLAZINGSPECTRALDATA_IMPL_HPP
#define MODEL_MATERIALPROPERTYGLAZINGSPECTRALDATA_IMPL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

  class SpectralDataField;

  namespace detail {

    /** MaterialPropertyGlazingSpectralData_Impl is a ResourceObject_Impl that is the implementation class for MaterialPropertyGlazingSpectralData.*/
    class MODEL_API MaterialPropertyGlazingSpectralData_Impl : public ResourceObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      MaterialPropertyGlazingSpectralData_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      MaterialPropertyGlazingSpectralData_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      MaterialPropertyGlazingSpectralData_Impl(const MaterialPropertyGlazingSpectralData_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~MaterialPropertyGlazingSpectralData_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      std::vector<SpectralDataField> spectralDataFields() const;

      //@}
      /** @name Setters */
      //@{

      bool setSpectralDataFields(const std::vector<SpectralDataField>& spectralDataFields);

      bool addSpectralDataField(const SpectralDataField& spectralDataField);

      bool addSpectralDataField(double wavelength, double transmittance, double frontReflectance, double backReflectance);

      void resetSpectralDataFields();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.MaterialPropertyGlazingSpectralData");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_MATERIALPROPERTYGLAZINGSPECTRALDATA_IMPL_HPP
