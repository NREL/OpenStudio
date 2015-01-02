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

#ifndef OPENSTUDIO_CONSTRUCTIONCFACTORUNDERGROUNDWALLINSPECTORVIEW_HPP
#define OPENSTUDIO_CONSTRUCTIONCFACTORUNDERGROUNDWALLINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"
#include "ModelObjectVectorController.hpp"

class QComboBox;

namespace openstudio {

namespace model {

 class CFactorUndergroundWallConstruction;

}

class OSLineEdit;

class OSQuantityEdit;

class OSComboBox2;

class StandardsInformationConstructionWidget;

class ConstructionCfactorUndergroundWallInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    ConstructionCfactorUndergroundWallInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent = 0);

    virtual ~ConstructionCfactorUndergroundWallInspectorView() {}

  protected:

    virtual void onClearSelection();

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);

    virtual void onUpdate();

  private:

    void createLayout();

    void attach(openstudio::model::CFactorUndergroundWallConstruction & cFactorUndergroundWallConstruction);

    void detach();

    bool m_isIP;

    OSLineEdit * m_nameEdit; 

    StandardsInformationConstructionWidget * m_standardsInformationWidget;
      
    OSQuantityEdit * m_cfactorEdit;

    OSQuantityEdit * m_heightEdit;
};

} // openstudio

#endif // OPENSTUDIO_CONSTRUCTIONCFACTORUNDERGROUNDWALLINSPECTORVIEW_HPP

