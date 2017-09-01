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

#include "RoofGeometry.hpp"

#include "Geometry.hpp"
#include <boost/math/constants/constants.hpp>

namespace openstudio{

  RoofGeometry::RoofGeometry() 
  {
    // nop
  }

  std::vector< std::vector<Point3d> > RoofGeometry::makeShedRoof(std::vector<Point3d>& polygon, double roofPitchDegrees, double directionDegrees) 
  {
    // FIXME implement
    std::vector< std::vector<Point3d> > roofs;
    return roofs;
  }

  std::vector< std::vector<Point3d> > RoofGeometry::makeGableRoof(std::vector<Point3d>& polygon, double roofPitchDegrees) 
  {
    std::vector< std::vector<Point3d> > roofs;
    try 
    {
      roofs = doStraightSkeleton(polygon);
      if (roofs.size() == 0) {
        return roofs;
      }
    }
    catch (...)
    {
      return roofs;
    }

    // FIXME implement adjustments for gables

    return roofs;
  }

  std::vector< std::vector<Point3d> > RoofGeometry::makeHipRoof(std::vector<Point3d>& polygon, double roofPitchDegrees)
  {
    // FIXME: catch LOG_AND_THROW
    std::vector< std::vector<Point3d> > roofs;
    try 
    {
      roofs = doStraightSkeleton(polygon);
      if (roofs.size() == 0) {
        return roofs;
      }
    }
    catch (...) 
    {
      return roofs;
    }

    return roofs;
  }

  std::vector< std::vector<Point3d> > RoofGeometry::doStraightSkeleton(std::vector<Point3d>& polygon) {
    
    /* Straight skeleton algorithm implementation. Based on highly modified Petr
     * Felkel and Stepan Obdrzalek algorithm.
     * Translated from https://github.com/kendzi/kendzi-math
     */

    std::vector< std::vector<Point3d> > roofs;
    std::priority_queue<Event, std::vector<Event>, EventCompare> queue;
    std::vector<Event> processedEvents;
    std::vector<FaceQueue> faces;
    std::vector<Edge> edges;
    std::set< std::vector<Vertex> > sLav;

    if (!initPolygon(polygon)) {
      std::vector< std::vector<Point3d> > noRoofs;
      return noRoofs;
    }

    initSlav(polygon, sLav, edges, faces);
    initEvents(sLav, queue, edges);

    int count = 0;

    while (!queue.empty()) {
      // start processing skeleton level
      count = assertMaxNumberOfInteraction(count);
      if (count < 0) {
        std::vector< std::vector<Point3d> > noRoofs;
        return noRoofs;
      }

      double levelHeight = queue.top().distance;

      std::vector<Event> levelEvents = loadAndGroupLevelEvents(queue);

      for (Event event : levelEvents) {

        if (event.obsolete) {
          /*
          * Event is outdated some of parent vertex was processed
          * before
          */
          continue;
        }

        processedEvents.push_back(event);

        if (event.eventType == EVENT_EDGE) {
          LOG_AND_THROW("All edge events should be converted to MultiEdgeEvents for given level");
        } else if (event.eventType == EVENT_SPLIT) {
          LOG_AND_THROW("All split events should be converted to MultiSplitEvents for given level");
        } else if (event.eventType == EVENT_MULTI_SPLIT) {
          multiSplitEvent(event, sLav, queue, edges);
          continue;
        } else if (event.eventType == EVENT_PICK) {
          pickEvent(event, sLav, queue, edges);
          continue;
        } else if (event.eventType == EVENT_MULTI_EDGE) {
          multiEdgeEvent(event, sLav, queue, edges);
          continue;
        } else {
          LOG_AND_THROW("Unknown event type");
        }
      }

      processTwoNodeLavs(sLav);

      removeEventsUnderHeight(queue, levelHeight);
      removeEmptyLav(sLav);

    }

    // FIXME return addFacesToOutput(faces);
    return roofs;
  }

  void RoofGeometry::initSlav(std::vector<Point3d>& polygon, std::set< std::vector<Vertex> >& sLav, std::vector<Edge>& edges, std::vector<FaceQueue>& faces)
  {
    std::vector<Edge> edgesList;

    int size = polygon.size();
    for (int i = 0; i < size; i++) {
      int j = (i + 1) % size;
      edgesList.push_back(Edge(polygon.at(i), polygon.at(j)));
    }

    for (Edge edge : edgesList) {
      Edge nextEdge = edge.getOffsetEdge(edgesList, 1);
      
      Ray2d bisector = calcBisector(edge.end, edge, nextEdge);

      edge.bisectorNext = bisector;
      nextEdge.bisectorPrevious = bisector;

      edges.push_back(edge);
    }

    std::vector<Vertex> lav;

    for (Edge edge : edgesList) {

      Edge nextEdge = edge.getOffsetEdge(edgesList, 1);

      Vertex vertex = Vertex(edge.end, 0, edge.bisectorNext, edge, nextEdge);

      // FIXME lav.insert(vertex);
    }
    sLav.insert(lav);

    for (Vertex vertex : lav) {
      Vertex next = vertex.getOffsetVertex(lav, 1);

      // create face on right site of vertex
      FaceNode rightFace = FaceNode(vertex);

      FaceQueue faceQueue;
      faceQueue.edge = vertex.nextEdge.get();

      faceQueue.push(rightFace);
      
      faces.push_back(faceQueue);

      //FIXME vertex.rightFace = rightFace;

      // create face on left side of next vertex
      FaceNode leftFace = FaceNode(next);
      //FIXME rightFace.addPush(leftFace);
      //FIXME next.leftFace = leftFace;
    }
  }

  void RoofGeometry::initEvents(std::set< std::vector<Vertex> >& sLav, std::priority_queue<Event, std::vector<Event>, EventCompare>& queue, std::vector<Edge>& edges)
  {
    for (std::vector<Vertex> lav : sLav) {
      for (Vertex vertex : lav) {
        computeSplitEvents(vertex, edges, queue, boost::none);
      }
    }

    for (std::vector<Vertex> lav : sLav) {
      for (Vertex vertex : lav) {
        Vertex nextVertex = vertex.getOffsetVertex(lav, 1);
        computeEdgeEvents(vertex, nextVertex, queue);
      }
    }

  }

  bool RoofGeometry::initPolygon(std::vector<Point3d>& polygon)
  {
    if (polygon.size() < 3) {
      LOG(Error, "Polygon must have at least 3 points");
      return false;
    } else if (polygon[0] == polygon[polygon.size() - 1]) {
      LOG(Error, "Polygon can't start and end with the same point");
      return false;
    }

    makeCounterClockwise(polygon);

    return true;
  }

  // Updates points ordered as counter clockwise.
  void RoofGeometry::makeCounterClockwise(std::vector<Point3d>& polygon)
  {
    if (isClockwisePolygon(polygon))
    {
      reverse(polygon);
    }
  }

  // Check if polygon is clockwise.
  bool RoofGeometry::isClockwisePolygon(std::vector<Point3d>& polygon)
  {
    return (area(polygon) < 0);
  }

