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

#ifndef OPENSTUDIO_MATERIALBASEINSPECTORVIEW_HPP
#define OPENSTUDIO_MATERIALBASEINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"

class QComboBox;

class QGridLayout;

namespace openstudio {

namespace model {

  class Material;

}

class OSComboBox;

class OSLineEdit;

class MaterialBaseInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    MaterialBaseInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent = 0);

    virtual ~MaterialBaseInspectorView() {}

  protected:

    virtual void onClearSelection();

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);

    virtual void onUpdate();

    void createLayout();

    void attach(openstudio::model::Material & material);

    void detach();

    void refresh();

    QGridLayout * m_mainGridLayout = nullptr;

    OSLineEdit * m_nameEdit = nullptr;
    
    QComboBox * m_standard = nullptr;

    QComboBox * m_standardSource = nullptr;

    QComboBox * m_standardsCategory = nullptr;

    QComboBox * m_standardsIdentifier = nullptr;

    bool m_isIP;

  public slots:

    void toggleUnits(bool displayIP);
};

} // openstudio

#endif // OPENSTUDIO_MATERIALBASEINSPECTORVIEW_HPP

