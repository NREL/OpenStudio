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
#include "Vector3d.hpp"
#include <boost/optional.hpp>
#include <boost/math/constants/constants.hpp>

using namespace openstudio;

const double SPLIT_EPSILON = 1E-10;

class LineLinear2d // Geometry line in linear form. General form: Ax + By + C = 0;
{ 
public:
  double A = 0.0;
  double B = 0.0;
  double C = 0.0;

  LineLinear2d(double a, double b, double c) {
    A = a;
    B = b;
    C = c;
  }

  LineLinear2d(Point3d& p1, Point3d& p2) {
    A = p1.y() - p2.y();
    B = p2.x() - p1.x();
    C = p1.x() * p2.y() - p2.x() * p1.y();
  }

  boost::optional<Point3d> collide(LineLinear2d& line2) {
    double WAB = A*line2.B - line2.A*B;
    double WBC = B*line2.C - line2.B*C;
    double WCA = C*line2.A - line2.C*A;

    if (WAB == 0) {
      return boost::none;
    }

    return Point3d(WBC / WAB, WCA / WAB, 0);
  }

private:
  REGISTER_LOGGER("utilities.LineLinear2d");
};

class Ray2d
{
public:
  Ray2d() {
    // nop
  }

  Point3d point;
  Vector3d vector;

  Ray2d(Point3d& aPoint, Vector3d& aVector) {
    point = aPoint;
    vector = aVector;
  }

