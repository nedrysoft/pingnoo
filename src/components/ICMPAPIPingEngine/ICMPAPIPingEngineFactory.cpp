#include "ICMPAPIPingEngineFactory.h"
#include "ICMPAPIPingEngine.h"

class FizzyAde::Pingnoo::ICMPAPIPingEngineFactoryData
{

public:
    ICMPAPIPingEngineFactoryData(FizzyAde::Pingnoo::ICMPAPIPingEngineFactory* parent)
    {
        m_pingEngineFactory = parent;
    }

    friend class ICMPAPIPingEngineFactory;

private:
    FizzyAde::Pingnoo::ICMPAPIPingEngineFactory* m_pingEngineFactory;

    QList<FizzyAde::Pingnoo::ICMPAPIPingEngine *> m_engineList;
};

FizzyAde::Pingnoo::ICMPAPIPingEngineFactory::ICMPAPIPingEngineFactory() :
    d(std::make_shared<FizzyAde::ICMPAPIPingEngine::ICMPAPIPingEngineFactoryData>(this))
{

}

FizzyAde::Pingnoo::ICMPAPIPingEngineFactory::~ICMPAPIPingEngineFactory()
{
    for(auto engineInstance : d->m_engineList) {
        delete engineInstance;
    }

    delete d;
}

QObject *FizzyAde::Pingnoo::ICMPAPIPingEngineFactory::asQObject()
{
    return(this);
}

FizzyAde::Pingnoo::IPingEngine *FizzyAde::Pingnoo::ICMPAPIPingEngineFactory::createEngine()
{
    auto engineInstance = new FizzyAde::Pingnoo::ICMPAPIPingEngine;

    d->m_engineList.append(engineInstance);

    return(engineInstance);
}

QJsonObject FizzyAde::Pingnoo::ICMPAPIPingEngineFactory::saveConfiguration()
{
    return(QJsonObject());
}

bool FizzyAde::Pingnoo::ICMPAPIPingEngineFactory::loadConfiguration(QJsonObject configuration)
{
    Q_UNUSED(configuration)

    return(false);
}
