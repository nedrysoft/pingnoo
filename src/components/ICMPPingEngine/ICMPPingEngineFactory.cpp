#include "ICMPPingEngineFactory.h"
#include "ICMPPingEngine.h"

class FizzyAde::ICMPPingEngine::ICMPPingEngineFactoryData
{

public:
    ICMPPingEngineFactoryData(FizzyAde::ICMPPingEngine::ICMPPingEngineFactory *parent)
    {
        m_factory = parent;
    }

    friend class ICMPPingEngineFactory;

private:
    FizzyAde::ICMPPingEngine::ICMPPingEngineFactory *m_factory;

    QList<FizzyAde::ICMPPingEngine::ICMPPingEngine *> m_engineList;
};

FizzyAde::ICMPPingEngine::ICMPPingEngineFactory::ICMPPingEngineFactory() :
   d(std::make_shared<FizzyAde::ICMPPingEngine::ICMPPingEngineFactoryData>(this))
{

}

FizzyAde::ICMPPingEngine::ICMPPingEngineFactory::~ICMPPingEngineFactory()
{
    for(auto engineInstance : d->m_engineList) {
        delete engineInstance;
    }
}

FizzyAde::Core::IPingEngine *FizzyAde::ICMPPingEngine::ICMPPingEngineFactory::createEngine(FizzyAde::Core::IPVersion version)
{
    auto engineInstance = new FizzyAde::ICMPPingEngine::ICMPPingEngine(version);

    d->m_engineList.append(engineInstance);

    return engineInstance;
}

QJsonObject FizzyAde::ICMPPingEngine::ICMPPingEngineFactory::saveConfiguration()
{
    return QJsonObject();
}

bool FizzyAde::ICMPPingEngine::ICMPPingEngineFactory::loadConfiguration(QJsonObject configuration)
{
    Q_UNUSED(configuration)

    return false;
}

QString FizzyAde::ICMPPingEngine::ICMPPingEngineFactory::description()
{
    return tr("ICMP Ping Engine");
}

