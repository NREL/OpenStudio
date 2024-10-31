/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "Geometry.hpp"
#include "Vector3d.hpp"
#include "Point3d.hpp"
#include "../core/Logger.hpp"

#include <boost/optional.hpp>
#include <boost/math/constants/constants.hpp>

namespace openstudio {
constexpr double EPSILON = 1E-10;

class LineLinear2d  // Geometry line in linear form. General form: Ax + By + C = 0;
{
 public:
  double A = 0.0;
  double B = 0.0;
  double C = 0.0;

  LineLinear2d(double a, double b, double c) : A{a}, B{b}, C{c} {}

  LineLinear2d(const Point3d& p1, const Point3d& p2) : A{p1.y() - p2.y()}, B{p2.x() - p1.x()}, C{p1.x() * p2.y() - p2.x() * p1.y()} {}

  boost::optional<Point3d> collide(const LineLinear2d& line2) const {
    double WAB = A * line2.B - line2.A * B;
    double WBC = B * line2.C - line2.B * C;
    double WCA = C * line2.A - line2.C * A;

    if (WAB == 0) {
      return boost::none;
    }

    return Point3d(WBC / WAB, WCA / WAB, 0);
  }

  friend std::ostream& operator<<(std::ostream& os, const LineLinear2d& l) {
    os << "LineLinear2d [A=" << l.A << ", B=" << l.B << ", C=" << l.C << "]";
    return os;
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

  Ray2d(const Point3d& t_point, const Vector3d& t_vector) : point{t_point}, vector{t_vector} {}

  boost::optional<Point3d> collide(const LineLinear2d& line, double epsilon) const {
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

    double d = vector.dot(collideVector);

    if (d < epsilon) {
      return boost::none;
    }

    return collide;
  }

  LineLinear2d getLinearForm() const {
    double x = point.x();
    double y = point.y();

    double B = -vector.x();
    double A = vector.y();

    double C = -1 * (A * x + B * y);
    return {A, B, C};
  }

  bool isOnLeftSide(const Point3d& p, double epsilon) const {
    Vector3d direction = p - point;
    Vector3d orthRight = vector.orthogonalRight();
    return (orthRight.dot(direction) < epsilon);
  }

  bool isOnRightSide(const Point3d& p, double epsilon) const {
    Vector3d direction = p - point;
    Vector3d orthRight = vector.orthogonalRight();
    return (orthRight.dot(direction) > -epsilon);
  }

  boost::optional<Point3d> intersectRay2d(std::shared_ptr<Ray2d> other) const {
    /*
    * Calculate intersection points for rays. It can return more then one
    * intersection point when rays overlaps.
    *
    * see http://geomalgorithms.com/a05-_intersect-1.html
    * see http://softsurfer.com/Archive/algorithm_0102/algorithm_0102.htm
    */

    Point3d s1p0 = point;
    Point3d s1p1 = Point3d(point) + vector;

    Point3d s2p0 = other->point;
    // TODO: this is unread!
    // cppcheck-suppress unreadVariable
    Point3d s2p1 = Point3d(other->point) + other->vector;

    Vector3d u = vector;
    Vector3d v = other->vector;

    Vector3d w = s1p0 - s2p0;

    double d = perpDot(u, v);

    // test if they are parallel (includes either being a point)
    if (std::abs(d) < EPSILON) {  // S1 and S2 are parallel

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
      double t0;
      double t1;
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
        t1 = t;  // swap if not
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
      // TODO: this is unread!
      // cppcheck-suppress unreadVariable
      Point3d I1 = s2p0 + t1 * v;

      if (t0 == t1) {
        // intersect is a point
        return I0;
      }

      return I0;  // only need first intersection point
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
  static double perpDot(const Vector3d& p1, const Vector3d& p2) {
    return (p1.x() * p2.y() - p1.y() * p2.x());
  }

  bool inCollinearRay(const Point3d& p) const {
    // test if p is on ray
    Vector3d collideVector = p - point;

    double d = vector.dot(collideVector);
    if (d < 0) {
      return false;
    }
    return true;
  }

  friend std::ostream& operator<<(std::ostream& os, const Ray2d& r) {
    os << "Ray2d [A=" << r.point << ", U=" << r.vector << "]";
    return os;
  }

  // Returns true if this ray2d is equal to other
  bool operator==(const Ray2d& other) const {
    if (point == other.point && vector == other.vector) {
      return true;
    }
    return false;
  }

 private:
  REGISTER_LOGGER("utilities.Ray2d");
};

class Edge
{
 public:
  Point3d begin;
  Point3d end;
  std::shared_ptr<Ray2d> bisectorPrevious;
  std::shared_ptr<Ray2d> bisectorNext;
  std::shared_ptr<Edge> next;
  std::shared_ptr<Edge> previous;

  Edge() {
    // nop
  }

  Edge(const Point3d& t_begin, const Point3d& t_end) : begin{t_begin}, end{t_end} {}

  Vector3d normalize() const {
    Vector3d v = Vector3d(end.x(), end.y(), end.z()) - Vector3d(begin.x(), begin.y(), begin.z());
    v.normalize();
    return v;
  }

  friend std::ostream& operator<<(std::ostream& os, const Edge& e) {
    os << "EdgeEntry [p1=" << e.begin << ", p2=" << e.end << ", bp=" << *e.bisectorPrevious << ", bn=" << *e.bisectorNext << "]";
    return os;
  }

  // Returns true if this edge is equal to other
  bool operator==(const Edge& other) const {
    if (begin == other.begin && end == other.end) {
      return true;
    }
    return false;
  }

  // Returns true if this edge is not equal to other
  bool operator!=(const Edge& other) const {
    if (begin != other.begin || end != other.end) {
      return true;
    }
    return false;
  }

 private:
  REGISTER_LOGGER("utilities.Edge");
};

class Face;  // forward declaration

class FaceNode
{
 public:
  Point3d point;
  double distance = 0.0;
  std::shared_ptr<Face> face;

  FaceNode() {
    // nop
  }

  FaceNode(const Point3d& t_point, double t_distance, std::shared_ptr<Face> t_face) : point{t_point}, distance{t_distance}, face{t_face} {}

  // Returns true if this vertex is equal to other
  bool operator==(const FaceNode& other) const {
    if (point == other.point && distance == other.distance) {
      return true;
    }
    return false;
  }

  friend std::ostream& operator<<(std::ostream& os, const FaceNode& fn) {
    os << "FaceNode [point=" << fn.point << ", distance=" << fn.distance << "]";
    return os;
  }

 private:
  REGISTER_LOGGER("utilities.FaceNode");
};

class Vertex
{
 public:
  Point3d point;
  double distance = 0.0;
  std::shared_ptr<Ray2d> bisector;
  std::shared_ptr<Edge> previousEdge;
  std::shared_ptr<Edge> nextEdge;
  std::shared_ptr<FaceNode> leftFaceNode;
  std::shared_ptr<FaceNode> rightFaceNode;
  bool processed = false;

  Vertex() {
    // nop
  }

  Vertex(const Point3d& t_point, double t_distance, std::shared_ptr<Ray2d> t_bisector, std::shared_ptr<Edge> t_previousEdge,
         std::shared_ptr<Edge> t_nextEdge)
    : point{t_point}, distance{t_distance}, bisector{t_bisector}, previousEdge{t_previousEdge}, nextEdge{t_nextEdge} {}

  Vertex(const std::shared_ptr<Edge> t_previousEdge, const std::shared_ptr<Edge> t_nextEdge) : previousEdge{t_previousEdge}, nextEdge{t_nextEdge} {}

  static std::shared_ptr<Vertex> previous(std::shared_ptr<Vertex> vertex, std::vector<std::shared_ptr<Vertex>>& vertices) {
    int index = getOffsetVertexIndex(vertex, vertices, -1);
    return vertices[index];
  }

  static std::shared_ptr<Vertex> next(std::shared_ptr<Vertex> vertex, std::vector<std::shared_ptr<Vertex>>& vertices) {
    int index = getOffsetVertexIndex(vertex, vertices, 1);
    return vertices[index];
  }

  static std::shared_ptr<Vertex> previous(std::shared_ptr<Vertex> vertex, std::vector<std::vector<std::shared_ptr<Vertex>>>& sLav) {
    int index = Vertex::getLavIndex(vertex, sLav);
    return Vertex::previous(vertex, sLav[index]);
  }

  static std::shared_ptr<Vertex> next(std::shared_ptr<Vertex> vertex, std::vector<std::vector<std::shared_ptr<Vertex>>>& sLav) {
    int index = Vertex::getLavIndex(vertex, sLav);
    return Vertex::next(vertex, sLav[index]);
  }

  static int getLavIndex(std::shared_ptr<Vertex> vertex, std::vector<std::vector<std::shared_ptr<Vertex>>>& sLav) {
    for (unsigned i = 0; i < sLav.size(); i++) {
      if (std::find(sLav[i].begin(), sLav[i].end(), vertex) != sLav[i].end()) {
        return i;
      }
    }
    return -1;
  }

  static void removeFromLav(std::shared_ptr<Vertex> vertex, std::vector<std::shared_ptr<Vertex>>& lav) {
    auto it = std::find(lav.begin(), lav.end(), vertex);
    if (it != lav.end()) {
      lav.erase(it);
    }
  }

  static void removeFromLav(std::shared_ptr<Vertex> vertex, std::vector<std::vector<std::shared_ptr<Vertex>>>& sLav) {
    int i = Vertex::getLavIndex(vertex, sLav);
    if (i != -1) {
      Vertex::removeFromLav(vertex, sLav[i]);
    }
  }

  friend std::ostream& operator<<(std::ostream& os, const Vertex& v) {
    std::string processed_str = "false";
    if (v.processed) {
      processed_str = "true";
    }

    os << "VertexEntry [v=" << v.point << ", processed=" << processed_str << ", bisector=";

    if (v.bisector) {
      os << *v.bisector;
    } else {
      os << "null";
    }

    os << ", previousEdge=";

    if (v.previousEdge) {
      os << *v.previousEdge;
    } else {
      os << "null";
    }

    os << ", nextEdge=";

    if (v.nextEdge) {
      os << *v.nextEdge;
    } else {
      os << "null";
    }

    os << "]";
    return os;
  }

  // Returns true if this vertex is equal to other
  bool operator==(const Vertex& other) const {
    if (point == other.point && distance == other.distance && bisector == other.bisector && previousEdge == other.previousEdge
        && nextEdge == other.nextEdge) {
      return true;
    }
    return false;
  }

  // Returns true if this vertex is not equal to other
  bool operator!=(const Vertex& other) const {
    if (point != other.point || distance != other.distance || bisector != other.bisector || previousEdge != other.previousEdge
        || nextEdge != other.nextEdge) {
      return true;
    }
    return false;
  }

 private:
  REGISTER_LOGGER("utilities.Vertex");

  friend int getOffsetVertexIndex(std::shared_ptr<Vertex> vertex, std::vector<std::shared_ptr<Vertex>>& vertices, int offset) {
    auto it = std::find(vertices.begin(), vertices.end(), vertex);
    if (it == vertices.end()) {
      LOG_AND_THROW("Could not find vertex.");
    }
    int vsize = vertices.size();
    auto pos = std::distance(vertices.begin(), it);
    pos += offset;
    if (pos < 0) {
      pos += vertices.size();
    } else if (pos >= vsize) {
      pos -= vertices.size();
    }
    return static_cast<int>(pos);
  }
};

class Face
{
 public:
  std::vector<std::shared_ptr<FaceNode>> nodes;
  std::shared_ptr<Edge> edge;
  bool closed = false;

  unsigned getNodeIndex(std::shared_ptr<FaceNode> node) const {
    auto it = std::find(nodes.begin(), nodes.end(), node);
    if (it == nodes.end()) {
      LOG_AND_THROW("Could not find node in nodes.")
    }
    return static_cast<unsigned>(it - nodes.begin());
  }

  bool isEnd(std::shared_ptr<FaceNode> vertex) const {
    unsigned nodeIndex = getNodeIndex(vertex);
    return (nodeIndex == 0 || nodeIndex == nodes.size() - 1);
  }

  bool isUnconnected() const {
    if (edge) {
      return false;
    }
    return true;
  }

  std::shared_ptr<FaceNode> popNode(std::shared_ptr<FaceNode> node) {
    if (node->face->nodes != this->nodes) {
      LOG_AND_THROW("Node is not assigned to this list!");
    }
    if (nodes.empty()) {
      LOG_AND_THROW("List is empty, can't remove.");
    }
    if (!isEnd(node)) {
      LOG_AND_THROW("Can pop only from end of queue.");
    }

    node->face = nullptr;

    unsigned nodeIndex = getNodeIndex(node);
    nodes.erase(nodes.begin() + nodeIndex);

    if (nodes.empty()) {
      return nullptr;
    } else if (nodeIndex > 0) {
      return nodes[nodes.size() - 1];
    } else {
      return nodes[0];
    }
  }

  friend std::ostream& operator<<(std::ostream& os, const Face& f) {
    os << "Face [size=" << f.nodes.size() << ", edge=";

    if (f.edge) {
      os << *f.edge;
    } else {
      os << "null";
    }

    os << ", closed=" << f.closed << "]";

    return os;
  }

 private:
  REGISTER_LOGGER("utilities.Face");
};

class QueueEvent
{
 public:
  enum QueueEventType
  {
    TYPE_EDGE = 0,
    TYPE_SPLIT = 1,
    TYPE_SPLIT_VERTEX = 2,
  };

  QueueEventType eventType;
  Point3d point;
  double distance = 0.0;
  std::shared_ptr<Vertex> previousVertex;
  std::shared_ptr<Vertex> nextVertex;
  std::shared_ptr<Vertex> parent;
  std::shared_ptr<Edge> oppositeEdge;

  QueueEvent() : eventType{TYPE_EDGE} {
    // nop
  }

  QueueEvent(const Point3d& t_point, double t_distance, std::shared_ptr<Vertex> t_previousVertex, std::shared_ptr<Vertex> t_nextVertex)
    : eventType{TYPE_EDGE}, point{t_point}, distance{t_distance}, previousVertex{t_previousVertex}, nextVertex{t_nextVertex} {}

  QueueEvent(const Point3d& t_point, double t_distance, std::shared_ptr<Vertex> t_parent)
    : eventType{TYPE_SPLIT_VERTEX}, point{t_point}, distance{t_distance}, parent{t_parent} {}

  QueueEvent(const Point3d& t_point, double t_distance, std::shared_ptr<Vertex> t_parent, std::shared_ptr<Edge> t_oppositeEdge)
    : eventType{TYPE_SPLIT}, point{t_point}, distance{t_distance}, parent{t_parent}, oppositeEdge{t_oppositeEdge} {}

  friend std::ostream& operator<<(std::ostream& os, const QueueEvent& e) {
    if (e.eventType == TYPE_EDGE) {
      os << "EdgeEvent [v=" << e.point << ", previousVertex=" << e.previousVertex->point << ", nextVertex=" << e.nextVertex->point
         << ", distance=" << e.distance << "]";
    } else if (e.eventType == TYPE_SPLIT) {
      os << "SplitEvent [v=" << e.point << ", parent=" << e.parent->point << ", distance=" << e.distance << "]";
    } else if (e.eventType == TYPE_SPLIT_VERTEX) {
      os << "VertexSplitEvent [v=" << e.point << ", parent=" << e.parent->point << ", distance=" << e.distance << "]";
    }
    return os;
  }

  bool operator<(const QueueEvent& other) const {
    return distance < other.distance;
  }

  // Returns true if this queue event is equal to other
  bool operator==(const QueueEvent& other) const {
    if (eventType == other.eventType && point == other.point && distance == other.distance && parent == other.parent
        && oppositeEdge == other.oppositeEdge && previousVertex == other.previousVertex && nextVertex == other.nextVertex) {
      return true;
    }
    return false;
  }

  void addEventToGroup(std::vector<std::shared_ptr<Vertex>>& parentGroup) const {
    if (eventType == TYPE_SPLIT || eventType == TYPE_SPLIT_VERTEX) {
      parentGroup.push_back(parent);
    } else if (eventType == TYPE_EDGE) {
      parentGroup.push_back(previousVertex);
      parentGroup.push_back(nextVertex);
    }
  }

  bool isEventInGroup(std::vector<std::shared_ptr<Vertex>>& parentGroup) {
    if (eventType == TYPE_SPLIT || eventType == TYPE_SPLIT_VERTEX) {
      bool foundParent = std::find(parentGroup.begin(), parentGroup.end(), parent) != parentGroup.end();
      return foundParent;
    } else if (eventType == TYPE_EDGE) {
      bool foundPreviousVertex = std::find(parentGroup.begin(), parentGroup.end(), previousVertex) != parentGroup.end();
      bool foundNextVertex = std::find(parentGroup.begin(), parentGroup.end(), nextVertex) != parentGroup.end();
      return (foundPreviousVertex || foundNextVertex);
    }
    return false;
  }

  std::shared_ptr<Edge> getOppositeEdgePrevious() const {
    return oppositeEdge;
  }

  bool isObsolete() const {
    if (eventType == TYPE_EDGE) {
      return (previousVertex->processed || nextVertex->processed);
    } else {
      return parent->processed;
    }
  }

  struct Comparer
  {
    bool operator()(std::shared_ptr<QueueEvent> q1, std::shared_ptr<QueueEvent> q2) const {
      return *q1 < *q2;
    }
  };

  static void insert_sorted(std::vector<std::shared_ptr<QueueEvent>>& queue, std::shared_ptr<QueueEvent> item) {
    queue.insert(std::upper_bound(queue.begin(), queue.end(), item, QueueEvent::Comparer()), item);
  }

 private:
  REGISTER_LOGGER("utilities.QueueEvent");
};

class Chain  // Chains of queue events
{
 public:
  enum ChainType
  {
    TYPE_EDGE = 0,
    TYPE_SPLIT = 1,
    TYPE_SINGLE_EDGE = 2
  };

  enum ChainMode
  {
    MODE_EDGE = 0,
    MODE_SPLIT = 1,
    MODE_CLOSED_EDGE = 2
  };

  ChainType chainType;
  std::vector<std::shared_ptr<QueueEvent>> edgeList;
  std::shared_ptr<QueueEvent> splitEvent;

  bool closed = false;
  bool split = false;

  Chain() : chainType{TYPE_EDGE} {
    // nop
  }

  Chain(std::shared_ptr<QueueEvent> t_splitEvent) : chainType{TYPE_SPLIT}, splitEvent{t_splitEvent} {}

  Chain(std::vector<std::shared_ptr<QueueEvent>>& t_edgeList, std::vector<std::vector<std::shared_ptr<Vertex>>>& sLav)
    : chainType{TYPE_EDGE}, edgeList{t_edgeList}, closed{(getPreviousVertex(sLav) == getNextVertex(sLav))} {}

  Chain(std::shared_ptr<Edge> t_oppositeEdge, std::shared_ptr<Vertex> t_nextVertex, std::vector<std::vector<std::shared_ptr<Vertex>>>& sLav)
    : chainType{TYPE_SINGLE_EDGE}, nextVertex{t_nextVertex}, oppositeEdge{t_oppositeEdge} {
    /*
     * previous vertex for opposite edge event is valid only before
     * processing of multi split event start .We need to store vertex before
     * processing starts.
    */
    this->previousVertex = Vertex::previous(nextVertex, sLav);
  }

  ChainMode getChainMode() const {
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

  std::shared_ptr<Edge> getPreviousEdge() const {
    if (chainType == TYPE_EDGE) {
      return edgeList[0]->previousVertex->previousEdge;
    } else if (chainType == TYPE_SINGLE_EDGE) {
      return oppositeEdge;
    } else {
      return splitEvent->parent->previousEdge;
    }
  }

  std::shared_ptr<Edge> getNextEdge() const {
    if (chainType == TYPE_EDGE) {
      return edgeList[edgeList.size() - 1]->nextVertex->nextEdge;
    } else if (chainType == TYPE_SINGLE_EDGE) {
      return oppositeEdge;
    } else {
      return splitEvent->parent->nextEdge;
    }
  }

  std::shared_ptr<Vertex> getPreviousVertex(std::vector<std::vector<std::shared_ptr<Vertex>>>& sLav) const {
    if (chainType == TYPE_EDGE) {
      return edgeList[0]->previousVertex;
    } else if (chainType == TYPE_SINGLE_EDGE) {
      return previousVertex;
    } else {
      return Vertex::previous(splitEvent->parent, sLav);
    }
  }

  std::shared_ptr<Vertex> getNextVertex(std::vector<std::vector<std::shared_ptr<Vertex>>>& sLav) const {
    if (chainType == TYPE_EDGE) {
      return edgeList[edgeList.size() - 1]->nextVertex;
    } else if (chainType == TYPE_SINGLE_EDGE) {
      return nextVertex;
    } else {
      return Vertex::next(splitEvent->parent, sLav);
    }
  }

  std::shared_ptr<Vertex> getCurrentVertex() const {
    std::shared_ptr<Vertex> ret;
    if (chainType == TYPE_SPLIT) {
      return splitEvent->parent;
    }
    return ret;
  }

  std::shared_ptr<Edge> getOppositeEdge() const {
    if (chainType == TYPE_SPLIT) {
      if (splitEvent->eventType != QueueEvent::TYPE_SPLIT_VERTEX) {
        return splitEvent->oppositeEdge;
      }
    }
    return oppositeEdge;
  }

  friend std::ostream& operator<<(std::ostream& os, const Chain& c) {
    std::string type_s;
    if (c.chainType == TYPE_EDGE) {
      type_s = "Edge";
    } else if (c.chainType == TYPE_SPLIT) {
      type_s = "Split";
    } else {
      type_s = "SingleEdge";
    }

    std::string mode_s;
    if (c.getChainMode() == MODE_EDGE) {
      mode_s = "EDGE";
    } else if (c.getChainMode() == MODE_SPLIT) {
      mode_s = "SPLIT";
    } else {
      mode_s = "CLOSED_EDGE";
    }

    os << "Chain [type=" << type_s << ", mode=" << mode_s << ", pe=";

    if (c.getPreviousEdge()) {
      os << *c.getPreviousEdge();
    } else {
      os << "null";
    }

    os << ", ne=";

    if (c.getNextEdge()) {
      os << *c.getNextEdge();
    } else {
      os << "null";
    }

    os << ", oe=";

    if (c.getOppositeEdge()) {
      os << *c.getOppositeEdge();
    } else {
      os << "null";
    }

    os << ", cv=";

    if (c.getCurrentVertex()) {
      os << *c.getCurrentVertex();
    } else {
      os << "null";
    }

    os << "]";

    return os;
  }

  // Returns true if this chain is equal to other
  bool operator==(const Chain& other) const {
    if (chainType == other.chainType && splitEvent == other.splitEvent && edgeList == other.edgeList) {
      return true;
    }
    return false;
  }

  struct Comparer
  {
    Point3d center;

    Comparer(Point3d center) {
      this->center = center;
    }

    static double angle(const Point3d& p1, const Point3d& p2) {
      double dx = p2.x() - p1.x();
      double dy = p2.y() - p1.y();
      return atan2(dy, dx);
    }

    bool operator()(const Chain& c1, const Chain& c2) const {
      double angle1 = angle(center, c1.getPreviousEdge()->begin);
      double angle2 = angle(center, c2.getPreviousEdge()->begin);
      return (angle1 < angle2);
    }
  };

 private:
  REGISTER_LOGGER("utilities.Chain");
  std::shared_ptr<Edge> previousEdge;
  std::shared_ptr<Edge> nextEdge;
  std::shared_ptr<Vertex> previousVertex;
  std::shared_ptr<Vertex> nextVertex;
  std::shared_ptr<Vertex> currentVertex;
  std::shared_ptr<Edge> oppositeEdge;
};

class LevelEvent
{
 public:
  enum LevelEventType
  {
    TYPE_PICK = 0,
    TYPE_MULTI_EDGE = 1,
    TYPE_MULTI_SPLIT = 2
  };

  LevelEventType eventType;
  Point3d point;
  double distance = 0.0;
  Chain chain;
  std::vector<Chain> chains;
  bool obsolete = false;

  LevelEvent() : eventType{TYPE_PICK} {
    // nop
  }

  LevelEvent(const Point3d& t_point, double t_distance, const Chain& t_chain, bool isPickEvent)
    : point{t_point}, distance{t_distance}, chain{t_chain} {
    if (isPickEvent) {
      this->eventType = TYPE_PICK;
    } else {
      this->eventType = TYPE_MULTI_EDGE;
    }
  }

  LevelEvent(const Point3d& t_point, double t_distance, const std::vector<Chain>& t_chains)
    : eventType{TYPE_MULTI_SPLIT}, point{t_point}, distance{t_distance}, chains{t_chains} {}

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
  std::shared_ptr<Edge> oppositeEdge;
  boost::optional<Point3d> oppositePoint;

  SplitCandidate(const Point3d& t_point, double t_distance, std::shared_ptr<Edge> t_oppositeEdge, boost::optional<Point3d&> t_oppositePoint)
    : point{t_point}, distance{t_distance}, oppositeEdge{t_oppositeEdge}, oppositePoint{t_oppositePoint} {}

  // Returns true if this SplitCandidate is less than other
  bool operator<(const SplitCandidate& other) const {
    return (distance < other.distance);
  }

  // Returns true if this SplitCandidate is equal to other
  bool operator==(const SplitCandidate& other) const {
    return (point == other.point && distance == other.distance);
  }

  friend std::ostream& operator<<(std::ostream& os, const SplitCandidate& s) {
    os << "SplitCandidate [point=" << s.point << ", distance=" << s.distance << "]";
    return os;
  }

 private:
  REGISTER_LOGGER("utilities.SplitCandidate");
};

static std::vector<std::vector<Point3d>> facesToPoint3d(const std::vector<std::shared_ptr<Face>>& faces, double roofPitchDegrees, double zcoord) {
  std::vector<std::vector<Point3d>> roofsPoint3d;
  double roofSlope = tan(degToRad(roofPitchDegrees));
  for (const std::shared_ptr<Face>& face : faces) {
    if (face->nodes.empty()) {
      continue;
    }
    std::vector<Point3d> roofPoint3d;
    for (const std::shared_ptr<FaceNode>& v : face->nodes) {
      Point3d p = Point3d(v->point.x(), v->point.y(), zcoord + v->distance * roofSlope);
      roofPoint3d.push_back(p);
    }
    roofsPoint3d.push_back(roofPoint3d);
  }
  return roofsPoint3d;
}

static Vector3d calcVectorBisector(const Vector3d& norm1, const Vector3d& norm2) {
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

static Ray2d calcBisector(const Point3d& p, std::shared_ptr<Edge> e1, std::shared_ptr<Edge> e2) {

  Vector3d norm1 = e1->normalize();
  Vector3d norm2 = e2->normalize();
  Vector3d bisector = calcVectorBisector(norm1, norm2);
  return Ray2d(p, bisector);
}

static void addPush(std::shared_ptr<FaceNode> node, std::shared_ptr<FaceNode> newNode) {
  std::shared_ptr<Face> face = node->face;
  if (face->closed) {
    LOG_AND_THROW("Can't add node to closed Face");
  }

  unsigned nodeIndex = face->getNodeIndex(node);
  if (nodeIndex == face->nodes.size() - 1) {
    // if vertex is end of list, add newVertex to end
    face->nodes.push_back(newNode);
  } else if (nodeIndex == 0) {
    // if vertex is beginning of list, add newVertex to beginning
    face->nodes.insert(face->nodes.begin(), newNode);
  } else {
    LOG_AND_THROW("Can't push new node. Node is inside a queue. New node can only be added at the ends of the queue.");
  }

  if (face != newNode->face) {
    newNode->face = face;
  }
}

static void moveNodes(std::shared_ptr<FaceNode> firstNode, std::shared_ptr<FaceNode> secondNode) {
  if (firstNode->face == secondNode->face) {
    return;
  }
  std::shared_ptr<FaceNode> currentQueue = firstNode;
  std::shared_ptr<FaceNode> current = secondNode;
  std::shared_ptr<FaceNode> next = nullptr;
  while (current) {
    next = current->face->popNode(current);
    addPush(currentQueue, current);
    currentQueue = current;
    current = next;
  }
}

static void connectFaces(std::shared_ptr<FaceNode> firstFaceNode, std::shared_ptr<FaceNode> secondFaceNode) {
  std::shared_ptr<Face> firstFace = firstFaceNode->face;
  std::shared_ptr<Face> secondFace = secondFaceNode->face;
  if (firstFace == secondFace) {
    if (!firstFace->isEnd(firstFaceNode) || !secondFace->isEnd(secondFaceNode)) {
      LOG_AND_THROW("Tried to connect the same list not on end nodes.");
    }
    if (firstFace->isUnconnected() || secondFace->isUnconnected()) {
      LOG_AND_THROW("Can't close node queue not conected with edges.");
    }

    firstFace->closed = true;
    return;
  }

  if (!firstFace->isUnconnected() && !secondFace->isUnconnected()) {
    LOG_AND_THROW("Can't connect two different queues if each of them is connected to an edge.");
  }

  if (!firstFace->isUnconnected()) {
    moveNodes(firstFaceNode, secondFaceNode);
    secondFace->closed = true;
  } else {
    moveNodes(secondFaceNode, firstFaceNode);
    firstFace->closed = true;
  }
}

static void initSlav(const std::vector<Point3d>& polygon, std::vector<std::vector<std::shared_ptr<Vertex>>>& sLav,
                     std::vector<std::shared_ptr<Edge>>& edges, std::vector<std::shared_ptr<Face>>& faces) {
  int size = polygon.size();
  for (int i = 0; i < size; i++) {
    int j = (i + 1) % size;
    std::shared_ptr<Edge> e(new Edge(polygon[i], polygon[j]));
    edges.push_back(e);
  }

  for (int i = 0; i < size; i++) {
    int h = (i - 1 + size) % size;
    int j = (i + 1) % size;
    edges[i]->previous = edges[h];
    edges[i]->next = edges[j];
  }

  for (const std::shared_ptr<Edge>& edge : edges) {
    std::shared_ptr<Edge> nextEdge = edge->next;

    std::shared_ptr<Ray2d> bisector(new Ray2d(calcBisector(edge->end, edge, nextEdge)));

    edge->bisectorNext = bisector;

    nextEdge->bisectorPrevious = bisector;
  }

  std::vector<std::shared_ptr<Vertex>> lav;

  for (const std::shared_ptr<Edge>& edge : edges) {
    std::shared_ptr<Edge> nextEdge = edge->next;

    std::shared_ptr<Vertex> vertex(new Vertex(edge->end, 0, edge->bisectorNext, edge, nextEdge));

    lav.push_back(vertex);
  }
  sLav.push_back(lav);

  for (const std::shared_ptr<Vertex>& vertex : lav) {
    std::shared_ptr<Vertex> next = Vertex::next(vertex, lav);

    // create face on right site of vertex
    std::shared_ptr<Face> face(new Face());
    face->edge = vertex->nextEdge;
    std::shared_ptr<FaceNode> rightFaceNode(new FaceNode(vertex->point, vertex->distance, face));
    face->nodes.push_back(rightFaceNode);
    vertex->rightFaceNode = rightFaceNode;

    // create face on left site of next vertex
    std::shared_ptr<FaceNode> leftFaceNode(new FaceNode(next->point, next->distance, face));
    addPush(rightFaceNode, leftFaceNode);
    next->leftFaceNode = leftFaceNode;

    faces.push_back(face);
  }
}

static bool edgeBehindBisector(std::shared_ptr<Ray2d> bisector, const LineLinear2d& edge) {
  /*
  * Simple intersection test between the bisector starting at V and the
  * whole line containing the currently tested line segment ei rejects
  * the line segments laying "behind" the vertex V
  */
  return (!bisector->collide(edge, EPSILON));
}

static std::shared_ptr<Edge> chooseLessParallelVertexEdge(std::shared_ptr<Vertex> vertex, std::shared_ptr<Edge> edge) {
  std::shared_ptr<Edge> edgeA = vertex->previousEdge;
  std::shared_ptr<Edge> edgeB = vertex->nextEdge;

  std::shared_ptr<Edge> vertexEdge = edgeA;

  double edgeADot = std::abs(edge->normalize().dot(edgeA->normalize()));
  double edgeBDot = std::abs(edge->normalize().dot(edgeB->normalize()));

  if (edgeADot + edgeBDot >= 2 - EPSILON) {
    // both lines are parallel to given edge
    return nullptr;
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
static Vector3d orthogonalProjection(const Vector3d& unitVector, const Vector3d& vectorToProject) {
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

  return {vx, vy, vz};
}

static double calcDistance(const Point3d& intersect, std::shared_ptr<Edge> currentEdge) {
  // TODO: Can be replaced by getDistancePointToLineSegment() ?
  Vector3d edge = currentEdge->end - currentEdge->begin;
  Vector3d vector = intersect - currentEdge->begin;

  Vector3d pointOnVector = orthogonalProjection(edge, vector);

  return getDistance(Point3d(vector.x(), vector.y(), vector.z()), Point3d(pointOnVector.x(), pointOnVector.y(), pointOnVector.z()));
}

static boost::optional<SplitCandidate> calcCandidatePointForSplit(std::shared_ptr<Vertex> vertex, std::shared_ptr<Edge> edge) {

  std::shared_ptr<Edge> vertexEdge = chooseLessParallelVertexEdge(vertex, edge);
  if (!vertexEdge) {
    return boost::none;
  }

  Vector3d vertexEdgeNormNegate = vertexEdge->normalize();
  Vector3d edgeNorm = edge->normalize();
  Vector3d edgesBisector = calcVectorBisector(vertexEdgeNormNegate, edgeNorm);

  LineLinear2d llv = LineLinear2d(vertexEdge->begin, vertexEdge->end);
  LineLinear2d lle = LineLinear2d(edge->begin, edge->end);

  boost::optional<Point3d> edgesCollide = llv.collide(lle);

  if (!edgesCollide) {
    /*
    * Check should be performed to exclude the case when one of the
    * line segments starting at V is parallel to ei.
    */
    return boost::none;
  }

  LineLinear2d edgesBisectorLine = Ray2d(edgesCollide.get(), edgesBisector).getLinearForm();

  /*
  * Compute the coordinates of the candidate point Bi as the intersection
  * between the bisector at V and the axis of the angle between one of
  * the edges starting at V and the tested line segment ei
  */
  boost::optional<Point3d> candidatePoint = vertex->bisector->collide(edgesBisectorLine, EPSILON);

  if (!candidatePoint) {
    return boost::none;
  }

  if (edge->bisectorPrevious->isOnRightSide(candidatePoint.get(), EPSILON) && edge->bisectorNext->isOnLeftSide(candidatePoint.get(), EPSILON)) {

    double distance = calcDistance(candidatePoint.get(), edge);

    if (edge->bisectorPrevious->isOnLeftSide(candidatePoint.get(), EPSILON) || edge->bisectorNext->isOnRightSide(candidatePoint.get(), EPSILON)) {

      Point3d oppositePoint = edge->begin;
      return SplitCandidate(candidatePoint.get(), distance, nullptr, oppositePoint);
    }

    return SplitCandidate(candidatePoint.get(), distance, edge, boost::none);
  }

  return boost::none;
}

static std::vector<SplitCandidate> calcOppositeEdges(std::shared_ptr<Vertex> vertex, const std::vector<std::shared_ptr<Edge>>& edges) {

  std::vector<SplitCandidate> ret;

  for (const std::shared_ptr<Edge>& edgeEntry : edges) {

    LineLinear2d edge = LineLinear2d(edgeEntry->begin, edgeEntry->end);

    // check if edge is behind bisector
    if (edgeBehindBisector(vertex->bisector, edge)) {
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

static void computeSplitEvents(std::shared_ptr<Vertex> vertex, const std::vector<std::shared_ptr<Edge>>& edges,
                               std::vector<std::shared_ptr<QueueEvent>>& queue, boost::optional<double> distanceSquared) {
  Point3d source = vertex->point;

  std::vector<SplitCandidate> oppositeEdges = calcOppositeEdges(vertex, edges);

  // check if it is vertex split event
  for (const SplitCandidate& oppositeEdge : oppositeEdges) {

    if (distanceSquared) {
      if (getDistanceSquared(source, oppositeEdge.point) > distanceSquared.get() + EPSILON) {
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
      std::shared_ptr<QueueEvent> e1(new QueueEvent(oppositeEdge.point, oppositeEdge.distance, vertex));  // SplitEvent
      QueueEvent::insert_sorted(queue, e1);
      continue;
    }

    std::shared_ptr<QueueEvent> e2(new QueueEvent(oppositeEdge.point, oppositeEdge.distance, vertex, oppositeEdge.oppositeEdge));  // SplitVertexEvent
    QueueEvent::insert_sorted(queue, e2);
  }
}

static boost::optional<Point3d> computeIntersectionBisectors(std::shared_ptr<Vertex> vertexPrevious, std::shared_ptr<Vertex> vertexNext) {
  boost::optional<Point3d> ret;

  std::shared_ptr<Ray2d> bisectorPrevious = vertexPrevious->bisector;
  std::shared_ptr<Ray2d> bisectorNext = vertexNext->bisector;

  boost::optional<Point3d> thisIntersect = bisectorPrevious->intersectRay2d(bisectorNext);

  if (!thisIntersect) {
    return ret;
  }

  if (vertexPrevious->point == thisIntersect.get() || vertexNext->point == thisIntersect.get()) {
    // skip the same points
    return ret;
  }

  ret = thisIntersect.get();
  return ret;
}

static std::shared_ptr<QueueEvent> createEdgeEvent(const Point3d& point, std::shared_ptr<Vertex> previousVertex, std::shared_ptr<Vertex> nextVertex) {
  std::shared_ptr<QueueEvent> e(new QueueEvent(point, calcDistance(point, previousVertex->nextEdge), previousVertex, nextVertex));  // EdgeEvent
  return e;
}

static void computeEdgeEvents(std::shared_ptr<Vertex> previousVertex, std::shared_ptr<Vertex> nextVertex,
                              std::vector<std::shared_ptr<QueueEvent>>& queue) {
  boost::optional<Point3d> point = computeIntersectionBisectors(previousVertex, nextVertex);
  if (point) {
    std::shared_ptr<QueueEvent> e(createEdgeEvent(point.get(), previousVertex, nextVertex));
    QueueEvent::insert_sorted(queue, e);
  }
}

static void initEvents(std::vector<std::vector<std::shared_ptr<Vertex>>>& sLav, std::vector<std::shared_ptr<QueueEvent>>& queue,
                       const std::vector<std::shared_ptr<Edge>>& edges) {
  for (const std::vector<std::shared_ptr<Vertex>>& lav : sLav) {
    for (const std::shared_ptr<Vertex>& vertex : lav) {
      computeSplitEvents(vertex, edges, queue, boost::none);
    }
  }

  for (std::vector<std::shared_ptr<Vertex>>& lav : sLav) {
    for (const std::shared_ptr<Vertex>& vertex : lav) {
      std::shared_ptr<Vertex> next = Vertex::next(vertex, lav);
      computeEdgeEvents(vertex, next, queue);
    }
  }
}

// Calculate area of polygon outline. For clockwise area will be less than
// zero, for counter-clockwise polygon area will be greater than zero.
static double area(const std::vector<Point3d>& polygon) {
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
static bool isClockwisePolygon(const std::vector<Point3d>& polygon) {
  return (area(polygon) < 0);
}

// Updates points ordered as counter clockwise.
static void makeCounterClockwise(std::vector<Point3d>& polygon) {
  if (isClockwisePolygon(polygon)) {
    polygon = reverse(polygon);
  }
}

static void setZcoordsToZero(std::vector<Point3d>& polygon) {
  for (Point3d& p : polygon) {
    p += Vector3d(0, 0, -p.z());
  }
}

static double initPolygon(std::vector<Point3d>& polygon) {
  if (polygon.size() < 3) {
    LOG_AND_THROW("Polygon must have at least 3 points.");
  } else if (polygon[0] == polygon[polygon.size() - 1]) {
    LOG_AND_THROW("Polygon can't start and end with the same point.");
  }

  for (unsigned i = 1; i < polygon.size(); i++) {
    if (polygon[i].z() != polygon[0].z()) {
      LOG_AND_THROW("All polygon z coordinates must be the same.");
    }
  }

  double zcoord = polygon[0].z();
  setZcoordsToZero(polygon);

  return zcoord;
}

static int assertMaxNumberOfIterations(int count) {
  count++;
  if (count > 10000) {
    LOG_AND_THROW("Maximum number of iterations reached. Bug?");
  }
  return count;
}

static std::vector<std::shared_ptr<QueueEvent>> loadLevelEvents(std::vector<std::shared_ptr<QueueEvent>>& queue) {
  /*
  * Loads all non obsolete events which are on one level.
  */

  std::vector<std::shared_ptr<QueueEvent>> level;

  std::shared_ptr<QueueEvent> levelStart = nullptr;
  while (!queue.empty()) {
    levelStart = queue[0];
    queue.erase(queue.begin());
    // skip all obsolete events in level
    if (!levelStart->isObsolete()) {
      break;
    }
  }

  if (!levelStart || levelStart->isObsolete()) {
    // all events obsolete
    return level;
  }

  double levelStartHeight = levelStart->distance;

  level.push_back(levelStart);

  std::shared_ptr<QueueEvent> event = nullptr;

  while (!queue.empty()) {
    event = queue[0];
    if (event->distance - levelStartHeight >= EPSILON) {
      break;
    }
    queue.erase(queue.begin());
    if (!event->isObsolete()) {
      level.push_back(event);
    }
  }

  return level;
}

static std::vector<std::shared_ptr<QueueEvent>> createEdgeChain(std::vector<std::shared_ptr<QueueEvent>>& edgeCluster) {
  std::vector<std::shared_ptr<QueueEvent>> edgeList;

  edgeList.push_back(edgeCluster[0]);
  edgeCluster.erase(edgeCluster.begin());

  // find all successors of edge event
  // find all predecessors of edge event

  do {
    std::shared_ptr<Vertex> beginVertex = edgeList[0]->previousVertex;
    std::shared_ptr<Vertex> endVertex = edgeList[edgeList.size() - 1]->nextVertex;

    bool do_continue = false;
    for (unsigned i = 0; i < edgeCluster.size(); i++) {
      if (edgeCluster[i]->previousVertex == endVertex) {
        // edge should be added as last in chain
        edgeList.push_back(edgeCluster[i]);
        edgeCluster.erase(edgeCluster.begin() + i);
        do_continue = true;
        break;
      } else if (edgeCluster[i]->nextVertex == beginVertex) {
        // edge should be added as first in chain
        edgeList.insert(edgeList.begin(), edgeCluster[i]);
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

  return edgeList;
}

static bool isInEdgeChain(std::shared_ptr<QueueEvent> split, const Chain& chain) {
  for (const std::shared_ptr<QueueEvent>& edgeEvent : chain.edgeList) {
    if (edgeEvent->previousVertex == split->parent || edgeEvent->nextVertex == split->parent) {
      return true;
    }
  }

  return false;
}

static std::vector<Chain> createChains(const std::vector<std::shared_ptr<QueueEvent>>& cluster,
                                       std::vector<std::vector<std::shared_ptr<Vertex>>>& sLav) {
  /*
  * Create chains of events from cluster. Cluster is set of events which meet
  * in the same result point. Try to connect all events which share the same
  * vertex into chain. Events in a chain are sorted. If events don't share a
  * vertex, returned chains contains only one event.
  */

  std::vector<std::shared_ptr<QueueEvent>> edgeCluster;
  std::vector<std::shared_ptr<QueueEvent>> splitCluster;
  std::vector<std::shared_ptr<Vertex>> vertexEventsParents;

  for (const std::shared_ptr<QueueEvent>& event : cluster) {
    if (event->eventType == QueueEvent::TYPE_EDGE) {
      edgeCluster.push_back(event);
    } else {
      if (event->eventType == QueueEvent::TYPE_SPLIT_VERTEX) {
        /*
        * It will be processed in the next loop to find unique split
        * events for one parent.
        */
        continue;
      } else if (event->eventType == QueueEvent::TYPE_SPLIT) {
        vertexEventsParents.push_back(event->parent);
        splitCluster.push_back(event);
      }
    }
  }

  for (const std::shared_ptr<QueueEvent>& event : cluster) {
    if (event->eventType == QueueEvent::TYPE_SPLIT_VERTEX) {
      bool found = std::find(vertexEventsParents.begin(), vertexEventsParents.end(), event->parent) != vertexEventsParents.end();
      if (!found) {
        /*
        * Multiple vertex events can be created for one parent.
        * It is caused by two edges that share one vertex and new
        * event will be added for both of them. When processing we
        * always need to group them into one per vertex. Always prefer
        * split events over vertex events.
        */
        vertexEventsParents.push_back(event->parent);
        splitCluster.push_back(event);
      }
    }
  }

  std::vector<Chain> edgeChains;

  while (!edgeCluster.empty()) {
    /*
    * We need to find all connected edge events, and create chains from
    * them. Two events are assumed to be connected if next parent of one
    * event is equal to previous parent of second event.
    */
    std::vector<std::shared_ptr<QueueEvent>> edgeList = createEdgeChain(edgeCluster);
    Chain edgeChain = Chain(edgeList, sLav);
    edgeChains.push_back(edgeChain);  // EdgeChain
  }

  std::vector<Chain> chains;
  for (const Chain& edgeChain : edgeChains) {
    chains.push_back(edgeChain);
  }

  while (!splitCluster.empty()) {
    std::shared_ptr<QueueEvent> split = splitCluster[0];
    splitCluster.erase(splitCluster.begin());

    bool inEdgeChain = false;
    for (const Chain& chain : edgeChains) {
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
    chains.push_back(Chain(split));  // SplitChain
  }

  /*
  * Return list of chains with type. Possible types are edge chain,
  * closed edge chain, split chain. Closed edge chain will produce pick
  * event. Always it can exist only one closed edge chain for point
  * cluster.
  */

  return chains;
}

static LevelEvent createLevelEvent(const Point3d& eventCenter, double distance, const std::vector<std::shared_ptr<QueueEvent>>& eventCluster,
                                   std::vector<std::vector<std::shared_ptr<Vertex>>>& sLav) {

  std::vector<Chain> chains = createChains(eventCluster, sLav);

  if (chains.size() == 1) {
    if (chains[0].getChainMode() == Chain::MODE_CLOSED_EDGE) {
      return LevelEvent(eventCenter, distance, chains[0], true);  // PickEvent
    } else if (chains[0].getChainMode() == Chain::MODE_EDGE) {
      return LevelEvent(eventCenter, distance, chains[0], false);  // MultiEdgeEvent
    } else if (chains[0].getChainMode() == Chain::MODE_SPLIT) {
      return LevelEvent(eventCenter, distance, chains);  // MultiSplitEvent
    }
  }

  for (const Chain& chain : chains) {
    if (chain.getChainMode() == Chain::MODE_CLOSED_EDGE) {
      LOG_AND_THROW("found closed chain of events for single point, but found more then one chain");
    }
  }

  return LevelEvent(eventCenter, distance, chains);  // MultiSplitEvent
}

static std::vector<LevelEvent> groupLevelEvents(std::vector<std::shared_ptr<QueueEvent>>& levelEvents,
                                                std::vector<std::vector<std::shared_ptr<Vertex>>>& sLav) {
  std::vector<LevelEvent> ret;
  std::vector<std::shared_ptr<Vertex>> parentGroup;

  while (!levelEvents.empty()) {
    parentGroup.clear();

    Point3d eventCenter = levelEvents[0]->point;
    double distance = levelEvents[0]->distance;

    levelEvents[0]->addEventToGroup(parentGroup);

    std::vector<std::shared_ptr<QueueEvent>> cluster;
    cluster.push_back(levelEvents[0]);

    levelEvents.erase(levelEvents.begin());

    for (unsigned j = 0; j < levelEvents.size(); j++) {

      if (levelEvents[j]->isEventInGroup(parentGroup)) {
        /* Because of numerical errors, split event and edge event
        * can appear in slight different points. Epsilon can be
        * applied to level but event point can move rapidly even for
        * little changes in level. If two events for the same level
        * share the same parent, they should be merge together.
        */
        cluster.push_back(levelEvents[j]);
        levelEvents[j]->addEventToGroup(parentGroup);
        levelEvents.erase(levelEvents.begin() + j);
        j--;
      } else if (getDistance(eventCenter, levelEvents[j]->point) < EPSILON) {
        // group all events when the result points are near each other
        cluster.push_back(levelEvents[j]);
        levelEvents[j]->addEventToGroup(parentGroup);
        levelEvents.erase(levelEvents.begin() + j);
        j--;
      }
    }

    // More than one event can share the same result point, we need to
    // create new level event.
    ret.push_back(createLevelEvent(eventCenter, distance, cluster, sLav));
  }

  return ret;
}

static std::vector<LevelEvent> loadAndGroupLevelEvents(std::vector<std::shared_ptr<QueueEvent>>& queue,
                                                       std::vector<std::vector<std::shared_ptr<Vertex>>>& sLav) {
  std::vector<std::shared_ptr<QueueEvent>> levelEvents = loadLevelEvents(queue);
  return groupLevelEvents(levelEvents, sLav);
}

static std::shared_ptr<Vertex> getEdgeInLav(std::vector<std::shared_ptr<Vertex>>& lav, std::shared_ptr<Edge> oppositeEdge) {
  for (std::shared_ptr<Vertex> vertex : lav) {
    if (vertex->previousEdge && oppositeEdge == vertex->previousEdge) {
      return vertex;
    } else {
      std::shared_ptr<Vertex> previous = Vertex::previous(vertex, lav);
      if (previous->nextEdge && oppositeEdge == previous->nextEdge) {
        return vertex;
      }
    }
  }
  return nullptr;
}

/// Test if point is inside polygon
static bool isInsidePolygon(const Point3d& point, const std::vector<Point3d>& points) {
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

    const double y = point.y();
    if ((node1.y() < y && node2.y() >= y) || (node2.y() < y && node1.y() >= y)) {
      const double x = point.x();
      if (node1.x() + (y - node1.y()) / (node2.y() - node1.y()) * (node2.x() - node1.x()) < x) {
        oddNodes = !oddNodes;
      }
    }
  }

  return oddNodes;
}

static int chooseOppositeEdgeLavIndex(const std::vector<std::shared_ptr<Vertex>>& edgeLavs, std::shared_ptr<Edge> oppositeEdge, const Point3d& center,
                                      std::vector<std::vector<std::shared_ptr<Vertex>>>& sLav) {
  if (edgeLavs.empty()) {
    return -1;
  } else if (edgeLavs.size() == 1) {
    return 0;
  }

  const Point3d& edgeStart = oppositeEdge->begin;
  Vector3d edgeNorm = oppositeEdge->normalize();
  Vector3d centerVector = center - edgeStart;
  double centerDot = edgeNorm.dot(centerVector);

  for (unsigned i = 0; i < edgeLavs.size(); i++) {
    std::shared_ptr<Vertex> end = edgeLavs[i];
    std::shared_ptr<Vertex> begin = Vertex::previous(end, sLav);
    Vector3d beginVector = begin->point - edgeStart;
    Vector3d endVector = end->point - edgeStart;

    double beginDot = edgeNorm.dot(beginVector);
    double endDot = edgeNorm.dot(endVector);

    /*
    * Make projection of center, begin and end into edge. Begin and end
    * are vertices chosen by opposite edge (then point to opposite edge).
    * Chose lav only when center is between begin and end. Only one lav
    * should meet criteria.
    */

    if ((beginDot < centerDot && centerDot < endDot) || (beginDot > centerDot && centerDot > endDot)) {
      return i;
    }
  }

  // Additional check if center is inside lav
  for (unsigned i = 0; i < edgeLavs.size(); i++) {
    std::shared_ptr<Vertex> end = edgeLavs[i];
    int index = Vertex::getLavIndex(end, sLav);
    unsigned size = sLav[index].size();
    std::vector<Point3d> points;
    std::shared_ptr<Vertex> next = end;
    for (unsigned j = 0; j < size; j++) {
      points.push_back(next->point);
      next = Vertex::next(next, sLav[index]);
    }
    if (isInsidePolygon(center, points)) {
      return i;
    }
  }

  LOG_AND_THROW("could not find lav for opposite edge, it could be correct but need some test data to check.")
}

static std::shared_ptr<Vertex> findOppositeEdgeLav(std::vector<std::vector<std::shared_ptr<Vertex>>>& sLav, std::shared_ptr<Edge> oppositeEdge,
                                                   const Point3d& center) {
  std::vector<std::shared_ptr<Vertex>> edgeLavs;
  for (std::vector<std::shared_ptr<Vertex>>& lav : sLav) {
    std::shared_ptr<Vertex> vertexInLav = getEdgeInLav(lav, oppositeEdge);
    if (vertexInLav) {
      edgeLavs.push_back(vertexInLav);
    }
  }
  int lavIndex = chooseOppositeEdgeLavIndex(edgeLavs, oppositeEdge, center, sLav);
  if (lavIndex > -1) {
    return edgeLavs[lavIndex];
  }
  return nullptr;
}

static void createOppositeEdgeChains(std::vector<std::vector<std::shared_ptr<Vertex>>>& sLav, std::vector<Chain>& chains, Point3d& center) {
  /*
  * Add chain created from opposite edge, this chain have to be
  * calculated during processing event because lav could change during
  * processing another events on the same level
  */
  std::vector<std::shared_ptr<Edge>> oppositeEdges;

  std::vector<Chain> oppositeEdgeChains;
  std::vector<Chain> chainsForRemoval;

  for (const Chain& chain : chains) {
    // add opposite edges as chain parts
    if (chain.chainType == Chain::TYPE_SPLIT) {
      std::shared_ptr<Edge> oppositeEdge = chain.getOppositeEdge();

      bool hasOppositeEdge = false;
      if (oppositeEdge) {
        hasOppositeEdge = std::find(oppositeEdges.begin(), oppositeEdges.end(), oppositeEdge) != oppositeEdges.end();
      }
      if (oppositeEdge && !hasOppositeEdge) {
        // find lav vertex for opposite edge

        Point3d c = Point3d(center);
        std::shared_ptr<Vertex> nextVertex = findOppositeEdgeLav(sLav, oppositeEdge, c);
        if (nextVertex) {
          oppositeEdgeChains.push_back(Chain(oppositeEdge, nextVertex, sLav));  // SingleEdgeChain
        } else {
          findOppositeEdgeLav(sLav, oppositeEdge, center);
          chainsForRemoval.push_back(chain);
        }
        oppositeEdges.push_back(oppositeEdge);
      }

    } else if (chain.chainType == Chain::TYPE_EDGE) {
      if (chain.getChainMode() == Chain::MODE_SPLIT) {
        std::shared_ptr<Edge> oppositeEdge = chain.getOppositeEdge();
        if (oppositeEdge) {
          // never happen?
          // find lav vertex for opposite edge
          oppositeEdges.push_back(oppositeEdge);
        }
      }
    }
  }

  /*
  * if opposite edge can't be found in active lavs then split chain with
  * that edge should be removed
  */
  for (Chain& chain : chainsForRemoval) {
    auto it = std::find(chains.begin(), chains.end(), chain);
    if (it == chains.end()) {
      LOG_AND_THROW("Could not find chain for removal.");
    }
    chains.erase(it);
  }

  for (const Chain& chain : oppositeEdgeChains) {
    chains.push_back(chain);
  }
}

static std::shared_ptr<Vertex> createMultiSplitVertex(std::shared_ptr<Edge> nextEdge, std::shared_ptr<Edge> previousEdge, const Point3d& center,
                                                      double distance) {
  std::shared_ptr<Ray2d> bisector(new Ray2d(calcBisector(center, previousEdge, nextEdge)));

  // edges are mirrored for event
  std::shared_ptr<Vertex> vertex(new Vertex(center, distance, bisector, previousEdge, nextEdge));
  return vertex;
}

static void correctBisectorDirection(std::shared_ptr<Ray2d> bisector, std::shared_ptr<Vertex> beginNextVertex,
                                     std::shared_ptr<Vertex> endPreviousVertex, std::shared_ptr<Edge> beginEdge, std::shared_ptr<Edge> endEdge) {
  /*
  * New bisector for vertex is created using connected edges. For
  * parallel edges numerical error may appear and direction of created
  * bisector is wrong. For parallel edges direction of edge need to be
  * corrected using location of vertex.
  */

  std::shared_ptr<Edge> beginEdge2 = beginNextVertex->previousEdge;
  std::shared_ptr<Edge> endEdge2 = endPreviousVertex->nextEdge;

  if (beginEdge != beginEdge2 || endEdge != endEdge2) {
    LOG_AND_THROW("Unexpected situation");
  }

  /*
  * Check if edges are parallel and in opposite direction to each other.
  */
  if (beginEdge->normalize().dot(endEdge->normalize()) < -0.97) {
    Point3d epvp = endPreviousVertex->point;
    Point3d bp = bisector->point;
    Point3d bnvp = beginNextVertex->point;
    Vector3d n1 = Vector3d(bp.x(), bp.y(), bp.z()) - Vector3d(epvp.x(), epvp.y(), epvp.z());
    Vector3d n2 = Vector3d(bnvp.x(), bnvp.y(), bnvp.z()) - Vector3d(bp.x(), bp.y(), bp.z());
    n1.normalize();
    n2.normalize();
    Vector3d bisectorPrediction = calcVectorBisector(n1, n2);

    if (bisector->vector.dot(bisectorPrediction) < 0) {
      /*
      * Bisector is calculated in opposite direction to edges and
      * center.
      */
      bisector->vector = bisector->vector.reverseVector();
    }
  }
}

static bool areSameLav(const std::vector<std::shared_ptr<Vertex>>& lav1, const std::vector<std::shared_ptr<Vertex>>& lav2) {
  if (lav1.size() != lav2.size()) {
    return false;
  }
  for (unsigned i = 0; i < lav1.size(); i++) {
    if (lav1[i] != lav2[i]) {
      return false;
    }
  }
  return true;
}

/// Add all vertex from "merged" lav into "base" lav. Vertex are added before
/// base vertex. Merged vertex order is reversed.
static void mergeBeforeBaseVertex(std::shared_ptr<Vertex> base, std::vector<std::shared_ptr<Vertex>>& baseList, std::shared_ptr<Vertex> merged,
                                  std::vector<std::shared_ptr<Vertex>>& mergedList) {
  int size = mergedList.size();

  for (int i = 0; i < size; i++) {
    std::shared_ptr<Vertex> nextMerged = Vertex::next(merged, mergedList);
    auto it = std::find(baseList.begin(), baseList.end(), base);
    baseList.insert(it, nextMerged);
  }

  mergedList.erase(mergedList.begin(), mergedList.end());
}

static boost::optional<double> computeCloserEdgeEvent(std::shared_ptr<Vertex> vertex, std::vector<std::shared_ptr<QueueEvent>>& queue,
                                                      std::vector<std::vector<std::shared_ptr<Vertex>>>& sLav) {
  /*
  * Calculate two new edge events for given vertex. Events are generated
  * using current, previous and next vertex in current lav. When two edge
  * events are generated distance from source is check. To queue is added
  * only closer event or both if they have the same distance.
  */

  std::shared_ptr<Vertex> nextVertex = Vertex::next(vertex, sLav);
  std::shared_ptr<Vertex> previousVertex = Vertex::previous(vertex, sLav);

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
    distance1 = getDistanceSquared(vertex->point, point1.get());
  }
  if (point2) {
    distance2 = getDistanceSquared(vertex->point, point2.get());
  }

  if (distance1 - EPSILON < distance2) {
    std::shared_ptr<QueueEvent> e(createEdgeEvent(point1.get(), vertex, nextVertex));
    QueueEvent::insert_sorted(queue, e);
  }
  if (distance2 - EPSILON < distance1) {
    std::shared_ptr<QueueEvent> e(createEdgeEvent(point2.get(), previousVertex, vertex));
    QueueEvent::insert_sorted(queue, e);
  }

  if (distance1 < distance2) {
    return distance1;
  }
  return distance2;
}

static void computeEvents(std::shared_ptr<Vertex> vertex, std::vector<std::shared_ptr<QueueEvent>>& queue,
                          const std::vector<std::shared_ptr<Edge>>& edges, std::vector<std::vector<std::shared_ptr<Vertex>>>& sLav) {
  boost::optional<double> distanceSquared = computeCloserEdgeEvent(vertex, queue, sLav);
  computeSplitEvents(vertex, edges, queue, distanceSquared);
}

static std::shared_ptr<Vertex> createOppositeEdgeVertex(std::shared_ptr<Vertex> newVertex) {
  /*
  * When opposite edge is processed we need to create copy of vertex to
  * use in opposite face. When opposite edge chain occur vertex is shared
  * by additional output face.
  */

  std::shared_ptr<Vertex> vertex(
    new Vertex(newVertex->point, newVertex->distance, newVertex->bisector, newVertex->previousEdge, newVertex->nextEdge));

  std::shared_ptr<Face> oppFace(new Face());

  // create new empty node queue
  std::shared_ptr<FaceNode> fn(new FaceNode(vertex->point, vertex->distance, oppFace));
  vertex->leftFaceNode = fn;
  vertex->rightFaceNode = fn;

  // add one node for queue to present opposite side of edge split event
  oppFace->nodes.push_back(fn);

  return vertex;
}

static void addFaceLeft(std::shared_ptr<Vertex> newVertex, std::shared_ptr<Vertex> va, std::vector<std::shared_ptr<Face>>& /*faces*/) {
  std::shared_ptr<Face> face = va->leftFaceNode->face;
  std::shared_ptr<FaceNode> fn(new FaceNode(newVertex->point, newVertex->distance, face));
  addPush(va->leftFaceNode, fn);
  newVertex->leftFaceNode = fn;
}

static void addFaceRight(std::shared_ptr<Vertex> newVertex, std::shared_ptr<Vertex> vb, std::vector<std::shared_ptr<Face>>& /*faces*/) {
  std::shared_ptr<Face> face = vb->rightFaceNode->face;
  std::shared_ptr<FaceNode> fn(new FaceNode(newVertex->point, newVertex->distance, face));
  addPush(vb->rightFaceNode, fn);
  newVertex->rightFaceNode = fn;
}

static std::shared_ptr<FaceNode> addSplitFaces(std::shared_ptr<FaceNode> lastFaceNode, const Chain& chainBegin, const Chain& chainEnd,
                                               std::shared_ptr<Vertex> newVertex, std::vector<std::shared_ptr<Face>>& faces,
                                               std::vector<std::vector<std::shared_ptr<Vertex>>>& sLav) {
  if (chainBegin.chainType == Chain::TYPE_SINGLE_EDGE) {
    /*
    * When chain is generated by opposite edge we need to share face
    * between two chains. Number of that chains shares is always odd.
    */
    // TODO: Unread variable!
    // cppcheck-suppress unreadVariable
    std::shared_ptr<Vertex> beginVertex = chainBegin.getNextVertex(sLav);

    // right face
    if (lastFaceNode) {
      // face queue exist simply assign it to new node
      newVertex->rightFaceNode = lastFaceNode;
      lastFaceNode = nullptr;
    } else {
      /*
      * Vertex generated by opposite edge share three faces, but
      * vertex can store only left and right face. So we need to
      * create vertex clone to store additional back face.
      */
      beginVertex = createOppositeEdgeVertex(newVertex);

      /* same face in two vertex, original and in opposite edge clone */
      newVertex->rightFaceNode = beginVertex->rightFaceNode;
      lastFaceNode = beginVertex->leftFaceNode;
    }

  } else {
    std::shared_ptr<Vertex> beginVertex = chainBegin.getCurrentVertex();

    // right face
    addFaceRight(newVertex, beginVertex, faces);
  }

  if (chainEnd.chainType == Chain::TYPE_SINGLE_EDGE) {
    // TODO: Unread variable!
    // cppcheck-suppress unreadVariable
    std::shared_ptr<Vertex> endVertex = chainEnd.getPreviousVertex(sLav);

    // left face
    if (lastFaceNode) {
      // face queue exist simply assign it to new node
      newVertex->leftFaceNode = lastFaceNode;
      lastFaceNode = nullptr;
    } else {
      /*
      * Vertex generated by opposite edge share three faces, but
      * vertex can store only left and right face. So we need to
      * create vertex clone to store additional back face.
      */
      endVertex = createOppositeEdgeVertex(newVertex);

      /* same face in two vertex, original and in opposite edge clone */
      newVertex->leftFaceNode = endVertex->leftFaceNode;
      lastFaceNode = endVertex->leftFaceNode;
    }

  } else {
    std::shared_ptr<Vertex> endVertex = chainEnd.getCurrentVertex();

    // left face
    addFaceLeft(newVertex, endVertex, faces);
  }

  return lastFaceNode;
}

// Returns cut portion of lav from startVertex to endVertex, including both
static std::vector<std::shared_ptr<Vertex>> cutLavPart(std::vector<std::shared_ptr<Vertex>>& lav, std::shared_ptr<Vertex> startVertex,
                                                       std::shared_ptr<Vertex> endVertex) {
  std::vector<std::shared_ptr<Vertex>> ret;

  std::shared_ptr<Vertex> current = startVertex;
  ret.push_back(current);

  if (startVertex == endVertex) {
    return ret;
  }

  while (true) {
    current = Vertex::next(current, lav);
    ret.push_back(current);
    if (current == endVertex) {
      break;
    }
    if (ret.size() > lav.size()) {
      LOG_AND_THROW("End vertex not found in start vertex lav.");
    }
  }

  for (const std::shared_ptr<Vertex>& v : ret) {
    Vertex::removeFromLav(v, lav);
  }

  return ret;
}

static void multiSplitEvent(LevelEvent& event, std::vector<std::vector<std::shared_ptr<Vertex>>>& sLav,
                            std::vector<std::shared_ptr<QueueEvent>>& queue, const std::vector<std::shared_ptr<Edge>>& edges,
                            std::vector<std::shared_ptr<Face>>& faces) {

  createOppositeEdgeChains(sLav, event.chains, event.point);

  // sort list of chains clock wise
  std::sort(event.chains.begin(), event.chains.end(), Chain::Comparer(event.point));

  // face vertex for split event is shared between two chains
  std::shared_ptr<FaceNode> lastFaceNode;

  // connect all edges into new bisectors and lavs
  int edgeListSize = event.chains.size();
  for (int i = 0; i < edgeListSize; i++) {
    Chain& chainBegin = event.chains[i];
    Chain& chainEnd = event.chains[(i + 1) % edgeListSize];

    Point3d p = Point3d(event.point);
    std::shared_ptr<Vertex> newVertex(createMultiSplitVertex(chainBegin.getNextEdge(), chainEnd.getPreviousEdge(), p, event.distance));

    // Split and merge lavs...
    std::shared_ptr<Vertex> beginNextVertex = chainBegin.getNextVertex(sLav);
    std::shared_ptr<Vertex> endPreviousVertex = chainEnd.getPreviousVertex(sLav);

    correctBisectorDirection(newVertex->bisector, beginNextVertex, endPreviousVertex, chainBegin.getNextEdge(), chainEnd.getPreviousEdge());

    std::vector<std::shared_ptr<Vertex>>& beginNextVertexLav = sLav[Vertex::getLavIndex(beginNextVertex, sLav)];
    std::vector<std::shared_ptr<Vertex>>& endPreviousVertexLav = sLav[Vertex::getLavIndex(endPreviousVertex, sLav)];

    if (areSameLav(beginNextVertexLav, endPreviousVertexLav)) {
      /*
      * if vertices are in same lav we need to cut part of lav in the
      * middle of vertex and create new lav from that points
      */

      std::vector<std::shared_ptr<Vertex>> lavPart = cutLavPart(beginNextVertexLav, beginNextVertex, endPreviousVertex);

      std::vector<std::shared_ptr<Vertex>> lav;
      lav.push_back(newVertex);
      for (const std::shared_ptr<Vertex>& vertex : lavPart) {
        lav.push_back(vertex);
      }
      sLav.push_back(lav);

    } else {
      /*
      * if vertices are in different lavs we need to merge them into
      * one.
      */
      mergeBeforeBaseVertex(beginNextVertex, beginNextVertexLav, endPreviousVertex, endPreviousVertexLav);

      int lavIndex = Vertex::getLavIndex(endPreviousVertex, sLav);
      if (lavIndex == -1) {
        LOG_AND_THROW("Could not find vertex in sLav.")
      }
      auto it = std::find(sLav[lavIndex].begin(), sLav[lavIndex].end(), endPreviousVertex);
      sLav[lavIndex].insert(it + 1, newVertex);
    }

    computeEvents(newVertex, queue, edges, sLav);

    lastFaceNode = addSplitFaces(lastFaceNode, chainBegin, chainEnd, newVertex, faces, sLav);
  }

  // remove all centers of events from lav
  edgeListSize = event.chains.size();
  for (int i = 0; i < edgeListSize; i++) {
    const Chain& chainBegin = event.chains[i];
    const Chain& chainEnd = event.chains[(i + 1) % edgeListSize];

    if (chainBegin.getCurrentVertex()) {
      chainBegin.getCurrentVertex()->processed = true;
      Vertex::removeFromLav(chainBegin.getCurrentVertex(), sLav);
    }
    if (chainEnd.getCurrentVertex()) {
      chainEnd.getCurrentVertex()->processed = true;
      Vertex::removeFromLav(chainEnd.getCurrentVertex(), sLav);
    }
  }
}

static void addFaceBack(std::shared_ptr<Vertex> newVertex, std::shared_ptr<Vertex> va, std::shared_ptr<Vertex> vb,
                        std::vector<std::shared_ptr<Face>>& /*faces*/) {
  std::shared_ptr<Face> face = va->rightFaceNode->face;
  std::shared_ptr<FaceNode> fn(new FaceNode(newVertex->point, newVertex->distance, face));
  addPush(va->rightFaceNode, fn);
  connectFaces(fn, vb->leftFaceNode);
}

static void addMultiBackFaces(const std::vector<std::shared_ptr<QueueEvent>>& edgeList, std::shared_ptr<Vertex> edgeVertex,
                              std::vector<std::vector<std::shared_ptr<Vertex>>>& sLav, std::vector<std::shared_ptr<QueueEvent>>& /*queue*/,
                              std::vector<std::shared_ptr<Face>>& faces) {
  for (const std::shared_ptr<QueueEvent>& edgeEvent : edgeList) {

    edgeEvent->previousVertex->processed = true;
    Vertex::removeFromLav(edgeEvent->previousVertex, sLav);

    edgeEvent->nextVertex->processed = true;
    Vertex::removeFromLav(edgeEvent->nextVertex, sLav);

    addFaceBack(edgeVertex, edgeEvent->previousVertex, edgeEvent->nextVertex, faces);
  }
}

static void pickEvent(const LevelEvent& event, std::vector<std::vector<std::shared_ptr<Vertex>>>& sLav,
                      std::vector<std::shared_ptr<QueueEvent>>& queue, std::vector<std::shared_ptr<Edge>>& /*edges*/,
                      std::vector<std::shared_ptr<Face>>& faces) {
  // lav will be removed so it is final vertex.
  std::shared_ptr<Vertex> pickVertex(new Vertex(event.point, event.distance, nullptr, nullptr, nullptr));
  pickVertex->processed = true;

  addMultiBackFaces(event.chain.edgeList, pickVertex, sLav, queue, faces);
}

static void multiEdgeEvent(LevelEvent& event, std::vector<std::vector<std::shared_ptr<Vertex>>>& sLav,
                           std::vector<std::shared_ptr<QueueEvent>>& queue, const std::vector<std::shared_ptr<Edge>>& edges,
                           std::vector<std::shared_ptr<Face>>& faces) {

  std::shared_ptr<Vertex> prevVertex = event.chain.getPreviousVertex(sLav);
  std::shared_ptr<Vertex> nextVertex = event.chain.getNextVertex(sLav);

  prevVertex->processed = true;
  nextVertex->processed = true;

  std::shared_ptr<Ray2d> bisector(new Ray2d(calcBisector(event.point, prevVertex->previousEdge, nextVertex->nextEdge)));
  Point3d p = Point3d(event.point);
  std::shared_ptr<Vertex> edgeVertex(new Vertex(p, event.distance, bisector, prevVertex->previousEdge, nextVertex->nextEdge));

  // left face
  addFaceLeft(edgeVertex, event.chain.getPreviousVertex(sLav), faces);

  // right face
  addFaceRight(edgeVertex, event.chain.getNextVertex(sLav), faces);

  int lavIndex = Vertex::getLavIndex(prevVertex, sLav);
  auto it = std::find(sLav[lavIndex].begin(), sLav[lavIndex].end(), prevVertex);
  auto pos = std::distance(sLav[lavIndex].begin(), it);
  if (pos == 0) {
    sLav[lavIndex].push_back(edgeVertex);
  } else {
    sLav[lavIndex].insert(it, edgeVertex);
  }

  // back faces
  addMultiBackFaces(event.chain.edgeList, edgeVertex, sLav, queue, faces);

  computeEvents(edgeVertex, queue, edges, sLav);
}

static void processTwoNodeLavs(std::vector<std::vector<std::shared_ptr<Vertex>>>& sLav, std::vector<std::shared_ptr<QueueEvent>>& /*queue*/,
                               std::vector<std::shared_ptr<Face>>& /*faces*/) {
  for (std::vector<std::shared_ptr<Vertex>>& lav : sLav) {
    if (lav.size() == 2) {
      std::shared_ptr<Vertex> first = lav[0];
      std::shared_ptr<Vertex> last = Vertex::next(first, lav);

      connectFaces(first->leftFaceNode, last->rightFaceNode);
      connectFaces(first->rightFaceNode, last->leftFaceNode);

      first->processed = true;
      last->processed = true;

      Vertex::removeFromLav(first, lav);
      Vertex::removeFromLav(last, lav);
    }
  }
}

static void removeEventsUnderHeight(std::vector<std::shared_ptr<QueueEvent>>& queue, double levelHeight) {
  while (!queue.empty()) {
    if (queue[0]->distance > levelHeight + EPSILON) {
      break;
    }
    queue.erase(queue.begin());
  }
}

static void removeEmptyLav(std::vector<std::vector<std::shared_ptr<Vertex>>>& sLav) {
  for (unsigned i = 0; i < sLav.size(); i++) {
    if (sLav[i].empty()) {
      sLav.erase(sLav.begin() + i);
      i--;
    }
  }
}

[[maybe_unused]] static std::vector<std::vector<Point3d>> doStraightSkeleton(std::vector<Point3d>& polygon, double roofPitchDegrees) {

  /* Straight skeleton algorithm implementation. Based on highly modified Petr
  * Felkel and Stepan Obdrzalek algorithm.
  * Translated from https://github.com/kendzi/kendzi-math
  */

  std::vector<std::shared_ptr<QueueEvent>> queue;
  std::vector<std::shared_ptr<Face>> faces;
  std::vector<std::shared_ptr<Edge>> edges;
  std::vector<std::vector<std::shared_ptr<Vertex>>> sLav;

  double zcoord = initPolygon(polygon);
  makeCounterClockwise(polygon);
  initSlav(polygon, sLav, edges, faces);
  initEvents(sLav, queue, edges);

  int count = 0;

  while (!queue.empty()) {
    // start processing skeleton level
    count = assertMaxNumberOfIterations(count);

    std::sort(queue.begin(), queue.end(), QueueEvent::Comparer());

    double levelHeight = queue[0]->distance;

    std::vector<LevelEvent> levelEvents = loadAndGroupLevelEvents(queue, sLav);

    for (LevelEvent& event : levelEvents) {

      if (event.obsolete) {
        /*
        * Event is outdated some of parent vertex was processed
        * before
        */
        continue;
      }

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

    processTwoNodeLavs(sLav, queue, faces);
    removeEventsUnderHeight(queue, levelHeight);
    removeEmptyLav(sLav);
  }

  return facesToPoint3d(faces, roofPitchDegrees, zcoord);
}

static std::vector<Point3d> getGableTopAndBottomVertices(const std::vector<Point3d>& surface) {
  std::vector<Point3d> ret;
  if (surface.size() != 3) {
    // gable must have 3 vertices
    return ret;
  }
  if (surface[0].z() > surface[1].z() && surface[0].z() > surface[2].z() && surface[1].z() == surface[2].z()) {
    ret.push_back(surface[0]);  // top
    ret.push_back(surface[1]);  // bottom
    ret.push_back(surface[2]);  // bottom
  } else if (surface[1].z() > surface[0].z() && surface[1].z() > surface[2].z() && surface[0].z() == surface[2].z()) {
    ret.push_back(surface[1]);  // top
    ret.push_back(surface[0]);  // bottom
    ret.push_back(surface[2]);  // bottom
  } else if (surface[2].z() > surface[0].z() && surface[2].z() > surface[1].z() && surface[0].z() == surface[1].z()) {
    ret.push_back(surface[2]);  // top
    ret.push_back(surface[0]);  // bottom
    ret.push_back(surface[1]);  // bottom
  }
  return ret;
}

static int getOppositeGableIndex(const std::vector<std::vector<Point3d>>& surfaces, const std::vector<unsigned>& connectedSurfaces,
                                 unsigned gableIndexNum) {
  // Obtain opposite gable index relative to gableIndexNum
  if (connectedSurfaces.size() < 4) {
    // There must be at least 4 connected surfaces (including gableIndexNum) for
    // there to be an opposite gable.
    return -1;
  }
  std::vector<Point3d> baseVertices = getGableTopAndBottomVertices(surfaces[gableIndexNum]);
  Edge base1 = Edge(baseVertices[0], baseVertices[1]);
  Edge base2 = Edge(baseVertices[0], baseVertices[2]);
  for (unsigned i = 0; i < connectedSurfaces.size(); i++) {
    if (connectedSurfaces[i] == gableIndexNum) {
      continue;
    }
    std::vector<Point3d> tryVertices = getGableTopAndBottomVertices(surfaces[connectedSurfaces[i]]);
    if (tryVertices.empty()) {
      continue;
    }
    Edge try1 = Edge(tryVertices[0], tryVertices[1]);
    Edge try2 = Edge(tryVertices[0], tryVertices[2]);
    if (base1 != try1 && base1 != try2 && base2 != try1 && base2 != try2) {
      return i;
    }
  }
  return -1;
}

static void applyGableLogicTriangles(std::vector<std::vector<Point3d>>& surfaces) {
  // For any roof surface that has 3 vertices, convert from an angled
  // surface to a gable surface.

  std::set<int> processedSurfaces;

  for (unsigned i = 0; i < surfaces.size(); i++) {
    if (std::find(processedSurfaces.begin(), processedSurfaces.end(), i) != processedSurfaces.end()) {
      continue;  // already processed
    }

    std::vector<Point3d>& gable = surfaces[i];
    std::vector<Point3d> gableVertices = getGableTopAndBottomVertices(gable);
    if (gableVertices.empty()) {
      continue;
    }

    processedSurfaces.insert(i);
    Point3d gableTop = gableVertices[0];

    std::vector<unsigned> connectedSurfaces;
    for (unsigned j = 0; j < surfaces.size(); j++) {
      for (const Point3d& vertex : surfaces[j]) {
        if (vertex != gableTop) {
          continue;
        }
        connectedSurfaces.push_back(j);
        processedSurfaces.insert(j);  // will be processed below
      }
    }

    int oppositeGableIndex = getOppositeGableIndex(surfaces, connectedSurfaces, i);

    if (oppositeGableIndex != -1) {
      // Two gables opposite each other meet, we'll need to create a (arbitrary) ridge line.

      /*                  ___________          ___________
      *                  |\         /|        |     |     |
      *                  |  \     /  |        |     |     |
      *                  |    \ /    |        |     |     |
      *                  |    / \    |   =>   |     |     |
      *                  |  /     \  |        |     |     |
      *                  |/_________\|        |_____|_____|
      */

      // Shift gable top vertex for opposite surfaces
      std::vector<Point3d> newVertices;

      std::vector<unsigned> gableSurfaceIndices = {i, connectedSurfaces[oppositeGableIndex]};
      for (unsigned j = 0; j < gableSurfaceIndices.size(); j++) {
        std::vector<Point3d>& surface = surfaces[gableSurfaceIndices[j]];
        for (Point3d& vertex : surface) {
          if (vertex != gableTop) {
            continue;
          }
          std::vector<Point3d> surfVertices = getGableTopAndBottomVertices(surface);
          Point3d newVertex =
            Point3d((surfVertices[1].x() + surfVertices[2].x()) / 2.0, (surfVertices[1].y() + surfVertices[2].y()) / 2.0, surfVertices[0].z());
          newVertices.push_back(newVertex);
          vertex = newVertex;
        }
      }

      // Split gable top vertex (create ridge) for other surfaces
      for (unsigned j = 0; j < connectedSurfaces.size(); j++) {
        if (std::find(gableSurfaceIndices.begin(), gableSurfaceIndices.end(), connectedSurfaces[j]) != gableSurfaceIndices.end()) {
          continue;  // already processed these surfaces
        }
        std::vector<Point3d>& surface = surfaces[connectedSurfaces[j]];
        std::vector<Point3d> trySurface1;
        std::vector<Point3d> trySurface2;
        std::vector<Edge> tryEdges1;
        std::vector<Edge> tryEdges2;
        for (unsigned k = 0; k < surface.size(); k++) {
          Point3d vertex = surface[k];
          if (vertex != gableTop) {
            trySurface1.push_back(vertex);
            trySurface2.push_back(vertex);
            continue;
          }
          int prevk = k - 1 + surface.size();
          int nextk = k + 1;
          Point3d previousVertex = surface[prevk % surface.size()];
          Point3d nextVertex = surface[nextk % surface.size()];

          trySurface1.push_back(newVertices[0]);
          trySurface1.push_back(newVertices[1]);
          tryEdges1.push_back(Edge(newVertices[0], previousVertex));
          tryEdges1.push_back(Edge(newVertices[1], nextVertex));

          trySurface2.push_back(newVertices[1]);
          trySurface2.push_back(newVertices[0]);
          tryEdges2.push_back(Edge(newVertices[1], previousVertex));
          tryEdges2.push_back(Edge(newVertices[0], nextVertex));
        }

        // Correct surface will have no edge intersections
        LineLinear2d l1a = LineLinear2d(tryEdges1[0].begin, tryEdges1[0].end);
        LineLinear2d l1b = LineLinear2d(tryEdges1[1].begin, tryEdges1[1].end);
        LineLinear2d l2a = LineLinear2d(tryEdges2[0].begin, tryEdges2[0].end);
        LineLinear2d l2b = LineLinear2d(tryEdges2[1].begin, tryEdges2[1].end);

        if (l1a.collide(l1b) && !l2a.collide(l2b)) {
          surface = trySurface2;  // trySurface1 has self intersection
        } else if (l2a.collide(l2b) && !l1a.collide(l1b)) {
          surface = trySurface1;  // trySurface2 has self intersection
        } else {
          LOG_AND_THROW("Could not create gable ridge.");
        }
      }

    } else {

      // Shift gable top vertex for all connected surfaces.

      /*                  ___________          ___________
      *                  |\         /|        |     |     |
      *                  |  \     /  |        |     |     |
      *                  |    \ /    |        |     |     |
      *                  |     |     |        |     |     |
      *                  |     |     |   =>   |     |     |
      *                  |    / \    |        |     |     |
      *                  |  /     \  |        |     |     |
      *                  |/_________\|        |_____|_____|
      */

      Point3d newVertex =
        Point3d((gableVertices[1].x() + gableVertices[2].x()) / 2.0, (gableVertices[1].y() + gableVertices[2].y()) / 2.0, gableVertices[0].z());
      for (unsigned j = 0; j < connectedSurfaces.size(); j++) {
        std::vector<Point3d>& surface = surfaces[connectedSurfaces[j]];
        for (Point3d& vertex : surface) {
          if (vertex != gableTop) {
            continue;
          }
          vertex = newVertex;
        }
      }
    }
  }
}

static void applyGableLogicRidgeTwoAnglesForward(const std::vector<std::vector<Point3d>>& surfaces) {
  // TODO

  /*                  ___________          ___________
  *                  |     |     |        |     |     |
  *                  |     |     |        |     |     |
  *                  |     |     |        |     |     |
  *                  |     |     |        |     |     |
  *                  |    / \    |   =>   |     |     |
  *                  |   /   \   |        |     |     |
  *                  |  /     |  |        |     |     |
  *                  | /     /|  |        |     |     |
  *                  |/_____/ |  |        |_____|_____|
  *                        |  |  |              |  |  |
  *                        |__|__|              |__|__|
  */
}

static void applyGableLogicRidgeTwoAnglesInside(const std::vector<std::vector<Point3d>>& surfaces) {
  // TODO

  /*                  ___________          ___________
  *                  |     |     |        |     |     |
  *                  |     |     |        |     |     |
  *                  |     |     |        |     |     |
  *                  |     |     |        |     |     |
  *                  |    / \    |   =>   |     |     |
  *                  |   /   \   |        |     |     |
  *                  |  /     \  |        |     |     |
  *                  | /   /|\ \ |        |     |     |
  *                  |/___/ | \_\|        |_____|_____|
  *                       | | |                | | |
  *                       |_|_|                |_|_|
  */
}

static void applyGableLogicRidgeTwoAnglesBackward(const std::vector<std::vector<Point3d>>& surfaces) {
  // TODO

  /*    _______________________          _______________________
  *    |                      /|        |                       |
  *    |                     / |        |                       |
  *    |                    /  |        |                       |
  *    |                   /   |        |                       |
  *    |-------------------    |        |-----------------------|
  *    |                   \   |   =>   |                       |
  *    |                    \  |        |                       |
  *    |                    |  |        |                       |
  *    |                   /|  |        |                   /|\ |
  *    |__________________/ |  |        |__________________/ | \|
  *                      |  |  |                          |  |  |
  *                      |  |  |                          |  |  |
  *                      |__|__|                          |__|__|
  */
}

static void applyGableLogicTwoRidgesTwoOppositeAngles(const std::vector<std::vector<Point3d>>& surfaces) {
  // TODO

  /*    _______________________          _______________________
  *    |                      /|        |                       |
  *    |                     / |        |                       |
  *    |                    /  |        |                       |
  *    |                   /   |        |                       |
  *    |-------------------    |        |-----------------------|
  *    |                  /|   |   =>   |                 /|\   |
  *    |                 / |   |        |                / | \  |
  *    |                /  |   |        |               /  |  \ |
  *    |               /   |   |        |              /   |   \|
  *    |______________/    |   |        |_____________/    |    |
  *                   |    |   |                      |    |    |
  *                   |    |   |                      |    |    |
  *                   |____|___|                      |____|____|
  */
}

[[maybe_unused]] static void applyGables(std::vector<std::vector<Point3d>>& surfaces) {
  // Convert hip roof to gable roof

  // Simple logic
  applyGableLogicTriangles(surfaces);

  // Complex logic
  applyGableLogicRidgeTwoAnglesForward(surfaces);
  applyGableLogicRidgeTwoAnglesInside(surfaces);
  applyGableLogicRidgeTwoAnglesBackward(surfaces);
  applyGableLogicTwoRidgesTwoOppositeAngles(surfaces);
}

struct Point3dComparer  // needed to use std::map<Point3d, foo>
{
  bool operator()(Point3d p1, Point3d p2) const {
    if (p1.x() < p2.x()) {
      return true;
    } else if (p1.x() == p2.x()) {
      return (p1.y() < p2.y());
    }
    return false;
  }
};

static std::vector<Point3d> getShedLine(const std::vector<Point3d>& polygon, double directionDegrees) {
  boost::optional<Point3d> centroid = getCentroid(polygon);
  if (!centroid) {
    LOG_AND_THROW("Could not obtain centroid for polygon.");
  }

  // Get max distance of any polygon vertex from centroid
  double maxDistance = 0.0;
  for (const Point3d& vertex : polygon) {
    double distance = getDistance(vertex, centroid.get());
    if (distance > maxDistance) {
      maxDistance = distance;
    }
  }
  maxDistance *= 2.0;  // add buffer

  double angleStandard = 90 - directionDegrees;

  // Construct line segment through centroid perpendicular to directionDegrees
  double angleRad1 = degToRad(angleStandard - 90.0);
  double angleRad2 = degToRad(angleStandard + 90.0);
  Point3d start = Point3d(centroid.get().x() + maxDistance * cos(angleRad1), centroid.get().y() + maxDistance * sin(angleRad1), 0.0);
  Point3d end = Point3d(centroid.get().x() + maxDistance * cos(angleRad2), centroid.get().y() + maxDistance * sin(angleRad2), 0.0);

  // Shift line segment in opposite direction of directionDegrees
  double angle = atan2(end.x() - start.x(), end.y() - start.y());
  double dx = -maxDistance * cos(angle);
  double dy = maxDistance * sin(angle);
  start += Vector3d(dx, dy, 0.0);
  end += Vector3d(dx, dy, 0.0);

  std::vector<Point3d> line = {start, end};
  return line;
}

[[maybe_unused]] static std::vector<std::vector<Point3d>> doShedRoof(std::vector<Point3d>& polygon, double roofPitchDegrees,
                                                                     double directionDegrees) {
  std::vector<std::vector<Point3d>> surfaces;

  double zcoord = initPolygon(polygon);

  // Define arbitrary line outside of the polygon based on directionDegrees. The
  // closest point to the line defines the start (height = 0) of the shed roof.
  std::vector<Point3d> line = getShedLine(polygon, directionDegrees);

  // Calculate distance from each polygon vertex to the line.
  std::map<Point3d, double, Point3dComparer> distances;
  double minDistance = std::numeric_limits<double>::max();
  for (const Point3d& vertex : polygon) {
    distances[vertex] = getDistancePointToLineSegment(vertex, line);
    if (distances[vertex] < minDistance) {
      minDistance = distances[vertex];
    }
  }

  // Reduce all vertex distances by minimum vertex distance. Combined with
  // roofPitchDegrees, this defines the height of the shed roof vertex.
  double roofSlope = tan(degToRad(roofPitchDegrees));
  for (const auto& element : distances) {
    distances[element.first] = element.second - minDistance;
  }
  for (Point3d& vertex : polygon) {
    vertex += Vector3d(0.0, 0.0, zcoord + distances[vertex] * roofSlope);
  }

  // Construct vertical walls for each polygon edge up to the shed roof.
  for (unsigned i = 0; i < polygon.size(); i++) {
    Point3d vertex = polygon[i];
    Point3d nextVertex = polygon[(i + 1) % polygon.size()];
    std::vector<Point3d> wall = {vertex, nextVertex};
    if (nextVertex.z() - zcoord > EPSILON) {
      Point3d nextVertexFloor = Point3d(nextVertex.x(), nextVertex.y(), zcoord);
      wall.push_back(nextVertexFloor);
    }
    if (vertex.z() - zcoord > EPSILON) {
      Point3d vertexFloor = Point3d(vertex.x(), vertex.y(), zcoord);
      wall.push_back(vertexFloor);
    }
    if (wall.size() > 2) {
      surfaces.push_back(wall);
    }
  }
  surfaces.push_back(reverse(polygon));

  return surfaces;
}
}  // namespace openstudio
