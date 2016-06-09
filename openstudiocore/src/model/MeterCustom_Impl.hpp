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

#ifndef MODEL_METERCUSTOM_IMPL_HPP
#define MODEL_METERCUSTOM_IMPL_HPP

#include <model/ModelAPI.hpp>

#include <QObject>

#include <memory>
namespace openstudio {
namespace model {

namespace detail {

  /** MeterCustom_Impl is the implementation class for MeterCustom.*/
  class MODEL_API MeterCustom_Impl : public QObject, public std::enable_shared_from_this<MeterCustom_Impl> {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    MeterCustom_Impl(const IdfObject& idfObject,
                     Model_Impl* model,
                     bool keepHandle);

    MeterCustom_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                     Model_Impl* model,
                     bool keepHandle);

    MeterCustom_Impl(const MeterCustom_Impl& other,
                     Model_Impl* model,
                     bool keepHandle);

    virtual ~MeterCustom_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
    /** @name Getters */
    //@{

    boost::optional<std::string> fuelType() const;

    // TODO: Handle this object's extensible fields.

    //@}
    /** @name Setters */
    //@{

    bool setFuelType(boost::optional<std::string> fuelType);

    void resetFuelType();

    // TODO: Handle this object's extensible fields.

    //@}
    /** @name Other */
    //@{

    //@}
    /** @name Type Casting */
    //@{

    /** Get a public object that wraps this impl.*/
    template<typename T>
    T getPublicObject() const {
      T result(std::dynamic_pointer_cast<typename T::ImplType>(
                 std::const_pointer_cast<MeterCustom_Impl>(shared_from_this())));
      return result;
    }

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.MeterCustom");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_METERCUSTOM_IMPL_HPP

