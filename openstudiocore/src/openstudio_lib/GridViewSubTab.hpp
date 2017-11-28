/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

