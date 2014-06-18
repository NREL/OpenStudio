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

#ifndef MODEL_CURVE_IMPL_HPP
#define MODEL_CURVE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** Curve_Impl is a ModelObject_Impl that is the implementation class for Curve. */
  class MODEL_API Curve_Impl : public ModelObject_Impl {
    Q_OBJECT;
    Q_PROPERTY(int numVariables READ numVariables);
   public:
    /** @name Constructors and Destructors */
    //@{

    // Construct completely new object.
    Curve_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // Construct from existing workspace object (typically when Model is being constructed 
    // from Workspace).
    Curve_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
               Model_Impl* model, 
               bool keepHandle);

    // Clone copy constructor.
    Curve_Impl(const Curve_Impl& other, Model_Impl* model, bool keepHandle);

    virtual ~Curve_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual boost::optional<ParentObject> parent() const;

    //@}
    /** @name Other */
    //@{

    virtual int numVariables() const = 0;

    virtual double evaluate(const std::vector<double>& x) const = 0;

    double evaluate(double x) const;

    double evaluate(double x, double y) const;

    double evaluate(double x, double y, double z) const;

    //@}
   private:
    REGISTER_LOGGER("openstudio.model.Curve");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_CURVE_IMPL_HPP

