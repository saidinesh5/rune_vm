//
// Created by Kirill Delimbetov - github.com/delimbetov - on 29.03.2021
// Copyright (c) HAMMER OF THE GODS INC. - hotg.ai
//

#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <variant>
#include <rune_vm/DataView.hpp>
#include <rune_vm/VirtualInterface.hpp>

namespace rune_vm::capabilities {
    enum class Capability: uint8_t {
        Rand,
        Sound,
        Accel,
        Image,
        Raw
    };

    enum class ValueType: uint8_t {
        Uint8,
        Int16,
        Int32,
        Float32
    };

    using TId = uint32_t;
    using TKey = std::string;

    struct Parameter {
        using Data = std::variant<uint8_t, int16_t, int32_t, float>;

        Data m_data;
    };

    struct IContext : VirtualInterface<IContext> {
        using TKeyToParameterMap = std::unordered_map<TKey, Parameter>;

        // This method is how user can retract approve for requested capability
        // return false if capabilityId was not stored
        virtual bool setCapabilityAvailability(const TId capabilityId, const bool available) noexcept = 0;

        // Calling this method forces copy of internal parameters map. No need to do that frequently
        [[nodiscard]] virtual std::optional<TKeyToParameterMap> getParamsForCapability(
            const TId capabilityId) const noexcept = 0;
        [[nodiscard]] virtual std::optional<Parameter> getParamForCapabilityForKey(
            const TId capabilityId,
            const TKey& key) const noexcept = 0;
    };

    struct IDelegate : VirtualInterface<IDelegate> {
        // list of capabilities that are covered by this delegate
        // you may pass multiple delegates on the rune creation
        [[nodiscard]] virtual std::unordered_set<Capability> getSupportedCapabilities() const noexcept = 0;

        // method is being called to tell the user that specific capability is gonna be required
        // capabilityId is unique, unless requestCapability returns false - in that case it could be reused next time
        // return true if capability is available and false otherwise
        [[nodiscard]] virtual bool requestCapability(
            const Capability capability,
            const TId newCapabilityId) noexcept = 0;
        // request capability parameter change
        // return true if new param is possible to set and false otherwise
        [[nodiscard]] virtual bool requestCapabilityParamChange(
            const TId capabilityId,
            const TKey& key,
            const Parameter& parameter) noexcept = 0;
        // requests new input from capabilityId being written into buffer
        // return true if there's data for that capability and false otherwise
        [[nodiscard]] virtual bool requestRuneInputFromCapability(
            const rune_vm::DataView<uint8_t> buffer,
            const TId capabilityId) noexcept = 0;
    };
}