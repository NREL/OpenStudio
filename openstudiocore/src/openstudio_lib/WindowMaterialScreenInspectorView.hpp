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

#ifndef OPENSTUDIO_WINDOWMATERIALSCREENINSPECTORVIEW_HPP
#define OPENSTUDIO_WINDOWMATERIALSCREENINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"
#include "../model/Screen.hpp"

namespace openstudio {

class OSComboBox2;

class OSLineEdit2;

class OSQuantityEdit2;

class StandardsInformationMaterialWidget;

class WindowMaterialScreenInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    WindowMaterialScreenInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent = nullptr);

    virtual ~WindowMaterialScreenInspectorView() {}

  protected:

    virtual void onClearSelection() override;

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject) override;

    virtual void onUpdate() override;

  private:

    void createLayout();

    void attach(openstudio::model::Screen & Screen);

    void detach();

    void refresh();

    bool m_isIP;

    boost::optional<model::Screen> m_screen;

    OSLineEdit2 * m_nameEdit = nullptr;

    OSComboBox2 * m_reflectedBeamTransmittanceAccountingMethod = nullptr;

    OSComboBox2 * m_angleOfResolutionForScreenTransmittanceOutputMap = nullptr;

    OSQuantityEdit2 * m_diffuseSolarReflectance = nullptr;

    OSQuantityEdit2 * m_diffuseVisibleReflectance = nullptr;

    OSQuantityEdit2 * m_thermalHemisphericalEmissivity = nullptr;

    OSQuantityEdit2 * m_conductivity = nullptr;

    OSQuantityEdit2 * m_screenMaterialSpacing = nullptr;

    OSQuantityEdit2 * m_screenMaterialDiameter = nullptr;

    OSQuantityEdit2 * m_screenToGlassDistance = nullptr;

    OSQuantityEdit2 * m_topOpeningMultiplier = nullptr;

    OSQuantityEdit2 * m_bottomOpeningMultiplier = nullptr;

    OSQuantityEdit2 * m_leftSideOpeningMultiplier = nullptr;

    OSQuantityEdit2 * m_rightSideOpeningMultiplier = nullptr;

    StandardsInformationMaterialWidget * m_standardsInformationWidget = nullptr;

};

} // openstudio

#endif // OPENSTUDIO_WINDOWMATERIALSCREENINSPECTORVIEW_HPP

