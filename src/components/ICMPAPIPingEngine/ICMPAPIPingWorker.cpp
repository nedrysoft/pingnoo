//
// Created by adriancarpenter on 18/04/2021.
//

#include "ICMPAPIPingWorker.h"

#include "Core/PingResult.h"

#include <QMutexLocker>
#include <QRandomGenerator>
#include <QThread>
#include <WS2tcpip.h>
#include <WinSock2.h>
#include <array>
#include <cerrno>
#include <chrono>
#include <cstdint>
#include <fcntl.h>
#include <iphlpapi.h>
#include <string>

#include <IcmpAPI.h>

using namespace std::chrono_literals;

constexpr unsigned long PingPayloadLength = 1024;
constexpr auto DefaultTransmitTimeout = 3s;

Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingWorker::ICMPAPIPingWorker() {

}

void Nedrysoft::ICMPAPIPingEngine::ICMPAPIPingWorker::doWork() {
    QByteArray dataBuffer = QString("Hello World").toLatin1();
    QByteArray tempBuffer(PingPayloadLength, 0);
    struct sockaddr_in toAddress = {};
    HANDLE icmpHandle;
    Nedrysoft::Core::PingResult pingResult;

    icmpHandle = IcmpCreateFile();

    //auto remoteHost = gethostbyname(m_hostAddress.data());

    inet_pton(AF_INET, "172.29.13.1", &toAddress.sin_addr.S_un.S_addr);

    QByteArray replyBuffer(4096, 0);

    auto returnValue = IcmpSendEcho(icmpHandle, toAddress.sin_addr.S_un.S_addr,
                                dataBuffer.data(), static_cast<WORD>(dataBuffer.length()), nullptr,
                                replyBuffer.data(), static_cast<DWORD>(replyBuffer.length()),
                                std::chrono::duration<DWORD, std::milli>(
                                        DefaultTransmitTimeout).count()); // NOLINT(cppcoreguidelines-pro-type-union-access)
    if (returnValue) {
        PICMP_ECHO_REPLY pEchoReply = (PICMP_ECHO_REPLY) replyBuffer.data();

        struct in_addr ReplyAddr;

        ReplyAddr.S_un.S_addr = pEchoReply->Address;

        pEchoReply->RoundTripTime;
        pEchoReply->Address;
        pEchoReply->Status; //IP_REQ_TIMED_OUT IP_TTL_EXPIRED_TRANSIT IP_SUCCESS

        char *hostAddress = 0;
        auto epoch = std::chrono::system_clock::now();

        /*pingResult = Nedrysoft::Core::PingResult(
                0,
                Nedrysoft::Core::PingResult::ResultCode::Ok,
                hostAddress,
                epoch,
                std::chrono::duration<double, std::milli>(pEchoReply->RoundTripTime),
                nullptr);*/
    }

    emit result(pingResult);
}