  // Calculate area of polygon outline. For clockwise area will be less than
  // zero, for counter-clockwise polygon area will be greater than zero.
  double RoofGeometry::area(std::vector<Point3d>& polygon)
  {
    int n = polygon.size();
    double a = 0.0;
    for (int p = n - 1, q = 0; q < n; p = q++)
    {
      Point3d pp = polygon.at(p);
      Point3d pq = polygon.at(q);
      a += pp.x() * pq.y() - pq.x() * pp.y();
    }
    return a * 0.5;
  }

  int RoofGeometry::assertMaxNumberOfInteraction(int count)
  {
    count++;
    if (count > 10000) {
      LOG(Error, "Polygon must have at least 3 points");
      return -1;
    }
    return count;
  }

  std::vector<Event> RoofGeometry::loadAndGroupLevelEvents(std::priority_queue<Event, std::vector<Event>, EventCompare>& queue) {
    std::vector<Event> levelEvents = loadLevelEvents(queue);
    return groupLevelEvents(levelEvents);
  }

  std::vector<Event> RoofGeometry::loadLevelEvents(std::priority_queue<Event, std::vector<Event>, EventCompare>& queue) {
    /*
     * Loads all non obsolete events which are on one level.
     */

    std::vector<Event> level;

    Event levelStart;
    do {
      levelStart = queue.top();
      queue.pop();
      // skip all obsolete events in level
    } while (levelStart.obsolete);
      
    if (levelStart.obsolete) {
      // all events obsolete
      return level;
    }

    double levelStartHeight = levelStart.distance;

    level.push_back(levelStart);

    Event event = queue.top();

    while (event.distance - levelStartHeight < SPLIT_EPSILON) {
      Event nextLevelEvent = queue.top();
      queue.pop();
      if (!nextLevelEvent.obsolete) {
        level.push_back(nextLevelEvent);
      }
    }

    return level;
  }

  std::vector<Event> RoofGeometry::groupLevelEvents(std::vector<Event>& levelEvents) {
    std::vector<Event> ret;
    std::set<Vertex> parentGroup;

    while (levelEvents.size() > 0) {
      parentGroup.clear();
      
      Event event = levelEvents.at(0);
      levelEvents.pop_back();
      Point3d eventCenter = event.point;
      double distance = event.distance;

      event.addEventToGroup(parentGroup);

      std::vector<Event> cluster;
      cluster.push_back(event);

      for (int j = 0; j < levelEvents.size(); j++) {
        Event test = levelEvents[j];

        if (test.isEventInGroup(parentGroup)) {
          /* Because of numerical errors, split event and edge event
           * can appear in slight different points. Epsilon can be
           * applied to level but event point can move rapidly even for
           * little changes in level. If two events for the same level
           * share the same parent, they should be merge together.
           */
          cluster.push_back(levelEvents.at(j));
          levelEvents.erase(levelEvents.begin() + j - 1);
          test.addEventToGroup(parentGroup);
          j--;
        } else if (getDistance(eventCenter, test.point) < SPLIT_EPSILON) {
          // group all events when the result points are near each other
          cluster.push_back(levelEvents.at(j));
          levelEvents.erase(levelEvents.begin() + j - 1);
          test.addEventToGroup(parentGroup);
          j--;
        }

      }

      // More than one event can share the same result point, we need to
      // create new level event.
      ret.push_back(createLevelEvent(eventCenter, distance, cluster));

    }
  }

  Event RoofGeometry::createLevelEvent(Point3d& eventCenter, double distance, std::vector<Event>& eventCluster) {
    std::vector<Chain> chains = createChains(eventCluster);

    if (chains.size() == 1) {
      Chain chain = chains.at(0);
      if (chain.getChainMode() == CHAIN_MODE_CLOSED_EDGE) {
        return Event::Event(eventCenter, distance, chain, true);
      } else if (chain.getChainMode() == CHAIN_MODE_EDGE) {
        return Event::Event(eventCenter, distance, chain, false);
      } else if (chain.getChainMode() == CHAIN_MODE_SPLIT) {
        return Event::Event(eventCenter, distance, chains);
      }
    }

    for (Chain chain : chains) {
      if (chain.getChainMode() == CHAIN_MODE_CLOSED_EDGE) {
        LOG_AND_THROW("found closed chain of events for single point, but found more then one chain");
      }
    }

    return Event::Event(eventCenter, distance, chains);
  }

  std::vector<Chain> RoofGeometry::createChains(std::vector<Event>& cluster) {
    /*
     * Create chains of events from cluster. Cluster is set of events which meet
     * in the same result point. Try to connect all event which share the same
     * vertex into chain. Events in chain are sorted. If events don't share
     * vertex, returned chains contains only one event.
     */

    std::vector<Event> edgeCluster;
    std::vector<Event> splitCluster;
    std::set<Vertex> vertexEventsParents;

    for (Event Event : cluster) {
      if (Event.eventType == EVENT_EDGE) {
        edgeCluster.push_back(Event);
      } else {
        if (Event.eventType == EVENT_SPLIT_VERTEX) {
          /* 
           * It will be processed in the next loop to find unique split
           * events for one parent.
           */
          continue;
        } else if (Event.eventType == EVENT_SPLIT) {
          vertexEventsParents.insert(Event.parent);
          splitCluster.push_back(Event);
        }
      }
    }

    for (Event Event : cluster) {
      if (Event.eventType == EVENT_SPLIT_VERTEX) {
        if (!vertexEventsParents.count(Event.parent)) {
          /*
           * It can be created multiple vertex events for one parent.
           * Its is caused because two edges share one vertex and new
           * event will be added to both of them. When processing we
           * need always group them into one per vertex. Always prefer
           * split events over vertex events.
           */
          vertexEventsParents.insert(Event.parent);
          splitCluster.push_back(Event);
        }
      }
    }

    std::vector<Chain> edgeChains;

    while (edgeCluster.size() > 0) {
      /*
      * We need to find all connected edge events, and create chains from
      * them. Two event are assumed as connected if next parent of one
      * event is equal to previous parent of second event.
      */
      //FIXME edgeChains.push_back(Chain(createEdgeChain(edgeCluster)));
    }

    std::vector<Chain> chains;
    for (Chain edgeChain : edgeChains) {
      chains.push_back(edgeChain);
    }

    while (splitCluster.size() > 0) {
      Event split = splitCluster.at(0);
      splitCluster.erase(splitCluster.begin());

      for (Chain chain : edgeChains) {
        // check if chain is split type
        if (isInEdgeChain(split, chain)) {
          // if we have edge chain it can't share split event
          continue;
        }

        /*
        * split event is not part of any edge chain, it should be added as
        * new single element chain;
        */
        //FIXME chains.push_back(Chain(split));
      }

    }

    /*
    * Return list of chains with type. Possible types are edge chain,
    * closed edge chain, split chain. Closed edge chain will produce pick
    * event. Always it can exist only one closed edge chain for point
    * cluster.
    */
    return chains;

  }

