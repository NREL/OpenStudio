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

#ifndef MODEL_CONSTRUCTIONWITHINTERNALSOURCE_IMPL_HPP
#define MODEL_CONSTRUCTIONWITHINTERNALSOURCE_IMPL_HPP 

#include "LayeredConstruction_Impl.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {
namespace model {
namespace detail {

  class MODEL_API ConstructionWithInternalSource_Impl : public LayeredConstruction_Impl 
  {
    Q_OBJECT;

    Q_PROPERTY(int sourcePresentAfterLayerNumber READ sourcePresentAfterLayerNumber WRITE setSourcePresentAfterLayerNumber);

    Q_PROPERTY(int dimensionsForTheCTFCalculation READ dimensionsForTheCTFCalculation WRITE setDimensionsForTheCTFCalculation);
    
    Q_PROPERTY(int temperatureCalculationRequestedAfterLayerNumber READ temperatureCalculationRequestedAfterLayerNumber WRITE setTemperatureCalculationRequestedAfterLayerNumber);

    Q_PROPERTY(double tubeSpacing READ tubeSpacing WRITE setTubeSpacing);
    Q_PROPERTY(openstudio::Quantity tubeSpacing_SI READ tubeSpacing_SI WRITE setTubeSpacing);
    Q_PROPERTY(openstudio::Quantity tubeSpacing_IP READ tubeSpacing_IP WRITE setTubeSpacing);    

  public:

    // constructor
    ConstructionWithInternalSource_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    // construct from workspace
    ConstructionWithInternalSource_Impl(const openstudio::detail::WorkspaceObject_Impl& other, 
                                        Model_Impl* model, 
                                        bool keepHandle);

    ConstructionWithInternalSource_Impl(const ConstructionWithInternalSource_Impl& other,
                                        Model_Impl* model,
                                        bool keepHandle);

    // virtual destructor
    virtual ~ConstructionWithInternalSource_Impl(){}

    /** Get all output variables names that could be associated with this object. These variables
     *  may or may not be available for each simulation, need to check report variable dictionary
     *  to see if the variable is available. Each concrete class should override this method.*/
    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual int renderingColorIndex() const;

    virtual bool eraseLayer(unsigned layerIndex);

    virtual bool setLayers(const std::vector<Material>& materials);

    virtual bool setLayer(const ModelPartitionMaterial& modelPartitionMaterial);

    /// Returns sourcePresentAfterLayerNumber.
    int sourcePresentAfterLayerNumber() const;
    bool setSourcePresentAfterLayerNumber(int sourcePresentAfterLayerNumber);

    /// Returns temperatureCalculationRequestedAfterLayerNumber.
    int temperatureCalculationRequestedAfterLayerNumber() const;
    bool setTemperatureCalculationRequestedAfterLayerNumber(int temperatureCalculationRequestedAfterLayerNumber);

    /// Returns dimensionsForTheCTFCalculation.
    int dimensionsForTheCTFCalculation() const;
    bool setDimensionsForTheCTFCalculation(int dimensionsForTheCTFCalculation);

    /// Returns tubeSpacing.
    double tubeSpacing() const;
    Quantity getTubeSpacing(bool returnIP = false) const;
    bool setTubeSpacing(double tubeSpacing);
    bool setTubeSpacing(const Quantity& tubeSpacing);

    ConstructionWithInternalSource reverseConstructionWithInternalSource() const;

  private:

    REGISTER_LOGGER("openstudio.model.ConstructionWithInternalSource");
    
    openstudio::Quantity tubeSpacing_SI() const;
    openstudio::Quantity tubeSpacing_IP() const;

    void onNumLayersChanged();
  };

} // detail
} // model
} // openstudio

#endif // MODEL_CONSTRUCTIONWITHINTERNALSOURCE_IMPL_HPP
