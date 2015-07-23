
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

#ifndef OPENSTUDIO_SPACESSPACESGRIDVIEW_HPP
#define OPENSTUDIO_SPACESSPACESGRIDVIEW_HPP

#include "../shared_gui_components/OSGridController.hpp"

#include "SpacesSubtabGridView.hpp"
#include "OSItem.hpp"

#include "../model/Model.hpp"

namespace openstudio{

  class SpacesSpacesGridController;

  class SpacesSpacesGridView : public SpacesSubtabGridView
  {
    Q_OBJECT

  public:

    SpacesSpacesGridView(bool isIP, const model::Model & model, QWidget * parent = 0);

    virtual ~SpacesSpacesGridView() {}

  private:

    REGISTER_LOGGER("openstudio.SpacesSpacesGridView");

  protected slots:

   virtual void onSelectItem() override;

   virtual void onClearSelection() override;

  };

  class SpacesSpacesGridController : public OSGridController
  {

    Q_OBJECT

  public:

    SpacesSpacesGridController(bool isIP,
      const QString & headerText,
      IddObjectType iddObjectType,
      model::Model model,
      std::vector<model::ModelObject> modelObjects);

    virtual ~SpacesSpacesGridController() {}

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

#endif // OPENSTUDIO_SPACESSPACESGRIDVIEW_HPP