  std::vector<Event> RoofGeometry::createEdgeChain(std::vector<Event>& edgeCluster) {
    std::vector<Event> edgeList;

    edgeList.push_back(edgeCluster.at(0));
    edgeCluster.erase(edgeCluster.begin());

    // find all successors of edge event
    // find all predecessors of edge event

    do {
      Vertex beginVertex = edgeList.at(0).previousVertex;
      Vertex endVertex = edgeList.at(edgeList.size() - 1).nextVertex;

      bool do_continue = false;
      for (int i = 0; i < edgeCluster.size(); i++) {
        Event edge = edgeCluster.at(i);
        if (edge.previousVertex == endVertex) {
          // edge should be added as last in chain
          edgeCluster.erase(edgeCluster.begin() + i - 1);
          edgeList.push_back(edge);
          do_continue = true;
          break;
        } else if (edge.nextVertex == beginVertex) {
          // edge should be added as first in chain
          edgeCluster.erase(edgeCluster.begin() + i - 1);
          edgeList.insert(edgeCluster.begin(), edge);
          do_continue = true;
          break;
        }
      }

      if (do_continue) {
        continue;
      }

      break;

    } while (true);

    return edgeList;
  }

  bool RoofGeometry::isInEdgeChain(Event& split, Chain& chain) {
    Vertex splitParent = split.parent;

    std::vector<Event> edgeList; // FIXME = chain.edgeList;
    for (Event edgeEvent : edgeList) {
      if (edgeEvent.previousVertex == splitParent || edgeEvent.nextVertex == splitParent) {
        return true;
      }
    }

    return false;
  }

  void RoofGeometry::multiSplitEvent(Event& event, std::set< std::vector<Vertex> >& sLav, std::priority_queue<Event, std::vector<Event>, EventCompare>& queue, std::vector<Edge>& edges) {

    std::vector<Chain> chains = event.chains;
    Point3d center = event.point;

    createOppositeEdgeChains(sLav, chains, center);

    // sort list of chains clock wise
    std::sort(chains.begin(), chains.end()); // FIXME implement compare function

    // face node for split event is shared between two chains
    bool hasLastFaceNode = false;
    FaceNode lastFaceNode;

    // connect all edges into new bisectors and lavs
    int edgeListSize = chains.size();
    for (int i = 0; i < edgeListSize; i++) {
      Chain chainBegin = chains.at(i);
      Chain chainEnd = chains.at((i + 1) % edgeListSize);

      Vertex newVertex = createMultiSplitVertex(chainBegin.nextEdge, chainEnd.previousEdge, center, event.distance);

      // Split and merge lavs...
      Vertex beginNextVertex = chainBegin.nextVertex;
      Vertex endPreviousVertex = chainEnd.previousVertex;

      correctBisectorDirection(newVertex.bisector.get(), beginNextVertex, endPreviousVertex, chainBegin.nextEdge, chainEnd.previousEdge);

      std::vector<Vertex> beginNextVertexLav = beginNextVertex.getLav(sLav);
      std::vector<Vertex> endPreviousVertexLav = endPreviousVertex.getLav(sLav);

      if (areSameLav(beginNextVertexLav, endPreviousVertexLav)) {
        /*
        * if vertexes are in same lav we need to cut part of lav in the
        * middle of vertex and create new lav from that points
        */

        std::vector<Vertex> lavPart = beginNextVertex.cutLavPart(beginNextVertexLav, endPreviousVertex);

        std::vector<Vertex> lav;
        lav.push_back(newVertex);
        for (Vertex vertex : lavPart) {
          lav.push_back(vertex);
        }
        sLav.insert(lav);

      } else {
        /*
        * if vertexes are in different lavs we need to merge them into
        * one.
        */

        mergeBeforeBaseVertex(beginNextVertex, beginNextVertexLav, endPreviousVertex, endPreviousVertexLav);

        //FIXME endPreviousVertex.addNext(newVertex);
      }

      computeEvents(newVertex, queue, edges);

      std::tuple<bool, FaceNode> t = addSplitFaces(hasLastFaceNode, lastFaceNode, chainBegin, chainEnd, newVertex);
      hasLastFaceNode = std::get<0>(t);
      lastFaceNode = std::get<1>(t);

    }

    // remove all centers of events from lav
    edgeListSize = chains.size();
    for (int i = 0; i < edgeListSize; i++) {
      Chain chainBegin = chains.at(i);
      Chain chainEnd = chains.at((i + 1) % edgeListSize);

      //FIXME removeFromLav(chainBegin.currentVertex);
      //FIXME removeFromLav(chainEnd.currentVertex);

      if (chainBegin.currentVertex) {
        chainBegin.currentVertex.get().processed = true;
      }
      if (chainEnd.currentVertex) {
        chainEnd.currentVertex.get().processed = true;
      }

    }

  }

  bool RoofGeometry::areSameLav(std::vector<Vertex>& lav1, std::vector<Vertex>& lav2) {
    // FIXME Implement
    return false;
  }

  void RoofGeometry::pickEvent(Event& event, std::set< std::vector<Vertex> >& sLav, std::priority_queue<Event, std::vector<Event>, EventCompare>& queue, std::vector<Edge>& edges) {
    Point3d center = event.point;
    std::vector<Event> edgeList; //FIXME = event.chain.edgeList;

    // lav will be removed so it is final vertex.
    Vertex pickVertex = Vertex(center, event.distance, boost::none, boost::none, boost::none);
    pickVertex.processed = true;

    addMultiBackFaces(edgeList, pickVertex);
  }

  void RoofGeometry::addMultiBackFaces(std::vector<Event>& edgeList, Vertex& edgeVertex) {
    for (Event edgeEvent : edgeList) {

      Vertex leftVertex = edgeEvent.previousVertex;
      leftVertex.processed = true;
      //FIXME removeFromLav(leftVertex);

      Vertex rightVertex = edgeEvent.nextVertex;
      rightVertex.processed = true;
      //FIXME removeFromLav(rightVertex);

      addFaceBack(edgeVertex, leftVertex, rightVertex);

    }
  }

  FaceNode RoofGeometry::addFaceBack(Vertex& newVertex, Vertex& va, Vertex& vb) {
    // back face
    FaceNode fn = FaceNode(newVertex);
    //FIXME va.rightFace.addPush(fn);
    //FIXME FaceQueueUtil.connectQueues(fn, vb.leftFace);
    return fn;
  }

  FaceNode RoofGeometry::addFaceLeft(Vertex& newVertex, Vertex& va) {
    FaceNode fn = FaceNode(newVertex);
    //FIXME va.leftFace.addPush(fn);
    //FIXME newVertex.leftFace = fn;
    return fn;
  }

  FaceNode RoofGeometry::addFaceRight(Vertex& newVertex, Vertex& vb) {
    FaceNode fn = FaceNode(newVertex);
    //FIXME vb.rightFace.addPush(fn);
    //FIXME newVertex.rightFace = fn;
    return fn;
  }

  void RoofGeometry::multiEdgeEvent(Event& event, std::set< std::vector<Vertex> >& sLav, std::priority_queue<Event, std::vector<Event>, EventCompare>& queue, std::vector<Edge>& edges) {

    Point3d center = event.point;
    std::vector<Event> edgeList; //FIXME = event.chain.edgeList;

    Vertex previousVertex = event.chain.previousVertex;
    previousVertex.processed = true;

    Vertex nextVertex = event.chain.nextVertex;
    nextVertex.processed = true;

    Ray2d bisector = calcBisector(center, previousVertex.previousEdge.get(), nextVertex.nextEdge.get());
    Vertex edgeVertex = Vertex(center, event.distance, bisector, previousVertex.previousEdge.get(), nextVertex.nextEdge.get());

    // left face
    addFaceLeft(edgeVertex, previousVertex);

    // right face
    addFaceRight(edgeVertex, nextVertex);

    //FIXME previousVertex.addPrevious(edgeVertex);

    // back faces
    addMultiBackFaces(edgeList, edgeVertex);

    computeEvents(edgeVertex, queue, edges);
  }

