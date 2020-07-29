#include "RouteEngineFactory.h"
#include "RouteEngine.h"

class FizzyAde::RouteEngine::RouteEngineFactoryData
{

public:
    RouteEngineFactoryData(FizzyAde::RouteEngine::RouteEngineFactory *parent)
    {
        m_factory = parent;
    }

    friend class RouteEngineFactory;

private:
    FizzyAde::RouteEngine::RouteEngineFactory *m_factory;

    QList<FizzyAde::RouteEngine::RouteEngine *> m_engineList;
};

FizzyAde::RouteEngine::RouteEngineFactory::RouteEngineFactory() :
   d(std::make_shared<FizzyAde::RouteEngine::RouteEngineFactoryData>(this))
{

}

FizzyAde::RouteEngine::RouteEngineFactory::~RouteEngineFactory()
{
    for(auto engineInstance : d->m_engineList) {
        delete engineInstance;
    }
}

FizzyAde::Core::IRouteEngine *FizzyAde::RouteEngine::RouteEngineFactory::createEngine()
{
    auto engineInstance = new FizzyAde::RouteEngine::RouteEngine();

    d->m_engineList.append(engineInstance);

    return engineInstance;
}

QJsonObject FizzyAde::RouteEngine::RouteEngineFactory::saveConfiguration()
{
    return QJsonObject();
}

bool FizzyAde::RouteEngine::RouteEngineFactory::loadConfiguration(QJsonObject configuration)
{
    Q_UNUSED(configuration)

    return false;
}

QString FizzyAde::RouteEngine::RouteEngineFactory::description()
{
    return tr("Route Engine");
}

