#include "collider.hpp"
#include "DIDebuger/log.hpp"
#include "DIUtils/utils.hpp"

namespace DI{

  namespace PhysicsHandler{
    bool IsCollide_Point_Point(ColliderPoint* point_a, ColliderPoint* point_b){
      if (point_a->pos.x == point_b->pos.x && point_a->pos.y == point_b->pos.y)
        return true;
      return false;
    }
    bool IsCollide_Point_Square(ColliderPoint* point, ColliderSquare* square){
      if (point->pos.x >= square->pos.x &&                      // right of the left edge AND
          point->pos.x <= square->pos.x + square->size.x &&     // left of the right edge AND
          point->pos.y >= square->pos.y &&                      // below the top AND
          point->pos.y <= square->pos.y + square->size.y &&
          point->mask & square->group) {    // above the bottom
              return true;
      }
      return false;
    }
    bool _isCollide_Point_Circle(float pointX, float pointY, float circleX, float circleY, float radius){
      float distance = Utils::DistanceBetween(pointX,pointY,circleX,circleY);
      if (distance <= radius) {
          return true;
      }
      return false;
    }
    bool IsCollide_Point_Circle(ColliderPoint* point, ColliderCircle* circle){
      float distance = Utils::DistanceBetween(point->pos.x,point->pos.y,circle->pos.x,circle->pos.y);
      if (distance <= circle->radius) {
          return true;
      }
      return false;
    }
    bool _isCollide_Point_Line(int pointX, int pointY,int startLineX, int startLineY, int endLineX, int endLineY){
      float lenghth = Utils::DistanceBetween(startLineX,startLineY,endLineX,endLineY);
      float distToStartLine = Utils::DistanceBetween(pointX,pointY,startLineX,startLineY );
      float distToEndLine = Utils::DistanceBetween(pointX,pointY,endLineX,endLineY );
      float accuracy = 0.1;
      if (distToStartLine + distToEndLine >= lenghth - accuracy &&
          distToStartLine + distToEndLine <= lenghth + accuracy)
        return true;
      return false;
    }
    bool IsCollide_Point_Line(ColliderPoint* point, ColliderLine* line){
      float lenghth = Utils::DistanceBetween(line->start.x,line->start.y,line->end.x,line->end.y);
      float distToStartLine = Utils::DistanceBetween(point->pos.x,point->pos.y,line->start.x,line->start.y );
      float distToEndLine = Utils::DistanceBetween(point->pos.x,point->pos.y,line->end.x,line->end.y );
      float accuracy = 0.1;
      if (distToStartLine + distToEndLine >= lenghth - accuracy &&
          distToStartLine + distToEndLine <= lenghth + accuracy)
        return true;
      return false;
    }
    bool IsCollide_Point_Polygon(ColliderPoint* point, ColliderPolygon* polygon);

    bool IsCollide_Circle_Circle(ColliderCircle* circle_a, ColliderCircle* circle_b){
      float distance = Utils::DistanceBetween(circle_a->pos.x,circle_a->pos.y,circle_b->pos.x,circle_b->pos.y);
      if (distance <= circle_a->radius + circle_b->radius) {
          return true;
      }
      return false;
    }
    bool IsCollide_Circle_Square(ColliderCircle* circle, ColliderSquare* square){
      int testX;
      int testY;
      if (circle->pos.x < square->pos.x)         testX = square->pos.x;        // left edge
      else if (circle->pos.x > square->pos.x + square->size.x) testX = square->pos.x + square->size.x;     // right edge
      if (circle->pos.y < square->pos.y)         testY = square->pos.y;        // top edge
      else if (circle->pos.y > square->pos.y + square->size.y) testY = square->pos.y + square->size.y;     // bottom edge
      float distX = circle->pos.x-testX;

      float distY = circle->pos.y-testY;
      float distance = sqrt( (distX*distX) + (distY*distY) );

      if (distance <= circle->radius) {
          return true;
      }
      return false;
    }
    bool IsCollide_Circle_Line(ColliderCircle* circle, ColliderLine* line){
      bool inside1 = _isCollide_Point_Circle(line->start.x,line->start.y, circle->pos.x,circle->pos.y,circle->radius);
      bool inside2 = _isCollide_Point_Circle(line->end.x,line->end.y, circle->pos.x,circle->pos.y,circle->radius);
      if (inside1 || inside2) return true;
      float distX = line->start.x - line->end.x;
      float distY = line->start.y - line->end.y;
      float len = sqrt( pow(distX,2) + pow(distY,2) );
      float dot = ( ((circle->pos.x-line->start.x)*(line->end.x-line->start.x)) + ((circle->pos.y-line->start.y)*(line->end.y-line->start.y)) ) / pow(len,2);
      float closestX = line->start.x + (dot * (line->end.x-line->start.x));
      float closestY = line->start.y + (dot * (line->end.y-line->start.y));
      bool onSegment = _isCollide_Point_Line(closestX,closestY,line->start.x,line->start.y,line->end.x,line->end.y);
      if (!onSegment) return false;
      distX = closestX - circle->pos.x;
      distY = closestY - circle->pos.y;
      float distance = sqrt( pow(distX,2) + pow(distY,2) );
      if (distance <= circle->radius) {
          return true;
      }
      return false;
    }
    bool IsCollide_Circle_Polygon(ColliderCircle* circle, ColliderPolygon* polygon);

