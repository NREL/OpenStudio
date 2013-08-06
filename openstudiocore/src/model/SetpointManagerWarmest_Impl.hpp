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

#ifndef MODEL_SETPOINTMANAGERWARMEST_IMPL_HPP
#define MODEL_SETPOINTMANAGERWARMEST_IMPL_HPP

#include <model/ModelAPI.hpp>
#include <model/HVACComponent_Impl.hpp>

namespace openstudio {
namespace model {

class Node;

namespace detail {

/** SetpointManagerWarmest_Impl is a HVACComponent_Impl that is the implementation class for SetpointManagerWarmest.*/
class MODEL_API SetpointManagerWarmest_Impl : public HVACComponent_Impl {
  Q_OBJECT;
 public:
  /** @name Constructors and Destructors */
  //@{

  SetpointManagerWarmest_Impl(const IdfObject& idfObject,
                              Model_Impl* model,
                              bool keepHandle);

  SetpointManagerWarmest_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                              Model_Impl* model,
                              bool keepHandle);

  SetpointManagerWarmest_Impl(const SetpointManagerWarmest_Impl& other,
                              Model_Impl* model,
                              bool keepHandle);

  virtual ~SetpointManagerWarmest_Impl() {}

  //@}
  /** @name Virtual Methods */
  //@{

  virtual const std::vector<std::string>& outputVariableNames() const;

  virtual IddObjectType iddObjectType() const;

  //@}
  /** @name Getters */
  //@{

  std::string controlVariable() const;

  double minimumSetpointTemperature() const;

  double maximumSetpointTemperature() const;

  std::string strategy() const;

  boost::optional<Node> setpointNode() const;

  //@}
  /** @name Setters */
  //@{

  bool setControlVariable(std::string controlVariable);

  bool setMinimumSetpointTemperature(double minimumSetpointTemperature);

  bool setMaximumSetpointTemperature(double maximumSetpointTemperature);

  bool setStrategy(std::string strategy);

  bool setSetpointNode(const Node& node);

  //@}
  /** @name Other */
  //@{

  bool addToNode(Node & node);

  //@}
 protected:
 private:
  REGISTER_LOGGER("openstudio.model.SetpointManagerWarmest");
};

} // detail

} // model
} // openstudio

#endif // MODEL_SETPOINTMANAGERWARMEST_IMPL_HPP

