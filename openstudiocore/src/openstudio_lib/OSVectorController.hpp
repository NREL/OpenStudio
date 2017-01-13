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

#ifndef OPENSTUDIO_OSVECTORCONTROLLER_HPP
#define OPENSTUDIO_OSVECTORCONTROLLER_HPP

#include <QWidget>
#include "../model/ComponentData.hpp"
#include "../model/Model.hpp"
#include "../model/ModelObject.hpp"
#include <vector>
#include <nano/nano_signal_slot.hpp> // Signal-Slot replacement

namespace openstudio {

class OSItem;
class OSItemId;

class OSVectorController : public QObject, public Nano::Observer
{
  Q_OBJECT

public:

  // in the future, OSVectorController's will be managed by shared_ptr's on parent controller
  // for now, each controller must be parented by a widget
  OSVectorController();

  virtual ~OSVectorController() {}

public slots:

  void reportItems();

  void removeItem(OSItem* item);

  void replaceItem(OSItem * currentItem, const OSItemId& replacementItemId);

  void drop(const OSItemId& itemId);

  void makeNewItem();

signals:

  void itemIds(const std::vector<OSItemId>& itemIds);

  void selectedItemId(const OSItemId& itemId);

protected:

  virtual std::vector<OSItemId> makeVector() = 0;

  virtual void onRemoveItem(OSItem* item);

  virtual void onReplaceItem(OSItem * currentItem, const OSItemId& replacementItemId);

  virtual void onDrop(const OSItemId& itemId);

  virtual void onMakeNewItem();

};

} // openstudio

#endif // OPENSTUDIO_OSVECTORCONTROLLER_HPP

