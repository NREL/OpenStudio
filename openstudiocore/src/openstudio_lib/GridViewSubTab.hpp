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

#ifndef OPENSTUDIO_GRIDVIEWSUBTAB_HPP
#define OPENSTUDIO_GRIDVIEWSUBTAB_HPP

#include "../shared_gui_components/OSGridController.hpp"
#include "../shared_gui_components/OSGridView.hpp"

#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"

#include <QWidget>

class QScrollArea;
class QVBoxLayout;

namespace openstudio {

  class OSGridController;

  class OSGridView;

  class OSItemSelectorButtons;
  
class GridViewSubTab : public QWidget
{
  Q_OBJECT

  public:

    GridViewSubTab(bool isIP, const model::Model & model, QWidget * parent = nullptr);

    virtual ~GridViewSubTab() {}

    openstudio::IddObjectType m_iddObjectType;

  protected:
    
    void setGridView(OSGridView * gridView);

    void setGridController(OSGridController * gridController);

    virtual void addObject(const openstudio::IddObjectType& iddObjectType) = 0;

    virtual void addObject(const openstudio::model::ModelObject& modelObject);

    virtual void copyObject(const openstudio::model::ModelObject& modelObject);

    virtual void removeObject(openstudio::model::ModelObject modelObject);

    virtual void purgeObjects(const openstudio::IddObjectType& iddObjectType) = 0;

    virtual std::vector<model::ModelObject> selectedObjects() const;

    model::Model m_model;

    OSGridView * m_gridView = nullptr;

    OSGridController * m_gridController = nullptr;

    bool m_supportsMultipleObjectSelection = true;

    bool m_isIP;

    QVBoxLayout * m_scrollLayout = nullptr;

    OSItemSelectorButtons * m_itemSelectorButtons = nullptr;

  signals :

    void toggleUnitsClicked(bool displayIP);

    void dropZoneItemSelected(OSItem* item, bool readOnly);

    void selectionCleared();

  public slots:

    void onDropZoneItemClicked(OSItem* item);

    void gridRowSelectionChanged(int checkState);

  protected slots :

    void onAddClicked();

    void onCopyClicked();

    void onRemoveClicked();

    void onPurgeClicked();

    void toggleUnits(bool isIP);

    virtual void onSelectItem() = 0;

    virtual void onClearSelection() = 0;

};

} // openstudio

#endif // OPENSTUDIO_GRIDVIEWSUBTAB_HPP

