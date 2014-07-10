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

#ifndef OPENSTUDIO_WINDOWMATERIALGASMIXTUREINSPECTORVIEW_HPP
#define OPENSTUDIO_WINDOWMATERIALGASMIXTUREINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"

namespace openstudio {

namespace model {

  class GasMixture;

}

class OSComboBox;

class OSIntegerEdit;

class OSLineEdit;

class OSQuantityEdit;

class WindowMaterialGasMixtureInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    WindowMaterialGasMixtureInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent = 0);

    virtual ~WindowMaterialGasMixtureInspectorView() {}

  protected:

    virtual void onClearSelection();

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);

    virtual void onUpdate();

  private:

    void createLayout();

    void attach(openstudio::model::GasMixture & GasMixture);

    void detach();

    void refresh();

    OSComboBox * m_gas1Type;
    OSComboBox * m_gas2Type;
    OSComboBox * m_gas3Type;
    OSComboBox * m_gas4Type;

    OSLineEdit * m_nameEdit;

    OSLineEdit * m_2AEdit;

    OSLineEdit * m_3AEdit;

    OSLineEdit * m_4AEdit;

    OSQuantityEdit * m_thickness;

    OSIntegerEdit * m_numberOfGasesInMixture;

    OSQuantityEdit * m_gas1Fraction;

    OSQuantityEdit * m_gas2Fraction;

    OSQuantityEdit * m_gas3Fraction;

    OSQuantityEdit * m_gas4Fraction;

    bool m_isIP;

  public slots:

    void toggleUnits(bool displayIP);
};

} // openstudio

#endif // OPENSTUDIO_WINDOWMATERIALGASMIXTUREINSPECTORVIEW_HPP

