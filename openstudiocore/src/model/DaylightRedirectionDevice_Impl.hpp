/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef MODEL_DAYLIGHTREDIRECTIONDEVICE_IMPL_HPP
#define MODEL_DAYLIGHTREDIRECTIONDEVICE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ShadingMaterial_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class MODEL_API DaylightRedirectionDevice_Impl : public ShadingMaterial_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    DaylightRedirectionDevice_Impl(const IdfObject& idfObject,
               Model_Impl* model,
               bool keepHandle);

    DaylightRedirectionDevice_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
               Model_Impl* model,
               bool keepHandle);

    DaylightRedirectionDevice_Impl(const DaylightRedirectionDevice_Impl& other,
               Model_Impl* model,
               bool keepHandle);

    virtual ~DaylightRedirectionDevice_Impl() {}

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

} // detail

} // model
} // openstudio

#endif // MODEL_BLIND_IMPL_HPP

