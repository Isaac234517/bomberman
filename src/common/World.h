#ifndef SRC_SERVER_WORLD_H_
#define SRC_SERVER_WORLD_H_

#include <memory>
#include <QPoint>
#include "src/common/entity/Entity.h"
#include "src/common/entity/Character.h"
#include "GameEngine.h"

namespace common {

typedef std::vector<std::vector<std::vector<std::unique_ptr<entity::Entity> > > > grid_t;

class World {
  public:
    World(std::weak_ptr<GameEngine> game_engine, int width, int height);

    std::weak_ptr<GameEngine> GetGameEngine() const;

    int GetWidth() const;
    int GetHeight() const;
    bool CheckCoord(QPoint p) const;
    bool IsWalkable(QPoint p) const; // Returns true if a character can walk on this case
    bool StopsFire(QPoint p) const; // Returns true if the case stops fire propagation
    bool AddItem(std::unique_ptr<entity::Entity> entity); // Adds an entity to the grid. Returns false if it fail.
    bool AddCharacter(std::unique_ptr<entity::Character> character); // Adds a character to characters_. Returns false if it fail.
    void removeEntities(); // Removes entities that should be removed
    
  private:
    std::weak_ptr<GameEngine> game_engine_;
    int width_; // Width of the world eg number of blocks
    int height_;
    grid_t entities_;
    std::vector<std::unique_ptr<entity::Character> > characters_;
};

}

#endif