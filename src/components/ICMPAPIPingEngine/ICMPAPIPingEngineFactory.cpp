#include "ICMPAPIPingEngineFactory.h"
#include "ICMPAPIPingEngine.h"

class Nedrysoft::Pingnoo::ICMPAPIPingEngineFactoryData {

    public:
        ICMPAPIPingEngineFactoryData(Nedrysoft::Pingnoo::ICMPAPIPingEngineFactory *parent) {
            m_pingEngineFactory = parent;
        }

        friend class ICMPAPIPingEngineFactory;

    private:
        Nedrysoft::Pingnoo::ICMPAPIPingEngineFactory *m_pingEngineFactory;

        QList<Nedrysoft::Pingnoo::ICMPAPIPingEngine *> m_engineList;
};

Nedrysoft::Pingnoo::ICMPAPIPingEngineFactory::ICMPAPIPingEngineFactory() :
        d(std::make_shared<Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingEngineFactoryData>(this)) {

}

Nedrysoft::Pingnoo::ICMPAPIPingEngineFactory::~ICMPAPIPingEngineFactory() {
    for (auto engineInstance : d->m_engineList) {
        delete engineInstance;
    }

    delete d;
}

QObject *Nedrysoft::Pingnoo::ICMPAPIPingEngineFactory::asQObject() {
    return ( this );
}

Nedrysoft::Pingnoo::IPingEngine *Nedrysoft::Pingnoo::ICMPAPIPingEngineFactory::createEngine() {
    auto engineInstance = new Nedrysoft::Pingnoo::ICMPAPIPingEngine;

    d->m_engineList.append(engineInstance);

    return ( engineInstance );
}

QJsonObject Nedrysoft::Pingnoo::ICMPAPIPingEngineFactory::saveConfiguration() {
    return ( QJsonObject());
}

bool Nedrysoft::Pingnoo::ICMPAPIPingEngineFactory::loadConfiguration(QJsonObject configuration) {
    Q_UNUSED(configuration)

    return ( false );
}
