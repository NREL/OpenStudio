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

#ifndef OPENSTUDIO_CONSTRUCTIONSVIEW_HPP
#define OPENSTUDIO_CONSTRUCTIONSVIEW_HPP

#include "ModelObjectInspectorView.hpp"
#include "ModelSubTabView.hpp"

#include "../model/Model.hpp"

namespace openstudio {

class ConstructionsView : public ModelSubTabView
{
  Q_OBJECT

  public:

    ConstructionsView(bool isIP,
                      const openstudio::model::Model& model,
                      QWidget * parent = 0);

    virtual ~ConstructionsView() {}

  private:

    static std::vector<std::pair<IddObjectType, std::string> > modelObjectTypesAndNames();

};

class ConstructionsInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    ConstructionsInspectorView(bool isIP,
                               const model::Model& model,
                               QWidget * parent = 0);

    virtual ~ConstructionsInspectorView() {}

  protected:

    virtual void onClearSelection();

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);

    virtual void onUpdate();

  private:

    void showConstructionInspector(const openstudio::model::ModelObject & modelObject);

    void showCfactorUndergroundWallInspector(const openstudio::model::ModelObject & modelObject);

    void showFfactorGroundFloorInspector(const openstudio::model::ModelObject & modelObject);

    void showInternalSourceInspector(const openstudio::model::ModelObject & modelObject);

    void showWindowDataFileInspector(const openstudio::model::ModelObject & modelObject);

    void showDefaultView();

    void showInspector(QWidget * widget);
    
    std::map<openstudio::IddObjectType, int> m_inspectorIndexMap;

    bool m_isIP;

  public slots:

    void toggleUnits(bool displayIP);

};

} // openstudio

#endif // OPENSTUDIO_CONSTRUCTIONSVIEW_HPP
