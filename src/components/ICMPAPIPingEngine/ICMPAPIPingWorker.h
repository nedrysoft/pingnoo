//
// Created by adriancarpenter on 18/04/2021.
//

#ifndef PINGNOO_ICMPAPIPINGWORKER_H
#define PINGNOO_ICMPAPIPINGWORKER_H

#include <QObject>

#include "Core/IPingEngine.h"

namespace Nedrysoft::ICMPAPIPingEngine {
    class ICMPAPIPingWorker : public QObject {
        private:
            Q_OBJECT

        public:
            ICMPAPIPingWorker();

            Q_SLOT void doWork();

            Q_SIGNAL void result(Nedrysoft::Core::PingResult result);

        private:
            QString m_hostAddress;

    };
}

#endif //PINGNOO_ICMPAPIPINGWORKER_H
