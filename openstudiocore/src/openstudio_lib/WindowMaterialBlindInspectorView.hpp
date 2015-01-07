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

#ifndef OPENSTUDIO_WINDOWMATERIALBLINDINSPECTORVIEW_HPP
#define OPENSTUDIO_WINDOWMATERIALBLINDINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"

namespace openstudio {

namespace model {

  class Blind;

}

class OSComboBox;

class OSLineEdit;

class OSQuantityEdit;

class StandardsInformationMaterialWidget;

class WindowMaterialBlindInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    WindowMaterialBlindInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent = 0);

    virtual ~WindowMaterialBlindInspectorView() {}

  protected:

    virtual void onClearSelection();

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);

    virtual void onUpdate();

  private:

    void createLayout();

    void attach(openstudio::model::Blind & Blind);

    void detach();

    void refresh();

    bool m_isIP;

    OSLineEdit * m_nameEdit = nullptr;

    OSComboBox * m_slatOrientation = nullptr;

    OSQuantityEdit * m_slatWidth = nullptr;

    OSQuantityEdit * m_slatSeparation = nullptr;

    OSQuantityEdit * m_slatThickness = nullptr;

    OSQuantityEdit * m_slatAngle = nullptr;

    OSQuantityEdit * m_slatConductivity = nullptr;

    OSQuantityEdit * m_slatBeamSolarTransmittance = nullptr;

    OSQuantityEdit * m_frontSideSlatBeamSolarReflectance = nullptr;

    OSQuantityEdit * m_backSideSlatBeamSolarReflectance = nullptr;

    OSQuantityEdit * m_slatDiffuseSolarTransmittance = nullptr;

    OSQuantityEdit * m_frontSideSlatDiffuseSolarReflectance = nullptr;

    OSQuantityEdit * m_backSideSlatDiffuseSolarReflectance = nullptr;

    OSQuantityEdit * m_slatBeamVisibleTransmittance = nullptr;

    OSQuantityEdit * m_frontSideSlatBeamVisibleReflectance = nullptr;

    OSQuantityEdit * m_backSideSlatBeamVisibleReflectance = nullptr;

    OSQuantityEdit * m_slatDiffuseVisibleTransmittance = nullptr;

    OSQuantityEdit * m_frontSideSlatDiffuseVisibleReflectance = nullptr;

    OSQuantityEdit * m_backSideSlatDiffuseVisibleReflectance = nullptr;

    OSQuantityEdit * m_slatInfraredHemisphericalTransmittance = nullptr;

    OSQuantityEdit * m_frontSideSlatInfraredHemisphericalEmissivity = nullptr;

    OSQuantityEdit * m_backSideSlatInfraredHemisphericalEmissivity = nullptr;

    OSQuantityEdit * m_blindToGlassDistance = nullptr;

    OSQuantityEdit * m_blindTopOpeningMultiplier = nullptr;

    OSQuantityEdit * m_blindBottomOpeningMultiplier = nullptr;

    OSQuantityEdit * m_blindLeftSideOpeningMultiplier = nullptr;

    OSQuantityEdit * m_blindRightSideOpeningMultiplier = nullptr;

    OSQuantityEdit * m_minimumSlatAngle = nullptr;

    OSQuantityEdit * m_maximumSlatAngle = nullptr;

    StandardsInformationMaterialWidget * m_standardsInformationWidget = nullptr;

};

} // openstudio

#endif // OPENSTUDIO_WINDOWMATERIALBLINDINSPECTORVIEW_HPP

