/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef OPENSTUDIO_UTILITYBILLLISTVIEW_H
#define OPENSTUDIO_UTILITYBILLLISTVIEW_H

#include <openstudio_lib/OSItemList.hpp>
#include <openstudio_lib/OSVectorController.hpp>

#include <model/Model.hpp>
#include <model/ModelObject.hpp>

namespace openstudio {

class UtilityBillListController : public OSVectorController
{
  Q_OBJECT

public:

  UtilityBillListController(const openstudio::IddObjectType& iddObjectType, 
                            const model::Model& model,
                            bool showLocalBCL = false);

  virtual ~UtilityBillListController() {}

  IddObjectType iddObjectType() const;

private slots:
  void objectAdded(boost::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, const openstudio::UUID&);
  void objectRemoved(boost::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, const openstudio::UUID&);

protected:
  virtual std::vector<OSItemId> makeVector();

private:
  openstudio::IddObjectType m_iddObjectType;
  model::Model m_model;
  bool m_showLocalBCL;
};

class UtilityBillListView : public OSItemList
{
  Q_OBJECT

public:

  UtilityBillListView(const openstudio::IddObjectType& iddObjectType, 
                      const model::Model& model,
                      bool addScrollArea,
                      bool showLocalBCL = false,
                      QWidget * parent = 0);

  virtual ~UtilityBillListView() {}

  boost::optional<openstudio::model::ModelObject> selectedModelObject() const;

  IddObjectType iddObjectType() const;
};

  

} // openstudio

#endif // OPENSTUDIO_UTILITYBILLLISTVIEW_H

