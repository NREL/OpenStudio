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

#ifndef OPENSTUDIO_OSITEMLIST_HPP
#define OPENSTUDIO_OSITEMLIST_HPP

#include "OSItemSelector.hpp"
#include "OSItem.hpp"

class QVBoxLayout;

namespace openstudio {

class OSItem;
class OSVectorController;

class OSItemList : public OSItemSelector {
  Q_OBJECT
 public:

  OSItemList(OSVectorController* vectorController,
             bool addScrollArea,
             QWidget* parent = nullptr);

  virtual ~OSItemList() {}

  // return the selected item in the list, NULL if list is empty
  OSItem* selectedItem() const override;

  // return the first item in the list, NULL if list is empty
  OSItem* firstItem();

  // return the last item in the list, NULL if list is empty
  OSItem* lastItem();

  // return all items in the list
  std::vector<OSItem *> items();

  OSVectorController* vectorController() const;

  bool itemsDraggable() const;
  void setItemsDraggable(bool itemsDraggable);

  bool itemsRemoveable() const;
  void setItemsRemoveable(bool itemsRemoveable);

  OSItemType itemsType() const;
  void setItemsType(OSItemType type);

public slots:

  // adds an item, optionally selects it
  void addItem(OSItem* item, bool selectItem = true);

  // selects an item, emits objectSelected if object is selected, emits selectionCleared if item is null or not found
  void selectItem(OSItem* item);

  // selects an item by id
  void selectItemId(const OSItemId& itemId);

  // clears the selection, does not emit selectionCleared
  void clearSelection();

private slots:

  void setItemIds(const std::vector<OSItemId>& itemIds);

  void refresh();

signals:

  void itemsRequested();

protected:

  void paintEvent( QPaintEvent * event ) override;

private:

  OSVectorController* m_vectorController;
  QVBoxLayout * m_vLayout;
  OSItem * m_selectedItem;
  bool m_itemsDraggable;
  bool m_itemsRemoveable;
  OSItemType m_type;
  bool m_dirty;

};

} // openstudio

#endif // OPENSTUDIO_OSITEMLIST_HPP