  Vertex RoofGeometry::createMultiSplitVertex(Edge& nextEdge, Edge& previousEdge, Point3d& center, double distance) {
    Ray2d bisector = calcBisector(center, previousEdge, nextEdge);

    // edges are mirrored for event
    return Vertex(center, distance, bisector, previousEdge, nextEdge);
  }

  Ray2d RoofGeometry::calcBisector(Point3d& p, Edge& e1, Edge& e2) {
    Vector3d norm1 = e1.normalize();
    Vector3d norm2 = e2.normalize();
    Vector3d bisector = calcVectorBisector(norm1, norm2);
    return Ray2d(p, bisector);
  }

  Vector3d RoofGeometry::calcVectorBisector(Vector3d& norm1, Vector3d& norm2) {
    Vector3d e1v = orthogonalLeft(norm1);
    Vector3d e2v = orthogonalLeft(norm2);

    // 90 - 180 || 180 - 270
    if (norm1.dot(norm2) > 0) {
      e1v += e2v;
      return e1v;
    }

    // 0 - 180
    Vector3d ret = Vector3d(norm1);
    ret = ret.reverseVector();
    ret += norm2;

    if (e1v.dot(norm2) < 0) {
      // 270 - 360
      ret = ret.reverseVector();
    }

    return ret;
  }

  Vector3d RoofGeometry::orthogonalLeft(Vector3d& v) {
    return Vector3d(-v.y(), v.x(), v.z());
  }

  Vector3d RoofGeometry::orthogonalRight(Vector3d& v) {
    return Vector3d(v.y(), -v.x(), v.z());
  }

  void RoofGeometry::correctBisectorDirection(Ray2d& bisector, Vertex& beginNextVertex, Vertex& endPreviousVertex, Edge& beginEdge, Edge& endEdge) {
    /*
    * New bisector for vertex is created using connected edges. For
    * parallel edges numerical error may appear and direction of created
    * bisector is wrong. For parallel edges direction of edge need to be
    * corrected using location of vertex.
    */
    Edge beginEdge2 = beginNextVertex.previousEdge.get();
    Edge endEdge2 = endPreviousVertex.nextEdge.get();

    if (&beginEdge != &beginEdge2 || &endEdge != &endEdge2) {
      LOG_AND_THROW("Unexpected situation");
    }

    /*
    * Check if edges are parallel and in opposite direction to each other.
    */
    if (beginEdge.normalize().dot(endEdge.normalize()) < -0.97) {
      Point3d bp = bisector.point;
      Point3d bnvp = beginNextVertex.point;
      Vector3d n1 = Vector3d(bp.x(), bp.y(), bp.z()) - Vector3d(bnvp.x(), bnvp.y(), bnvp.z());
      Vector3d n2 = Vector3d(bnvp.x(), bnvp.y(), bnvp.z()) - Vector3d(bp.x(), bp.y(), bp.z());
      n1.normalize();
      n2.normalize();
      Vector3d bisectorPrediction = calcVectorBisector(n1, n2);

      if (bisector.vector.dot(bisectorPrediction) < 0) {
        /*
        * Bisector is calculated in opposite direction to edges and
        * center.
        */
        bisector.vector = bisector.vector.reverseVector();
      }
    }
  }

  /// Add all vertex from "merged" lav into "base" lav. Vertex are added before
  /// base vertex. Merged vertex order is reversed.
  void RoofGeometry::mergeBeforeBaseVertex(Vertex& base, std::vector<Vertex>& baseList, Vertex& merged, std::vector<Vertex>& mergedList) {
    int size = mergedList.size();

    for (int i = 0; i < size; i++) {
      Vertex nextMerged = merged.getOffsetVertex(mergedList, 1);
      auto it = std::find(baseList.begin(), baseList.end(), base);
      baseList.insert(it - 1, nextMerged);
    }

    mergedList.erase(mergedList.begin(), mergedList.end());
  }

  void RoofGeometry::computeEvents(Vertex& vertex, std::priority_queue<Event, std::vector<Event>, EventCompare>& queue, std::vector<Edge>& edges) {
    boost::optional<double> distanceSquared = computeCloserEdgeEvent(vertex, queue);
    computeSplitEvents(vertex, edges, queue, distanceSquared);
  }

  boost::optional<double> RoofGeometry::computeCloserEdgeEvent(Vertex& vertex, std::priority_queue<Event, std::vector<Event>, EventCompare>& queue) {
    /*
     * Calculate two new edge events for given vertex. Events are generated
     * using current, previous and next vertex in current lav. When two edge
     * events are generated distance from source is check. To queue is added
     * only closer event or both if they have the same distance.
     */

    Vertex nextVertex; // FIXME = vertex.next();
    Vertex previousVertex; // FIXME = vertex.previous();

    Point3d point = vertex.point;

    /*
     * We need to chose closer edge event. When two evens appear in epsilon
     * we take both. They will create single MultiEdgeEvent.
     */

    boost::optional<Point3d> point1 = computeIntersectionBisectors(vertex, nextVertex);
    boost::optional<Point3d> point2 = computeIntersectionBisectors(previousVertex, vertex);

    if (!point1 && !point2) {
      return boost::none;
    }

    double distance1 = std::numeric_limits<double>::max();
    double distance2 = std::numeric_limits<double>::max();

    if (point1) {
      distance1 = getDistanceSquared(point, point1.get());
    }
    if (point2) {
      distance2 = getDistanceSquared(point,point2.get());
    }

    if (distance1 - SPLIT_EPSILON < distance2) {
      queue.push(createEdgeEvent(point1.get(), vertex, nextVertex));
    }
    if (distance2 - SPLIT_EPSILON < distance1) {
      queue.push(createEdgeEvent(point2.get(), previousVertex, vertex));
    }

    if (distance1 < distance2){
      return distance1;
    }
    return distance2;
  }

  boost::optional<Point3d> RoofGeometry::computeIntersectionBisectors(Vertex& vertexPrevious, Vertex& vertexNext) {
    Ray2d bisectorPrevious = vertexPrevious.bisector.get();
    Ray2d bisectorNext = vertexNext.bisector.get();

    boost::optional<Point3d> intersect = bisectorPrevious.intersectRay2d(bisectorNext);

    if (!intersect) {
      return boost::none;
    }

    if (vertexPrevious.point == intersect.get() || vertexNext.point == intersect.get()) {
      // skip the same points
      return boost::none;
    }

    return intersect.get();
  }

  Event RoofGeometry::createEdgeEvent(Point3d& point, Vertex& previousVertex, Vertex& nextVertex) {
    return Event(point, calcDistance(point, previousVertex.nextEdge.get()), previousVertex, nextVertex);
  }

  double RoofGeometry::calcDistance(Point3d& intersect, Edge& currentEdge) {
    // TODO: Can be replaced by getDistancePointToLineSegment() ?
    Vector3d edge = currentEdge.end - currentEdge.begin;
    Vector3d vector = intersect - currentEdge.begin;

    Vector3d pointOnVector = orthogonalProjection(edge, vector);

    return getDistance(Point3d(vector.x(), vector.y(), vector.z()), Point3d(pointOnVector.x(), pointOnVector.y(), pointOnVector.z()));
  }

