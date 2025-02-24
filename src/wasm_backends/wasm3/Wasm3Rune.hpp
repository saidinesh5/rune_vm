//
// Created by Kirill Delimbetov - github.com/delimbetov - on 14.03.2021
// Copyright (c) HAMMER OF THE GODS INC. - hotg.ai
//

#pragma once

#include <rune_vm/RuneVm.hpp>
#include <HostFunctions.hpp>
#include <capabilities/CapabilitiesDelegatesManager.hpp>

struct M3Module;
struct M3Runtime;
struct M3Function;

namespace rune_vm_internal {
    class Wasm3Rune: public rune_vm::IRune {
    public:
        Wasm3Rune(
            const rune_vm::ILogger::CPtr& logger,
            const rune_vm::TRuneId runeId,
            std::shared_ptr<M3Module> module,
            std::shared_ptr<M3Runtime> runtime,
            const std::vector<rune_vm::capabilities::IDelegate::Ptr>& delegates,
            const inference::ModelManager::Ptr& modelManager);

    private:
        // IRune
        [[nodiscard]] rune_vm::TRuneId id() const noexcept final;
        [[nodiscard]] rune_vm::capabilities::IContext::Ptr getCapabilitiesContext() const noexcept final;
        [[nodiscard]] rune_vm::IResult::Ptr call() final;

        // internal
        template<auto functionName>
        void link();

        // data
        rune_vm::LoggingModule m_log;
        std::shared_ptr<M3Module> m_module;
        std::shared_ptr<M3Runtime> m_runtime;
        host_functions::HostContext m_hostContext;
        M3Function* m_callFunction;
    };
}


