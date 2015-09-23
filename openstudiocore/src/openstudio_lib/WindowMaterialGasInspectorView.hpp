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

#ifndef OPENSTUDIO_WINDOWMATERIALGASINSPECTORVIEW_HPP
#define OPENSTUDIO_WINDOWMATERIALGASINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"

namespace openstudio {

namespace model {

  class Gas;

}

class OSComboBox;

class OSLineEdit;

class OSQuantityEdit;

class StandardsInformationMaterialWidget;

class WindowMaterialGasInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    WindowMaterialGasInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent = nullptr);

    virtual ~WindowMaterialGasInspectorView() {}

  protected:

    virtual void onClearSelection() override;

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject) override;

    virtual void onUpdate() override;

  private:

    void createLayout();

    void attach(openstudio::model::Gas & Gas);

    void detach();

    void refresh();

    bool m_isIP;

    OSLineEdit * m_nameEdit = nullptr;

    OSComboBox * m_gasType = nullptr;

    OSQuantityEdit * m_thickness = nullptr;

    OSQuantityEdit * m_conductivityCoefficientA = nullptr;

    OSQuantityEdit * m_conductivityCoefficientB = nullptr;

    OSQuantityEdit * m_viscosityCoefficientA = nullptr;

    OSQuantityEdit * m_viscosityCoefficientB = nullptr;

    OSQuantityEdit * m_specificHeatCoefficientA = nullptr;

    OSQuantityEdit * m_specificHeatCoefficientB = nullptr;

    OSQuantityEdit * m_molecularWeight = nullptr;

    StandardsInformationMaterialWidget * m_standardsInformationWidget = nullptr;

};

} // openstudio

#endif // OPENSTUDIO_WINDOWMATERIALGASINSPECTORVIEW_HPP

