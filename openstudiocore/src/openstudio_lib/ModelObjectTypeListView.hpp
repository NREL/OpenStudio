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

#ifndef OPENSTUDIO_MODELOBJECTTYPELISTVIEW_HPP
#define OPENSTUDIO_MODELOBJECTTYPELISTVIEW_HPP

#include "OSCollapsibleItemList.hpp"

#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"

#include <boost/optional.hpp>

class QVBoxLayout;

class QHBoxLayout;

namespace openstudio {

class ModelObjectTypeListView : public OSCollapsibleItemList
{
  Q_OBJECT

  public:
    ModelObjectTypeListView(const model::Model& model, 
                            bool addScrollArea, 
                            OSItemType headerType,
                            bool showLocalBCL = false,
                            QWidget * parent = 0);

    ModelObjectTypeListView(const std::vector<std::pair<IddObjectType, std::string> >& modelObjectTypesAndNames,
                            const model::Model& model, bool addScrollArea, OSItemType headerType,
                            bool showLocalBCL = false,
                            QWidget * parent = 0);

    virtual ~ModelObjectTypeListView() {}

    void addModelObjectType(const IddObjectType& iddObjectType, const std::string& name);

    virtual IddObjectType currentIddObjectType() const;

    virtual boost::optional<openstudio::model::ModelObject> selectedModelObject() const;

  private:

    std::vector<std::pair<IddObjectType, std::string> > m_modelObjectTypesAndNames;

    model::Model m_model;
    OSItemType m_headerType;
    bool m_showLocalBCL;
};



} // openstudio

#endif // OPENSTUDIO_MODELOBJECTTYPELISTVIEW_HPP

