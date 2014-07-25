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

    OSComboBox * m_roughness;

    OSLineEdit * m_nameEdit;

    OSLineEdit * m_soilLayerName;

    OSLineEdit * m_moistureDiffusionCalculationMethod;

    OSQuantityEdit * m_heightOfPlants;

    OSQuantityEdit * m_leafAreaIndex;

    OSQuantityEdit * m_leafReflectivity;

    OSQuantityEdit * m_leafEmissivity;

    OSQuantityEdit * m_minimumStomatalResistance;

    OSQuantityEdit * m_thickness;

    OSQuantityEdit * m_conductivityOfDrySoil;

    OSQuantityEdit * m_densityOfDrySoil;

    OSQuantityEdit * m_specificHeatOfDrySoil;

    OSQuantityEdit * m_thermalAbsorptance;

    OSQuantityEdit * m_visibleAbsorptance;

    OSQuantityEdit * m_saturationVolumetricMoistureContentOfTheSoilLayer;

    OSQuantityEdit * m_residualVolumetricMoistureContentOfTheSoilLayer;

    OSQuantityEdit * m_initialVolumetricMoistureContentOfTheSoilLayer;

    OSQuantityEdit * m_solarAbsorptance;

    bool m_isIP;

  public slots:

    void toggleUnits(bool displayIP);
};

} // openstudio

#endif // OPENSTUDIO_MATERIALROOFVEGETATIONINSPECTORVIEW_HPP

