#pragma once
#include "DKUtil/Hook.hpp"

namespace CombatPathing
{
	using namespace DKUtil::Alias;

	class FallbackDistanceHook1
	{
	public:
		static void InstallHook()
		{
			SKSE::AllocTrampoline(1 << 4);
			auto& trampoline = SKSE::GetTrampoline();

			REL::Relocation<std::uintptr_t> Base{ REL::RelocationID(46712, 802680) };  // 7D73D0, 8149B0
			_GetFallbackDistance = trampoline.write_call<5>(Base.address() + REL::Relocate(0x1EB, 0x1B0), GetFallbackDistance);
			INFO("{} Done!", __FUNCTION__);
		}

	private:
		static float GetFallbackDistance(RE::Actor* a_actor);

		static inline REL::Relocation<decltype(GetFallbackDistance)> _GetFallbackDistance;
	};

	// TODO
	class FallbackDistanceHook2
	{
		static float GetMaxFallbackDistance(RE::Actor* a_me, RE::Actor* a_he);

		// 1-5-97-0 @ 0x7D7740
		static constexpr std::uintptr_t FuncID = 46713;
		static constexpr std::ptrdiff_t OffsetL = 0x246;
		static constexpr std::ptrdiff_t OffsetH = 0x24E;

		static constexpr Patch RelocateReturn{
			// addss xmm6, xmm0
			"\xF3\x0F\x58\xF0",
			4
		};

	public:
		static void InstallHook()
		{
			SKSE::AllocTrampoline(1 << 6);

			auto funcAddr = REL::ID(FuncID).address();
			Patch RelocatePointer{
				AsPointer(funcAddr + OffsetL + 0x10),  //7D7996
				6
			};

			auto handle = DKUtil::Hook::AddCaveHook(
				funcAddr,
				{ OffsetL, OffsetH },
				FUNC_INFO(GetMaxFallbackDistance),
				&RelocatePointer,
				&RelocateReturn,
				DKUtil::Hook::HookFlag::kSkipNOP);

			handle->Enable();

			INFO("{} Done!", __FUNCTION__);
		}
	};

	// TODO
	class FallbackWaitTimeHook1
	{
	public:
		static void InstallHook()
		{
			SKSE::AllocTrampoline(1 << 4);
			auto& trampoline = SKSE::GetTrampoline();

			REL::Relocation<std::uintptr_t> Base{ REL::RelocationID(46713, 47909) };  //sub_1407D7740 // AE untested
			_GetFallbackWaitTime = trampoline.write_call<5>(Base.address() + REL::Relocate(0xEA, 0x12E), GetFallbackWaitTime);
			INFO("{} Done!", __FUNCTION__);
		}

	private:
		static float GetFallbackWaitTime(RE::Actor* a_actor);

		static inline REL::Relocation<decltype(GetFallbackWaitTime)> _GetFallbackWaitTime;
	};

	class FallbackWaitTimeHook2
	{
		static float GetMinFallbackWaitTime(RE::Actor* a_me, RE::Actor* a_he);

		static constexpr std::uintptr_t FuncID = 46713;   //1407d7740
		static constexpr std::ptrdiff_t OffsetL = 0x2C0;  //1407D7A00
		static constexpr std::ptrdiff_t OffsetH = 0x2C8;  //1407D7A08

		static constexpr Patch RelocateReturn{
			// movss xmm1, xmm0
			"\xF3\x0F\x10\xC8",
			4
		};

	public:
		static void InstallHook()
		{
			SKSE::AllocTrampoline(1 << 6);

			auto funcAddr = REL::ID(FuncID).address();
			Patch RelocatePointer{
				AsPointer(funcAddr + 0x256),  //7D7996
				6
			};

			auto handle = DKUtil::Hook::AddCaveHook(
				funcAddr,
				{ OffsetL, OffsetH },
				FUNC_INFO(GetMinFallbackWaitTime),
				&RelocatePointer,
				&RelocateReturn,
				DKUtil::Hook::HookFlag::kSkipNOP);

			handle->Enable();

			INFO("{} Done!", __FUNCTION__);
		}
	};
}