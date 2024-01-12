#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "CommonSessionSubsystem.generated.h"

class APlayerController;

/*
*	UCommonSession_HostSessionRequest
*	Gameplay 세션을 호스팅할 때 사용되는 인자들을 저장한 request 오브젝트.
*	이 클래스 객체를 만들어서 CommonSessionSubsystem에게 전달하면, 
*   CommonSessionSubsystem에서 MapID와 CmdArgs로 최종적인 CmdArgs를 만들어 맵 로딩을 진행함.
*/
UCLASS(BlueprintType)
class COMMONUSER_API UCommonSession_HostSessionRequest : public UObject
{
	GENERATED_BODY()
public:
	// MapID -> Text 변환.
	FString GetMapName() const;

	// ServerTravel에 전달할 최종 URL을 생성한다.
	FString ConstructTravelURL() const;

	// 준비할 MapID(맵 경로)
	UPROPERTY(BlueprintReadWrite, Category = Session)
	FPrimaryAssetId MapID;

	// 전달할 CmdArgs (Experience 이름을 전달함) -> 곧 UserFacingExperienceDefinition에서 보게 될 것임.
	UPROPERTY(BlueprintReadWrite, Category = Session)
	TMap<FString, FString> ExtraArgs;

};

/*
*	UCommonSessionSubsystem
*	온라인 게임을 호스팅하고 입장하는 요청을 처리하는 Game 서브시스템 클래스.
*	하나의 게임 인스턴스 당 하나의 서브시스템이 생성되며, 블루프린트나 C++ 코드로 접근이 가능함.
*	만약, 게임별 서브 클래스가 존재하면, 이 기본 서브시스템은 생성되지 않음에 유의.
*/
UCLASS()
class COMMONUSER_API UCommonSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UCommonSessionSubsystem() {}

	/** 세션 요청 정보를 사용해 새 온라인 게임을 생성함. 성공적으로 생성했다면, hard map transfer를 시작함. */
	UFUNCTION(BlueprintCallable, Category = Session)
	void HostSession(APlayerController* HostingPlayer, UCommonSession_HostSessionRequest* Request);

protected:
	// PendingTravelURL은 흔히 맵의 경로로 생각하면 된다.
	FString PendingTravelURL;
};