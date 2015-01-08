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

#ifndef OPENSTUDIO_MATERIALROOFVEGETATIONINSPECTORVIEW_HPP
#define OPENSTUDIO_MATERIALROOFVEGETATIONINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"

namespace openstudio {

namespace model {

  class RoofVegetation;

}

class OSComboBox;

class OSLineEdit;

class OSQuantityEdit;

class StandardsInformationMaterialWidget;

class MaterialRoofVegetationInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    MaterialRoofVegetationInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent = 0);

    virtual ~MaterialRoofVegetationInspectorView() {}

  protected:

    virtual void onClearSelection();

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);

    virtual void onUpdate();

  private:

    void createLayout();

    void attach(openstudio::model::RoofVegetation & RoofVegetation);

    void detach();

    void refresh();

    bool m_isIP;

    OSLineEdit * m_nameEdit = nullptr;

    OSComboBox * m_roughness = nullptr;

    OSLineEdit * m_soilLayerName = nullptr;

    OSLineEdit * m_moistureDiffusionCalculationMethod = nullptr;

    OSQuantityEdit * m_heightOfPlants = nullptr;

    OSQuantityEdit * m_leafAreaIndex = nullptr;

    OSQuantityEdit * m_leafReflectivity = nullptr;

    OSQuantityEdit * m_leafEmissivity = nullptr;

    OSQuantityEdit * m_minimumStomatalResistance = nullptr;

    OSQuantityEdit * m_thickness = nullptr;

    OSQuantityEdit * m_conductivityOfDrySoil = nullptr;

    OSQuantityEdit * m_densityOfDrySoil = nullptr;

    OSQuantityEdit * m_specificHeatOfDrySoil = nullptr;

    OSQuantityEdit * m_thermalAbsorptance = nullptr;

    OSQuantityEdit * m_visibleAbsorptance = nullptr;

    OSQuantityEdit * m_saturationVolumetricMoistureContentOfTheSoilLayer = nullptr;

    OSQuantityEdit * m_residualVolumetricMoistureContentOfTheSoilLayer = nullptr;

    OSQuantityEdit * m_initialVolumetricMoistureContentOfTheSoilLayer = nullptr;

    OSQuantityEdit * m_solarAbsorptance = nullptr;

    StandardsInformationMaterialWidget * m_standardsInformationWidget = nullptr;

};

} // openstudio

#endif // OPENSTUDIO_MATERIALROOFVEGETATIONINSPECTORVIEW_HPP

