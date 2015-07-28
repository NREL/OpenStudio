/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#ifndef MODEL_SOLARCOLLECTORINTEGRALCOLLECTORSTORAGE_HPP
#define MODEL_SOLARCOLLECTORINTEGRALCOLLECTORSTORAGE_HPP

#include <model/ModelAPI.hpp>
#include <model/StraightComponent.hpp>

namespace openstudio {

namespace model {

// TODO: Check the following class names against object getters and setters.
class CollectorStoragePerformance;
class AllShadingAndHTSurf;
class Connection;
class Connection;

namespace detail {

  class SolarCollectorIntegralCollectorStorage_Impl;

} // detail

/** SolarCollectorIntegralCollectorStorage is a StraightComponent that wraps the OpenStudio IDD object 'OS:SolarCollector:IntegralCollectorStorage'. */
class MODEL_API SolarCollectorIntegralCollectorStorage : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit SolarCollectorIntegralCollectorStorage(const Model& model);

  virtual ~SolarCollectorIntegralCollectorStorage() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> bottomSurfaceBoundaryConditionsTypeValues();

  /** @name Getters */
  //@{

  // TODO: Check return type. From object lists, some candidates are: CollectorStoragePerformance.
  CollectorStoragePerformance integralCollectorStorageParameters() const;

  // TODO: Check return type. From object lists, some candidates are: AllShadingAndHTSurf.
  AllShadingAndHTSurf surface() const;

  std::string bottomSurfaceBoundaryConditionsType() const;

  bool isBottomSurfaceBoundaryConditionsTypeDefaulted() const;

  boost::optional<std::string> boundaryConditionModelName() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  boost::optional<Connection> inletNode() const;

  // TODO: Check return type. From object lists, some candidates are: Connection.
  boost::optional<Connection> outletNode() const;

  boost::optional<double> maximumFlowRate() const;

  //@}
  /** @name Setters */
  //@{

  // TODO: Check argument type. From object lists, some candidates are: CollectorStoragePerformance.
  bool setIntegralCollectorStorageParameters(const CollectorStoragePerformance& collectorStoragePerformance);

  // TODO: Check argument type. From object lists, some candidates are: AllShadingAndHTSurf.
  bool setSurface(const AllShadingAndHTSurf& allShadingAndHTSurf);

  bool setBottomSurfaceBoundaryConditionsType(std::string bottomSurfaceBoundaryConditionsType);

  void resetBottomSurfaceBoundaryConditionsType();

  void setBoundaryConditionModelName(std::string boundaryConditionModelName);

  void resetBoundaryConditionModelName();

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setInletNode(const Connection& connection);

  void resetInletNode();

  // TODO: Check argument type. From object lists, some candidates are: Connection.
  bool setOutletNode(const Connection& connection);

  void resetOutletNode();

  bool setMaximumFlowRate(double maximumFlowRate);

  void resetMaximumFlowRate();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::SolarCollectorIntegralCollectorStorage_Impl ImplType;

  explicit SolarCollectorIntegralCollectorStorage(std::shared_ptr<detail::SolarCollectorIntegralCollectorStorage_Impl> impl);

  friend class detail::SolarCollectorIntegralCollectorStorage_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.SolarCollectorIntegralCollectorStorage");
};

/** \relates SolarCollectorIntegralCollectorStorage*/
typedef boost::optional<SolarCollectorIntegralCollectorStorage> OptionalSolarCollectorIntegralCollectorStorage;

/** \relates SolarCollectorIntegralCollectorStorage*/
typedef std::vector<SolarCollectorIntegralCollectorStorage> SolarCollectorIntegralCollectorStorageVector;

} // model
} // openstudio

#endif // MODEL_SOLARCOLLECTORINTEGRALCOLLECTORSTORAGE_HPP

