#include "Hooks.h"

#include "Settings.h"

#include "HUDMarkerManager.h"

namespace hooks
{
	bool UpdateQuests(const RE::HUDMarkerManager* a_hudMarkerManager, RE::HUDMarker::ScaleformData* a_markerData,
							RE::NiPoint3* a_pos, const RE::RefHandle& a_refHandle, std::uint32_t a_markerGotoFrame,
							RE::TESQuest*& a_quest)
	{
		if (HUDMarkerManager::AddMarker(a_hudMarkerManager, a_markerData, a_pos, a_refHandle, a_markerGotoFrame))
		{
			RE::TESObjectREFR* marker = RE::TESObjectREFR::LookupByHandle(a_refHandle).get();

			extended::HUDMarkerManager::GetSingleton()->ProcessQuestMarker(a_quest, marker, a_markerGotoFrame);

			return true;
		}

		return false;
	}

	RE::TESWorldSpace* AllowedToShowMapMarker(const RE::TESObjectREFR* a_marker)
	{
		RE::TESWorldSpace* markerWorldspace = a_marker->GetWorldspace();

		if (g_settings.display.showInteriorMarkers)
		{
			RE::TESWorldSpace* playerWorldspace = RE::PlayerCharacter::GetSingleton()->GetWorldspace();

			return markerWorldspace->parentWorld == playerWorldspace ? playerWorldspace : markerWorldspace;
		}
		else
		{
			return markerWorldspace;
		}
	}

	bool UpdateLocations(const RE::HUDMarkerManager* a_hudMarkerManager, RE::HUDMarker::ScaleformData* a_markerData,
							   RE::NiPoint3* a_pos, const RE::RefHandle& a_refHandle, std::uint32_t a_markerGotoFrame)
	{
		RE::TESObjectREFR* marker = RE::TESObjectREFR::LookupByHandle(a_refHandle).get();
		RE::TESWorldSpace* markerWorldspace = marker->GetWorldspace();

		RE::PlayerCharacter* player = RE::PlayerCharacter::GetSingleton();
		RE::TESWorldSpace* playerWorldspace = player->GetWorldspace();

		RE::NiPoint3 markerRealPos = util::GetRealPosition(marker, markerWorldspace);
		RE::NiPoint3 playerRealPos = util::GetRealPosition(player, playerWorldspace);

		float sqDistanceToMarker = playerRealPos.GetSquaredDistance(markerRealPos);

		if (sqDistanceToMarker < RE::HUDMarkerManager::GetSingleton()->sqRadiusToAddLocation &&
			(g_settings.display.showUndiscoveredLocationMarkers || false))
		{
			if (HUDMarkerManager::AddMarker(a_hudMarkerManager, a_markerData, a_pos, a_refHandle, a_markerGotoFrame)) 
			{
				extended::HUDMarkerManager::GetSingleton()->ProcessLocationMarker(marker->extraList.GetByType<RE::ExtraMapMarker>(),
																					marker, a_markerGotoFrame);

				return true;
			}
		}

		return false;
	}

	bool UpdateEnemies(const RE::HUDMarkerManager* a_hudMarkerManager, RE::HUDMarker::ScaleformData* a_markerData,
							RE::NiPoint3* a_pos, const RE::RefHandle& a_refHandle, std::uint32_t a_markerGotoFrame)
	{
		if (HUDMarkerManager::AddMarker(a_hudMarkerManager, a_markerData, a_pos, a_refHandle, a_markerGotoFrame)) 
		{
			RE::TESObjectREFR* marker = RE::TESObjectREFR::LookupByHandle(a_refHandle).get();

			extended::HUDMarkerManager::GetSingleton()->ProcessEnemyMarker(marker->As<RE::Character>(), a_markerGotoFrame);

			return true;
		}

		return false;
	}

	bool UpdatePlayerSetMarker(const RE::HUDMarkerManager* a_hudMarkerManager, RE::HUDMarker::ScaleformData* a_markerData,
								RE::NiPoint3* a_pos, const RE::RefHandle& a_refHandle, std::uint32_t a_markerGotoFrame)
	{
		if (HUDMarkerManager::AddMarker(a_hudMarkerManager, a_markerData, a_pos, a_refHandle, a_markerGotoFrame)) 
		{
			RE::TESObjectREFR* marker = RE::TESObjectREFR::LookupByHandle(a_refHandle).get();

			extended::HUDMarkerManager::GetSingleton()->ProcessPlayerSetMarker(marker, a_markerGotoFrame);

			return true;
		}

		return false;
	}

	bool SetCompassMarkers(RE::GFxValue::ObjectInterface* a_objectInterface, void* a_data,
						   RE::GFxValue* a_result, const char* a_name, const RE::GFxValue* a_args,
						   std::uint32_t a_numArgs, bool a_isDObj)
	{
		if (a_objectInterface->Invoke(a_data, a_result, a_name, a_args, a_numArgs, a_isDObj)) 
		{
			extended::HUDMarkerManager::GetSingleton()->SetMarkersExtraInfo();

			return true;
		}

		return false;
	}
}
