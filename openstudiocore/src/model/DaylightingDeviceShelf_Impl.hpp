/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

class SubSurface;
class InteriorPartitionSurface;
class ShadingSurface;
class DaylightingDeviceShelf;

namespace detail {

  /** DaylightingDeviceShelf_Impl is a ModelObject_Impl that is the implementation class for DaylightingDeviceShelf.*/
  class MODEL_API DaylightingDeviceShelf_Impl : public ModelObject_Impl {
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

    boost::optional<InteriorPartitionSurface> insideShelf() const;

    boost::optional<ShadingSurface> outsideShelf() const;

    boost::optional<double> viewFactortoOutsideShelf() const;

    //@}
    /** @name Setters */
    //@{

    bool setInsideShelf(const InteriorPartitionSurface& insideShelf);

    bool setOutsideShelf(const ShadingSurface& outsideShelf);

    bool setViewFactortoOutsideShelf(double viewFactortoOutsideShelf);

    void resetViewFactortoOutsideShelf();

    //@}
      
    /// Get the related sub surface.
    SubSurface subSurface() const;

   protected:
   private:

    REGISTER_LOGGER("openstudio.model.DaylightingDeviceShelf");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_DAYLIGHTINGDEVICESHELF_IMPL_HPP

