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

#ifndef OPENSTUDIO_CONSTRUCTIONFFACTORGROUNDFLOORINSPECTORVIEW_HPP
#define OPENSTUDIO_CONSTRUCTIONFFACTORGROUNDFLOORINSPECTORVIEW_HPP

#include "ModelObjectInspectorView.hpp"
#include "ModelObjectVectorController.hpp"
#include "../model/FFactorGroundFloorConstruction.hpp"

class QComboBox;

namespace openstudio {

class OSLineEdit2;

class OSQuantityEdit2;

class OSComboBox2;

class StandardsInformationConstructionWidget;

class ConstructionFfactorGroundFloorInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    ConstructionFfactorGroundFloorInspectorView(bool isIP, const openstudio::model::Model& model, QWidget * parent = nullptr);

    virtual ~ConstructionFfactorGroundFloorInspectorView() {}

  protected:

    virtual void onClearSelection() override;

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject) override;

    virtual void onUpdate() override;

  private:

    void createLayout();

    void attach(openstudio::model::FFactorGroundFloorConstruction & fFactorGroundFloorConstruction);

    void detach();

    bool m_isIP;

    OSLineEdit2 * m_nameEdit;

    StandardsInformationConstructionWidget * m_standardsInformationWidget;

    OSQuantityEdit2 * m_ffactorEdit;

    OSQuantityEdit2 * m_areaEdit;

    OSQuantityEdit2 * m_perimeterExposedEdit;

    boost::optional<model::FFactorGroundFloorConstruction> m_fFactorGroundFloorConstruction;
};

} // openstudio

#endif // OPENSTUDIO_CONSTRUCTIONFFACTORGROUNDFLOORINSPECTORVIEW_HPP

