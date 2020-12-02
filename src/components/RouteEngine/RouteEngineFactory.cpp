#include "RouteEngineFactory.h"
#include "RouteEngine.h"

class Nedrysoft::RouteEngine::RouteEngineFactoryData {

    public:
        RouteEngineFactoryData(Nedrysoft::RouteEngine::RouteEngineFactory *parent) {
            m_factory = parent;
        }

        friend class RouteEngineFactory;

    private:
        Nedrysoft::RouteEngine::RouteEngineFactory *m_factory;

        QList<Nedrysoft::RouteEngine::RouteEngine *> m_engineList;
};

Nedrysoft::RouteEngine::RouteEngineFactory::RouteEngineFactory() :
        d(std::make_shared<Nedrysoft::RouteEngine::RouteEngineFactoryData>(this)) {

}

Nedrysoft::RouteEngine::RouteEngineFactory::~RouteEngineFactory() {
    for (auto engineInstance : d->m_engineList) {
        delete engineInstance;
    }
}

Nedrysoft::Core::IRouteEngine *Nedrysoft::RouteEngine::RouteEngineFactory::createEngine() {
    auto engineInstance = new Nedrysoft::RouteEngine::RouteEngine();

    d->m_engineList.append(engineInstance);

    return engineInstance;
}

QJsonObject Nedrysoft::RouteEngine::RouteEngineFactory::saveConfiguration() {
    return QJsonObject();
}

bool Nedrysoft::RouteEngine::RouteEngineFactory::loadConfiguration(QJsonObject configuration) {
    Q_UNUSED(configuration)

    return false;
}

QString Nedrysoft::RouteEngine::RouteEngineFactory::description() {
    return tr("Route Engine");
}