    bool IsCollide_Square_Square(ColliderSquare* square_a, ColliderSquare* square_b){
      if (square_a->pos.x + square_a->size.x >= square_b->pos.x &&
          square_a->pos.x <= square_b->pos.x + square_b->size.x &&
          square_a->pos.y + square_a->size.y >= square_b->pos.y &&
          square_a->pos.y <= square_b->pos.y + square_b->size.y)
        return true;
      return false;
    }
    bool _isCollide_Line_Line(int startX1, int startY1, int endX1, int endY1,int startX2, int startY2, int endX2, int endY2){
      float x1 = startX1, x3 = startX2;
      float y1 = startY1, y3 = startY2;
      float x2 = endX1, x4 = endX2;
      float y2 = endY1, y4 = endY2;
      float uA = ((x4-x3)*(y1-y3) - (y4-y3)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));
      float uB = ((x2-x1)*(y1-y3) - (y2-y1)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));
      
      if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {
          return true;
      }
      return false;
    }
    bool IsCollide_Square_Line(ColliderSquare* square, ColliderLine* line){
      float x1 = line->start.x;
      float y1 = line->start.y;
      float x2 = line->end.x;
      float y2 = line->end.y;
      int rx = square->pos.x;
      int ry = square->pos.y;
      int rw = square->size.x;
      int rh = square->size.y;
      bool left =   _isCollide_Line_Line(x1,y1,x2,y2, rx,ry,rx, ry+rh);
      bool right =  _isCollide_Line_Line(x1,y1,x2,y2, rx+rw,ry, rx+rw,ry+rh);
      bool top =    _isCollide_Line_Line(x1,y1,x2,y2, rx,ry, rx+rw,ry);
      bool bottom = _isCollide_Line_Line(x1,y1,x2,y2, rx,ry+rh, rx+rw,ry+rh); 
      if (left || right || top || bottom) {
          return true;
      }
      return false;
    }
    bool IsCollide_Square_Polygon(ColliderSquare* square, ColliderPolygon* polygon);

    bool IsCollide_Line_Line(ColliderLine* line_a, ColliderLine* line_b){
      float x1 = line_a->start.x, x3 = line_b->start.x;
      float y1 = line_a->start.y, y3 = line_b->start.y;
      float x2 = line_a->end.x, x4 = line_b->end.x;
      float y2 = line_a->end.y, y4 = line_b->end.y;
      float uA = ((x4-x3)*(y1-y3) - (y4-y3)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));
      float uB = ((x2-x1)*(y1-y3) - (y2-y1)*(x1-x3)) / ((y4-y3)*(x2-x1) - (x4-x3)*(y2-y1));
      
      if (uA >= 0 && uA <= 1 && uB >= 0 && uB <= 1) {
          return true;
      }
      return false;
    }
    bool IsCollide_Line_Polygon(ColliderLine* line, ColliderPolygon* polygon);

    bool IsCollide_Polygon_Polygon(ColliderPolygon* polygon_a, ColliderPolygon* polygon_b);



    void UnSet(Collider* cldr){
      delete cldr;
    }

    void _setCollider(Collider* cldr, int group, int mask){
      cldr->group = group;
      cldr->mask = mask;
    }
    // Set diffrent types of collider{}
    void SetColliderPoint(ColliderPoint* cldr,glm::vec2 pos, int group, int mask){
      _setCollider(cldr,group,mask);
      cldr->pos = pos;
    }
    void SetColliderSquare(ColliderSquare* cldr,glm::vec2 pos, glm::vec2 size, int group, int mask){
      SetColliderPoint(cldr,pos,group,mask);
      cldr->size = size;
    }
    void SetColliderLine(ColliderLine* cldr,  glm::vec2 start, glm::vec2 end, int group, int mask){
      cldr->start = start;
      cldr->end = end;
    }
    void SetColliderCircle(ColliderCircle* cldr, glm::vec2 pos, int radius, int group, int mask){
      SetColliderPoint(cldr,pos,group,mask);
      cldr->radius = radius;
    }

  }
}
