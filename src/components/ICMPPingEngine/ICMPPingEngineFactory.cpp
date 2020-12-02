#include "ICMPPingEngineFactory.h"
#include "ICMPPingEngine.h"

class Nedrysoft::ICMPPingEngine::ICMPPingEngineFactoryData {

    public:
        ICMPPingEngineFactoryData(Nedrysoft::ICMPPingEngine::ICMPPingEngineFactory *parent) {
            m_factory = parent;
        }

        friend class ICMPPingEngineFactory;

    private:
        Nedrysoft::ICMPPingEngine::ICMPPingEngineFactory *m_factory;

        QList<Nedrysoft::ICMPPingEngine::ICMPPingEngine *> m_engineList;
};

Nedrysoft::ICMPPingEngine::ICMPPingEngineFactory::ICMPPingEngineFactory() :
        d(std::make_shared<Nedrysoft::ICMPPingEngine::ICMPPingEngineFactoryData>(this)) {

}

Nedrysoft::ICMPPingEngine::ICMPPingEngineFactory::~ICMPPingEngineFactory() {
    for (auto engineInstance : d->m_engineList) {
        delete engineInstance;
    }
}

Nedrysoft::Core::IPingEngine *
Nedrysoft::ICMPPingEngine::ICMPPingEngineFactory::createEngine(Nedrysoft::Core::IPVersion version) {
    auto engineInstance = new Nedrysoft::ICMPPingEngine::ICMPPingEngine(version);

    d->m_engineList.append(engineInstance);

    return engineInstance;
}

QJsonObject Nedrysoft::ICMPPingEngine::ICMPPingEngineFactory::saveConfiguration() {
    return QJsonObject();
}

bool Nedrysoft::ICMPPingEngine::ICMPPingEngineFactory::loadConfiguration(QJsonObject configuration) {
    Q_UNUSED(configuration)

    return false;
}

QString Nedrysoft::ICMPPingEngine::ICMPPingEngineFactory::description() {
    return tr("ICMP Ping Engine");
}

