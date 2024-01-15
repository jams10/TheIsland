#include "TIGameMode.h"
#include "TIGameState.h"
#include "TheIsland/Character/TICharacter.h"
#include "TheIsland/Character/TIPawnData.h"
#include "TheIsland/Character/TIPawnExtensionComponent.h"
#include "TheIsland/Player/TIPlayerController.h"
#include "TheIsland/Player/TIPlayerState.h"
#include "TheIsland/TILogChannels.h"
#include "TIExperienceDefinition.h"
#include "TIExperienceManagerComponent.h"
#include "Kismet/GameplayStatics.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(TIGameMode)

ATIGameMode::ATIGameMode(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	GameStateClass = ATIGameState::StaticClass();
	PlayerControllerClass = ATIPlayerController::StaticClass();
	PlayerStateClass = ATIPlayerState::StaticClass();
	DefaultPawnClass = ATICharacter::StaticClass();
}

void ATIGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	// 아직 GameInstance를 통해 초기화 작업이 진행되므로, 현 프레임에는 Lyra에 새로 도입된 Experience 초기화를 진행할 수 없음.
	// 이를 처리하기 위해, 한 프레임 뒤에 이벤트를 받아 초기화 작업을 이어 나갈 수 있도록 함.
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ThisClass::HandleMatchAssignmentIfNotExpectingOne);
}

void ATIGameMode::InitGameState()
{
	Super::InitGameState();

	UTIExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UTIExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	// ExperienceManagerComponent 함수를 사용해 Experience가 로드 되었을 때 초기화 작업을 수행하기 위해 델리게이트를 걸어 놓음.
	ExperienceManagerComponent->CallOrRegister_OnExperienceLoaded(FOnTIExperienceLoaded::FDelegate::CreateUObject(this, &ThisClass::OnExperienceLoaded));
}

void ATIGameMode::HandleMatchAssignmentIfNotExpectingOne()
{
	// 이 함수에서는 우리가 로딩할 ExperienceDefinition 애셋의 PrimaryAssetId를 가져와서, OnMatchAssignmentGiven 함수로 넘겨줌.

	FPrimaryAssetId ExperienceId; // PrimaryAssetId의 경우 애셋 타입과 이름으로 구성됨.

	UWorld* World = GetWorld();

	// 우리가 앞서 URL의 명령 인자로 함께 넘겨준 Experience 이름을 사용해 해당 Experience를 로드할 수 있도록 ExperienceId를 설정해줌.
	// 명령 인자의 경우 GameMode의 OptionString에 저장되어 있음.
	if (!ExperienceId.IsValid() && UGameplayStatics::HasOption(OptionsString, TEXT("Experience")))
	{
		// Experience의 Value를 가져와서, PrimaryAssetId를 생성해줌. 이 때, ExperienceDefinition의 Class 이름을 사용함.
		const FString ExperienceFromOptions = UGameplayStatics::ParseOption(OptionsString, TEXT("Experience"));
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType(UTIExperienceDefinition::StaticClass()->GetFName()), FName(*ExperienceFromOptions));
	}

	// 아무런 설정도 하지 않은 경우 기본적으로 불러올 ExperienceDefinition.
	if (!ExperienceId.IsValid())
	{
		ExperienceId = FPrimaryAssetId(FPrimaryAssetType("TIExperienceDefinition"), FName("B_DefaultExperience"));
	}

	OnMatchAssignmentGiven(ExperienceId);
}

void ATIGameMode::OnMatchAssignmentGiven(FPrimaryAssetId ExperienceId)
{
	// 이 함수는 ExperienceManagerComponent를 활용하여 Experience를 로드를 시작함.
	check(ExperienceId.IsValid());

	UTIExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UTIExperienceManagerComponent>();
	check(ExperienceManagerComponent);
	ExperienceManagerComponent->ServerSetCurrentExperience(ExperienceId);
}

