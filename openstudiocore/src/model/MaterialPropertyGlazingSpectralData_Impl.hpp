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

#ifndef MODEL_MATERIALPROPERTYGLAZINGSPECTRALDATA_IMPL_HPP
#define MODEL_MATERIALPROPERTYGLAZINGSPECTRALDATA_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

class SpectralDataField;

namespace detail {

  /** MaterialPropertyGlazingSpectralData_Impl is a ResourceObject_Impl that is the implementation class for MaterialPropertyGlazingSpectralData.*/
  class MODEL_API MaterialPropertyGlazingSpectralData_Impl : public ResourceObject_Impl {
  
  public:
    /** @name Constructors and Destructors */
    //@{

    MaterialPropertyGlazingSpectralData_Impl(const IdfObject& idfObject,
                                             Model_Impl* model,
                                             bool keepHandle);

    MaterialPropertyGlazingSpectralData_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle);

    MaterialPropertyGlazingSpectralData_Impl(const MaterialPropertyGlazingSpectralData_Impl& other,
                                             Model_Impl* model,
                                             bool keepHandle);

    virtual ~MaterialPropertyGlazingSpectralData_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

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

} // detail

} // model
} // openstudio

#endif // MODEL_MATERIALPROPERTYGLAZINGSPECTRALDATA_IMPL_HPP

