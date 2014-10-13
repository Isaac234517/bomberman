#include "Event.h"

namespace common {
namespace net {

Event::Event(quint32 id, quint64 timestamp) :
    id_(id),
    timestamp_(timestamp) {
}

Event::Event(quint64 timestamp) :
  Event(0, timestamp) {}

Event::~Event() {}

quint32 Event::GetId() const {
  return id_;
}

void Event::SetId(quint32 id) {
  id_ = id;
}

quint64 Event::GetTimestamp() const {
  return timestamp_;
}

QString Event::GetClientVersion() const {
  return QString(CLIENT_VERSION);
}

void Event::SerializeBaseEvent(QDataStream& stream) const {
  stream << GetClientVersion();
  stream << GetId();
  stream << GetTimestamp();
}

bool Event::operator==(const Event& event) const {
  return id_ == event.id_ && timestamp_ == event.timestamp_;
}

QDataStream& operator<<(QDataStream& stream, const Event& event) {
  event.Serialize(stream);
  return stream;
}

}
}