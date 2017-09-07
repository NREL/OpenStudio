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

#ifndef UTILITIES_GEOMETRY_ROOFGEOMETRY_HPP
#define UTILITIES_GEOMETRY_ROOFGEOMETRY_HPP

#include "../UtilitiesAPI.hpp"
#include "../core/Logger.hpp"
#include "Point3d.hpp"
#include "Vector3d.hpp"
#include <boost/optional.hpp>
#include <queue>

namespace openstudio{

  const double SPLIT_EPSILON = 1E-10;

  // LINE LINEAR

  /// Geometry line in linear form. General form: Ax + By + C = 0;
  class LineLinear2d
  {
  public:
    LineLinear2d();
    LineLinear2d(double a, double b, double c);
    LineLinear2d(Point3d& p1, Point3d& p2);
    boost::optional<Point3d> collide(LineLinear2d& line2);
    double A = 0.0;
    double B = 0.0;
    double C = 0.0;
  private:
    REGISTER_LOGGER("utilities.LineLinear2d");
  };

  // RAY2DS

  class Ray2d
  {
  public:
    Ray2d();
    Ray2d(Point3d& aPoint, Vector3d& aVector);
    boost::optional<Point3d> collide(LineLinear2d& line, double epsilon);
    LineLinear2d getLinearForm();
    bool isOnLeftSide(Point3d& pt, double epsilon);
    bool isOnRightSide(Point3d& pt, double epsilon);
    boost::optional<Point3d> intersectRay2d(Ray2d& other);
    bool inCollinearRay(Point3d& p);
    double perpDot(Vector3d& p1, Vector3d& p2);
    friend std::ostream& operator<<(std::ostream& os, const Ray2d& ray2d);
    Point3d point;
    Vector3d vector;
  private:
    REGISTER_LOGGER("utilities.Ray2d");
  };

  // EDGES

  class Edge
  {
  public:
    Edge();
    Edge(Point3d& aBegin, Point3d& aEnd);
    int getOffsetEdgeIndex(std::vector<Edge>& edges, int offset);
    friend std::ostream& operator<<(std::ostream& os, const Edge& edge);
    bool operator<(const Edge& other) const;
    bool operator==(const Edge& other) const;
    Point3d begin;
    Point3d end;
    Ray2d bisectorPrevious;
    Ray2d bisectorNext;
    //LineLinear2d lineLinear2d;
    Vector3d normalize();
  private:
    REGISTER_LOGGER("utilities.Edge");
  };

  // FACES

  //class FaceNode
  //{
  //public:
  //  FaceNode();
  //private:
  //  REGISTER_LOGGER("utilities.FaceNode");
  //};

  //class FaceQueue : public std::queue<FaceNode>
  //{
  //public:
  //  Edge edge; // Edge for given queue.
  //  bool closed = false; //Flag if queue is closed. After closing can't be modified.
  //};

  // VERTEXES

  class Vertex
  {
  public:
    Vertex();
    Vertex(Point3d& aPoint, double aDistance, boost::optional<Ray2d&> aBisector, boost::optional<Edge&> aPreviousEdge, boost::optional<Edge&> aNextEdge);
    int getOffsetVertexIndex(std::vector<Vertex>& vertexes, int offset);
    int getLavIndex(std::vector< std::vector<Vertex> >& sLav);
    std::vector<Vertex> cutLavPart(std::vector<Vertex>& lav, Vertex& endVertex);
    friend std::ostream& operator<<(std::ostream& os, const Vertex& vertex);
    bool operator<(const Vertex& other) const;
    bool operator==(const Vertex& other) const;
    Point3d point;
    boost::optional<Edge> previousEdge;
    boost::optional<Edge> nextEdge;
    boost::optional<Ray2d> bisector;
    double distance = 0.0;
    bool processed = false;
    //FIXME: FaceNode leftFace;
    //FIXME: FaceNode rightFace;
  private:
    REGISTER_LOGGER("utilities.Vertex");
  };

  // QUEUE EVENTS

  enum QueueEventType
  {
    QUEUE_EVENT_EDGE = 0, QUEUE_EVENT_SPLIT = 1, QUEUE_EVENT_SPLIT_VERTEX = 2,
  };

  class QueueEvent
  {
  public:
    QueueEvent();
    QueueEvent(Point3d& aPoint, double aDistance, Vertex& aPreviousVertex, Vertex& aNextVertex); // EdgeEvent
    QueueEvent(Point3d& aPoint, double aDistance, Vertex& aParent); // VertexSplitEvent
    QueueEvent(Point3d& aPoint, double aDistance, Vertex& aParent, Edge& aOppositeEdge); // SplitEvent
    void addEventToGroup(std::vector<Vertex>& parentGroup);
    bool isEventInGroup(std::vector<Vertex>& parentGroup);
    friend std::ostream& operator<<(std::ostream& os, const QueueEvent& event);
    bool operator<(const QueueEvent& other) const;
    Edge getOppositeEdgePrevious();
    Edge getOppositeEdgeNext();
    Vertex previousVertex;
    Vertex nextVertex;
    Vertex parent;
    Edge oppositeEdge;
    Point3d point;
    QueueEventType eventType;
    double distance = 0.0;
    bool isObsolete();
  private:
    REGISTER_LOGGER("utilities.QueueEvent");
    bool obsolete = false;
  };


