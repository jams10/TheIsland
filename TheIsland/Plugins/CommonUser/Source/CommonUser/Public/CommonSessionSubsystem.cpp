
#include "CommonSessionSubsystem.h"
#include "Engine/AssetManager.h"
#include "Engine/LocalPlayer.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(CommonSessionSubsystem)

FString UCommonSession_HostSessionRequest::GetMapName() const
{
	// AssetManager를 통해, MapID를 통해, FAssetData를 반환하고, 이를 통해 PackageName을 통해 Map 경로를 반환함.
	FAssetData MapAssetData;
	if (UAssetManager::Get().GetPrimaryAssetData(MapID, MapAssetData))
	{
		return MapAssetData.PackageName.ToString();
	}
	return FString();
}

FString UCommonSession_HostSessionRequest::ConstructTravelURL() const
{
	FString CombinedExtraArgs;

	for (const auto& ExtraArg : ExtraArgs)
	{
		if (ExtraArg.Key.IsEmpty())
		{
			continue;
		}

		/*
			'?'글자를 separate로 하여 복수개의 ExtraArg를 추가함.
			Key 값 유무에 따라, =(assignment)를 통해 알맞는 CmdArg를 생성함.
		*/
		if (ExtraArg.Value.IsEmpty())
		{
			CombinedExtraArgs += FString::Printf(TEXT("?%s"), *ExtraArg.Key);
		}
		else
		{
			CombinedExtraArgs += FString::Printf(TEXT("?%s=%s"), *ExtraArg.Key, *ExtraArg.Value);
		}
	}

	// GetMapName() 함수를 통해 얻어온 Map 경로를 extra args 앞에 추가하여, 최종 TravelURL 생성.
	return FString::Printf(TEXT("%s%s"), *GetMapName(), *CombinedExtraArgs);
}

void UCommonSessionSubsystem::HostSession(APlayerController* HostingPlayer, UCommonSession_HostSessionRequest* Request)
{
	// 인자로 들어온 호스팅 플레이어 컨트롤러가 우리가 직접 조종하는 플레이어인지 체크함.
	ULocalPlayer* LocalPlayer = (HostingPlayer != nullptr) ? HostingPlayer->GetLocalPlayer() : nullptr;
	if (!LocalPlayer)
	{
		return;
	}

	// HostSessionRequest에 있는 정보인 MapID와 ExtraArgs를 통해 URL를 생성, MapLoad를 시작함.
	GetWorld()->ServerTravel(Request->ConstructTravelURL());
}