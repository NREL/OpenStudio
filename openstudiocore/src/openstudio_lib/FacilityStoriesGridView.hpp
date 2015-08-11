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

#ifndef OPENSTUDIO_FACILITYSTORIESGRIDVIEW_HPP
#define OPENSTUDIO_FACILITYSTORIESGRIDVIEW_HPP

#include "../shared_gui_components/OSGridController.hpp"

#include "GridViewSubTab.hpp"
#include "OSItem.hpp"

#include "../model/Model.hpp"

class QLineEdit;

namespace openstudio{

  class FacilityStoriesGridController;

  class FacilityStoriesGridView : public GridViewSubTab
  {
    Q_OBJECT

  public:

    FacilityStoriesGridView(bool isIP, const model::Model & model, QWidget * parent = 0);

    virtual ~FacilityStoriesGridView() {}

  private:

    REGISTER_LOGGER("openstudio.FacilityStoriesGridView");

    virtual void addObject(const IddObjectType& iddObjectType) override;

    virtual void purgeObjects(const IddObjectType& iddObjectType) override;

    void filterChanged();

    QLineEdit *  m_lessThanFilter = nullptr;

    QLineEdit *  m_greaterThanFilter = nullptr; 

  protected slots :

    virtual void onSelectItem() override;

    virtual void onClearSelection() override;

  private slots :

    void greaterThanFilterChanged();

    void lessThanFilterChanged();

  };

  class FacilityStoriesGridController : public OSGridController
  {

    Q_OBJECT

  public:

    FacilityStoriesGridController(bool isIP,
      const QString & headerText,
      IddObjectType iddObjectType,
      model::Model model,
      std::vector<model::ModelObject> modelObjects);

    virtual ~FacilityStoriesGridController() {}

    virtual void refreshModelObjects();

    virtual void categorySelected(int index);

  protected:

    virtual void setCategoriesAndFields();

    virtual void addColumns(const QString &category, std::vector<QString> & fields);

    virtual void checkSelectedFields();

    virtual QString getColor(const model::ModelObject & modelObject);

  public slots:

    virtual void onItemDropped(const OSItemId& itemId);

  };

} // openstudio

#endif // OPENSTUDIO_FACILITYSTORIESGRIDVIEW_HPP

