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

#ifndef OPENSTUDIO_FACILITYSHADINGGRIDVIEW_HPP
#define OPENSTUDIO_FACILITYSHADINGGRIDVIEW_HPP

#include "../shared_gui_components/OSGridController.hpp"

#include "GridViewSubTab.hpp"
#include "OSItem.hpp"

#include "../model/Model.hpp"

class QComboBox;
class QLineEdit;

namespace openstudio{

  class OSQuantityEdit2;

  class FacilityShadingGridController;

  class FacilityShadingGridView : public GridViewSubTab
  {
    Q_OBJECT

  public:

    FacilityShadingGridView(bool isIP, const model::Model & model, QWidget * parent = 0);

    virtual ~FacilityShadingGridView() {}

    QLineEdit *  m_nameFilter = nullptr;

    QLineEdit *  m_tiltGreaterThanFilter = nullptr;

    QLineEdit *  m_tiltLessThanFilter = nullptr;

    QComboBox *  m_typeFilter = nullptr;

    QLineEdit *  m_orientationGreaterThanFilter = nullptr;

    QLineEdit *  m_orientationLessThanFilter = nullptr;

  private:

    REGISTER_LOGGER("openstudio.FacilityShadingGridView");

    virtual void addObject(const openstudio::IddObjectType& iddObjectType) override;

    virtual void purgeObjects(const openstudio::IddObjectType& iddObjectType) override;

    void filterChanged();

    std::set<openstudio::model::ModelObject> m_objectsFilteredByName;

    std::set<openstudio::model::ModelObject> m_objectsFilteredByTilt;

    std::set<openstudio::model::ModelObject> m_objectsFilterdByType;

    std::set<openstudio::model::ModelObject> m_objectsFilteredByOrientation;

  protected slots :

    virtual void onSelectItem() override;

    virtual void onClearSelection() override;
    
  private slots :

    void tiltFilterChanged();

    void orientationFilterChanged();

    void nameFilterChanged();

    void typeFilterChanged(const QString & text);

  };

  class FacilityShadingGridController : public OSGridController
  {

    Q_OBJECT

  public:

    FacilityShadingGridController(bool isIP,
      const QString & headerText,
      IddObjectType iddObjectType,
      model::Model model,
      std::vector<model::ModelObject> modelObjects);

    virtual ~FacilityShadingGridController() {}

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

#endif // OPENSTUDIO_FACILITYSHADINGGRIDVIEW_HPP

