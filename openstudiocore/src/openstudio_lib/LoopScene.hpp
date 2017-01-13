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

#ifndef OPENSTUDIO_LOOPSCENE_HPP
#define OPENSTUDIO_LOOPSCENE_HPP

#include <QPainter>
#include <QRectF>
#include <boost/smart_ptr.hpp>
#include "../model/Loop.hpp"
#include "OSItem.hpp"
#include "GridScene.hpp"

namespace openstudio {

namespace model {
  class Splitter;

  class Mixer;
  
  class Node;

  class HVACComponent;

  class ModelObject;
}

class HorizontalBranchItem;

class VerticalBranchItem;

class HorizontalBranchGroupItem;

class DemandSideItem;

class SupplySideItem;

class OASystemItem;

class LoopScene : public GridScene
{

  Q_OBJECT

  public:
  
  LoopScene( model::Loop loop,
             QObject *parent = nullptr );

  virtual ~LoopScene() {}

  model::Loop loop();

  public slots:

  void addedWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> wPtr, const openstudio::IddObjectType& type, const openstudio::UUID& uuid);

  void removedWorkspaceObject(std::shared_ptr<openstudio::detail::WorkspaceObject_Impl> wPtr, const openstudio::IddObjectType& type, const openstudio::UUID& uuid);
  
  void layout();

  //signals:
  //
  //void modelObjectSelected( model::OptionalModelObject &, bool readOnly );

  //void removeModelObjectClicked( model::ModelObject & );

  //void hvacComponentDropped(OSItemId itemid, model::HVACComponent & node);

  //void innerNodeClicked( model::ModelObject & );

  private:

  DemandSideItem * createDemandSide();

  SupplySideItem * createSupplySide();

  void initDefault();

  model::Loop m_loop;

  bool m_dirty;
};

} // openstudio

#endif // OPENSTUDIO_LOOPSCENE_HPP