  // see http://en.wikipedia.org/wiki/Vector_projection
  Vector3d RoofGeometry::orthogonalProjection(Vector3d& unitVector, Vector3d& vectorToProject) {
    Vector3d n = Vector3d(unitVector);
    n.normalize();

    double px = vectorToProject.x();
    double py = vectorToProject.y();
    double pz = vectorToProject.z();

    double ax = n.x();
    double ay = n.y();
    double az = n.z();

    double vx = px * ax * ax + py * ax * ay + pz * ax * az;
    double vy = px * ax * ay + py * ay * ay + pz * ay * az;
    double vz = px * ax * az + py * ay * az + pz * az * az;

    return Vector3d(vx, vy, vz);
  }

  void RoofGeometry::computeEdgeEvents(Vertex& previousVertex, Vertex& nextVertex, std::priority_queue<Event, std::vector<Event>, EventCompare>& queue) {
    boost::optional<Point3d> point = computeIntersectionBisectors(previousVertex, nextVertex);
    if (point) {
      queue.push(createEdgeEvent(point.get(), previousVertex, nextVertex));
    }
  }

  void RoofGeometry::computeSplitEvents(Vertex& vertex, std::vector<Edge>& edges, std::priority_queue<Event, std::vector<Event>, EventCompare>& queue, boost::optional<double> distanceSquared) {
    Point3d source = vertex.point;
    
    std::vector<SplitCandidate> oppositeEdges = calcOppositeEdges(vertex, edges);

    // check if it is vertex split event
    for (SplitCandidate oppositeEdge : oppositeEdges) {
      Point3d point = oppositeEdge.point;

      if (distanceSquared) {
        if (getDistanceSquared(source, point) > distanceSquared.get() + SPLIT_EPSILON) {
          /*
          * Current split event distance from source of event is
          * greater then for edge event. Split event can be reject.
          * Distance from source is not the same as distance for
          * edge. Two events can have the same distance to edge but
          * they will be in different distance form its source.
          * Unnecessary events should be reject otherwise they cause
          * problems for degenerate cases.
          */
          continue;
        }
      }

      // check if it is vertex split event
      if (oppositeEdge.oppositePoint) {
        // some of vertex event can share the same opposite
        // point
        queue.push(Event(point, oppositeEdge.distance, vertex));
        continue;
      }

      queue.push(Event(point, oppositeEdge.distance, vertex, oppositeEdge.oppositeEdge.get()));
      continue;

    }
  }

  std::vector<SplitCandidate> RoofGeometry::calcOppositeEdges(Vertex& vertex, std::vector<Edge>& edges) {

    std::vector<SplitCandidate> ret;

    for (Edge edgeEntry : edges) {

      LineLinear2d edge = LineLinear2d(edgeEntry.begin, edgeEntry.end);

      // check if edge is behind bisector
      if (edgeBehindBisector(vertex.bisector.get(), edge)) {
        continue;
      }

      // compute the coordinates of the candidate point Bi
      boost::optional<SplitCandidate> candidatePoint = calcCandidatePointForSplit(vertex, edgeEntry);

      if (candidatePoint) {
        ret.push_back(candidatePoint.get());
      }

    }

    // FIXME implement sort here

    return ret;
  }

  bool RoofGeometry::edgeBehindBisector(Ray2d& bisector, LineLinear2d& edge) {
    /*
    * Simple intersection test between the bisector starting at V and the
    * whole line containing the currently tested line segment ei rejects
    * the line segments laying "behind" the vertex V
    */
    return (!bisector.collide(edge, SPLIT_EPSILON));
  }

  boost::optional<SplitCandidate> RoofGeometry::calcCandidatePointForSplit(Vertex& vertex, Edge& edge) {
    
    boost::optional<Edge> vertexEdge = chooseLessParallelVertexEdge(vertex, edge);
    if (!vertexEdge) {
      return boost::none;
    }

    Vector3d vertexEdgeNormNegate = vertexEdge.get().normalize();
    Vector3d edgesBisector = calcVectorBisector(vertexEdgeNormNegate, edge.normalize());

    LineLinear2d llv = LineLinear2d(vertexEdge.get().begin, vertexEdge.get().end);
    LineLinear2d lle = LineLinear2d(edge.begin, edge.end);
    boost::optional<Point3d> edgesCollide = llv.collide(lle);

    if (!edgesCollide) {
      /*
      * Check should be performed to exclude the case when one of the
      * line segments starting at V is parallel to ei.
      */
      LOG_AND_THROW("Unexpected situation");
    }

    LineLinear2d edgesBisectorLine = Ray2d(edgesCollide.get(), edgesBisector).getLinearForm();

    /*
    * Compute the coordinates of the candidate point Bi as the intersection
    * between the bisector at V and the axis of the angle between one of
    * the edges starting at V and the tested line segment ei
    */
    boost::optional<Point3d> candidatePoint = vertex.bisector.get().collide(edgesBisectorLine, SPLIT_EPSILON);

    if (!candidatePoint) {
      return boost::none;
    }

    if (edge.bisectorPrevious.isOnRightSide(candidatePoint.get(), SPLIT_EPSILON) && edge.bisectorNext.isOnLeftSide(candidatePoint.get(), SPLIT_EPSILON)) {

      double distance = calcDistance(candidatePoint.get(), edge);

      if (edge.bisectorPrevious.isOnLeftSide(candidatePoint.get(), SPLIT_EPSILON) || edge.bisectorNext.isOnRightSide(candidatePoint.get(), SPLIT_EPSILON)) {

        Point3d oppositePoint = edge.begin;
        return SplitCandidate(candidatePoint.get(), distance, boost::none, oppositePoint);

      }

      return SplitCandidate(candidatePoint.get(), distance, edge, boost::none);

    }

    return boost::none;

  }

  boost::optional<Edge> RoofGeometry::chooseLessParallelVertexEdge(Vertex& vertex, Edge& edge) {
    Edge edgeA = vertex.previousEdge.get();
    Edge edgeB = vertex.nextEdge.get();

    Edge vertexEdge = edgeA;

    double edgeADot = std::abs(edge.normalize().dot(edgeA.normalize()));
    double edgeBDot = std::abs(edge.normalize().dot(edgeB.normalize()));

    if (edgeADot + edgeBDot >= 2 - SPLIT_EPSILON) {
      // both lines are parallel to given edge
      return boost::none;
    }

    if (edgeADot > edgeBDot) {
      /*
      * Simple check should be performed to exclude the case when one of
      * the line segments starting at V (vertex) is parallel to e_i
      * (edge) we always chose edge which is less parallel.
      */
      vertexEdge = edgeB;
    }

    return vertexEdge;
  }

