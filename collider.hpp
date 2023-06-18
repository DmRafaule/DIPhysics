#pragma once 

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace DI{

  struct Collider{
    int group;
    int mask;
  };
  struct ColliderPoint : public Collider{
    glm::vec2 pos;
  };
  struct ColliderSquare : public ColliderPoint{
    glm::vec2 size;
  };
  struct ColliderCircle : public ColliderPoint{
    int radius;
  };
  struct ColliderLine : public Collider{
    glm::vec2 start;
    glm::vec2 end;
  };
  struct ColliderPolygon : public Collider{
    // NOTE: PLACEHOLDER
  };

  namespace PhysicsHandler{
    // Colliders checkers
    bool IsCollide_Point_Point(ColliderPoint* point_a, ColliderPoint* point_b);
    bool IsCollide_Point_Square(ColliderPoint* point, ColliderSquare* square);
    bool IsCollide_Point_Circle(ColliderPoint* point, ColliderCircle* circle);
    bool IsCollide_Point_Line(ColliderPoint* point, ColliderLine* line);
    bool IsCollide_Point_Polygon(ColliderPoint* point, ColliderPolygon* polygon);

    bool IsCollide_Circle_Circle(ColliderCircle* circle_a, ColliderCircle* circle_b);
    bool IsCollide_Circle_Square(ColliderCircle* circle, ColliderSquare* square);
    bool IsCollide_Circle_Line(ColliderCircle* circle, ColliderLine* line);
    bool IsCollide_Circle_Polygon(ColliderCircle* circle, ColliderPolygon* polygon);

    bool IsCollide_Square_Square(ColliderSquare* square_a, ColliderSquare* square_b);
    bool IsCollide_Square_Line(ColliderSquare* square, ColliderLine* line);
    bool IsCollide_Square_Polygon(ColliderSquare* square, ColliderPolygon* polygon);

    bool IsCollide_Line_Line(ColliderLine* line_a, ColliderLine* line_b);
    bool IsCollide_Line_Polygon(ColliderLine* line, ColliderPolygon* polygon);

    bool IsCollide_Polygon_Polygon(ColliderPolygon* polygon_a, ColliderPolygon* polygon_b);

    void UnSet(Collider* cldr);

    // Set colliders
    // cldf -> id of new Collider struct
    // group -> collider group for 
    // mask -> with what this collider gonna collide with
    void SetColliderPoint(ColliderPoint* cldr, glm::vec2 pos, int group = 0b00000001, int mask = 0b00000001);
    void SetColliderSquare(ColliderSquare* cldr, glm::vec2 pos, glm::vec2 size, int group = 0b00000001, int mask = 0b00000001);
    void SetColliderLine(ColliderLine* cldr, glm::vec2 start, glm::vec2 end, int group = 0b00000001, int mask = 0b00000001);
    void SetColliderCircle(ColliderCircle* cldr, glm::vec2 pos, float radius, int group = 0b00000001, int mask = 0b00000001);
    // NOTE: PLACEHOLDER
    void SetColliderPolygon(ColliderPolygon* cldr);
  }
}
// TODO: Implement SetColliderPolygon function

