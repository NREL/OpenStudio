/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/AirLoopHVACZoneSplitter.hpp"
#include "../../model/AirLoopHVACZoneSplitter_Impl.hpp"
#include "../../model/AirLoopHVACSupplyPlenum.hpp"
#include "../../model/AirLoopHVACSupplyPlenum_Impl.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/idf/WorkspaceObjectOrder.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/AirLoopHVAC_SupplyPath_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::createAirLoopHVACSupplyPath(const Node& demandInletNode) {
    boost::optional<IdfObject> supplyPathIdf;

    if (auto t_airLoopHVAC = demandInletNode.airLoopHVAC()) {
      IdfObject supplyPathIdf(openstudio::IddObjectType::AirLoopHVAC_SupplyPath);
      m_idfObjects.push_back(supplyPathIdf);

      supplyPathIdf.setName(t_airLoopHVAC->name().get() + " " + demandInletNode.name().get() + " Supply Path");

      supplyPathIdf.setString(openstudio::AirLoopHVAC_SupplyPathFields::SupplyAirPathInletNodeName, demandInletNode.name().get());

      auto t_comps = t_airLoopHVAC->demandComponents(demandInletNode, t_airLoopHVAC->demandOutletNode());
      auto splitters = subsetCastVector<model::AirLoopHVACZoneSplitter>(t_comps);
      OS_ASSERT(splitters.size() == 1u);

      boost::optional<IdfObject> _zoneSplitter = translateAndMapModelObject(splitters.front());
      OS_ASSERT(_zoneSplitter);
      IdfExtensibleGroup eg = supplyPathIdf.pushExtensibleGroup();
      eg.setString(AirLoopHVAC_SupplyPathExtensibleFields::ComponentObjectType, _zoneSplitter->iddObject().name());
      eg.setString(AirLoopHVAC_SupplyPathExtensibleFields::ComponentName, _zoneSplitter->name().get());

      std::vector<ModelObject> supplyPlenums =
        t_airLoopHVAC->demandComponents(demandInletNode, t_airLoopHVAC->demandOutletNode(), AirLoopHVACSupplyPlenum::iddObjectType());
      for (auto& supplyPlenum : supplyPlenums) {
        eg = supplyPathIdf.pushExtensibleGroup();
        boost::optional<IdfObject> _supplyPlenum = translateAndMapModelObject(supplyPlenum);
        OS_ASSERT(_supplyPlenum);
        eg.setString(AirLoopHVAC_SupplyPathExtensibleFields::ComponentObjectType, _supplyPlenum->iddObject().name());
        eg.setString(AirLoopHVAC_SupplyPathExtensibleFields::ComponentName, _supplyPlenum->name().get());
      }
    }

    return supplyPathIdf;
  }

}  // namespace energyplus

}  // namespace openstudio