  boost::optional<Point3d> collide(LineLinear2d& line, double epsilon) {
    LineLinear2d ll2d = getLinearForm();

    // rewrite?
    boost::optional<Point3d> collide = ll2d.collide(line);
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

  LineLinear2d getLinearForm() {
    double x = point.x();
    double y = point.y();

    double B = -vector.x();
    double A = vector.y();

    double C = -1 * (A * x + B * y);
    return LineLinear2d(A, B, C);
  }

  bool isOnLeftSide(Point3d& p, double epsilon) {
    Vector3d direction = p - point;
    Vector3d orthRight = vector.orthogonalRight();
    return (orthRight.dot(direction) < epsilon);
  }

  bool isOnRightSide(Point3d& p, double epsilon) {
    Vector3d direction = p - point;
    Vector3d orthRight = vector.orthogonalRight();
    return (orthRight.dot(direction) > -epsilon);
  }

  boost::optional<Point3d> intersectRay2d(Ray2d& other) {
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
  double perpDot(Vector3d& p1, Vector3d& p2) {
    return (p1.x() * p2.y() - p1.y() * p2.x());
  }

  bool inCollinearRay(Point3d& p) {
    // test if p is on ray
    Vector3d collideVector = p - point;

    double dot = vector.dot(collideVector);
    if (dot < 0) {
      return false;
    }
    return true;
  }

  friend std::ostream& operator<<(std::ostream& os, const Ray2d& r) {
    os << "Ray2d [A=" << r.point << ", U=" << r.vector << "]";
    return os;
  }

private:
  REGISTER_LOGGER("utilities.Ray2d");
};

class Edge
{
public:
  Point3d begin;
  Point3d end;
  Ray2d bisectorPrevious;
  Ray2d bisectorNext;

  Edge() {
    // nop
  }

  Edge(Point3d& aBegin, Point3d& aEnd) {
    begin = aBegin;
    end = aEnd;
  }

  Vector3d normalize() {
    Vector3d v = Vector3d(end.x(), end.y(), end.z()) - Vector3d(begin.x(), begin.y(), begin.z());
    v.normalize();
    return v;
  }

  int getOffsetEdgeIndex(std::vector<Edge>& edges, int offset) {
    auto it = std::find(edges.begin(), edges.end(), *this);
    if (it == edges.end()) {
      LOG_AND_THROW("Could not find edge in edges.");
    }
    int pos = std::distance(edges.begin(), it);
    pos += offset;
    if (pos < 0) {
      pos += edges.size();
    } else if (pos > edges.size() - 1) {
      pos -= edges.size();
    }
    return pos;
  }

  friend std::ostream& operator<<(std::ostream& os, const Edge& e) {
    os << "EdgeEntry [p1=" << e.begin << ", p2=" << e.end << "]";
    return os;
  }

  // Returns true if this edge is less than other
  bool operator<(const Edge& other) const {
    LOG_AND_THROW("FIXME");
    // FIXME implement
    return false;

  }

  // Returns true if this edge is equal to other
  bool operator==(const Edge& other) const {
    if (begin == other.begin && end == other.end) {
      return true;
    }
    return false;

  }

private:
  REGISTER_LOGGER("utilities.Edge");
};

class Vertex
{
public:
  Point3d point;
  boost::optional<Edge> previousEdge;
  boost::optional<Edge> nextEdge;
  boost::optional<Ray2d> bisector;
  double distance = 0.0;
  bool processed = false;

  Vertex() {
    // nop
  }

  Vertex(Point3d& aPoint, double aDistance, boost::optional<Ray2d&> aBisector, boost::optional<Edge&> aPreviousEdge, boost::optional<Edge&> aNextEdge) {
    point = aPoint;
    distance = aDistance;
    bisector = aBisector;
    previousEdge = aPreviousEdge;
    nextEdge = aNextEdge;
    processed = false;
  }

  int getOffsetVertexIndex(std::vector<Vertex>& vertexes, int offset) {
    auto it = std::find(vertexes.begin(), vertexes.end(), *this);
    if (it == vertexes.end()) {
      LOG_AND_THROW("Could not find edge in edges.");
    }
    int pos = std::distance(vertexes.begin(), it);
    pos += offset;
    if (pos < 0) {
      pos += vertexes.size();
    } else if (pos > vertexes.size() - 1) {
      pos -= vertexes.size();
    }
    return pos;
  }

  int getLavIndex(std::vector< std::vector<Vertex> >& sLav) {
    for (int i = 0; i < sLav.size(); i++) {
      if (std::find(sLav[i].begin(), sLav[i].end(), *this) != sLav[i].end()) {
        return i;
      }
    }
    return -1;
  }

  void removeFromLav(std::vector< std::vector<Vertex> >& sLav) {
    int i = getLavIndex(sLav);
    if (i != -1) {
      removeFromLav(sLav[i]);
    }
  }

  void removeFromLav(std::vector<Vertex>& lav) {
    auto it = std::find(lav.begin(), lav.end(), *this);
    if (it != lav.end()) {
      lav.erase(it);
    }
  }

  // Returns cut portion of lav from this vertex to endVertex, including both
  std::vector<Vertex> cutLavPart(std::vector<Vertex>& lav, Vertex& endVertex) {
    std::vector<Vertex> ret;

    int size = lav.size();
    LOG_AND_THROW("FIXME");
    // FIXME implement
    return ret;
  }

  friend std::ostream& operator<<(std::ostream& os, const Vertex& v) {
    os << "VertexEntry [v=" << v.point << ", processed=" << v.processed << ", bisector=" << v.bisector << ", previousEdge=" << v.previousEdge << ", nextEdge=" << v.nextEdge << ", distance=" << v.distance << "]";
    return os;
  }

  // Returns true if this vertex is less than other
  bool operator<(const Vertex& other) const {
    LOG_AND_THROW("FIXME");
    // FIXME implement
    return false;

  }

  // Returns true if this vertex is equal to other
  bool operator==(const Vertex& other) const {

    if (point == other.point && distance == other.distance) {
      return true;
    }
    return false;

  }

private:
  REGISTER_LOGGER("utilities.Vertex");
};

class Face
{
public:
  std::vector<Vertex> vertexes;
  
  Face::Face() {
    // nop
  }

private:
  REGISTER_LOGGER("utilities.Face");
};

class QueueEvent
{
public:
  enum QueueEventType
  {
    TYPE_EDGE = 0, TYPE_SPLIT = 1, TYPE_SPLIT_VERTEX = 2,
  };

  Vertex previousVertex;
  Vertex nextVertex;
  Vertex parent;
  Edge oppositeEdge;
  Point3d point;
  QueueEventType eventType;
  double distance = 0.0;

  QueueEvent() {
    // nop
  }

  QueueEvent(Point3d& aPoint, double aDistance, Vertex& aPreviousVertex, Vertex& aNextVertex) {
    eventType = TYPE_EDGE;
    point = aPoint;
    distance = aDistance;
    previousVertex = aPreviousVertex;
    nextVertex = aNextVertex;
  }

  QueueEvent(Point3d& aPoint, double aDistance, Vertex& aParent) {
    eventType = TYPE_SPLIT_VERTEX;
    point = aPoint;
    distance = aDistance;
    parent = aParent;
  }

  QueueEvent(Point3d& aPoint, double aDistance, Vertex& aParent, Edge& aOppositeEdge) {
    eventType = TYPE_SPLIT;
    point = aPoint;
    distance = aDistance;
    parent = aParent;
    oppositeEdge = aOppositeEdge;
  }

  friend std::ostream& operator<<(std::ostream& os, const QueueEvent& e) {
    if (e.eventType == TYPE_EDGE) {
      os << "EdgeEvent [v=" << e.point << ", previousVertex=" << e.previousVertex.point << ", nextVertex=" << e.nextVertex.point << ", distance=" << e.distance << "]";
    } else if (e.eventType == TYPE_SPLIT) {
      os << "SplitEvent [v=" << e.point << ", parent=" << e.parent.point << ", distance=" << e.distance << "]";
    } else if (e.eventType == TYPE_SPLIT_VERTEX) {
      os << "VertexSplitEvent [v=" << e.point << ", parent=" << e.parent.point << ", distance=" << e.distance << "]";
    }
    return os;
  }

  bool operator<(const QueueEvent& other) const {
    return distance < other.distance;
  }

  void addEventToGroup(std::vector<Vertex>& parentGroup) {
    //std::cout << "addEventToGroup size before " << parentGroup.size() << std::endl;
    if (eventType == TYPE_SPLIT || eventType == TYPE_SPLIT_VERTEX) {
      //std::cout << "addEventToGroup add parent " << parent << std::endl;
      parentGroup.push_back(parent);
    } else if (eventType == TYPE_EDGE) {
      //std::cout << "addEventToGroup add previousVertex " << previousVertex << std::endl;
      //std::cout << "addEventToGroup add nextVertex " << nextVertex << std::endl;
      parentGroup.push_back(previousVertex);
      parentGroup.push_back(nextVertex);
    }
    //std::cout << "addEventToGroup size after " << parentGroup.size() << std::endl;
  }

  bool isEventInGroup(std::vector<Vertex>& parentGroup) {
    //std::cout << "isEventInGroup event " << *this << std::endl;
    //for (Vertex v : parentGroup) {
    //  std::cout << "isEventInGroup parentGroupVertex " << v << std::endl;
    //}
    if (eventType == TYPE_SPLIT || eventType == TYPE_SPLIT_VERTEX) {
      bool foundParent = std::find(parentGroup.begin(), parentGroup.end(), parent) != parentGroup.end();
      //std::cout << "isEventInGroup SplitEvent " << foundParent << std::endl;
      return foundParent;
    } else if (eventType == TYPE_EDGE) {
      bool foundPreviousVertex = std::find(parentGroup.begin(), parentGroup.end(), previousVertex) != parentGroup.end();
      bool foundNextVertex = std::find(parentGroup.begin(), parentGroup.end(), nextVertex) != parentGroup.end();
      //std::cout << "isEventInGroup EdgeEvent " << (foundPreviousVertex || foundNextVertex) << std::endl;
      return (foundPreviousVertex || foundNextVertex);
    }
    //std::cout << "isEventInGroup other false" << std::endl;
    return false;
  }

  Edge& getOppositeEdgePrevious() {
    return oppositeEdge;
  }

  Edge& getOppositeEdgeNext() {
    LOG_AND_THROW("FIXME");
    return oppositeEdge; // FIXME: return oppositeEdge.next();
  }

  bool isObsolete() {
    if (eventType == TYPE_EDGE) {
      return (previousVertex.processed || nextVertex.processed);
    } else {
      return parent.processed;
    }
  }

private:
  REGISTER_LOGGER("utilities.QueueEvent");
  bool obsolete = false;
};

class Chain // Chains of queue events
{
public:
  enum ChainType
  {
    TYPE_EDGE = 0, TYPE_SPLIT = 1, TYPE_SINGLE_EDGE = 2
  };
  
  enum ChainMode
  {
    MODE_EDGE = 0, MODE_SPLIT = 1, MODE_CLOSED_EDGE = 2
  };

  std::vector<QueueEvent> edgeList;
  QueueEvent splitEvent;
  ChainType chainType;
  bool closed = false;
  bool split = false;

  Chain() {
    // nop
  }

  Chain(QueueEvent& aSplitEvent) {
    chainType = TYPE_SPLIT;
    splitEvent = aSplitEvent;
  }

  Chain(std::vector<QueueEvent>& aEdgeList) {
    chainType = TYPE_EDGE;
    edgeList = aEdgeList;
    if (getPreviousVertex() == getNextVertex()) {
      closed = true;
    }
  }

  Chain(Edge& aOppositeEdge, Vertex& aNextVertex) {
    chainType = TYPE_SINGLE_EDGE;
    oppositeEdge = aOppositeEdge;
    nextVertex = aNextVertex;
  }

  ChainMode getChainMode() {
    if (chainType == TYPE_EDGE) {
      if (closed && split) {
        LOG_AND_THROW("Chain can't be closed and split");
      } else if (closed) {
        return MODE_CLOSED_EDGE;
      } else if (split) {
        return MODE_SPLIT;
      }
      return MODE_EDGE;
    } else if (chainType == TYPE_SPLIT) {
      return MODE_SPLIT;
    }
    return MODE_SPLIT;
  }

  Edge& getPreviousEdge() {
    if (chainType == TYPE_EDGE) {
      return edgeList[0].previousVertex.previousEdge.get();
    } else if (chainType == TYPE_SINGLE_EDGE) {
      return oppositeEdge.get();
    } else {
      return splitEvent.parent.previousEdge.get();
    }
  }

  Edge& getNextEdge() {
    if (chainType == TYPE_EDGE) {
      return edgeList[edgeList.size() - 1].nextVertex.nextEdge.get();
    } else if (chainType == TYPE_SINGLE_EDGE) {
      return oppositeEdge.get();
    } else {
      return splitEvent.parent.nextEdge.get();
    }
  }

  Vertex& getPreviousVertex() {
    if (chainType == TYPE_EDGE) {
      return edgeList[0].previousVertex;
    } else if (chainType == TYPE_SINGLE_EDGE) {
      return previousVertex;
    } else {
      LOG_AND_THROW("FIXME");
      return splitEvent.parent; // FIXME: return splitEvent.getParent().previous();
    }
  }

  Vertex& getNextVertex() {
    if (chainType == TYPE_EDGE) {
      return edgeList[edgeList.size() - 1].nextVertex;
    } else if (chainType == TYPE_SINGLE_EDGE) {
      return nextVertex;
    } else {
      LOG_AND_THROW("FIXME");
      return splitEvent.parent; // FIXME: return splitEvent.getParent().next();
    }
  }

  boost::optional<Vertex&> getCurrentVertex() {
    boost::optional<Vertex&> ret;
    if (chainType == TYPE_SPLIT) {
      return splitEvent.parent;
    }
    return ret;
  }

  boost::optional<Edge&> getOppositeEdge() {
    if (chainType == TYPE_SPLIT) {
      if (splitEvent.eventType != QueueEvent::TYPE_SPLIT_VERTEX) {
        return splitEvent.oppositeEdge;
      }
      boost::optional<Edge&> null;
      return null;
    }
    return oppositeEdge;
  }

private:
  REGISTER_LOGGER("utilities.Chain");
  Edge previousEdge;
  Edge nextEdge;
  Vertex previousVertex;
  Vertex nextVertex;
  boost::optional<Vertex> currentVertex;
  boost::optional<Edge&> oppositeEdge;
};

class LevelEvent
{
public:
  enum LevelEventType
  {
    TYPE_PICK = 0, TYPE_MULTI_EDGE = 1, TYPE_MULTI_SPLIT = 2
  };

  Chain chain;
  std::vector<Chain> chains;
  Point3d point;
  LevelEventType eventType;
  double distance = 0.0;
  bool obsolete = false;

  LevelEvent() {
    // nop
  }

  LevelEvent(Point3d& aPoint, double aDistance, Chain& aChain, bool isPickEvent) {
    if (isPickEvent) {
      eventType = TYPE_PICK;
    } else {
      eventType = TYPE_MULTI_EDGE;
    }
    point = aPoint;
    distance = aDistance;
    chain = aChain;
  }

  LevelEvent(Point3d& aPoint, double aDistance, std::vector<Chain>& aChains) {
    eventType = TYPE_MULTI_SPLIT;
    point = aPoint;
    distance = aDistance;
    chains = aChains;
  }

  friend std::ostream& operator<<(std::ostream& os, const LevelEvent& e) {
    os << "IntersectEntry [v=" << e.point << ", distance=" << e.distance << "]";
    return os;
  }

  bool operator<(const LevelEvent& other) const {
    return distance < other.distance;
  }

private:
  REGISTER_LOGGER("utilities.LevelEvent");
};

class SplitCandidate
{
public:
  Point3d point;
  double distance = 0.0;
  boost::optional<Edge> oppositeEdge;
  boost::optional<Point3d> oppositePoint;

  SplitCandidate(Point3d& aPoint, double aDistance, boost::optional<Edge&> aOppositeEdge, boost::optional<Point3d&> aOppositePoint) {
    point = aPoint;
    distance = aDistance;
    oppositeEdge = aOppositeEdge;
    oppositePoint = aOppositePoint;
  }

  // Returns true if this SplitCandidate is less than other
  bool operator<(const SplitCandidate& other) const {
    return (distance < other.distance);
  }

  // Returns true if this SplitCandidate is equal to other
  bool operator==(const SplitCandidate& other) const {
    return (point == other.point && distance == other.distance);
  }

private:
  REGISTER_LOGGER("utilities.SplitCandidate");
};

void debugFaces(std::string caller, std::vector<Face> faces) {
  return;
  std::cout << caller << std::endl;
  for (Face f : faces) {
    std::cout << "FACE: ";
    for (Vertex v : f.vertexes) {
      std::cout << v.point << ",";
    }
    std::cout << std::endl;
  }
}

void debugSlav(std::string caller, std::vector< std::vector<Vertex> > sLav) {
  return;
  std::cout << caller << std::endl;
  for (std::vector<Vertex> lav : sLav) {
    for (Vertex v : lav) {
      std::cout << "lav v " << v << std::endl;
    }
  }
}

void debugQueue(std::string caller, std::vector<QueueEvent> queue) {
  return;
  std::cout << caller << std::endl;
  for (QueueEvent q : queue) {
    std::cout << "queue event " << q << std::endl;
  }
}

// TODO: Shouldn't need this method eventually; shouldn't have separate vertex objects
void setProcessed(Vertex& v, std::vector< std::vector<Vertex> >& sLav, std::vector<QueueEvent>& queue) {
  v.processed = true;

  int lavIndex = v.getLavIndex(sLav);
  if (lavIndex > -1) {
    auto it = std::find(sLav[lavIndex].begin(), sLav[lavIndex].end(), v);
    int pos = std::distance(sLav[lavIndex].begin(), it);
    sLav[lavIndex][pos].processed = true;
  }

  for (QueueEvent& e : queue) {
    if (e.previousVertex == v) {
      e.previousVertex.processed = true;
    }
    if (e.nextVertex == v) {
      e.nextVertex.processed = true;
    }
    if (e.parent == v) {
      e.parent.processed = true;
    }
  }
}

std::vector< std::vector<Point3d> > facesToRoofs(std::vector<Face>& faces, double roofPitchDegrees) {

  double roofSlope = tan(degToRad(roofPitchDegrees));

  std::vector< std::vector<Point3d> > roofs;
  for (Face face : faces) {
    std::vector<Point3d> roof;
    for (Vertex v : face.vertexes) {
      Point3d p = Point3d(v.point.x(), v.point.y(), v.distance * roofSlope);
      roof.push_back(p);
    }
    roofs.push_back(roof);
  }
  return roofs;
}

Vector3d calcVectorBisector(Vector3d& norm1, Vector3d& norm2) {
  Vector3d e1v = norm1.orthogonalLeft();
  Vector3d e2v = norm2.orthogonalLeft();

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

Ray2d calcBisector(Point3d& p, Edge& e1, Edge& e2) {
  Vector3d norm1 = e1.normalize();
  Vector3d norm2 = e2.normalize();
  Vector3d bisector = calcVectorBisector(norm1, norm2);
  return Ray2d(p, bisector);
}

void initSlav(std::vector<Point3d>& polygon, std::vector< std::vector<Vertex> >& sLav, std::vector<Edge>& edges, std::vector<Face>& faces)
{
  std::vector<Edge> edgesList;

  int size = polygon.size();
  for (int i = 0; i < size; i++) {
    int j = (i + 1) % size;
    edgesList.push_back(Edge(polygon[i], polygon[j]));
  }

  for (Edge& edge : edgesList) {
    int i = edge.getOffsetEdgeIndex(edgesList, 1);
    Edge& nextEdge = edgesList[i];
      
    Ray2d bisector = calcBisector(edge.end, edge, nextEdge);

    edge.bisectorNext = bisector;
    nextEdge.bisectorPrevious = bisector;

    edges.push_back(edge);
  }

  std::vector<Vertex> lav;

  for (Edge& edge : edgesList) {
    int i = edge.getOffsetEdgeIndex(edgesList, 1);
    Edge& nextEdge = edgesList[i];

    Vertex vertex = Vertex(edge.end, 0, edge.bisectorNext, edge, nextEdge);

    lav.push_back(vertex);
  }
  sLav.push_back(lav);

  for (int i = 0; i < lav.size(); i++) {
    int j = lav[i].getOffsetVertexIndex(lav, 1);

    Face face;
    face.vertexes.push_back(lav[i]);
    face.vertexes.push_back(lav[j]);
    faces.push_back(face);
  }

  debugFaces("initSlav", faces);
}

bool edgeBehindBisector(Ray2d& bisector, LineLinear2d& edge) {
  /*
  * Simple intersection test between the bisector starting at V and the
  * whole line containing the currently tested line segment ei rejects
  * the line segments laying "behind" the vertex V
  */
  return (!bisector.collide(edge, SPLIT_EPSILON));
}

boost::optional<Edge> chooseLessParallelVertexEdge(Vertex& vertex, Edge& edge) {
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

// see http://en.wikipedia.org/wiki/Vector_projection
Vector3d orthogonalProjection(Vector3d& unitVector, Vector3d& vectorToProject) {
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

double calcDistance(Point3d& intersect, Edge& currentEdge) {
  // TODO: Can be replaced by getDistancePointToLineSegment() ?
  Vector3d edge = currentEdge.end - currentEdge.begin;
  Vector3d vector = intersect - currentEdge.begin;

  Vector3d pointOnVector = orthogonalProjection(edge, vector);

  return getDistance(Point3d(vector.x(), vector.y(), vector.z()), Point3d(pointOnVector.x(), pointOnVector.y(), pointOnVector.z()));
}

boost::optional<SplitCandidate> calcCandidatePointForSplit(Vertex& vertex, Edge& edge) {

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

std::vector<SplitCandidate> calcOppositeEdges(Vertex& vertex, std::vector<Edge>& edges) {

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

  if (ret.size() > 1) {
    std::sort(ret.begin(), ret.end());
  }

  return ret;
}

void computeSplitEvents(Vertex& vertex, std::vector<Edge>& edges, std::vector<QueueEvent>& queue, boost::optional<double> distanceSquared) {
  Point3d source = vertex.point;
  //std::cout << "computeSplitEvents begin" << std::endl;

  std::vector<SplitCandidate> oppositeEdges = calcOppositeEdges(vertex, edges);

  // check if it is vertex split event
  for (SplitCandidate oppositeEdge : oppositeEdges) {

    if (distanceSquared) {
      if (getDistanceSquared(source, oppositeEdge.point) > distanceSquared.get() + SPLIT_EPSILON) {
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
      QueueEvent e1 = QueueEvent(oppositeEdge.point, oppositeEdge.distance, vertex);
      //std::cout << "computeSplitEvents queue add " << e1 << std::endl;
      queue.push_back(e1);
      continue;
    }

    QueueEvent e2 = QueueEvent(oppositeEdge.point, oppositeEdge.distance, vertex, oppositeEdge.oppositeEdge.get());
    //std::cout << "computeSplitEvents queue add " << e2 << std::endl;
    queue.push_back(e2);
    continue;

  }
}

boost::optional<Point3d> computeIntersectionBisectors(Vertex& vertexPrevious, Vertex& vertexNext) {
  boost::optional<Point3d> ret;

  Ray2d bisectorPrevious = vertexPrevious.bisector.get();
  Ray2d bisectorNext = vertexNext.bisector.get();

  boost::optional<Point3d> intersect = bisectorPrevious.intersectRay2d(bisectorNext);

  if (!intersect) {
    return ret;
  }

  if (vertexPrevious.point == intersect.get() || vertexNext.point == intersect.get()) {
    // skip the same points
    return ret;
  }

  ret = intersect.get();
  return ret;
}

QueueEvent createEdgeEvent(Point3d& point, Vertex& previousVertex, Vertex& nextVertex) {
  return QueueEvent(point, calcDistance(point, previousVertex.nextEdge.get()), previousVertex, nextVertex);
}

void computeEdgeEvents(Vertex& previousVertex, Vertex& nextVertex, std::vector<QueueEvent>& queue) {
  boost::optional<Point3d> point = computeIntersectionBisectors(previousVertex, nextVertex);
  if (point) {
    QueueEvent e = createEdgeEvent(point.get(), previousVertex, nextVertex);
    //std::cout << "computeEdgeEvents queue add " << e << std::endl;
    queue.push_back(e);
  }
}

void initEvents(std::vector< std::vector<Vertex> >& sLav, std::vector<QueueEvent>& queue, std::vector<Edge>& edges)
{
  for (int i = 0; i < sLav.size(); i++) {
    for (int j = 0; j < sLav[i].size(); j++) {
      computeSplitEvents(sLav[i][j], edges, queue, boost::none);
    }
  }

  for (int i = 0; i < sLav.size(); i++) {
    for (int j = 0; j < sLav[i].size(); j++) {
      int k = sLav[i][j].getOffsetVertexIndex(sLav[i], 1);
      computeEdgeEvents(sLav[i][j], sLav[i][k], queue);
    }
  }

}

// Calculate area of polygon outline. For clockwise area will be less than
// zero, for counter-clockwise polygon area will be greater than zero.
double area(std::vector<Point3d>& polygon) {
  int n = polygon.size();
  double a = 0.0;
  for (int p = n - 1, q = 0; q < n; p = q++) {
    Point3d pp = polygon[p];
    Point3d pq = polygon[q];
    a += pp.x() * pq.y() - pq.x() * pp.y();
  }
  return a * 0.5;
}

// Check if polygon is clockwise.
bool isClockwisePolygon(std::vector<Point3d>& polygon) {
  return (area(polygon) < 0);
}

// Updates points ordered as counter clockwise.
void makeCounterClockwise(std::vector<Point3d>& polygon) {
  if (isClockwisePolygon(polygon)) {
    polygon = reverse(polygon);
  }
}

void initPolygon(std::vector<Point3d>& polygon)
{
  if (polygon.size() < 3) {
    LOG_AND_THROW("Polygon must have at least 3 points");
  } else if (polygon[0] == polygon[polygon.size() - 1]) {
    LOG_AND_THROW("Polygon can't start and end with the same point");
  }

  makeCounterClockwise(polygon);
}

int assertMaxNumberOfIterations(int count)
{
  count++;
  if (count > 10000) {
    LOG_AND_THROW("Maximum number of iterations reached. Bug?");
  }
  return count;
}

std::vector<QueueEvent> loadLevelEvents(std::vector<QueueEvent>& queue) {
  /*
  * Loads all non obsolete events which are on one level.
  */

  std::vector<QueueEvent> level;

  //for (QueueEvent e : queue) {
  //  std::cout << "loadLevelEvents queue event " << e << std::endl;
  //}

  QueueEvent& levelStart = queue[0];
  levelStart = queue[0];
  while (queue.size() > 0 && levelStart.isObsolete()) {
    levelStart = queue[0];
    queue.erase(queue.begin());
    // skip all obsolete events in level
  }

  if (levelStart.isObsolete()) {
    // all events obsolete
    return level;
  }

  double levelStartHeight = levelStart.distance;

  level.push_back(levelStart);

  QueueEvent& event = queue[0];
  queue.erase(queue.begin());

  while (queue.size() > 0 && event.distance - levelStartHeight < SPLIT_EPSILON) {
    if (!event.isObsolete()) {
      level.push_back(event);
    }
    if (queue.size() == 0) {
      break;
    }
    event = queue[0];
    queue.erase(queue.begin());
  }

  //std::cout << "loadLevelEvents queue size " << queue.size() << std::endl;
  //std::cout << "loadLevelEvents level size " << level.size() << std::endl;
  //for (QueueEvent e : level) {
  //  std::cout << "loadLevelEvents level " << e << std::endl;
  //}

  return level;
}

std::vector<QueueEvent> createEdgeChain(std::vector<QueueEvent>& edgeCluster) {
  std::vector<QueueEvent> edgeList;

  edgeList.push_back(edgeCluster[0]);
  edgeCluster.erase(edgeCluster.begin());

  // find all successors of edge event
  // find all predecessors of edge event

  do {
    Vertex& beginVertex = edgeList[0].previousVertex;
    Vertex& endVertex = edgeList[edgeList.size() - 1].nextVertex;

    bool do_continue = false;
    for (int i = 0; i < edgeCluster.size(); i++) {
      if (edgeCluster[i].previousVertex == endVertex) {
        // edge should be added as last in chain
        edgeList.push_back(edgeCluster[i]);
        edgeCluster.erase(edgeCluster.begin() + i);
        do_continue = true;
        break;
      } else if (edgeCluster[i].nextVertex == beginVertex) {
        // edge should be added as first in chain
        edgeList.insert(edgeCluster.begin(), edgeCluster[i]);
        edgeCluster.erase(edgeCluster.begin() + i);
        do_continue = true;
        break;
      }
    }

    if (do_continue) {
      continue;
    }

    break;

  } while (true);

  debugQueue("createEdgeChain", edgeList);

  return edgeList;
}

bool isInEdgeChain(QueueEvent& split, Chain& chain) {
  for (QueueEvent& edgeEvent : chain.edgeList) {
    if (edgeEvent.previousVertex == split.parent || edgeEvent.nextVertex == split.parent) {
      return true;
    }
  }

  return false;
}

std::vector<Chain> createChains(std::vector<QueueEvent>& cluster) {
  /*
  * Create chains of events from cluster. Cluster is set of events which meet
  * in the same result point. Try to connect all events which share the same
  * vertex into chain. Events in a chain are sorted. If events don't share a
  * vertex, returned chains contains only one event.
  */

  std::vector<QueueEvent> edgeCluster;
  std::vector<QueueEvent> splitCluster;
  std::vector<Vertex> vertexEventsParents;

  for (QueueEvent& event : cluster) {
    if (event.eventType == QueueEvent::TYPE_EDGE) {
      edgeCluster.push_back(event);
    } else {
      if (event.eventType == QueueEvent::TYPE_SPLIT_VERTEX) {
        /*
        * It will be processed in the next loop to find unique split
        * events for one parent.
        */
        continue;
      } else if (event.eventType == QueueEvent::TYPE_SPLIT) {
        vertexEventsParents.push_back(event.parent);
        splitCluster.push_back(event);
      }
    }
  }

  //std::cout << "createChains edgeCluster size1 " << edgeCluster.size() << std::endl;
  //std::cout << "createChains splitCluster size1 " << splitCluster.size() << std::endl;
  //std::cout << "createChains vertexEventsParents size1 " << vertexEventsParents.size() << std::endl;

  for (QueueEvent& event : cluster) {
    if (event.eventType == QueueEvent::TYPE_SPLIT_VERTEX) {
      //std::cout << "createChains event.parent " << event.parent << std::endl;
      //for (Vertex v : vertexEventsParents) {
      //  std::cout << "createChains vertexEventsParents " << v << std::endl;
      //}
      bool found = std::find(vertexEventsParents.begin(), vertexEventsParents.end(), event.parent) != vertexEventsParents.end();
      if (!found) {
        /*
        * Multiple vertex events can be created for one parent.
        * It is caused by two edges that share one vertex and new
        * event will be added for both of them. When processing we
        * always need to group them into one per vertex. Always prefer
        * split events over vertex events.
        */
        //std::cout << "createChains not found" << std::endl;
        vertexEventsParents.push_back(event.parent);
        splitCluster.push_back(event);
      }
    }
  }

  //std::cout << "createChains splitCluster size2 " << splitCluster.size() << std::endl;
  //std::cout << "createChains vertexEventsParents size2 " << vertexEventsParents.size() << std::endl;

  std::vector<Chain> edgeChains;

  while (edgeCluster.size() > 0) {
    /*
    * We need to find all connected edge events, and create chains from
    * them. Two events are assumed to be connected if next parent of one
    * event is equal to previous parent of second event.
    */
    edgeChains.push_back(Chain(createEdgeChain(edgeCluster))); // EdgeChain
  }

  std::vector<Chain> chains;
  for (Chain& edgeChain : edgeChains) {
    chains.push_back(edgeChain);
  }

  for (Chain c : chains) {
    debugQueue("createChains", c.edgeList);
  }

  //std::cout << "createChains chains1 size " << chains.size() << std::endl;

  while (splitCluster.size() > 0) {
    QueueEvent split = splitCluster[0];
    splitCluster.erase(splitCluster.begin());

    bool inEdgeChain = false;
    for (Chain& chain : edgeChains) {
      // check if chain is split type
      if (isInEdgeChain(split, chain)) {
        // if we have edge chain it can't share split event
        inEdgeChain = true;
        break;
      }
    }

    if (inEdgeChain) {
      continue;
    }

    /*
    * split event is not part of any edge chain, it should be added as
    * new single element chain;
    */
    chains.push_back(Chain(split)); // SplitChain

  }

  /*
  * Return list of chains with type. Possible types are edge chain,
  * closed edge chain, split chain. Closed edge chain will produce pick
  * event. Always it can exist only one closed edge chain for point
  * cluster.
  */

  //for (Chain c : chains) {
  //  std::cout << "createChains chain " << c << std::endl;
  //}

  //std::cout << "createChains chains2 size " << chains.size() << std::endl;

  for (Chain c : chains) {
    debugQueue("createChains2", c.edgeList);
  }

  return chains;

}

LevelEvent createLevelEvent(Point3d& eventCenter, double distance, std::vector<QueueEvent>& eventCluster) {
  //for (QueueEvent e : eventCluster) {
  //  std::cout << "createLevelEvent eventCluster e " << e << std::endl;
  //}
  std::vector<Chain> chains = createChains(eventCluster);

  if (chains.size() == 1) {
    if (chains[0].getChainMode() == Chain::MODE_CLOSED_EDGE) {
      return LevelEvent(eventCenter, distance, chains[0], true); // PickEvent
    } else if (chains[0].getChainMode() == Chain::MODE_EDGE) {
      return LevelEvent(eventCenter, distance, chains[0], false); // MultiEdgeEvent
    } else if (chains[0].getChainMode() == Chain::MODE_SPLIT) {
      return LevelEvent(eventCenter, distance, chains); // MultiSplitEvent
    }
  }

  for (Chain chain : chains) {
    if (chain.getChainMode() == Chain::MODE_CLOSED_EDGE) {
      LOG_AND_THROW("found closed chain of events for single point, but found more then one chain");
    }
  }

  return LevelEvent(eventCenter, distance, chains); // MultiSplitEvent
}

std::vector<LevelEvent> groupLevelEvents(std::vector<QueueEvent>& levelEvents) {
  std::vector<LevelEvent> ret;
  std::vector<Vertex> parentGroup;

  while (levelEvents.size() > 0) {
    parentGroup.clear();

    Point3d eventCenter = levelEvents[0].point;
    double distance = levelEvents[0].distance;

    levelEvents[0].addEventToGroup(parentGroup);

    std::vector<QueueEvent> cluster;
    cluster.push_back(levelEvents[0]);

    levelEvents.erase(levelEvents.begin());

    for (int j = 0; j < levelEvents.size(); j++) {

      if (levelEvents[j].isEventInGroup(parentGroup)) {
        /* Because of numerical errors, split event and edge event
        * can appear in slight different points. Epsilon can be
        * applied to level but event point can move rapidly even for
        * little changes in level. If two events for the same level
        * share the same parent, they should be merge together.
        */
        //std::cout << "groupLevelEvents if " << levelEvents[j] << std::endl;
        cluster.push_back(levelEvents[j]);
        levelEvents[j].addEventToGroup(parentGroup);
        levelEvents.erase(levelEvents.begin() + j);
        j--;
      } else if (getDistance(eventCenter, levelEvents[j].point) < SPLIT_EPSILON) {
        // group all events when the result points are near each other
        //std::cout << "groupLevelEvents else if " << levelEvents[j] << std::endl;
        cluster.push_back(levelEvents[j]);
        levelEvents[j].addEventToGroup(parentGroup);
        levelEvents.erase(levelEvents.begin() + j);
        j--;
      } else {
        //std::cout << "groupLevelEvents else " << levelEvents[j] << std::endl;
      }

    }

    // More than one event can share the same result point, we need to
    // create new level event.
    //std::cout << "groupLevelEvents ret add" << std::endl;
    ret.push_back(createLevelEvent(eventCenter, distance, cluster));

  }

  //std::cout << "groupLevelEvents ret size " << ret.size() << std::endl;
  for (LevelEvent e : ret) {
    //std::cout << "groupLevelEvents event " << e << std::endl;
    debugQueue("groupLevelEvents", e.chain.edgeList);
  }

  return ret;
}

std::vector<LevelEvent> loadAndGroupLevelEvents(std::vector<QueueEvent>& queue) {
  std::sort(queue.begin(), queue.end());
  std::vector<QueueEvent> levelEvents = loadLevelEvents(queue);
  return groupLevelEvents(levelEvents);
}

boost::optional<Vertex> getEdgeInLav(std::vector<Vertex> lav, Edge oppositeEdge) {
  for (Vertex vertex : lav) {
    if (vertex.previousEdge && oppositeEdge == vertex.previousEdge.get()) {
      return vertex;
    } else {
      int i = vertex.getOffsetVertexIndex(lav, -1);
      boost::optional<Edge> nextEdge = lav[i].nextEdge;
      if (nextEdge && oppositeEdge == nextEdge.get()) {
        return vertex;
      }
    }
  }
  return boost::none;
}

std::vector<Vertex> findEdgeLavs(std::vector< std::vector<Vertex> > sLav, Edge oppositeEdge) {
  std::vector<Vertex> edgeLavs;
  for (std::vector<Vertex> lav : sLav) {
    boost::optional<Vertex> vertexInLav = getEdgeInLav(lav, oppositeEdge);
    if (vertexInLav) {
      edgeLavs.push_back(vertexInLav.get());
    }
  }
  return edgeLavs;
}

/// Test if point is inside polygon
bool isInsidePolygon(Point3d point, std::vector<Point3d> points) {
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

  Point3d first = points[it];

  bool oddNodes = false;

  Point3d node1;
  Point3d node2;

  for (int i = 0; i < numpoints; i++) {
    node1 = points[it];
    it++;
    if (i == numpoints - 1) {
      node2 = first;
    } else {
      node2 = points[it];
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

boost::optional<Vertex> chooseOppositeEdgeLav(std::vector<Vertex> edgeLavs, Edge oppositeEdge, Point3d center) {
  if (edgeLavs.size() == 0) {
    return boost::none;
  } else if (edgeLavs.size() == 1) {
    return edgeLavs[0];
  }

  Point3d edgeStart = oppositeEdge.begin;
  Vector3d edgeNorm = oppositeEdge.normalize();
  Vector3d centerVector = center - edgeStart;
  double centerDot = edgeNorm.dot(centerVector);

  for (Vertex end : edgeLavs) {
    int j = end.getOffsetVertexIndex(edgeLavs, -1);
    Vector3d beginVector = edgeLavs[j].point - edgeStart;
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
    LOG_AND_THROW("FIXME");
    int size = 0; // FIXME = end.list.size();
    std::vector<Point3d> points;
    Vertex next = end;
    for (int i = 0; i < size; i++) {
      points.push_back(next.point);
      //FIXME next = next.getOffsetVertex(edgeLavs, 1);
    }
    if (isInsidePolygon(center, points)) {
      return end;
    }
  }

  LOG_AND_THROW("could not find lav for opposite edge, it could be correct but need some test data to check.")

}

boost::optional<Vertex> findOppositeEdgeLav(std::vector< std::vector<Vertex> > sLav, Edge oppositeEdge, Point3d center) {
  std::vector<Vertex> edgeLavs = findEdgeLavs(sLav, oppositeEdge);
  return chooseOppositeEdgeLav(edgeLavs, oppositeEdge, center);
}

void createOppositeEdgeChains(std::vector< std::vector<Vertex> >& sLav, std::vector<Chain>& chains, Point3d& center) {
  /*
  * Add chain created from opposite edge, this chain have to be
  * calculated during processing event because lav could change during
  * processing another events on the same level
  */
  std::vector<Edge> oppositeEdges;

  std::vector<Chain> oppositeEdgeChains;
  std::vector<Chain> chainsForRemoval;

  for (Chain chain : chains) {
    // add opposite edges as chain parts
    if (chain.chainType == Chain::TYPE_SPLIT) {
      boost::optional<Edge&> oppositeEdge = chain.getOppositeEdge();

      LOG_AND_THROW("FIXME");
      if (false) { // FIXME if (oppositeEdge && !oppositeEdges.count(oppositeEdge.get())) {
        // find lav vertex for opposite edge

        boost::optional<Vertex> nextVertex = findOppositeEdgeLav(sLav, oppositeEdge.get(), center);
        if (nextVertex) {
          oppositeEdgeChains.push_back(Chain(oppositeEdge.get(), nextVertex.get())); // SingleEdgeChain
        } else {
          findOppositeEdgeLav(sLav, oppositeEdge.get(), center);
          chainsForRemoval.push_back(chain);
        }
        oppositeEdges.push_back(oppositeEdge.get());
      }

    } else if (chain.chainType == Chain::TYPE_EDGE) {
      if (chain.getChainMode() == Chain::MODE_SPLIT) {
        boost::optional<Edge&> oppositeEdge = chain.getOppositeEdge();
        if (oppositeEdge) {
          // never happen?
          // find lav vertex for opposite edge
          oppositeEdges.push_back(oppositeEdge.get());
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
    LOG_AND_THROW("FIXME");
  }
  for (Chain chain : oppositeEdgeChains) {
    chains.push_back(chain);
  }

}

Vertex createMultiSplitVertex(Edge& nextEdge, Edge& previousEdge, Point3d& center, double distance) {
  Ray2d bisector = calcBisector(center, previousEdge, nextEdge);

  // edges are mirrored for event
  return Vertex(center, distance, bisector, previousEdge, nextEdge);
}

void correctBisectorDirection(Ray2d& bisector, Vertex& beginNextVertex, Vertex& endPreviousVertex, Edge& beginEdge, Edge& endEdge) {
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

bool areSameLav(std::vector<Vertex>& lav1, std::vector<Vertex>& lav2) {
  LOG_AND_THROW("FIXME");
  // FIXME Implement
  return false;
}

/// Add all vertex from "merged" lav into "base" lav. Vertex are added before
/// base vertex. Merged vertex order is reversed.
void mergeBeforeBaseVertex(Vertex& base, std::vector<Vertex>& baseList, Vertex& merged, std::vector<Vertex>& mergedList) {
  int size = mergedList.size();

  for (int i = 0; i < size; i++) {
    int j = merged.getOffsetVertexIndex(mergedList, 1);
    auto it = std::find(baseList.begin(), baseList.end(), base);
    baseList.insert(it, mergedList[j]);
  }

  mergedList.erase(mergedList.begin(), mergedList.end());
}

boost::optional<double> computeCloserEdgeEvent(Vertex& vertex, std::vector<QueueEvent>& queue, std::vector< std::vector<Vertex> >& sLav) {
  /*
  * Calculate two new edge events for given vertex. Events are generated
  * using current, previous and next vertex in current lav. When two edge
  * events are generated distance from source is check. To queue is added
  * only closer event or both if they have the same distance.
  */

  int vertexLavIndex = vertex.getLavIndex(sLav);

  //for (Vertex v : sLav[vertexLavIndex]) {
  //  std::cout << "computeCloserEdgeEvent lav v " << v << std::endl;
  //}

  int vertexNextIndex = vertex.getOffsetVertexIndex(sLav[vertexLavIndex], 1);
  int vertexPreviousIndex = vertex.getOffsetVertexIndex(sLav[vertexLavIndex], -1);

  Vertex nextVertex = sLav[vertexLavIndex][vertexNextIndex];
  Vertex previousVertex = sLav[vertexLavIndex][vertexPreviousIndex];

  //std::cout << "computeCloserEdgeEvent vertex " << vertex << std::endl;
  //std::cout << "computeCloserEdgeEvent nextVertex " << nextVertex << std::endl;
  //std::cout << "computeCloserEdgeEvent previousVertex " << previousVertex << std::endl;
  //std::cout << "computeCloserEdgeEvent point " << vertex.point << std::endl;

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
    //std::cout << "computeCloserEdgeEvent point1 " << point1 << std::endl;
    distance1 = getDistanceSquared(vertex.point, point1.get());
  }
  if (point2) {
    //std::cout << "computeCloserEdgeEvent point2 " << point2 << std::endl;
    distance2 = getDistanceSquared(vertex.point, point2.get());
  }

  //std::cout << "computeCloserEdgeEvent distance1 " << distance1 << std::endl;
  //std::cout << "computeCloserEdgeEvent distance2 " << distance2 << std::endl;

  if (distance1 - SPLIT_EPSILON < distance2) {
    QueueEvent e = createEdgeEvent(point1.get(), vertex, nextVertex);
    queue.push_back(e);
  }
  if (distance2 - SPLIT_EPSILON < distance1) {
    QueueEvent e = createEdgeEvent(point2.get(), previousVertex, vertex);
    queue.push_back(e);
  }

  if (distance1 < distance2) {
    return distance1;
  }
  return distance2;
}

void computeEvents(Vertex& vertex, std::vector<QueueEvent>& queue, std::vector<Edge>& edges, std::vector< std::vector<Vertex> >& sLav) {
  boost::optional<double> distanceSquared = computeCloserEdgeEvent(vertex, queue, sLav);
  computeSplitEvents(vertex, edges, queue, distanceSquared);
}

void multiSplitEvent(LevelEvent& event, std::vector< std::vector<Vertex> >& sLav, std::vector<QueueEvent>& queue, std::vector<Edge>& edges, std::vector<Face>& faces) {

  std::vector<Chain> chains = event.chains;
  Point3d center = event.point;

  createOppositeEdgeChains(sLav, chains, center);

  // sort list of chains clock wise
  LOG_AND_THROW("FIXME");
  //std::sort(chains.begin(), chains.end()); // FIXME implement compare function

  // connect all edges into new bisectors and lavs
  int edgeListSize = chains.size();
  for (int i = 0; i < edgeListSize; i++) {
    Chain& chainBegin = chains[i];
    Chain& chainEnd = chains[(i + 1) % edgeListSize];

    Vertex newVertex = createMultiSplitVertex(chainBegin.getNextEdge(), chainEnd.getPreviousEdge(), center, event.distance);

    // Split and merge lavs...
    Vertex& beginNextVertex = chainBegin.getNextVertex();
    Vertex& endPreviousVertex = chainEnd.getPreviousVertex();

    correctBisectorDirection(newVertex.bisector.get(), beginNextVertex, endPreviousVertex, chainBegin.getNextEdge(), chainEnd.getPreviousEdge());

    std::vector<Vertex> beginNextVertexLav = sLav[beginNextVertex.getLavIndex(sLav)];
    std::vector<Vertex> endPreviousVertexLav = sLav[endPreviousVertex.getLavIndex(sLav)];

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
      sLav.push_back(lav);

    } else {
      /*
      * if vertexes are in different lavs we need to merge them into
      * one.
      */

      mergeBeforeBaseVertex(beginNextVertex, beginNextVertexLav, endPreviousVertex, endPreviousVertexLav);

      int lavIndex = endPreviousVertex.getLavIndex(sLav);
      auto it = std::find(sLav[lavIndex].begin(), sLav[lavIndex].end(), endPreviousVertex);
      sLav[lavIndex].insert(it + 1, newVertex);
    }

    computeEvents(newVertex, queue, edges, sLav);

    LOG_AND_THROW("FIXME");
    //std::tuple<bool, FaceNode> t = addSplitFaces(hasLastFaceNode, lastFaceNode, chainBegin, chainEnd, newVertex, faces);
    // FIXME: hasLastFaceNode = std::get<0>(t);
    // FIXME: lastFaceNode = std::get<1>(t);

  }

  // remove all centers of events from lav
  edgeListSize = chains.size();
  for (int i = 0; i < edgeListSize; i++) {
    Chain& chainBegin = chains[i];
    Chain& chainEnd = chains[(i + 1) % edgeListSize];

    chainBegin.getCurrentVertex().get().removeFromLav(sLav);
    chainEnd.getCurrentVertex().get().removeFromLav(sLav);

    if (chainBegin.getCurrentVertex()) {
      setProcessed(chainBegin.getCurrentVertex().get(), sLav, queue);
    }
    if (chainEnd.getCurrentVertex()) {
      setProcessed(chainEnd.getCurrentVertex().get(), sLav, queue);
    }

  }

}

void addFaceBack(Vertex& newVertex, Vertex& va, Vertex& vb, std::vector<Face>& faces) {
  //std::cout << "addFaceBack newVertex: " << newVertex.point << std::endl;
  //std::cout << "addFaceBack va: " << va.point << std::endl;
  //std::cout << "addFaceBack vb: " << vb.point << std::endl;
  for (Face& face : faces) {
    for (int i = 0; i < face.vertexes.size(); i++) {
      int j = (i + 1) % face.vertexes.size();
      if (face.vertexes[i] == va && face.vertexes[j] == vb) {
        int k = (j + 1) % face.vertexes.size();
        face.vertexes.insert(face.vertexes.begin() + k, newVertex);
        break;
      }
    }
    if (face.vertexes[face.vertexes.size() - 1] == va) {
      face.vertexes.push_back(newVertex);
    }
  }
  debugFaces("addFaceBack", faces);
}

void addMultiBackFaces(std::vector<QueueEvent>& edgeList, Vertex& edgeVertex, std::vector< std::vector<Vertex> >& sLav, std::vector<QueueEvent>& queue, std::vector<Face>& faces) {
  for (QueueEvent& edgeEvent : edgeList) {

    setProcessed(edgeEvent.previousVertex, sLav, queue);
    edgeEvent.previousVertex.removeFromLav(sLav);

    setProcessed(edgeEvent.nextVertex, sLav, queue);
    edgeEvent.nextVertex.removeFromLav(sLav);

    addFaceBack(edgeVertex, edgeEvent.previousVertex, edgeEvent.nextVertex, faces);

  }
}

void pickEvent(LevelEvent& event, std::vector< std::vector<Vertex> >& sLav, std::vector<QueueEvent>& queue, std::vector<Edge>& edges, std::vector<Face>& faces) {


  // lav will be removed so it is final vertex.
  Vertex pickVertex = Vertex(event.point, event.distance, boost::none, boost::none, boost::none);
  setProcessed(pickVertex, sLav, queue);

  addMultiBackFaces(event.chain.edgeList, pickVertex, sLav, queue, faces);

}

void addFaceLeft(Vertex& newVertex, Vertex& va, std::vector<Face>& faces) {
  //std::cout << "addFaceLeft newVertex: " << newVertex.point << std::endl;
  //std::cout << "addFaceLeft va: " << va.point << std::endl;
  for (Face& face : faces) {
    if (face.vertexes[face.vertexes.size() - 1] == va) {
      face.vertexes.push_back(newVertex);
      break;
    }
  }
  debugFaces("addFaceLeft", faces);
}

void addFaceRight(Vertex& newVertex, Vertex& vb, std::vector<Face>& faces) {
  //std::cout << "addFaceRight newVertex: " << newVertex.point << std::endl;
  //std::cout << "addFaceRight vb: " << vb.point << std::endl;
  for (Face& face : faces) {
    if (face.vertexes[0] == vb) {
      face.vertexes.insert(face.vertexes.begin(), newVertex);
      break;
    }
  }
  debugFaces("addFaceRight", faces);
}

void multiEdgeEvent(LevelEvent& event, std::vector< std::vector<Vertex> >& sLav, std::vector<QueueEvent>& queue, std::vector<Edge>& edges, std::vector<Face>& faces) {

  Vertex& prevVertex = event.chain.getPreviousVertex();
  Vertex& nextVertex = event.chain.getNextVertex();

  setProcessed(prevVertex, sLav, queue);
  setProcessed(nextVertex, sLav, queue);

  Ray2d bisector = calcBisector(event.point, prevVertex.previousEdge.get(), nextVertex.nextEdge.get());
  Vertex edgeVertex = Vertex(event.point, event.distance, bisector, prevVertex.previousEdge.get(), nextVertex.nextEdge.get());

  // left face
  addFaceLeft(edgeVertex, event.chain.getPreviousVertex(), faces);

  // right face
  addFaceRight(edgeVertex, event.chain.getNextVertex(), faces);

  int lavIndex = prevVertex.getLavIndex(sLav);
  auto it = std::find(sLav[lavIndex].begin(), sLav[lavIndex].end(), prevVertex);
  sLav[lavIndex].insert(it, edgeVertex);

  // back faces
  addMultiBackFaces(event.chain.edgeList, edgeVertex, sLav, queue, faces);

  computeEvents(edgeVertex, queue, edges, sLav);
}

//std::tuple<bool, FaceNode> addSplitFaces(bool hasLastFaceNode, FaceNode& lastFaceNode, Chain& chainBegin, Chain& chainEnd, Vertex& newVertex, std::vector<Face>& faces) {
//  if (chainBegin.chainType == TYPE_SINGLE_EDGE) {
//    /*
//    * When chain is generated by opposite edge we need to share face
//    * between two chains. Number of that chains shares is always odd.
//    */
//    Vertex beginVertex = chainBegin.getNextVertex();

//    // right face
//    if (!hasLastFaceNode) {
//      /*
//      * Vertex generated by opposite edge share three faces, but
//      * vertex can store only left and right face. So we need to
//      * create vertex clone to store additional back face.
//      */
//      beginVertex = createOppositeEdgeVertex(newVertex);

//      /* same face in two vertex, original and in opposite edge clone */
//      // FIXME newVertex.rightFace = beginVertex.rightFace;
//      // FIXME lastFaceNode = beginVertex.leftFace;
//      hasLastFaceNode = true;
//    } else {
//      // face queue exist simply assign it to new node
//      // FIXME newVertex.rightFace = lastFaceNode;
//      hasLastFaceNode = false;
//    }

//  } else {
//    Vertex beginVertex = chainBegin.getCurrentVertex().get();

//    // right face
//    addFaceRight(newVertex, beginVertex);
//  }

//  if (chainEnd.chainType == TYPE_SINGLE_EDGE) {
//    Vertex endVertex = chainEnd.getPreviousVertex();

//    // left face
//    if (!hasLastFaceNode) {
//      /*
//      * Vertex generated by opposite edge share three faces, but
//      * vertex can store only left and right face. So we need to
//      * create vertex clone to store additional back face.
//      */
//      endVertex = createOppositeEdgeVertex(newVertex);

//      /* same face in two vertex, original and in opposite edge clone */
//      // FIXME newVertex.leftFace = endVertex.leftFace;
//      // FIXME lastFaceNode = endVertex.leftFace;
//      hasLastFaceNode = true;
//    } else {
//      // face queue exist simply assign it to new node
//      // FIXME newVertex.leftFace = lastFaceNode;
//      hasLastFaceNode = false;
//    }

//  } else {
//    Vertex endVertex = chainEnd.getCurrentVertex().get();

//    // left face
//    addFaceLeft(newVertex, endVertex);
//  }

//  return std::tuple<bool, FaceNode>(hasLastFaceNode, lastFaceNode);
//}

Vertex createOppositeEdgeVertex(Vertex& newVertex) {
  LOG_AND_THROW("FIXME");
  /*
  * When opposite edge is processed we need to create copy of vertex to
  * use in opposite face. When opposite edge chain occur vertex is shared
  * by additional output face.
  */

  Vertex vertex = Vertex(newVertex.point, newVertex.distance, newVertex.bisector.get(), newVertex.previousEdge.get(), newVertex.nextEdge.get());

  // create new empty node queue
  // FIXME: FaceNode fn = FaceNode(vertex);
  // FIXME vertex.leftFace = fn;
  // FIXME vertex.rightFace = fn;

  // add one node for queue to present opposite site of edge split event
  // FIXME: FaceQueue rightFace;
  // FIXME rightFace.addFirst(fn);

  return vertex;
}

void processTwoNodeLavs(std::vector< std::vector<Vertex> >& sLav, std::vector<QueueEvent>& queue, std::vector<Face>& faces) {
  for (std::vector<Vertex> lav : sLav) {
    if (lav.size() == 2) {
      int j = lav[0].getOffsetVertexIndex(lav, 1);

      // FIXME FaceQueueUtil.connectQueues(lav[0].leftFace, lav[j].rightFace);
      // FIXME FaceQueueUtil.connectQueues(lav[0].rightFace, lav[j].leftFace);

      setProcessed(lav[0], sLav, queue);
      setProcessed(lav[j], sLav, queue);

      lav[0].removeFromLav(sLav);
      lav[lav.size() - 1].removeFromLav(sLav);

    }
  }
}

void removeEventsUnderHeight(std::vector<QueueEvent>& queue, double levelHeight) {
  std::sort(queue.begin(), queue.end());
  while (!queue.empty()) {
    if (queue[0].distance > levelHeight + SPLIT_EPSILON) {
      break;
    }
    queue.erase(queue.begin());
  }
}

void removeEmptyLav(std::vector< std::vector<Vertex> >& sLav) {
  for (int i = 0; i < sLav.size(); i++) {
    if (sLav[i].size() == 0) {
      sLav.erase(sLav.begin() + i);
      i--;
      break;
    }
  }
}

std::vector< std::vector<Point3d> > doStraightSkeleton(std::vector<Point3d>& polygon, double roofPitchDegrees) {

  /* Straight skeleton algorithm implementation. Based on highly modified Petr
  * Felkel and Stepan Obdrzalek algorithm.
  * Translated from https://github.com/kendzi/kendzi-math
  */

  std::vector<QueueEvent> queue;
  std::vector<LevelEvent> processedEvents;
  std::vector<Face> faces;
  std::vector<Edge> edges;
  std::vector< std::vector<Vertex> > sLav;

  initPolygon(polygon);
  initSlav(polygon, sLav, edges, faces);
  initEvents(sLav, queue, edges);

  debugQueue("doStraightSkeleton", queue);

  int count = 0;

  //std::cout << "queue " << queue.size() << std::endl;
  while (!queue.empty()) {
    // start processing skeleton level
    count = assertMaxNumberOfIterations(count);
    //std::cout << "count " << count << std::endl;

    double levelHeight = queue[0].distance;
    //std::cout << "levelHeight " << levelHeight << std::endl;

    std::vector<LevelEvent> levelEvents = loadAndGroupLevelEvents(queue);

    for (LevelEvent& event : levelEvents) {

      if (event.obsolete) {
        /*
        * Event is outdated some of parent vertex was processed
        * before
        */
        continue;
      }

      processedEvents.push_back(event);

      if (event.eventType == LevelEvent::TYPE_MULTI_SPLIT) {
        multiSplitEvent(event, sLav, queue, edges, faces);
        continue;
      } else if (event.eventType == LevelEvent::TYPE_PICK) {
        pickEvent(event, sLav, queue, edges, faces);
        continue;
      } else if (event.eventType == LevelEvent::TYPE_MULTI_EDGE) {
        multiEdgeEvent(event, sLav, queue, edges, faces);
        continue;
      } else {
        LOG_AND_THROW("Unexpected event type");
      }
    }

    //for (QueueEvent e : queue) {
    //  std::cout << "queue event " << e << " isObsolete " << e.isObsolete() << std::endl;
    //}

    processTwoNodeLavs(sLav, queue, faces);
    debugFaces("processTwoNodeLavs", faces);

    //std::cout << "queue size before " << queue.size() << std::endl;
    removeEventsUnderHeight(queue, levelHeight);
    //std::cout << "queue size after " << queue.size() << std::endl;
    removeEmptyLav(sLav);

  }

  //for (Edge e : edges) {
  //  std::cout << "doStraightSkeleton edge " << e << std::endl;
  //}

  std::vector< std::vector<Point3d> > roofs = facesToRoofs(faces, roofPitchDegrees);

  //std::cout << "OUTPUT" << std::endl;
  //for (std::vector<Point3d> roof : roofs) {
  //  for (Point3d p : roof) {
  //    std::cout << p << ",";
  //  }
  //  std::cout << std::endl;
  //}

  return roofs;
}

namespace openstudio {

  std::vector< std::vector<Point3d> > makeShedRoof(std::vector<Point3d>& polygon, double roofPitchDegrees, double directionDegrees) {
    // FIXME implement
    std::vector< std::vector<Point3d> > roofs;
    try {

    } catch (...) {
      return roofs;
    }

    return roofs;
  }

  std::vector< std::vector<Point3d> > makeGableRoof(std::vector<Point3d>& polygon, double roofPitchDegrees) {
    std::vector< std::vector<Point3d> > roofs;
    try {
      roofs = doStraightSkeleton(polygon, roofPitchDegrees);
      if (roofs.size() == 0) {
        return roofs;
      }
    } catch (...) {
      return roofs;
    }

    // FIXME implement adjustments for gables

    return roofs;
  }

  std::vector< std::vector<Point3d> > makeHipRoof(std::vector<Point3d>& polygon, double roofPitchDegrees) {
    std::vector< std::vector<Point3d> > roofs;
    try {
      roofs = doStraightSkeleton(polygon, roofPitchDegrees);
      if (roofs.size() == 0) {
        return roofs;
      }
    } catch (...) {
      return roofs;
    }

    return roofs;
  }

}

