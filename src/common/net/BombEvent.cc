#include "BombEvent.h"
#include "EventId.h"

namespace common {
namespace net {

BombEvent::BombEvent(QPoint position, quint32 id, quint64 timestamp)
    : InGameEvent(id, timestamp),
      position_(position) {

}

QPoint BombEvent::GetPosition() const {
  return position_;
}

void BombEvent::Serialize(QDataStream& stream) const {
  stream << (quint32) EventId::kBombEventId;
  SerializeBaseEvent(stream);
  stream << this->GetPosition();
}

void BombEvent::Accept(GameEventVisitor& visitor) {
  visitor.Visit(*this);
}

bool BombEvent::operator==(const BombEvent& event) const {
  return Event::operator==(event) && position_ == event.position_;
}

}
}