  // CHAINS (of queue events)

  enum ChainType
  {
    CHAIN_TYPE_EDGE = 0, CHAIN_TYPE_SPLIT = 1, CHAIN_TYPE_SINGLE_EDGE = 2
  };
  enum ChainMode
  {
    CHAIN_MODE_EDGE = 0, CHAIN_MODE_SPLIT = 1, CHAIN_MODE_CLOSED_EDGE = 2
  };

  class Chain
  {
  public:
    Chain();
    Chain(QueueEvent& aSplitEvent); // SplitChain
    Chain(std::vector<QueueEvent>& aEdgeList); // EdgeChain
    Chain(Edge& aOppositeEdge, Vertex& aNextVertex); // SingleEdgeChain
    ChainMode getChainMode();
    Edge getPreviousEdge();
    Edge getNextEdge();
    Vertex getPreviousVertex();
    Vertex getNextVertex();
    boost::optional<Vertex> getCurrentVertex();
    boost::optional<Edge> getOppositeEdge();
    std::vector<QueueEvent> edgeList;
    QueueEvent splitEvent;
    ChainType chainType;
    bool closed = false;
    bool split = false;
  private:
    REGISTER_LOGGER("utilities.Chain");
    Edge previousEdge;
    Edge nextEdge;
    Vertex previousVertex;
    Vertex nextVertex;
    boost::optional<Vertex> currentVertex;
    boost::optional<Edge> oppositeEdge;
  };


  // LEVEL EVENTS

  enum LevelEventType
  {
    LEVEL_EVENT_PICK = 0, LEVEL_EVENT_MULTI_EDGE = 1, LEVEL_EVENT_MULTI_SPLIT = 2
  };

  class LevelEvent
  {
  public:
    LevelEvent();
    LevelEvent(Point3d& aPoint, double aDistance, Chain& aChain, bool isPickEvent); // PickEvent or MultiEdgeEvent
    LevelEvent(Point3d& aPoint, double aDistance, std::vector<Chain>& aChains); // MultiSplitEvent
    friend std::ostream& operator<<(std::ostream& os, const LevelEvent& event);
    bool operator<(const LevelEvent& other) const;
    Chain chain;
    std::vector<Chain> chains;
    Point3d point;
    LevelEventType eventType;
    double distance = 0.0;
    bool obsolete = false;
  private:
    REGISTER_LOGGER("utilities.LevelEvent");
  };

  // SPLIT CANDIDATES

  class SplitCandidate
  {
  public:
    SplitCandidate();
    SplitCandidate(Point3d& aPoint, double aDistance, boost::optional<Edge&> aOppositeEdge, boost::optional<Point3d&> aOppositePoint);
    bool operator==(const SplitCandidate& other) const;
    Point3d point;
    double distance = 0.0;
    boost::optional<Edge> oppositeEdge;
    boost::optional<Point3d> oppositePoint;
  private:
    REGISTER_LOGGER("utilities.SplitCandidate");
  };

  /** RoofGeometry has methods for generating surfaces for simple roof types from polygon footprints.
  */
  class UTILITIES_API RoofGeometry
  {
  public:

    // default constructor creates empty RoofGeometry
    RoofGeometry();

    std::vector< std::vector<Point3d> > makeShedRoof(std::vector<Point3d>& polygon, double roofPitchDegrees, double directionDegrees);
    std::vector< std::vector<Point3d> > makeGableRoof(std::vector<Point3d>& polygon, double roofPitchDegrees);
    std::vector< std::vector<Point3d> > makeHipRoof(std::vector<Point3d>& polygon, double roofPitchDegrees);

    // FIXME move somewhere else
    static Vector3d orthogonalLeft(Vector3d& v);
    static Vector3d orthogonalRight(Vector3d& v);

  private:

    REGISTER_LOGGER("utilities.RoofGeometry");

    std::vector< std::vector<Point3d> > doStraightSkeleton(std::vector<Point3d>& polygon);

    // init
    void initSlav(std::vector<Point3d>& polygon, std::vector< std::vector<Vertex> >& sLav, std::vector<Edge>& edges); // FIXME: , std::vector<FaceQueue>& faces);
    void initEvents(std::vector< std::vector<Vertex> >& sLav, std::vector<QueueEvent>& queue, std::vector<Edge>& edges);
    void initPolygon(std::vector<Point3d>& polygon);

    // math
    void makeCounterClockwise(std::vector<Point3d>& polygon);
    bool isClockwisePolygon(std::vector<Point3d>& polygon);
    double area(std::vector<Point3d>& polygon);
    double calcDistance(Point3d& intersect, Edge& currentEdge);
    Ray2d calcBisector(Point3d& p, Edge& e1, Edge& e2);
    Vector3d calcVectorBisector(Vector3d& norm1, Vector3d& norm2);
    void correctBisectorDirection(Ray2d& bisector, Vertex& beginNextVertex, Vertex& endPreviousVertex, Edge& beginEdge, Edge& endEdge);
    boost::optional<Point3d> computeIntersectionBisectors(Vertex& vertexPrevious, Vertex& vertexNext);
    Vector3d orthogonalProjection(Vector3d& unitVector, Vector3d& vectorToProject);
    bool edgeBehindBisector(Ray2d& bisector, LineLinear2d& edge);
    boost::optional<Edge> chooseLessParallelVertexEdge(Vertex& vertex, Edge& edge);
    bool isInsidePolygon(Point3d point, std::vector<Point3d> points);

