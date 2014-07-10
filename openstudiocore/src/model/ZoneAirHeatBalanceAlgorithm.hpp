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

#ifndef MODEL_ZONEAIRHEATBALANCEALGORITHM_HPP
#define MODEL_ZONEAIRHEATBALANCEALGORITHM_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class ZoneAirHeatBalanceAlgorithm_Impl;

} // detail

/** ZoneAirHeatBalanceAlgorithm is a ModelObject that wraps the OpenStudio IDD object 'OS:ZoneAirHeatBalanceAlgorithm'. */
class MODEL_API ZoneAirHeatBalanceAlgorithm : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~ZoneAirHeatBalanceAlgorithm() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> validAlgorithmValues();

  /** @name Getters */
  //@{

  std::string algorithm() const;

  bool isAlgorithmDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setAlgorithm(std::string algorithm);

  void resetAlgorithm();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::ZoneAirHeatBalanceAlgorithm_Impl ImplType;

  explicit ZoneAirHeatBalanceAlgorithm(std::shared_ptr<detail::ZoneAirHeatBalanceAlgorithm_Impl> impl);

  friend class detail::ZoneAirHeatBalanceAlgorithm_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  explicit ZoneAirHeatBalanceAlgorithm(Model& model);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ZoneAirHeatBalanceAlgorithm");
};

/** \relates ZoneAirHeatBalanceAlgorithm*/
typedef boost::optional<ZoneAirHeatBalanceAlgorithm> OptionalZoneAirHeatBalanceAlgorithm;

/** \relates ZoneAirHeatBalanceAlgorithm*/
typedef std::vector<ZoneAirHeatBalanceAlgorithm> ZoneAirHeatBalanceAlgorithmVector;

} // model
} // openstudio

#endif // MODEL_ZONEAIRHEATBALANCEALGORITHM_HPP

