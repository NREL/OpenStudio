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

    OSComboBox * m_slatOrientation;

    OSLineEdit * m_nameEdit;

    OSQuantityEdit * m_slatWidth;

    OSQuantityEdit * m_slatSeparation;

    OSQuantityEdit * m_slatThickness;

    OSQuantityEdit * m_slatAngle;

    OSQuantityEdit * m_slatConductivity;

    OSQuantityEdit * m_slatBeamSolarTransmittance;

    OSQuantityEdit * m_frontSideSlatBeamSolarReflectance;

    OSQuantityEdit * m_backSideSlatBeamSolarReflectance;

    OSQuantityEdit * m_slatDiffuseSolarTransmittance;

    OSQuantityEdit * m_frontSideSlatDiffuseSolarReflectance;

    OSQuantityEdit * m_backSideSlatDiffuseSolarReflectance;

    OSQuantityEdit * m_slatBeamVisibleTransmittance;

    OSQuantityEdit * m_frontSideSlatBeamVisibleReflectance;

    OSQuantityEdit * m_backSideSlatBeamVisibleReflectance;

    OSQuantityEdit * m_slatDiffuseVisibleTransmittance;

    OSQuantityEdit * m_frontSideSlatDiffuseVisibleReflectance;

    OSQuantityEdit * m_backSideSlatDiffuseVisibleReflectance;

    OSQuantityEdit * m_slatInfraredHemisphericalTransmittance;

    OSQuantityEdit * m_frontSideSlatInfraredHemisphericalEmissivity;

    OSQuantityEdit * m_backSideSlatInfraredHemisphericalEmissivity;

    OSQuantityEdit * m_blindToGlassDistance;

    OSQuantityEdit * m_blindTopOpeningMultiplier;

    OSQuantityEdit * m_blindBottomOpeningMultiplier;

    OSQuantityEdit * m_blindLeftSideOpeningMultiplier;

    OSQuantityEdit * m_blindRightSideOpeningMultiplier;

    OSQuantityEdit * m_minimumSlatAngle;

    OSQuantityEdit * m_maximumSlatAngle;

    bool m_isIP;

  public slots:

    void toggleUnits(bool displayIP);
};

} // openstudio

#endif // OPENSTUDIO_WINDOWMATERIALBLINDINSPECTORVIEW_HPP

