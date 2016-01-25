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

#ifndef MODEL_SOLARCOLLECTORPERFORMANCEFLATPLATE_IMPL_HPP
#define MODEL_SOLARCOLLECTORPERFORMANCEFLATPLATE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  /** SolarCollectorPerformanceFlatPlate_Impl is a ModelObject_Impl that is the implementation class for SolarCollectorPerformanceFlatPlate.*/
  class MODEL_API SolarCollectorPerformanceFlatPlate_Impl : public ModelObject_Impl {

   public:
    /** @name Constructors and Destructors */
    //@{

    SolarCollectorPerformanceFlatPlate_Impl(const IdfObject& idfObject,
                                            Model_Impl* model,
                                            bool keepHandle);

    SolarCollectorPerformanceFlatPlate_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                            Model_Impl* model,
                                            bool keepHandle);

    SolarCollectorPerformanceFlatPlate_Impl(const SolarCollectorPerformanceFlatPlate_Impl& other,
                                            Model_Impl* model,
                                            bool keepHandle);

    virtual ~SolarCollectorPerformanceFlatPlate_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    //@}
    /** @name Getters */
    //@{

    double grossArea() const;

    std::string testFluid() const;

    bool isTestFluidDefaulted() const;

    double testFlowRate() const;

    std::string testCorrelationType() const;

    double coefficient1ofEfficiencyEquation() const;

    double coefficient2ofEfficiencyEquation() const;

    boost::optional<double> coefficient3ofEfficiencyEquation() const;

    boost::optional<double> coefficient2ofIncidentAngleModifier() const;

    boost::optional<double> coefficient3ofIncidentAngleModifier() const;

    //@}
    /** @name Setters */
    //@{

    bool setGrossArea(double grossArea);

    bool setTestFluid(std::string testFluid);

    void resetTestFluid();

    bool setTestFlowRate(double testFlowRate);

    bool setTestCorrelationType(std::string testCorrelationType);

    void setCoefficient1ofEfficiencyEquation(double coefficient1ofEfficiencyEquation);

    void setCoefficient2ofEfficiencyEquation(double coefficient2ofEfficiencyEquation);

    void setCoefficient3ofEfficiencyEquation(boost::optional<double> coefficient3ofEfficiencyEquation);

    void resetCoefficient3ofEfficiencyEquation();

    void setCoefficient2ofIncidentAngleModifier(boost::optional<double> coefficient2ofIncidentAngleModifier);

    void resetCoefficient2ofIncidentAngleModifier();

    void setCoefficient3ofIncidentAngleModifier(boost::optional<double> coefficient3ofIncidentAngleModifier);

    void resetCoefficient3ofIncidentAngleModifier();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.SolarCollectorPerformanceFlatPlate");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SOLARCOLLECTORPERFORMANCEFLATPLATE_IMPL_HPP

