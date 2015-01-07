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

#ifndef OPENSTUDIO_WINDOWMATERIALGLAZINGINSPECTORVIEW_HPP
#define OPENSTUDIO_WINDOWMATERIALGLAZINGINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"

namespace openstudio {

namespace model {

  class Glazing;

}

class OSComboBox;

class OSLineEdit;

class OSQuantityEdit;

class OSSwitch;

class StandardsInformationMaterialWidget;

class WindowMaterialGlazingInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    WindowMaterialGlazingInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent = 0);

    virtual ~WindowMaterialGlazingInspectorView() {}

  protected:

    virtual void onClearSelection();

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);

    virtual void onUpdate();

  private:

    void createLayout();

    void attach(openstudio::model::Glazing & Glazing);

    void detach();

    void refresh();

    bool m_isIP;

    OSLineEdit * m_nameEdit = nullptr;

    OSComboBox * m_opticalDataType = nullptr;

    OSSwitch * m_solarDiffusing = nullptr;

    OSLineEdit * m_windowGlassSpectralDataSetName = nullptr;

    OSQuantityEdit * m_thickness = nullptr;

    OSQuantityEdit * m_solarTransmittanceAtNormalIncidence = nullptr;

    OSQuantityEdit * m_frontSideSolarReflectanceAtNormalIncidence = nullptr;

    OSQuantityEdit * m_backSideSolarReflectanceAtNormalIncidence = nullptr;

    OSQuantityEdit * m_visibleTransmittanceAtNormalIncidence = nullptr;

    OSQuantityEdit * m_frontSideVisibleReflectanceAtNormalIncidence = nullptr;

    OSQuantityEdit * m_backSideVisibleReflectanceAtNormalIncidence = nullptr;

    OSQuantityEdit * m_infraredTransmittanceAtNormalIncidence = nullptr;

    OSQuantityEdit * m_frontSideInfraredHemisphericalEmissivity = nullptr;

    OSQuantityEdit * m_backSideInfraredHemisphericalEmissivity = nullptr;

    OSQuantityEdit * m_conductivity = nullptr;

    OSQuantityEdit * m_dirtCorrectionFactorForSolarAndVisibleTransmittance = nullptr;

    StandardsInformationMaterialWidget * m_standardsInformationWidget = nullptr;

};

} // openstudio

#endif // OPENSTUDIO_WINDOWMATERIALGLAZINGINSPECTORVIEW_HPP

