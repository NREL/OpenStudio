/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef OPENSTUDIO_MATERIALROOFVEGETATIONINSPECTORVIEW_HPP
#define OPENSTUDIO_MATERIALROOFVEGETATIONINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"
#include "../model/RoofVegetation.hpp"

namespace openstudio {

class OSComboBox2;

class OSLineEdit2;

class OSQuantityEdit2;

class StandardsInformationMaterialWidget;

class MaterialRoofVegetationInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    MaterialRoofVegetationInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent = nullptr);

    virtual ~MaterialRoofVegetationInspectorView() {}

  protected:

    virtual void onClearSelection() override;

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject) override;

    virtual void onUpdate() override;

  private:

    void createLayout();

    void attach(openstudio::model::RoofVegetation & RoofVegetation);

    void detach();

    void refresh();

    bool m_isIP;

    boost::optional<model::RoofVegetation> m_roofVegetation;

    OSLineEdit2 * m_nameEdit = nullptr;

    OSComboBox2 * m_roughness = nullptr;

    OSLineEdit2 * m_soilLayerName = nullptr;

    OSLineEdit2 * m_moistureDiffusionCalculationMethod = nullptr;

    OSQuantityEdit2 * m_heightOfPlants = nullptr;

    OSQuantityEdit2 * m_leafAreaIndex = nullptr;

    OSQuantityEdit2 * m_leafReflectivity = nullptr;

    OSQuantityEdit2 * m_leafEmissivity = nullptr;

    OSQuantityEdit2 * m_minimumStomatalResistance = nullptr;

    OSQuantityEdit2 * m_thickness = nullptr;

    OSQuantityEdit2 * m_conductivityOfDrySoil = nullptr;

    OSQuantityEdit2 * m_densityOfDrySoil = nullptr;

    OSQuantityEdit2 * m_specificHeatOfDrySoil = nullptr;

    OSQuantityEdit2 * m_thermalAbsorptance = nullptr;

    OSQuantityEdit2 * m_visibleAbsorptance = nullptr;

    OSQuantityEdit2 * m_saturationVolumetricMoistureContentOfTheSoilLayer = nullptr;

    OSQuantityEdit2 * m_residualVolumetricMoistureContentOfTheSoilLayer = nullptr;

    OSQuantityEdit2 * m_initialVolumetricMoistureContentOfTheSoilLayer = nullptr;

    OSQuantityEdit2 * m_solarAbsorptance = nullptr;

    StandardsInformationMaterialWidget * m_standardsInformationWidget = nullptr;

};

} // openstudio

#endif // OPENSTUDIO_MATERIALROOFVEGETATIONINSPECTORVIEW_HPP

