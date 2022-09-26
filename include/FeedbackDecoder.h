/*********************************************************************
 * Feedback Decoder
 *
 * Copyright (C) 2022 Marcel Maage
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * LICENSE file for more details.
 */

#pragma once
#include <Arduino.h>
#include <NmraDcc.h>
#include "ZCan/ZCanInterfaceObserver.h"
#include "Preferences.h"
#include <array>

class FeedbackDecoder : public ZCanInterfaceObserver
{
public:
    FeedbackDecoder(std::string namespaceFeedbackModul, std::string keyModulConfig, int buttonPin, bool debug, bool zcanDebug);
    virtual ~FeedbackDecoder();

    void begin();

    void cyclic();

    void callbackAccAddrReceived(uint16_t addr);

    void callbackLocoAddrReceived(uint16_t addr);

protected:
    virtual void onIdenticalNetworkId() override;

    virtual bool onAccessoryData(uint16_t accessoryId, uint8_t port, uint8_t type) override;

    virtual bool onAccessoryPort6(uint16_t accessoryId, uint8_t port, uint8_t type) override;

    virtual bool onRequestModulInfo(uint16_t id, uint16_t type) override;

    virtual bool onCmdModulInfo(uint16_t id, uint16_t type, uint32_t info) override;

    virtual bool onRequestModulObjectConfig(uint16_t id, uint32_t tag) override;

    virtual bool onCmdModulObjectConfig(uint16_t id, uint32_t tag, uint16_t value) override;

    virtual bool onRequestPing(uint16_t id) override;

    virtual bool onPing(uint16_t nid, uint32_t masterUid, uint16_t type, uint16_t sessionId) override;

    bool sendMessage(ZCanMessage &message) override;

    bool m_debug;

    uint8_t m_buttonPin;

    uint16_t m_modulId;

    unsigned long m_idPrgStartTimeINms;

    bool m_idPrgRunning;

    unsigned long m_idPrgIntervalINms;

    unsigned long m_lastCanCmdSendINms;

    uint16_t m_pingJitterINms;

    unsigned long m_pingIntervalINms;

    uint32_t m_masterId;

    uint16_t m_sessionId;

    uint16_t m_modulType{roco10808Type};

    // adress is 0x8000 up tp 0xC000
    bool notifyLocoInBlock(uint8_t port, uint16_t adress1, uint16_t adress2, uint16_t adress3, uint16_t adress4);

    bool notifyBlockOccupied(uint8_t port, uint8_t type, bool occupied);

    typedef struct
    {
        bool state;
        std::array<uint16_t, 4> adress;
    } TrackData;

    std::array<TrackData, 8> trackData;

private:
    typedef struct
    {
        uint16_t trackSetCurrentINmA;
        uint16_t trackFreeToSetTimeINms;
        uint16_t trackSetToFreeTimeINms;
    } TrackConfig;

    typedef struct
    {
        uint16_t networkId;
        uint8_t modulAdress;
        bool sendChannel2Data;
        // std::array<TrackConfig, 8> trackConfig;
        TrackConfig trackConfig;
    } ModulConfig;

    ModulConfig m_modulConfig;

        uint32_t m_firmwareVersion;
    uint32_t m_buildDate;
    uint32_t m_hardwareVersion;

    Preferences m_preferences;

    const std::string m_namespaceFeedbackModul;

    const std::string m_keyModulConfig;

    bool saveConfig();
};