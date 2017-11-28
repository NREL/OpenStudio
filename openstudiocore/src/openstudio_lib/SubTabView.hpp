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

#ifndef OPENSTUDIO_SUBTABVIEW_HPP
#define OPENSTUDIO_SUBTABVIEW_HPP

#include "MainTabView.hpp"
#include "OSInspectorView.hpp"

#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"

#include <QSplitter>

class QPushButton;
class QScrollArea;

namespace openstudio {

class OSItemSelector;
class OSItemSelectorButtons;
class OSItem;
class OSItemId;

class SubTabView : public QSplitter
{
  Q_OBJECT

  public:

    SubTabView(OSItemSelector* itemSelector,
      OSInspectorView* inspectorView,
      bool showGridViewLayout = false,
      QWidget * parent = nullptr);

    virtual ~SubTabView() {}

    OSItemSelector* itemSelector();

    OSItemSelectorButtons* itemSelectorButtons();

    OSInspectorView* inspectorView();

    const OSItemSelector* itemSelector() const;

    const OSItemSelectorButtons* itemSelectorButtons() const;

    const OSInspectorView* inspectorView() const;

  signals:

    void itemSelected(OSItem* item);

    void itemRemoveClicked(OSItem* item);

    void itemReplacementDropped(OSItem * currentItem, const OSItemId& replacementItemId);

    void selectionCleared();

    void itemDropped(const OSItemId& itemId);

    void addClicked();

    void copyClicked();

    void removeClicked();

    void purgeClicked();

    void dropZoneItemClicked(OSItem* item);

    void downloadComponentsClicked();

    void openLibDlgClicked();

  protected slots:

    virtual void onDropZoneItemClicked(OSItem* item);

  protected:

    void paintEvent ( QPaintEvent * event ) override;

  private:

    void connectItemSelector();

    void connectInspectorView();

    void connectItemSelectorButtons();

    void createLayout();

    void createGridViewLayout();

    QScrollArea* m_selectorScrollArea;

    OSItemSelector* m_itemSelector;

    OSItemSelectorButtons* m_itemSelectorButtons;

    OSInspectorView* m_inspectorView;

};

} // openstudio

#endif // OPENSTUDIO_SUBTABVIEW_HPP
