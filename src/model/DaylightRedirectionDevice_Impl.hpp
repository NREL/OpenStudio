/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_DAYLIGHTREDIRECTIONDEVICE_IMPL_HPP
#define MODEL_DAYLIGHTREDIRECTIONDEVICE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ShadingMaterial_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class MODEL_API DaylightRedirectionDevice_Impl : public ShadingMaterial_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      DaylightRedirectionDevice_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      DaylightRedirectionDevice_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      DaylightRedirectionDevice_Impl(const DaylightRedirectionDevice_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~DaylightRedirectionDevice_Impl() override = default;

      //@}

      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      /** Throws because blind has beam and diffuse visible transmittance. */
      virtual boost::optional<double> getVisibleTransmittance() const override;

      //@}
      /** @name Getters */
      //@{

      std::string daylightRedirectionDeviceType() const;

      bool isDaylightRedirectionDeviceTypeDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setDaylightRedirectionDeviceType(const std::string& daylightRedirectionDeviceType);

      void resetDaylightRedirectionDeviceType();

      //@}
      /** @name Other */
      //@{

      //@}

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.DaylightRedirectionDevice");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_BLIND_IMPL_HPP