  void RoofGeometry::createOppositeEdgeChains(std::set< std::vector<Vertex> >& sLav, std::vector<Chain>& chains, Point3d& center) {
    /*
    * Add chain created from opposite edge, this chain have to be
    * calculated during processing event because lav could change during
    * processing another events on the same level
    */
    std::set<Edge> oppositeEdges;

    std::vector<Chain> oppositeEdgeChains;
    std::vector<Chain> chainsForRemoval;

    for (Chain chain : chains) {
      // add opposite edges as chain parts
      if (chain.chainType == CHAIN_TYPE_SPLIT) {
        boost::optional<Edge> oppositeEdge = chain.oppositeEdge;

        if (oppositeEdge && !oppositeEdges.count(oppositeEdge.get())) {
          // find lav vertex for opposite edge

          boost::optional<Vertex> nextVertex = findOppositeEdgeLav(sLav, oppositeEdge.get(), center);
          if (nextVertex) {
            oppositeEdgeChains.push_back(Chain(oppositeEdge.get(), nextVertex.get()));
          } else {
            findOppositeEdgeLav(sLav, oppositeEdge.get(), center);
            chainsForRemoval.push_back(chain);
          }
          oppositeEdges.insert(oppositeEdge.get());
        }

      } else if (chain.chainType == CHAIN_TYPE_EDGE) {
        if (chain.getChainMode() == CHAIN_MODE_SPLIT) {
          boost::optional<Edge> oppositeEdge = chain.oppositeEdge;
          if (oppositeEdge) {
            // never happen?
            // find lav vertex for opposite edge
            oppositeEdges.insert(oppositeEdge.get());
          }
        }

      }

    }

    /*
    * if opposite edge can't be found in active lavs then split chain with
    * that edge should be removed
    */
    for (Chain chain : chainsForRemoval) {
      // FIXME chains.erase(chain);
    }
    for (Chain chain : oppositeEdgeChains) {
      chains.push_back(chain);
    }

  }

  boost::optional<Vertex> RoofGeometry::findOppositeEdgeLav(std::set< std::vector<Vertex> > sLav, Edge oppositeEdge, Point3d center) {
    std::vector<Vertex> edgeLavs = findEdgeLavs(sLav, oppositeEdge);
    return chooseOppositeEdgeLav(edgeLavs, oppositeEdge, center);
  }

  std::vector<Vertex> RoofGeometry::findEdgeLavs(std::set< std::vector<Vertex> > sLav, Edge oppositeEdge) {
    std::vector<Vertex> edgeLavs;
    for (std::vector<Vertex> lav : sLav) {
      boost::optional<Vertex> vertexInLav = getEdgeInLav(lav, oppositeEdge);
      if (vertexInLav) {
        edgeLavs.push_back(vertexInLav.get());
      }
    }
    return edgeLavs;
  }

  boost::optional<Vertex> RoofGeometry::getEdgeInLav(std::vector<Vertex> lav, Edge oppositeEdge) {
    for (Vertex vertex : lav) {
      if (vertex.previousEdge && oppositeEdge == vertex.previousEdge.get()){
        return vertex;
      } else{
        boost::optional<Edge> nextEdge = vertex.getOffsetVertex(lav, -1).nextEdge;
        if (nextEdge && oppositeEdge == nextEdge.get()) {
          return vertex;
        }
      }
    }
    return boost::none;
  }

  boost::optional<Vertex> RoofGeometry::chooseOppositeEdgeLav(std::vector<Vertex> edgeLavs, Edge oppositeEdge, Point3d center) {
    if (edgeLavs.size() == 0) {
      return boost::none;
    } else if (edgeLavs.size() == 1) {
      return edgeLavs.at(0);
    }

    Point3d edgeStart = oppositeEdge.begin;
    Vector3d edgeNorm = oppositeEdge.normalize();
    Vector3d centerVector = center - edgeStart;
    double centerDot = edgeNorm.dot(centerVector);

    for (Vertex end : edgeLavs) {
      Vertex begin = end.getOffsetVertex(edgeLavs, -1);
      Vector3d beginVector = begin.point - edgeStart;
      Vector3d endVector = end.point - edgeStart;

      double beginDot = edgeNorm.dot(beginVector);
      double endDot = edgeNorm.dot(endVector);

      /*
       * Make projection of center, begin and end into edge. Begin and end
       * are vertexes chosen by opposite edge (then point to opposite edge).
       * Chose lav only when center is between begin and end. Only one lav
       * should meet criteria.
       */

      if (beginDot < centerDot && centerDot < endDot || beginDot > centerDot && centerDot > endDot) {
        return end;
      }

    }

    // Additional check if center is inside lav
    for (Vertex end : edgeLavs) {
      int size = 0; // FIXME = end.list.size();
      std::vector<Point3d> points;
      Vertex next = end;
      for (int i = 0; i < size; i++) {
        points.push_back(next.point);
        next = next.getOffsetVertex(edgeLavs, 1);
      }
      if (isInsidePolygon(center, points)) {
        return end;
      }
    }

    LOG_AND_THROW("could not find lav for opposite edge, it could be correct but need some test data to check.")

  }

  /// Test if point is inside polygon
  bool RoofGeometry::isInsidePolygon(Point3d point, std::vector<Point3d> points) {
    /* 
     * see http://en.wikipedia.org/wiki/Point_in_polygon
     * see http://en.wikipedia.org/wiki/Even-odd_rule
     * see http://paulbourke.net/geometry/insidepoly/
     */

    int numpoints = points.size();

    if (numpoints < 3) {
      return false;
    }

    int it = 0;

    Point3d first = points.at(it);

    bool oddNodes = false;

    Point3d node1;
    Point3d node2;

    for (int i = 0; i < numpoints; i++) {
      node1 = points.at(it);
      it++;
      if (i == numpoints - 1) {
        node2 = first;
      } else {
        node2 = points.at(it);
      }

      double x = point.x();
      double y = point.y();

      // FIXME: missing parantheses?
      if (node1.y() < y && node2.y() >= y || node2.y() < y && node1.y() >= y) {
        if (node1.x() + (y - node1.y()) / (node2.y() - node1.y()) * (node2.x() - node1.x()) < x) {
          oddNodes = !oddNodes;
        }
      }
    }

    return oddNodes;
  }

  std::tuple<bool, FaceNode> RoofGeometry::addSplitFaces(bool hasLastFaceNode, FaceNode& lastFaceNode, Chain& chainBegin, Chain& chainEnd, Vertex& newVertex) {
    if (chainBegin.chainType == CHAIN_TYPE_SINGLE_EDGE) {
      /*
      * When chain is generated by opposite edge we need to share face
      * between two chains. Number of that chains shares is always odd.
      */
      Vertex beginVertex = chainBegin.nextVertex;

      // right face
      if (!hasLastFaceNode) {
        /*
        * Vertex generated by opposite edge share three faces, but
        * vertex can store only left and right face. So we need to
        * create vertex clone to store additional back face.
        */
        beginVertex = createOppositeEdgeVertex(newVertex);

        /* same face in two vertex, original and in opposite edge clone */
        // FIXME newVertex.rightFace = beginVertex.rightFace;
        // FIXME lastFaceNode = beginVertex.leftFace;
        hasLastFaceNode = true;
      } else {
        // face queue exist simply assign it to new node
        // FIXME newVertex.rightFace = lastFaceNode;
        hasLastFaceNode = false;
      }

    } else {
      Vertex beginVertex = chainBegin.currentVertex.get();

      // right face
      addFaceRight(newVertex, beginVertex);
    }

    if (chainEnd.chainType == CHAIN_TYPE_SINGLE_EDGE) {
      Vertex endVertex = chainEnd.previousVertex;

      // left face
      if (!hasLastFaceNode) {
        /*
        * Vertex generated by opposite edge share three faces, but
        * vertex can store only left and right face. So we need to
        * create vertex clone to store additional back face.
        */
        endVertex = createOppositeEdgeVertex(newVertex);

        /* same face in two vertex, original and in opposite edge clone */
        // FIXME newVertex.leftFace = endVertex.leftFace;
        // FIXME lastFaceNode = endVertex.leftFace;
        hasLastFaceNode = true;
      } else {
        // face queue exist simply assign it to new node
        // FIXME newVertex.leftFace = lastFaceNode;
        hasLastFaceNode = false;
      }

    } else {
      Vertex endVertex = chainEnd.currentVertex.get();

      // left face
      addFaceLeft(newVertex, endVertex);
    }

    return std::tuple<bool, FaceNode>(hasLastFaceNode, lastFaceNode);
  }