void ATIGameMode::OnExperienceLoaded(const UTIExperienceDefinition* CurrentExperience)
{
	// PlayerController를 순회.
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Cast<APlayerController>(*Iterator);

		// PlayerController가 Pawn을 Possess하지 않았다면, RestartPlayer를 통해 Pawn을 다시 spawn함.
		if (PC && PC->GetPawn() == nullptr)
		{
			if (PlayerCanRestart(PC))
			{
				RestartPlayer(PC);
			}
		}
	}
}

APawn* ATIGameMode::SpawnDefaultPawnAtTransform_Implementation(AController* NewPlayer, const FTransform& SpawnTransform)
{
	FActorSpawnParameters SpawnInfo;
	SpawnInfo.Instigator = GetInstigator();
	SpawnInfo.ObjectFlags |= RF_Transient;
	SpawnInfo.bDeferConstruction = true;

	// PawnData에 있는 PawnClass 얻어옴.
	if (UClass* PawnClass = GetDefaultPawnClassForController(NewPlayer))
	{
		// Spawn이 성공 했으면, PawnExtensionComponent를 찾고, 해당 Component의 PawnData에 GetPawnDataForController()를 통해 가져온 PawnData를 세팅.
		if (APawn* SpawnedPawn = GetWorld()->SpawnActor<APawn>(PawnClass, SpawnTransform, SpawnInfo))
		{
			// FindPawnExtensionComponent() 함수를 통해 PawnExtensionComponent에 접근, PlayerState에 설정한 PawnData를 설정해줌.
			if (UTIPawnExtensionComponent* PawnExtComp = UTIPawnExtensionComponent::FindPawnExtensionComponent(SpawnedPawn))
			{
				if (const UTIPawnData* PawnData = GetPawnDataForController(NewPlayer))
				{
					PawnExtComp->SetPawnData(PawnData);
				}
			}
			SpawnedPawn->FinishSpawning(SpawnTransform);
			return SpawnedPawn;
		}
	}

	return nullptr;
}

UClass* ATIGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	// Experience를 로드 해서 PawnData를 가져올 수 있다면, DefaultPawn 클래스가 아닌 Experience에 있는 PawnData의 Pawn 클래스를 사용함.
	if (const UTIPawnData* PawnData = GetPawnDataForController(InController))
	{
		if (PawnData->PawnClass)
		{
			return PawnData->PawnClass;
		}
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

void ATIGameMode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	if (IsExperienceLoaded()) // Experience가 로드 되었을 때만 HandleStartingNewPlayer() 함수를 호출하도록 함.
	{
		Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	}
}

const UTIPawnData* ATIGameMode::GetPawnDataForController(const AController* InController) const
{
	// PlayerState에 이미 PawnData가 있는 경우, PawnData를 PlayerState에서 그냥 가져와서 리턴.
	if (InController)
	{
		if (const ATIPlayerState* PS = InController->GetPlayerState<ATIPlayerState>())
		{
			// GetPawnData 구현
			if (const UTIPawnData* PawnData = PS->GetPawnData<UTIPawnData>())
			{
				return PawnData;
			}
		}
	}

	// fall back to the default for the current experience.
	// 아직 PlayerState에 PawnData가 설정되어 있지 않은 경우, ExperienceManagerComponent의 CurrentExperience로부터 가져와서 설정함.
	check(GameState);
	UTIExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UTIExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	if (ExperienceManagerComponent->IsExperienceLoaded())
	{
		const UTIExperienceDefinition* Experience = ExperienceManagerComponent->GetCurrentExperienceChecked();
		if (Experience->DefaultPawnData)
		{
			return Experience->DefaultPawnData;
		}
	}

	// 어떠한 케이스에도 핸들링 안되어있으면 nullptr 리턴.
	return nullptr;
}

bool ATIGameMode::IsExperienceLoaded() const
{
	check(GameState);
	// ExperienceManagerComponent를 통해 Experience 로드 여부를 가져옴.
	UTIExperienceManagerComponent* ExperienceManagerComponent = GameState->FindComponentByClass<UTIExperienceManagerComponent>();
	check(ExperienceManagerComponent);

	return ExperienceManagerComponent->IsExperienceLoaded();
}
