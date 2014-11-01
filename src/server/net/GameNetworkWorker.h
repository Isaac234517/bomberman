#ifndef SRC_SERVER_NET_NETWORKWORKER_H_
#define SRC_SERVER_NET_NETWORKWORKER_H_

#include <memory>
#include <QObject>
#include <QUdpSocket>
#include <QDataStream>
#include <QTimer>
#include "src/common/GameTimer.h"
#include "src/common/net/BombEvent.h"
#include "src/common/net/MoveEvent.h"
#include "src/common/net/PlayerLeftEvent.h"
#include "BaseClientEvent.h"
#include "ClientEvent.h"
#include "Client.h"
#include "EmitterVisitor.h"
#include "src/common/World.h"

#define EVENT_READY_LOG_LEVEL 3

using common::GameTimer;
using common::World;
using namespace common::net;

namespace net {

class GameNetworkWorker : public QObject {
  Q_OBJECT

  public:
    GameNetworkWorker(quint16 port, std::shared_ptr<GameTimer> game_timer, std::weak_ptr<World> world, std::vector<Client> clients);

  signals:
    void BombEventReceived(ClientEvent<BombEvent> event);
    void MoveEventReceived(ClientEvent<MoveEvent> event);
    void PlayerLeftEventReceived(ClientEvent<PlayerLeftEvent> event);

  private slots:
    void ReadPendingDatagrams();
    void SocketError(QAbstractSocket::SocketError error);

  private:
    quint16 port_;
    std::shared_ptr<GameTimer> game_timer_;
    std::map<int, Client> clients_;
    QUdpSocket socket_;
    quint32 last_packet_id_;
    std::map<quint8, std::map<quint32, std::unique_ptr<BaseClientEvent>>> event_cache_;
    std::map<quint8, quint32> last_event_ids_;
    std::weak_ptr<World> world_ptr_;
    QTimer send_entities_timer_;

    static const unsigned char kServerVersion = 0x01;
    static const unsigned char kProtocolId = 0xBC;
    static const unsigned char kPingPacketId = 0x01;
    static const unsigned char kEventPacketId = 0x02;
    static const unsigned char kEntitiesPacketId = 0x03;

    quint32 GetNextPacketId();

    quint32 PrepareHeader(QDataStream& stream, quint8 packet_type);
    void ProcessDatagram(const QByteArray& datagram);
    bool CheckProtocolAndVersion(QDataStream& stream);
    quint32 GetPacketId(QDataStream& stream);
    quint8 GetPacketType(QDataStream& stream);
    quint8 GetClientId(QDataStream& stream);
    void ProcessPingPacket(QDataStream& stream, const Client& client, quint32 packet_id);
    void ProcessEventPacket(QDataStream& stream, const Client& client, quint32 packet_id);
    void HandlePendingEvent(std::unique_ptr<BaseClientEvent> event);
    void EmitReadyEvents(quint8 client_id);
    void SendPongPacket(const Client& client, quint32 packet_id);

    void BroadcastWorld();

    bool CheckStreamStatus(const QDataStream& stream) const;

    friend class EmitterVisitor;
    void EmitEvent(ClientEvent<BombEvent> event);
    void EmitEvent(ClientEvent<MoveEvent> event);
    void EmitEvent(ClientEvent<PlayerLeftEvent> event);

};

}
#endif