  Vertex RoofGeometry::createOppositeEdgeVertex(Vertex& newVertex) {
    /*
    * When opposite edge is processed we need to create copy of vertex to
    * use in opposite face. When opposite edge chain occur vertex is shared
    * by additional output face.
    */

    Vertex vertex = Vertex(newVertex.point, newVertex.distance, newVertex.bisector.get(), newVertex.previousEdge.get(), newVertex.nextEdge.get());

    // create new empty node queue
    FaceNode fn = FaceNode(vertex);
    // FIXME vertex.leftFace = fn;
    // FIXME vertex.rightFace = fn;

    // add one node for queue to present opposite site of edge split event
    FaceQueue rightFace;
    // FIXME rightFace.addFirst(fn);

    return vertex;
  }

  void RoofGeometry::processTwoNodeLavs(std::set< std::vector<Vertex> >& sLav) {
    for (std::vector<Vertex> lav : sLav) {
      if (lav.size() == 2) {
        Vertex first = lav.front();
        Vertex last = first.getOffsetVertex(lav, 1);

        // FIXME FaceQueueUtil.connectQueues(first.leftFace, last.rightFace);
        // FIXME FaceQueueUtil.connectQueues(first.rightFace, last.leftFace);

        first.processed = true;
        last.processed = true;

        // FIXME lav.erase(first);
        // FIXME lav.erase(last);

      }
    }
  }

  void RoofGeometry::removeEventsUnderHeight(std::priority_queue<Event, std::vector<Event>, EventCompare>& queue, double levelHeight) {
    while (!queue.empty()) {
      if (queue.top().distance > levelHeight + SPLIT_EPSILON) {
        break;
      }
      queue.pop();
    }
  }

  void RoofGeometry::removeEmptyLav(std::set< std::vector<Vertex> >& sLav) {
    bool foundEmptyLav = true;
    while (foundEmptyLav) {
      foundEmptyLav = false;
      for (std::vector<Vertex> lav : sLav) {
        if (lav.size() == 0) {
          sLav.erase(lav);
          foundEmptyLav = true;
          break;
        }
      }
    }
  }

  // Events

  Event::Event(Point3d& aPoint, double aDistance) {
    point = aPoint;
    distance = aDistance;
  }

  Event::Event(Point3d& aPoint, double aDistance, Vertex& aPreviousVertex, Vertex& aNextVertex) {
    eventType = EVENT_EDGE;
    point = aPoint;
    distance = aDistance;
    previousVertex = aPreviousVertex;
    nextVertex = aNextVertex;
  }
  
  Event::Event(Point3d& aPoint, double aDistance, Vertex& aParent) {
    eventType = EVENT_SPLIT_VERTEX;
    point = aPoint;
    distance = aDistance;
    parent = aParent;
  }
  
  Event::Event(Point3d& aPoint, double aDistance, Vertex& aParent, Edge& aOppositeEdge) {
    eventType = EVENT_SPLIT;
    point = aPoint;
    distance = aDistance;
    parent = aParent;
    oppositeEdge = aOppositeEdge;
  }
  
  Event::Event(Point3d& aPoint, double aDistance, Chain& aChain, bool isPickEvent) {
    if (isPickEvent) {
      eventType = EVENT_PICK;
    } else {
      eventType = EVENT_MULTI_EDGE;
    }
    point = aPoint;
    distance = aDistance;
    chain = aChain;
  }
  
  Event::Event(Point3d& aPoint, double aDistance, std::vector<Chain>& aChains) {
    eventType = EVENT_MULTI_SPLIT;
    point = aPoint;
    distance = aDistance;
    chains = aChains;
  }

  // FACE NODES

  FaceNode::FaceNode(Vertex& aVertex) {
    vertex = aVertex;
  }

  // VERTEX

  Vertex::Vertex(Point3d& aPoint, double aDistance, boost::optional<Ray2d&> aBisector, boost::optional<Edge&> aPreviousEdge, boost::optional<Edge&> aNextEdge) {
    point = aPoint;
    distance = aDistance;
    bisector = aBisector;
    previousEdge = aPreviousEdge;
    nextEdge = aNextEdge;
    processed = false;
  }

  Vertex Vertex::getOffsetVertex(std::vector<Vertex>& vertexes, int offset) {
    // FIXME implement
    // offset=-1 implies previous vertex, offset=1 implies next vertex
  }

  std::vector<Vertex> Vertex::getLav(std::set< std::vector<Vertex> >& sLav) {
    for (std::vector<Vertex> lav : sLav) {
      if (std::find(lav.begin(), lav.end(), *this) != lav.end()) {
        return lav;
      }
    }
  }

  // Returns cut portion of lav from this vertex to endVertex, including both
  std::vector<Vertex> Vertex::cutLavPart(std::vector<Vertex>& lav, Vertex& endVertex) {
    std::vector<Vertex> ret;

    int size = lav.size();

    // FIXME implement

  }

  // Returns true if this vertex is less than other
  bool Vertex::operator<(const Vertex& other) const {

    // FIXME implement
    return false;

  }

  // Returns true if this vertex is equal to other
  bool Vertex::operator==(const Vertex& other) const {

    // FIXME implement
    return false;

  }

  // EDGE

  Edge::Edge(Point3d& aBegin, Point3d& aEnd) {
    begin = aBegin;
    end = aEnd;
  }

  Vector3d Edge::normalize() {
    Vector3d v = Vector3d(end.x(), end.y(), end.z()) - Vector3d(begin.x(), begin.y(), begin.z());
    v.normalize();
    return v;
  }

  Edge Edge::getOffsetEdge(std::vector<Edge>& edges, int offset) {
    // FIXME implement
  }

  // Returns true if this edge is less than other
  bool Edge::operator<(const Edge& other) const {

    // FIXME implement
    return false;

  }

  // Returns true if this edge is equal to other
  bool Edge::operator==(const Edge& other) const {

    // FIXME implement
    return false;

  }
  // RAY2D

  Ray2d::Ray2d(Point3d& aPoint, Vector3d& aVector) {
    point = aPoint;
    vector = aVector;
  }

