/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_DAYLIGHTREDIRECTIONDEVICE_HPP
#define MODEL_DAYLIGHTREDIRECTIONDEVICE_HPP

#include "ModelAPI.hpp"
#include "ShadingMaterial.hpp"

namespace openstudio {

namespace model {

  namespace detail {

    class DaylightRedirectionDevice_Impl;

  }  // namespace detail

  /** DaylightRedirectionDevice is a ShadingMaterial that wraps the OpenStudio IDD object 'OS:WindowMaterial:DaylightRedirectionDevice'.
 ** This object is not in EnergyPlus, this represents a film or louver which redirects daylighting.
 **/
  class MODEL_API DaylightRedirectionDevice : public ShadingMaterial
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit DaylightRedirectionDevice(const Model& model);

    virtual ~DaylightRedirectionDevice() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    DaylightRedirectionDevice(const DaylightRedirectionDevice& other) = default;
    DaylightRedirectionDevice(DaylightRedirectionDevice&& other) = default;
    DaylightRedirectionDevice& operator=(const DaylightRedirectionDevice&) = default;
    DaylightRedirectionDevice& operator=(DaylightRedirectionDevice&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> daylightRedirectionDeviceTypeValues();

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
    /// @cond
    using ImplType = detail::DaylightRedirectionDevice_Impl;

    explicit DaylightRedirectionDevice(std::shared_ptr<detail::DaylightRedirectionDevice_Impl> impl);

    friend class detail::DaylightRedirectionDevice_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.DaylightRedirectionDevice");
  };

  /** \relates DaylightRedirectionDevice*/
  using OptionalDaylightRedirectionDevice = boost::optional<DaylightRedirectionDevice>;

  /** \relates DaylightRedirectionDevice*/
  using DaylightRedirectionDeviceVector = std::vector<DaylightRedirectionDevice>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DAYLIGHTREDIRECTIONDEVICE_HPP
