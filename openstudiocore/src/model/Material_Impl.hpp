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

#ifndef MODEL_MATERIAL_IMPL_HPP
#define MODEL_MATERIAL_IMPL_HPP


#include "ResourceObject_Impl.hpp"

#include <QObject>

namespace openstudio {
namespace model {

// forward declaration
class Material;

namespace detail {

  class MODEL_API Material_Impl : public ResourceObject_Impl {
    Q_OBJECT;

    Q_PROPERTY(double thickness READ thickness WRITE setThickness);

    Q_PROPERTY(boost::optional<double> visibleTransmiattance READ getVisibleTransmittance);
    Q_PROPERTY(boost::optional<double> interiorVisibleAbsorptance READ interiorVisibleAbsorptance);
    Q_PROPERTY(boost::optional<double> exteriorVisibleAbsorptance READ exteriorVisibleAbsorptance);
   public:

    /** @name Constructors and Destructors */
    //@{

    // Construct completely new object.
    Material_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // Construct from existing workspace object (typically when Model is being constructed 
    // from Workspace).
    Material_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                  Model_Impl* model, 
                  bool keepHandle);

    // Clone copy constructor.
    Material_Impl(const Material_Impl& other, Model_Impl* model, bool keepHandle);

    virtual ~Material_Impl() {}

    //@}
    /** @name Getters */
    //@{

    /** Get the thickness of the material. For some materials, 0.0 is always returned. */
    virtual double thickness() const;

    virtual boost::optional<double> getVisibleTransmittance() const = 0;

    virtual boost::optional<double> interiorVisibleAbsorptance() const;

    virtual boost::optional<double> exteriorVisibleAbsorptance() const;

    //@}
    /** @name Setters */
    //@{

    /** Set thickness to value (m). For some materials, false is always returned. */
    virtual bool setThickness(double value);

    //@}

   private:

    REGISTER_LOGGER("openstudio.model.Material");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_MATERIAL_IMPL_HPP
