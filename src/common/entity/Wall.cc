
#include "Wall.h"

namespace common {
namespace entity {

Wall::Wall(QPoint position) :
  Entity(position, true, true, "res/wall.png")
{

}

void Wall::Update(std::weak_ptr<GameEngine> game_engine, int t) {
  (void) game_engine;
  (void) t;
}

}
}