  boost::optional<Point3d> Ray2d::collide(LineLinear2d& line, double epsilon) {
    // rewrite?
    boost::optional<Point3d> collide = getLinearForm().collide(line);
    if (!collide) {
      return boost::none;
    }

    /*
    * Portably there is better way to do this. this is from graphical.
    */
    Vector3d collideVector = collide.get() - point;

    double dot = vector.dot(collideVector);

    if (dot < epsilon) {
      return boost::none;
    }

    return collide;
  }

  LineLinear2d Ray2d::getLinearForm() {
    double x = point.x();
    double y = point.y();

    double B = -vector.x();
    double A = vector.y();

    double C = -1 * (A * x + B * y);
    return LineLinear2d(A, B, C);
  }
  
  bool Ray2d::isOnLeftSide(Point3d& p, double epsilon) {
    Vector3d direction = p - point;
    Vector3d orthRight = RoofGeometry::orthogonalRight(vector);
    return (orthRight.dot(direction) < epsilon);
  }

  bool Ray2d::isOnRightSide(Point3d& p, double epsilon) {
    Vector3d direction = p - point;
    Vector3d orthRight = RoofGeometry::orthogonalRight(vector);
    return (orthRight.dot(direction) > -epsilon);
  }

  boost::optional<Point3d> Ray2d::intersectRay2d(Ray2d& other) {
    /*
     * Calculate intersection points for rays. It can return more then one
     * intersection point when rays overlaps.
     *
     * see http://geomalgorithms.com/a05-_intersect-1.html
     * see http://softsurfer.com/Archive/algorithm_0102/algorithm_0102.htm
     */

    const double SMALL_NUM = 0.00000001;

    Point3d s1p0 = point;
    Point3d s1p1 = Point3d(point) + vector;

    Point3d s2p0 = other.point;
    Point3d s2p1 = Point3d(other.point) + other.vector;

    Vector3d u = vector;
    Vector3d v = other.vector;

    Vector3d w = s1p0 - s2p0;

    double d = perpDot(u, v);

    // test if they are parallel (includes either being a point)
    if (std::abs(d) < SMALL_NUM) { // S1 and S2 are parallel
      
      if (perpDot(u, w) != 0 || perpDot(v, w) != 0) {
        // they are NOT collinear
        return boost::none;
      }
      
      // they are collinear or degenerate
      // check if they are degenerate points
      double du = u.dot(u);
      double dv = v.dot(v);
      
      if (du == 0 && dv == 0) {
        // both segments are points
        if (s1p0 != s2p0) {
          return boost::none;
        }
        // they are the same point
        return s1p0;
      }
      
      if (du == 0) {
        // S1 is a single point
        Ray2d r = Ray2d(s2p0, v);
        if (!r.inCollinearRay(s1p0)) {
          return boost::none;
        }
        return s1p0;
      }
      
      if (dv == 0) {
        // S2 is a single point
        Ray2d r = Ray2d(s1p0, u);
        if (!r.inCollinearRay(s2p0)) {
          return boost::none;
        }
        return s2p0;
      }

      //they are collinear segments - get overlap (or not)
      double t0, t1;
      // endpoints of S1 in eqn for S2
      Vector3d w2 = s1p1 - s2p0;
      
      if (v.x() != 0) {
        t0 = w.x() / v.x();
        t1 = w2.x() / v.x();
      } else {
        t0 = w.y() / v.y();
        t1 = w2.y() / v.y();
      }

      if (t0 > t1) {
        // must have t0 smaller than t1
        double t = t0;
        t0 = t1;
        t1 = t; // swap if not
      }
      
      if (t1 < 0) {
        return boost::none;
      }

      // clip to min 0
      if (t0 < 0) {
        t0 = 0;
      }

      // they overlap in a valid subsegment

      // I0 = S2_P0 + t0 * v;
      // I1 = S2_P0 + t1 * v;
      Point3d I0 = s2p0 + t0 * v;
      Point3d I1 = s2p0 + t1 * v;

      if (t0 == t1) {
        // intersect is a point
        return I0;
      }

      return I0; // only need first intersection point
    }

    // the segments are skewed and may intersect in a point
    
    // get the intersect parameter for S1
    double sI = perpDot(v, w) / d;
    if (sI < 0) {
      return boost::none;
    }

    // get the intersect parameter for S2
    double tI = perpDot(u, w) / d;
    if (tI < 0) {
      return boost::none;
    }

    // I0 = S1_P0 + sI * u; 
    // compute S1 intersect point
    Point3d I0 = s1p0 + sI * u;
    return I0;

  }

  /// perp dot product between two vectors
  double Ray2d::perpDot(Vector3d& p1, Vector3d& p2) {
    return (p1.x() * p2.y() - p1.y() - p2.x());
  }

  bool Ray2d::inCollinearRay(Point3d& p) {
    // test if p is on ray
    Vector3d collideVector = p - point;
    
    double dot = vector.dot(collideVector);
    if (dot < 0) {
      return false;
    }
    return true;
  }

  // SPLIT CANDIDATE

  SplitCandidate::SplitCandidate(Point3d& aPoint, double aDistance, boost::optional<Edge&> aOppositeEdge, boost::optional<Point3d&> aOppositePoint) {
    point = aPoint;
    distance = aDistance;
    oppositeEdge = aOppositeEdge;
    oppositePoint = aOppositePoint;
  }

  // Returns true if this SplitCandidate is equal to other
  bool SplitCandidate::operator==(const SplitCandidate& other) const {

    // FIXME implement
    return false;

  }


  // LINE LINEAR

  LineLinear2d::LineLinear2d(double a, double b, double c) {
    A = a;
    B = b;
    C = c;
  }

  LineLinear2d::LineLinear2d(Point3d& p1, Point3d& p2) {
    A = p1.y() - p2.y();
    B = p2.x() - p1.x();
    C = p1.x() * p2.y() - p2.x() * p1.y();
  }

  boost::optional<Point3d> LineLinear2d::collide(LineLinear2d& line2) {
    double WAB = A*line2.B - line2.A*B;
    double WBC = B*line2.C - line2.B*C;
    double WCA = C*line2.A - line2.C*A;

    if (WAB == 0) {
      return boost::none;
    }

    return Point3d(WBC / WAB, WCA / WAB, 0);
  }

  // CHAIN
  //Chain::Chain(Event& aSplitEvent) {
  //  chainType = CHAIN_TYPE_SPLIT;
  //  splitEvent = aSplitEvent;
  //}

  //Chain::Chain(std::vector<Event>& aEdgeList) {
  //  chainType = CHAIN_TYPE_EDGE;
  //  edgeList = aEdgeList;
  //}

  Chain::Chain(Edge& aOppositeEdge, Vertex& aNextVertex) {
    chainType = CHAIN_TYPE_SINGLE_EDGE;
    oppositeEdge = aOppositeEdge;
    nextVertex = aNextVertex;
  }

  ChainMode Chain::getChainMode() {
    if (chainType == CHAIN_TYPE_EDGE) {
      if (closed && split) {
        // FIXME LOG_AND_THROW("Chain can't be closed and split");
      } else if (closed) {
        return CHAIN_MODE_CLOSED_EDGE;
      } else if (split) {
        return CHAIN_MODE_SPLIT;
      }
      return CHAIN_MODE_EDGE;
    } else if (chainType == CHAIN_TYPE_SPLIT) {
      return CHAIN_MODE_SPLIT;
    }
    return CHAIN_MODE_SPLIT;
  }


}