    // convergence
    int assertMaxNumberOfInteraction(int count);

    // events
    std::vector<LevelEvent> loadAndGroupLevelEvents(std::vector<QueueEvent>& queue);
    std::vector<QueueEvent> loadLevelEvents(std::vector<QueueEvent>& queue);
    std::vector<LevelEvent> groupLevelEvents(std::vector<QueueEvent>& levelEvents);
    LevelEvent createLevelEvent(Point3d& eventCenter, double distance, std::vector<QueueEvent>& eventCluster);
    QueueEvent createEdgeEvent(Point3d& point, Vertex& previousVertex, Vertex& nextVertex);
    void multiSplitEvent(LevelEvent& event, std::vector< std::vector<Vertex> >& sLav, std::vector<QueueEvent>& queue, std::vector<Edge>& edges);
    void pickEvent(LevelEvent& event, std::vector< std::vector<Vertex> >& sLav, std::vector<QueueEvent>& queue, std::vector<Edge>& edges);
    void multiEdgeEvent(LevelEvent& event, std::vector< std::vector<Vertex> >& sLav, std::vector<QueueEvent>& queue, std::vector<Edge>& edges);
    void computeEvents(Vertex& vertex, std::vector<QueueEvent>& queue, std::vector<Edge>& edges, std::vector< std::vector<Vertex> >& sLav);
    void computeEdgeEvents(Vertex& previousVertex, Vertex& nextVertex, std::vector<QueueEvent>& queue);
    void computeSplitEvents(Vertex& vertex, std::vector<Edge>& edges, std::vector<QueueEvent>& queue, boost::optional<double> distanceSquared);
    boost::optional<double> computeCloserEdgeEvent(Vertex& vertex, std::vector<QueueEvent>& queue, std::vector< std::vector<Vertex> >& sLav);
    std::vector<SplitCandidate> calcOppositeEdges(Vertex& vertex, std::vector<Edge>& edges);
    boost::optional<SplitCandidate> calcCandidatePointForSplit(Vertex& vertex, Edge& edge);
    void removeEventsUnderHeight(std::vector<QueueEvent>& queue, double levelHeight);

    // chains
    std::vector<Chain> createChains(std::vector<QueueEvent>& cluster);
    std::vector<QueueEvent> createEdgeChain(std::vector<QueueEvent>& edgeCluster);
    bool isInEdgeChain(QueueEvent& split, Chain& chain);
    void createOppositeEdgeChains(std::vector< std::vector<Vertex> >& sLav, std::vector<Chain>& chains, Point3d& center);

    // faces
    void addMultiBackFaces(std::vector<QueueEvent>& edgeList, Vertex& edgeVertex);
    // FIXME: FaceNode addFaceBack(Vertex& newVertex, Vertex& va, Vertex& vb);
    // FIXME: FaceNode addFaceLeft(Vertex& newVertex, Vertex& va);
    // FIXME: FaceNode addFaceRight(Vertex& newVertex, Vertex& vb);
    // FIXME: std::tuple<bool, FaceNode> addSplitFaces(bool hasLastFaceNode, FaceNode& lastFaceNode, Chain& chainBegin, Chain& chainEnd, Vertex& newVertex);

    // vertexes/lavs
    Vertex createMultiSplitVertex(Edge& nextEdge, Edge& previousEdge, Point3d& center, double distance);
    Vertex createOppositeEdgeVertex(Vertex& newVertex);
    void mergeBeforeBaseVertex(Vertex& base, std::vector<Vertex>& baseList, Vertex& merged, std::vector<Vertex>& mergedList);
    void processTwoNodeLavs(std::vector< std::vector<Vertex> >& sLav);
    void removeEmptyLav(std::vector< std::vector<Vertex> >& sLav);
    boost::optional<Vertex> findOppositeEdgeLav(std::vector< std::vector<Vertex> > sLav, Edge oppositeEdge, Point3d center);
    std::vector<Vertex> findEdgeLavs(std::vector< std::vector<Vertex> > sLav, Edge oppositeEdge);
    boost::optional<Vertex> getEdgeInLav(std::vector<Vertex> lav, Edge oppositeEdge);
    boost::optional<Vertex> chooseOppositeEdgeLav(std::vector<Vertex> edgeLavs, Edge oppositeEdge, Point3d center);
    bool areSameLav(std::vector<Vertex>& lav1, std::vector<Vertex>& lav2);

    // FIXME: Add some private member variables so we dont have to pass around so much?
  };

} // openstudio

#endif //UTILITIES_GEOMETRY_BOUNDINGBOX_HPP
