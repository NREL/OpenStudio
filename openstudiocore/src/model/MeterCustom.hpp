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

#ifndef MODEL_METERCUSTOM_HPP
#define MODEL_METERCUSTOM_HPP

#include <model/ModelAPI.hpp>

#include <memory>
#include <boost/optional.hpp>

#include <vector>

namespace openstudio {
namespace model {

namespace detail {

  class MeterCustom_Impl;

} // detail

/** MeterCustom is a  that wraps the OpenStudio IDD object 'OS:Meter:Custom'. */
class MODEL_API MeterCustom {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit MeterCustom(const Model& model);

  virtual ~MeterCustom() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> fuelTypeValues();

  /** @name Getters */
  //@{

  boost::optional<std::string> fuelType() const;

  // TODO: Handle this object's extensible fields.

  //@}
  /** @name Setters */
  //@{

  bool setFuelType(std::string fuelType);

  void resetFuelType();

  // TODO: Handle this object's extensible fields.

  //@}
  /** @name Other */
  //@{

  //@}
  /** @name Type Casting */
  //@{

  /** Get the impl pointer */
  template<typename T>
  std::shared_ptr<T> getImpl() const {
    return std::dynamic_pointer_cast<T>(m_impl);
  }

  /** Cast to type T. Throws std::bad_cast if object is not a T. */
  template<typename T>
  T cast() const {
    std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (!impl) {
      throw(std::bad_cast());
    }
    return T(impl);
  }

  /** Cast to boost::optional<T>. Return value is boost::none (evaluates to false) 
   *  if object is not a T. */
  template<typename T>
  boost::optional<T> optionalCast() const{
    boost::optional<T> result;
    std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (impl){
      result = T(impl);
    }
    return result;
  }

  //@}
 protected:
  /// @cond
  typedef detail::MeterCustom_Impl ImplType;

  explicit MeterCustom(std::shared_ptr<detail::MeterCustom_Impl> impl);

  friend class detail::MeterCustom_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  std::shared_ptr<detail::MeterCustom_Impl> m_impl;

  REGISTER_LOGGER("openstudio.model.MeterCustom");
};

/** \relates MeterCustom*/
typedef boost::optional<MeterCustom> OptionalMeterCustom;

/** \relates MeterCustom*/
typedef std::vector<MeterCustom> MeterCustomVector;

} // model
} // openstudio

#endif // MODEL_METERCUSTOM_HPP

