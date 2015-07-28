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

#ifndef MODEL_SOLARCOLLECTORINTEGRALCOLLECTORSTORAGE_IMPL_HPP
#define MODEL_SOLARCOLLECTORINTEGRALCOLLECTORSTORAGE_IMPL_HPP

#include <model/ModelAPI.hpp>
#include <model/StraightComponent_Impl.hpp>

namespace openstudio {
namespace model {

// TODO: Check the following class names against object getters and setters.
class CollectorStoragePerformance;
class AllShadingAndHTSurf;
class Connection;
class Connection;

namespace detail {

  /** SolarCollectorIntegralCollectorStorage_Impl is a StraightComponent_Impl that is the implementation class for SolarCollectorIntegralCollectorStorage.*/
  class MODEL_API SolarCollectorIntegralCollectorStorage_Impl : public StraightComponent_Impl {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    SolarCollectorIntegralCollectorStorage_Impl(const IdfObject& idfObject,
                                                Model_Impl* model,
                                                bool keepHandle);

    SolarCollectorIntegralCollectorStorage_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                Model_Impl* model,
                                                bool keepHandle);

    SolarCollectorIntegralCollectorStorage_Impl(const SolarCollectorIntegralCollectorStorage_Impl& other,
                                                Model_Impl* model,
                                                bool keepHandle);

    virtual ~SolarCollectorIntegralCollectorStorage_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    //@}
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

    void setBoundaryConditionModelName(boost::optional<std::string> boundaryConditionModelName);

    void resetBoundaryConditionModelName();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setInletNode(const boost::optional<Connection>& connection);

    void resetInletNode();

    // TODO: Check argument type. From object lists, some candidates are: Connection.
    bool setOutletNode(const boost::optional<Connection>& connection);

    void resetOutletNode();

    bool setMaximumFlowRate(boost::optional<double> maximumFlowRate);

    void resetMaximumFlowRate();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.SolarCollectorIntegralCollectorStorage");

    // TODO: Check the return types of these methods.
    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<CollectorStoragePerformance> optionalIntegralCollectorStorageParameters() const;
    boost::optional<AllShadingAndHTSurf> optionalSurface() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SOLARCOLLECTORINTEGRALCOLLECTORSTORAGE_IMPL_HPP

