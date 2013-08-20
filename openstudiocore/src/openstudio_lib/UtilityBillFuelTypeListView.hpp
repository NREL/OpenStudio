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

#ifndef OPENSTUDIO_UTILITYBILLFUELTYPELISTVIEW_H
#define OPENSTUDIO_UTILITYBILLFUELTYPELISTVIEW_H

#include <openstudio_lib/OSItemList.hpp>
#include <openstudio_lib/OSVectorController.hpp>

#include <model/Model.hpp>
#include <model/ModelObject.hpp>

#include <utilities/data/DataEnums.hpp>

namespace openstudio {

class FuelType;

class UtilityBillFuelTypeListController : public OSVectorController
{
  Q_OBJECT

public:

  UtilityBillFuelTypeListController(const model::Model& model,
    openstudio::FuelType fuelType);

  virtual ~UtilityBillFuelTypeListController() {}

  IddObjectType iddObjectType() const;

  FuelType fuelType() const;

private slots:
  void objectAdded(boost::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, const openstudio::UUID&);
  void objectRemoved(boost::shared_ptr<openstudio::detail::WorkspaceObject_Impl>, const openstudio::IddObjectType&, const openstudio::UUID&);

protected:
  virtual std::vector<OSItemId> makeVector();

private:
  openstudio::IddObjectType m_iddObjectType;
  openstudio::FuelType m_fuelType;
  model::Model m_model;
};

class UtilityBillFuelTypeListView : public OSItemList
{
  Q_OBJECT

public:

  UtilityBillFuelTypeListView(const model::Model& model,
    openstudio::FuelType fuelType,
    bool addScrollArea,
    QWidget * parent = 0);

  virtual ~UtilityBillFuelTypeListView() {}

  boost::optional<openstudio::model::ModelObject> selectedModelObject() const;

  IddObjectType iddObjectType() const;

  FuelType fuelType() const;

private:

  openstudio::FuelType m_fuelType;

};

} // openstudio

#endif // OPENSTUDIO_UTILITYBILLFUELTYPELISTVIEW_H

