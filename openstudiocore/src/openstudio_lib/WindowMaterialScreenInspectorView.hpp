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

#ifndef OPENSTUDIO_WINDOWMATERIALSCREENINSPECTORVIEW_HPP
#define OPENSTUDIO_WINDOWMATERIALSCREENINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"

namespace openstudio {

namespace model {

  class Screen;

}

class OSComboBox;

class OSLineEdit;

class OSQuantityEdit;

class WindowMaterialScreenInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    WindowMaterialScreenInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent = 0);

    virtual ~WindowMaterialScreenInspectorView() {}

  protected:

    virtual void onClearSelection();

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);

    virtual void onUpdate();

  private:

    void createLayout();

    void attach(openstudio::model::Screen & Screen);

    void detach();

    void refresh();

    OSComboBox * m_reflectedBeamTransmittanceAccountingMethod;

    OSComboBox * m_angleOfResolutionForScreenTransmittanceOutputMap;

    OSLineEdit * m_nameEdit;

    OSQuantityEdit * m_diffuseSolarReflectance;

    OSQuantityEdit * m_diffuseVisibleReflectance;

    OSQuantityEdit * m_thermalHemisphericalEmissivity;

    OSQuantityEdit * m_conductivity;

    OSQuantityEdit * m_screenMaterialSpacing;

    OSQuantityEdit * m_screenMaterialDiameter;

    OSQuantityEdit * m_screenToGlassDistance;

    OSQuantityEdit * m_topOpeningMultiplier;

    OSQuantityEdit * m_bottomOpeningMultiplier;

    OSQuantityEdit * m_leftSideOpeningMultiplier;

    OSQuantityEdit * m_rightSideOpeningMultiplier;

    bool m_isIP;

  public slots:

    void toggleUnits(bool displayIP);
};

} // openstudio

#endif // OPENSTUDIO_WINDOWMATERIALSCREENINSPECTORVIEW_HPP

