#include "TIUserFacingExperienceDefinition.h"
#include "CommonUser/Public/CommonSessionSubsystem.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TIUserFacingExperienceDefinition)

UCommonSession_HostSessionRequest* UTIUserFacingExperienceDefinition::CreateHostingRequest() const
{
	const FString ExperienceName = ExperienceID.PrimaryAssetName.ToString();

	// UCommonSession_HostSessionRequest 클래스는 UObject를 상속 받은 클래스이기 때문에, 인스턴스는 알아서 GC에 의해 관리됨.
	// 이 인스턴스의 경우 현재 프레임을 벗어나면 GC에 의해 제거되므로, 만약 추가적인 프레임에서 사용할 경우, 
	// dangling 포인터 문제를 방지하기 위해 객체 수명 주기를 따로 관리해주기 위해 멤버 변수로 선언하는 등 관리가 필요함.
	UCommonSession_HostSessionRequest* Result = NewObject<UCommonSession_HostSessionRequest>();
	Result->MapID = MapID;
	Result->ExtraArgs.Add(TEXT("Experience"), ExperienceName);

	return Result;
}
