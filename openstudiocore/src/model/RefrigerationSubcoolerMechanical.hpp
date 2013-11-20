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

#ifndef MODEL_REFRIGERATIONSUBCOOLERMECHANICAL_HPP
#define MODEL_REFRIGERATIONSUBCOOLERMECHANICAL_HPP

#include <model/ModelAPI.hpp>
#include <model/ModelObject.hpp>

namespace openstudio {

namespace model {

// TODO: Check the following class names against object getters and setters.
class RefrigerationSystem;

namespace detail {

  class RefrigerationSubcoolerMechanical_Impl;

} // detail

/** RefrigerationSubcoolerMechanical is a ModelObject that wraps the OpenStudio IDD object 'OS:Refrigeration:Subcooler:Mechanical'. */
class MODEL_API RefrigerationSubcoolerMechanical : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit RefrigerationSubcoolerMechanical(const Model& model);

  virtual ~RefrigerationSubcoolerMechanical() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  // TODO: Check return type. From object lists, some candidates are: RefrigerationSystem.
  boost::optional<RefrigerationSystem> capacityProvidingSystem() const;

  boost::optional<double> outletControlTemperature() const;

  //@}
  /** @name Setters */
  //@{

  // TODO: Check argument type. From object lists, some candidates are: RefrigerationSystem.
  bool setCapacityProvidingSystem(const RefrigerationSystem& refrigerationSystem);

  void resetCapacityProvidingSystem();

  void setOutletControlTemperature(double outletControlTemperature);

  void resetOutletControlTemperature();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::RefrigerationSubcoolerMechanical_Impl ImplType;

  explicit RefrigerationSubcoolerMechanical(boost::shared_ptr<detail::RefrigerationSubcoolerMechanical_Impl> impl);

  friend class detail::RefrigerationSubcoolerMechanical_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.RefrigerationSubcoolerMechanical");
};

/** \relates RefrigerationSubcoolerMechanical*/
typedef boost::optional<RefrigerationSubcoolerMechanical> OptionalRefrigerationSubcoolerMechanical;

/** \relates RefrigerationSubcoolerMechanical*/
typedef std::vector<RefrigerationSubcoolerMechanical> RefrigerationSubcoolerMechanicalVector;

} // model
} // openstudio

#endif // MODEL_REFRIGERATIONSUBCOOLERMECHANICAL_HPP

