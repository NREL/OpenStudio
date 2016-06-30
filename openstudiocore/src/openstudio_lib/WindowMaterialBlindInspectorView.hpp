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

#ifndef OPENSTUDIO_WINDOWMATERIALBLINDINSPECTORVIEW_HPP
#define OPENSTUDIO_WINDOWMATERIALBLINDINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"

namespace openstudio {

namespace model {

  class Blind;

}

class OSComboBox2;

class OSLineEdit2;

class OSQuantityEdit2;

class StandardsInformationMaterialWidget;

class WindowMaterialBlindInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    WindowMaterialBlindInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent = nullptr);

    virtual ~WindowMaterialBlindInspectorView() {}

  protected:

    virtual void onClearSelection() override;

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject) override;

    virtual void onUpdate() override;

  private:

    void createLayout();

    void attach(openstudio::model::Blind & Blind);

    void detach();

    void refresh();

    bool m_isIP;

    OSLineEdit2 * m_nameEdit = nullptr;

    OSComboBox2 * m_slatOrientation = nullptr;

    OSQuantityEdit2 * m_slatWidth = nullptr;

    OSQuantityEdit2 * m_slatSeparation = nullptr;

    OSQuantityEdit2 * m_slatThickness = nullptr;

    OSQuantityEdit2 * m_slatAngle = nullptr;

    OSQuantityEdit2 * m_slatConductivity = nullptr;

    OSQuantityEdit2 * m_slatBeamSolarTransmittance = nullptr;

    OSQuantityEdit2 * m_frontSideSlatBeamSolarReflectance = nullptr;

    OSQuantityEdit2 * m_backSideSlatBeamSolarReflectance = nullptr;

    OSQuantityEdit2 * m_slatDiffuseSolarTransmittance = nullptr;

    OSQuantityEdit2 * m_frontSideSlatDiffuseSolarReflectance = nullptr;

    OSQuantityEdit2 * m_backSideSlatDiffuseSolarReflectance = nullptr;

    OSQuantityEdit2 * m_slatBeamVisibleTransmittance = nullptr;

    OSQuantityEdit2 * m_frontSideSlatBeamVisibleReflectance = nullptr;

    OSQuantityEdit2 * m_backSideSlatBeamVisibleReflectance = nullptr;

    OSQuantityEdit2 * m_slatDiffuseVisibleTransmittance = nullptr;

    OSQuantityEdit2 * m_frontSideSlatDiffuseVisibleReflectance = nullptr;

    OSQuantityEdit2 * m_backSideSlatDiffuseVisibleReflectance = nullptr;

    OSQuantityEdit2 * m_slatInfraredHemisphericalTransmittance = nullptr;

    OSQuantityEdit2 * m_frontSideSlatInfraredHemisphericalEmissivity = nullptr;

    OSQuantityEdit2 * m_backSideSlatInfraredHemisphericalEmissivity = nullptr;

    OSQuantityEdit2 * m_blindToGlassDistance = nullptr;

    OSQuantityEdit2 * m_blindTopOpeningMultiplier = nullptr;

    OSQuantityEdit2 * m_blindBottomOpeningMultiplier = nullptr;

    OSQuantityEdit2 * m_blindLeftSideOpeningMultiplier = nullptr;

    OSQuantityEdit2 * m_blindRightSideOpeningMultiplier = nullptr;

    OSQuantityEdit2 * m_minimumSlatAngle = nullptr;

    OSQuantityEdit2 * m_maximumSlatAngle = nullptr;

    StandardsInformationMaterialWidget * m_standardsInformationWidget = nullptr;

};

} // openstudio

#endif // OPENSTUDIO_WINDOWMATERIALBLINDINSPECTORVIEW_HPP

