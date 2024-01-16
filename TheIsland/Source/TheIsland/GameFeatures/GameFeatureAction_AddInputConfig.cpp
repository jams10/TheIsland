#include "GameFeatureAction_AddInputConfig.h"
#include "EnhancedInputSubsystems.h"
#include "PlayerMappableInputConfig.h"
#include "Components/GameFrameworkComponentManager.h"
#include "TheIsland/Character/TIHeroComponent.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(GameFeatureAction_AddInputConfig)

void UGameFeatureAction_AddInputConfig::OnGameFeatureActivating(FGameFeatureActivatingContext& Context)
{
	// FGameFeatureActivatingContext�� FGameFeatureStateChangeContext�� ��� �ޱ� ������ 
	// ContextData Map�� FGameFeatureActivatingContext Ÿ�� Context�� �Ѱܼ� �����Ǵ� FPerContextData�� �ִ��� ã�� ������ �߰�����.
	FPerContextData& ActiveData = ContextData.FindOrAdd(Context);
	if (!ensure(ActiveData.ExtensionRequestHandles.IsEmpty()) ||
		!ensure(ActiveData.PawnsAddedTo.IsEmpty()))
	{
		Reset(ActiveData);
	}

	// UGameFeatureAction_WorldActionBase�� ȣ���ϸ鼭, AddToWorld() ȣ��!
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
	UWorld* World = WorldContext.World(); // Editor ���� ��������, ���� ���� �������� üũ�ϱ� ���� World�� ����.
	UGameInstance* GameInstance = WorldContext.OwningGameInstance;
	FPerContextData& ActiveData = ContextData.FindOrAdd(ChangeContext);
	if (GameInstance && World && World->IsGameWorld()) // �Է����� �־��� worldcontext�� �ش��ϴ� game instance, world, �׸��� game world�� ���.
	{
		// GameFrameworkComponentManager�� �̿��Ͽ�, ExtensionHandler�� �߰��Ͽ� ��� ����.
		// GameFrameworkComponentManager�� �����ϴ� Ȯ�� �ڵ鷯 �ý����� �̿�, 
		// �̺�Ʈ�� �߻� ��Ű�� �ش� �̺�Ʈ�� �޾Ƽ� ó�����ִ� ��������Ʈ�� FExtensionHandlerDelegate Ÿ���� AddConfigDelegate ������ �������ְ� ����.
		// �ش� ��������Ʈ�� HandlePawnExtension �ݹ� �Լ��� ����, �̺�Ʈ�� �߻� ���� �� HandlePawnExtension() �Լ��� ó�����ְ� ��.
		if (UGameFrameworkComponentManager* ComponentMan = UGameInstance::GetSubsystem<UGameFrameworkComponentManager>(GameInstance))
		{
			UGameFrameworkComponentManager::FExtensionHandlerDelegate AddConfigDelegate =
				UGameFrameworkComponentManager::FExtensionHandlerDelegate::CreateUObject(this, &ThisClass::HandlePawnExtension, ChangeContext);

			// ��ϵ� �ݹ� �Լ��� �ڵ��� ActiveData�� ExtensionRequesetHandles�� ���.
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
	// InputConfigs�� FTIMappableConfigPair�� �߰��ϴ� ���� �ƴ϶�, �̹� InputConfigs�� �ִ� FTIMappableConfigPair�� �������� PlayerMappableInputConfig�� �߰���.

	APlayerController* PlayerController = Cast<APlayerController>(Pawn->GetController());
	if (ULocalPlayer* LP = PlayerController ? PlayerController->GetLocalPlayer() : nullptr)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			FModifyContextOptions Options = {};
			Options.bIgnoreAllPressedKeysUntilRelease = false;

			// �߰��� InputConfigs�� ��ȸ�ϸ�, EnhandedInputSubsystem�� PlayerMappableConfig�� ���� �߰�.
			for (const FTIMappableConfigPair& Pair : InputConfigs)
			{
				if (Pair.bShouldActivateAutomatically)
				{
					Subsystem->AddPlayerMappableConfig(Pair.Config.LoadSynchronous(), Options);
				}
			}

			// ActiveData�� Pawn�� ���� ������� ���.
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
			// InputConfig�� ��ȸ�ϸ�, Config�� ���� �ϴ� �۾��� ����.
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
	// ExtensionRequestHandles�� �ʱ�ȭ.
	ActiveData.ExtensionRequestHandles.Empty();

	// PawnsAddedTo�� ���ؼ�, �ϳ��� Stack ������� ������ �Ʒ��� ���� InputConfig�� �����ϴ� �۾��� ����.
	while (!ActiveData.PawnsAddedTo.IsEmpty())
	{
		TWeakObjectPtr<APawn> PawnPtr = ActiveData.PawnsAddedTo.Top();
		if (PawnPtr.IsValid())
		{
			RemoveInputConfig(PawnPtr.Get(), ActiveData);
		}
		else
		{
			// WeakObjectPtr�� PawnsAddedTo�� �����ϰ� �ֱ� ������, GC �Ǿ��ٸ�, nullptr�� �� ����.
			ActiveData.PawnsAddedTo.Pop();
		}
	}
}