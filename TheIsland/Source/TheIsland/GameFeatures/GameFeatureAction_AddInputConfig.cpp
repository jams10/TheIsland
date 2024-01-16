#include "GameFeatureAction_AddInputConfig.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerMappableInputConfig.h"
#include "Components/GameFrameworkComponentManager.h"
#include "TheIsland/Character/TIHeroComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameFeatureAction_AddInputConfig)

void UGameFeatureAction_AddInputConfig::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	// FGameFeatureActivatingContext는 FGameFeatureStateChangeContext를 상속 받기 때문에 
	// ContextData Map에 FGameFeatureActivatingContext 타입 Context를 넘겨서 대응되는 FPerContextData가 있는지 찾고 없으면 추가해줌.
	FPerContextData& ActiveData = ContextData.FindOrAdd(Context);
	if (!ensure(ActiveData.ExtensionRequestHandles.IsEmpty()) ||
		!ensure(ActiveData.PawnsAddedTo.IsEmpty()))
	{
		Reset(ActiveData);
	}

	// UGameFeatureAction_WorldActionBase를 호출하면서, AddToWorld() 호출!
	Super::OnGameFeatureActivating(Context);
}

void UGameFeatureAction_AddInputConfig::OnGameFeatureDeactivating(FGameFeatureDeactivatingContext& Context)
{
	Super::OnGameFeatureDeactivating(Context);
	FPerContextData* ActiveData = ContextData.Find(Context);
	if (ensure(ActiveData))
	{
		Reset(*ActiveData);
	}
}

void UGameFeatureAction_AddInputConfig::AddToWorld(const FWorldContext& WorldContext, const FGameFeatureStateChangeContext& ChangeContext)
{
	UWorld* World = WorldContext.World(); // Editor 상의 월드인지, 실제 게임 월드인지 체크하기 위해 World를 얻어옴.
	UGameInstance* GameInstance = WorldContext.OwningGameInstance;
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);
	if (GameInstance && World && World->IsGameWorld()) // 입력으로 주어진 worldcontext에 해당하는 game instance, world, 그리고 game world일 경우.
	{
		// GameFrameworkComponentManager를 이용하여, ExtensionHandler를 추가하여 등록 진행.
		// GameFrameworkComponentManager가 제공하는 확장 핸들러 시스템을 이용, 
		// 이벤트를 발생 시키면 해당 이벤트를 받아서 처리해주는 델리게이트로 FExtensionHandlerDelegate 타입의 AddConfigDelegate 변수를 선언해주고 있음.
		// 해당 델리게이트에 HandlePawnExtension 콜백 함수를 연결, 이벤트가 발생 했을 때 HandlePawnExtension() 함수가 처리해주게 됨.
		if (UGameFrameworkComponentManager* ComponentMan = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
		{
			UGameFrameworkComponentManager::FExtensionHandlerDelegate AddConfigDelegate =
				UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &ThisClass::HandlePawnExtension, ChangeContext);

			// 등록된 콜백 함수의 핸들을 ActiveData의 ExtensionRequesetHandles에 등록.
			TSharedPtr<FComponentRequestHandle> ExtenstionRequestHandle = ComponentMan->AddExtensionHandler(APawn::StaticClass(), AddConfigDelegate);
			ActiveData.ExtensionRequestHandles.Add(ExtenstionRequestHandle);
		}
	}
}

void UGameFeatureAction_AddInputConfig::HandlePawnExtension(AActor* Actor, FName EventName, FGameFeatureStateChangeContext ChangeContext)
{
	APawn* AsPawn = CastChecked<APawn>(Actor);
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);

	if (EventName == UGameFrameworkComponentManager::NAME_ExtensionAdded || EventName == UTIHeroComponent::NAME_BindInputsNow)
	{
		AddInputConfig(AsPawn, ActiveData);
	}
	else if (EventName == UGameFrameworkComponentManager::NAME_ExtensionRemoved || EventName == UGameFrameworkComponentManager::NAME_ReceiverRemoved)
	{
		RemoveInputConfig(AsPawn, ActiveData);
	}
}

void UGameFeatureAction_AddInputConfig::AddInputConfig(APawn* Pawn, FPerContextData& ActiveData)
{
	// InputConfigs에 FTIMappableConfigPair를 추가하는 것이 아니라, 이미 InputConfigs에 있는 FTIMappableConfigPair를 바탕으로 PlayerMappableInputConfig를 추가함.

	APlayerController* PlayerController = Cast<APlayerController>(Pawn->GetController());
	if (ULocalPlayer* LP = PlayerController ? PlayerController->GetLocalPlayer() : nullptr)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			FModifyContextOptions Options = {};
			Options.bIgnoreAllPressedKeysUntilRelease = false;

			// 추가된 InputConfigs를 순회하며, EnhandedInputSubsystem에 PlayerMappableConfig를 직접 추가.
			for (const FTIMappableConfigPair& Pair : InputConfigs)
			{
				if (Pair.bShouldActivateAutomatically)
				{
					Subsystem->AddPlayerMappableConfig(Pair.Config.LoadSynchronous(), Options);
				}
			}

			// ActiveData에 Pawn을 관리 대상으로 등록.
			ActiveData.PawnsAddedTo.AddUnique(Pawn);
		}
	}
}

void UGameFeatureAction_AddInputConfig::RemoveInputConfig(APawn* Pawn, FPerContextData& ActiveData)
{
	APlayerController* PlayerController = Cast<APlayerController>(Pawn->GetController());
	if (ULocalPlayer* LP = PlayerController ? PlayerController->GetLocalPlayer() : nullptr)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			// InputConfig를 순회하며, Config를 제거 하는 작업을 진행.
			for (const FTIMappableConfigPair& Pair : InputConfigs)
			{
				Subsystem->RemovePlayerMappableConfig(Pair.Config.LoadSynchronous());
			}

			ActiveData.PawnsAddedTo.Remove(Pawn);
		}
	}
}

void UGameFeatureAction_AddInputConfig::Reset(FPerContextData& ActiveData)
{
	// ExtensionRequestHandles를 초기화.
	ActiveData.ExtensionRequestHandles.Empty();

	// PawnsAddedTo에 대해서, 하나씩 Stack 방식으로 위에서 아래로 직접 InputConfig를 제거하는 작업을 진행.
	while (!ActiveData.PawnsAddedTo.IsEmpty())
	{
		TWeakObjectPtr<APawn> PawnPtr = ActiveData.PawnsAddedTo.Top();
		if (PawnPtr.IsValid())
		{
			RemoveInputConfig(PawnPtr.Get(), ActiveData);
		}
		else
		{
			// WeakObjectPtr로 PawnsAddedTo를 관리하고 있기 때문에, GC 되엇다면, nullptr일 수 있음.
			ActiveData.PawnsAddedTo.Pop();
		}
	}
}