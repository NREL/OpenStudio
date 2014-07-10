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

#ifndef MODEL_SHADOWCALCULATION_IMPL_HPP
#define MODEL_SHADOWCALCULATION_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

namespace detail {

  class MODEL_API ShadowCalculation_Impl : public ModelObject_Impl {
    Q_OBJECT;
    Q_PROPERTY(int calculationFrequency READ calculationFrequency WRITE setCalculationFrequency RESET resetCalculationFrequency);
    Q_PROPERTY(bool isCalculationFrequencyDefaulted READ isCalculationFrequencyDefaulted);
    Q_PROPERTY(int maximumFiguresInShadowOverlapCalculations READ maximumFiguresInShadowOverlapCalculations WRITE setMaximumFiguresInShadowOverlapCalculations RESET resetMaximumFiguresInShadowOverlapCalculations);
    Q_PROPERTY(bool isMaximumFiguresInShadowOverlapCalculationsDefaulted READ isMaximumFiguresInShadowOverlapCalculationsDefaulted);
    Q_PROPERTY(boost::optional<std::string> polygonClippingAlgorithm READ polygonClippingAlgorithm WRITE setPolygonClippingAlgorithm RESET resetPolygonClippingAlgorithm);
    Q_PROPERTY(boost::optional<std::string> skyDiffuseModelingAlgorithm READ skyDiffuseModelingAlgorithm WRITE setSkyDiffuseModelingAlgorithm RESET resetSkyDiffuseModelingAlgorithm);
   public:

    // constructor
    ShadowCalculation_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // construct from workspace
    ShadowCalculation_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                           Model_Impl* model, 
                           bool keepHandle);

    // clone copy constructor
    ShadowCalculation_Impl(const ShadowCalculation_Impl& other, Model_Impl* model, bool keepHandle);

    // virtual destructor
    virtual ~ShadowCalculation_Impl(){}

    // return the parent object in the hierarchy
    virtual boost::optional<ParentObject> parent() const;

    // set the parent, child may have to call methods on the parent
    virtual bool setParent(ParentObject& newParent);

    // Get all output variable names that could be associated with this object.
    virtual const std::vector<std::string>& outputVariableNames() const;

    /** @name Getters */
    //@{

    int calculationFrequency() const;

    bool isCalculationFrequencyDefaulted() const;

    int maximumFiguresInShadowOverlapCalculations() const;

    bool isMaximumFiguresInShadowOverlapCalculationsDefaulted() const;

    boost::optional<std::string> polygonClippingAlgorithm() const;

    boost::optional<std::string> skyDiffuseModelingAlgorithm() const;

    //@}
    /** @name Setters */
    //@{

    bool setCalculationFrequency(int calculationFrequency);

    void resetCalculationFrequency();

    bool setMaximumFiguresInShadowOverlapCalculations(
        int maximumFiguresInShadowOverlapCalculations);

    void resetMaximumFiguresInShadowOverlapCalculations();

    bool setPolygonClippingAlgorithm(boost::optional<std::string> polygonClippingAlgorithm);

    void resetPolygonClippingAlgorithm();

    bool setSkyDiffuseModelingAlgorithm(boost::optional<std::string> skyDiffuseModelingAlgorithm);

    void resetSkyDiffuseModelingAlgorithm();

    //@}

    virtual IddObjectType iddObjectType() const;
  
   private:

    REGISTER_LOGGER("openstudio.model.ShadowCalculation");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_SHADOWCALCULATION_IMPL_HPP
