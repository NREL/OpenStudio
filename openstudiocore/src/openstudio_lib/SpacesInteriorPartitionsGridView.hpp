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

#ifndef OPENSTUDIO_SPACESINTERIORPARTITIONSGRIDVIEW_HPP
#define OPENSTUDIO_SPACESINTERIORPARTITIONSGRIDVIEW_HPP

#include "../shared_gui_components/OSGridController.hpp"

#include "OSItem.hpp"

#include "../model/Model.hpp"

#include <QWidget>

class QComboBox;
class QLabel;

namespace openstudio{

  class ModelSubTabView;
  class SpacesInteriorPartitionsGridController;

  class SpacesInteriorPartitionsGridView : public QWidget
  {
    Q_OBJECT

  public:

    SpacesInteriorPartitionsGridView(bool isIP, const model::Model & model, QWidget * parent = 0);

    virtual ~SpacesInteriorPartitionsGridView() {}

    std::vector<model::ModelObject> selectedObjects() const;

    void enableFilter();

    void disableFilter();

    QComboBox * m_filters = nullptr;

  private:

    REGISTER_LOGGER("openstudio.SpacesInteriorPartitionsGridView");

    bool m_isIP;

    SpacesInteriorPartitionsGridController * m_gridController = nullptr;

    QLabel * m_filterLabel = nullptr;

  signals:

    void toggleUnitsClicked(bool displayIP);

    void dropZoneItemClicked(OSItem* item);

    void itemSelected(OSItem * item);

    void selectionCleared();

    void gridRowSelected(OSItem*);

    private slots:

    void onDropZoneItemClicked(OSItem* item);

  };

  class SpacesInteriorPartitionsGridController : public OSGridController
  {

    Q_OBJECT

  public:

    SpacesInteriorPartitionsGridController(bool isIP,
      const QString & headerText,
      IddObjectType iddObjectType,
      model::Model model,
      std::vector<model::ModelObject> modelObjects);

    virtual ~SpacesInteriorPartitionsGridController() {}

    virtual void refreshModelObjects();

    virtual void categorySelected(int index);

  protected:

    virtual void setCategoriesAndFields();

    virtual void addColumns(const QString &category, std::vector<QString> & fields);

    virtual void checkSelectedFields();

    virtual QString getColor(const model::ModelObject & modelObject);

    public slots:

    virtual void onItemDropped(const OSItemId& itemId);

    virtual void onComboBoxIndexChanged(int index);

    void filterChanged(const QString & text);

  private:

    SpacesInteriorPartitionsGridView * gridView();

  };

} // openstudio

#endif // OPENSTUDIO_SPACESINTERIORPARTITIONSGRIDVIEW_HPP

