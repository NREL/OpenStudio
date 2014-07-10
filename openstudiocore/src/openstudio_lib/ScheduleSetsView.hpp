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

#ifndef OPENSTUDIO_SCHEDULESETSVIEW_HPP
#define OPENSTUDIO_SCHEDULESETSVIEW_HPP

#include "ModelSubTabView.hpp"
#include "ModelObjectInspectorView.hpp"

#include "../model/Model.hpp"

class QStackedWidget;

namespace openstudio {

class ScheduleSetsView : public ModelSubTabView
{
  Q_OBJECT

  public:

    ScheduleSetsView(const openstudio::model::Model& model,
                      QWidget * parent = 0);

    virtual ~ScheduleSetsView() {}

  private:

    static std::vector<std::pair<IddObjectType, std::string> > modelObjectTypesAndNames();

};

class ScheduleSetsInspectorView : public ModelObjectInspectorView
{
  Q_OBJECT

  public:

    ScheduleSetsInspectorView(const model::Model& model,
                               QWidget * parent = 0);

    virtual ~ScheduleSetsInspectorView() {}

  protected:

    virtual void onClearSelection();

    virtual void onSelectModelObject(const openstudio::model::ModelObject& modelObject);

    virtual void onUpdate();

  private:
    
    std::map<openstudio::IddObjectType, int> m_inspectorIndexMap;
};

} // openstudio

#endif // OPENSTUDIO_SCHEDULESETSVIEW_HPP
