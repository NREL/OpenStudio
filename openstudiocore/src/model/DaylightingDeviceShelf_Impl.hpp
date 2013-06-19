/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef MODEL_DAYLIGHTINGDEVICESHELF_IMPL_HPP
#define MODEL_DAYLIGHTINGDEVICESHELF_IMPL_HPP

#include <model/ModelAPI.hpp>
#include <model/ModelObject_Impl.hpp>

namespace openstudio {
namespace model {

class SubSurface;
class DaylightingDeviceShelf;

namespace detail {

  /** DaylightingDeviceShelf_Impl is a ModelObject_Impl that is the implementation class for DaylightingDeviceShelf.*/
  class MODEL_API DaylightingDeviceShelf_Impl : public ModelObject_Impl {
    Q_OBJECT;
    Q_PROPERTY(boost::optional<double> viewFactortoOutsideShelf READ viewFactortoOutsideShelf WRITE setViewFactortoOutsideShelf RESET resetViewFactortoOutsideShelf);
   public:
    /** @name Constructors and Destructors */
    //@{

    DaylightingDeviceShelf_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    DaylightingDeviceShelf_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                Model_Impl* model,
                                bool keepHandle);

    DaylightingDeviceShelf_Impl(const DaylightingDeviceShelf_Impl& other,
                                Model_Impl* model,
                                bool keepHandle);

    virtual ~DaylightingDeviceShelf_Impl() {}

    //@}

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    /** @name Getters */
    //@{

    // TODO: Handle Non-Extensible IddField Window Name.

    // TODO: Handle Non-Extensible IddField Inside Shelf Name.

    // TODO: Handle Non-Extensible IddField Outside Shelf Name.

    boost::optional<double> viewFactortoOutsideShelf() const;

    //@}
    /** @name Setters */
    //@{

    // TODO: Handle Non-Extensible IddField Window Name.

    // TODO: Handle Non-Extensible IddField Inside Shelf Name.

    // TODO: Handle Non-Extensible IddField Outside Shelf Name.

    bool setViewFactortoOutsideShelf(boost::optional<double> viewFactortoOutsideShelf);

    bool setViewFactortoOutsideShelf(double viewFactortoOutsideShelf);

    void resetViewFactortoOutsideShelf();

    //@}
      
    /// Get the related sub surface.
    SubSurface subSurface() const;

    /// Set the related sub surface.
    bool setSubSurface(const SubSurface& subSurface);

   protected:
   private:
    REGISTER_LOGGER("openstudio.model.DaylightingDeviceShelf");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_DAYLIGHTINGDEVICESHELF_IMPL_HPP

