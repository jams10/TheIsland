#include "TIHeroComponent.h"
#include "Components/GameFrameworkComponentManager.h"
#include "EnhancedInputSubsystems.h"
#include "TheIsland/Camera/TICameraMode.h"
#include "TheIsland/Camera/TICameraComponent.h"
#include "TheIsland/Character/TIPawnData.h"
#include "TheIsland/Input/TIMappableConfigPair.h"
#include "TheIsland/Input/TIInputComponent.h"
#include "TheIsland/Player/TIPlayerState.h"
#include "TheIsland/Player/TIPlayerController.h"
#include "TheIsland/TIGameplayTag.h"
#include "TheIsland/TILogChannels.h"
#include "TIPawnExtensionComponent.h"
#include "PlayerMappableInputConfig.h"
#include UE_INLINE_GENERATED_CPP_BY_NAME(TIHeroComponent)

const FName UTIHeroComponent::NAME_ActorFeatureName("Hero");

// InputConfig의 GameFeatureAction 활성화 ExtensionEvent 이름.
const FName UTIHeroComponent::NAME_BindInputsNow("BindInputsNow");

UTIHeroComponent::UTIHeroComponent(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// 이 컴포넌트 클래스는 초기화를 위해 우리가 임의적으로 트리거 시켜서 이벤트 방식으로 로직을 수행할 것이기 때문에 Tick은 필요 없으므로 꺼줌.
	PrimaryComponentTick.bStartWithTickEnabled = false;
	PrimaryComponentTick.bCanEverTick = false;
}

void UTIHeroComponent::OnRegister()
{
	Super::OnRegister();

	// 올바른 Actor에 등록 되었는지 확인.
	{
		if (!GetPawn<APawn>())
		{
			UE_LOG(LogTI, Error, TEXT("this component has been added to BP whose class is not a Pawn!"));
		}
	}

	// IGameFrameworkInitStateInterface 클래스 인터페이스 함수를 사용해 GameFrameworkComponentManager에 InitState 사용을 위해 등록!
	// 이 함수를 호출하면, InitState의 초기 값은 NAME_None.
	RegisterInitStateFeature();
}

void UTIHeroComponent::BeginPlay()
{
	// BeginPlay에서 본격적인 InitState의 변화가 시작됨. 
	Super::BeginPlay();

	// PawnExtension feature에 대해 InitState가 변경될 때 마다 OnActorInitStateChanged() 함수가 호출되도록 델리게이트를 바인딩. 
	BindOnActorInitStateChanged(UTIPawnExtensionComponent::NAME_ActorFeatureName, FGameplayTag(), false);

	// 나(PawnExtensionComponent)의 InitState를 InitState_Spawned 상태로 변환 시도.
	ensure(TryToChangeInitState(FTIGameplayTags::Get().InitState_Spawned));

	// 다음 InitState로 변환. 강제로 상태를 업데이트.
	CheckDefaultInitialization();
}

void UTIHeroComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 앞서, OnRegister의 RegisterInitStateFeature()를 통해 InitState 시스템에 feature를 등록해 주었다면,
	// 여기서는 UnregisterInitStateFeature() 함수를 호출해서 InitState 시스템에서 feature를 등록 해제 해줌.
	UnregisterInitStateFeature();

	Super::EndPlay(EndPlayReason);
}

void UTIHeroComponent::OnActorInitStateChanged(const FActorInitStateChangedParams& Params)
{
	const FTIGameplayTags& InitTags = FTIGameplayTags::Get();
	// 바뀐 feature가 PawnExtension인지 확인.
	if (Params.FeatureName == UTIPawnExtensionComponent::NAME_ActorFeatureName)
	{
		// PawnExtensionComponent의 상태가 DataInitialized인지 체크 후, DataInitialized이면 다음 상태로 전환 될 수 있도록 CheckDefaultInitialization() 함수 호출.
		if (Params.FeatureState == InitTags.InitState_DataInitialized)
		{
			CheckDefaultInitialization();
		}
	}
}

bool UTIHeroComponent::CanChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState) const
{
	check(Manager);

	const FTIGameplayTags& InitTags = FTIGameplayTags::Get();
	APawn* Pawn = GetPawn<APawn>();
	ATIPlayerState* PS = GetPlayerState<ATIPlayerState>();

	// None -> Spawned 상태로 전환 가능한지 체크.
	if (!CurrentState.IsValid() && DesiredState == InitTags.InitState_Spawned)
	{
		// Pawn이 유효한 경우 전환 가능.
		if (Pawn)
		{
			return true;
		}
	}

	// Spawned -> DataAvailable 상태로 전환 가능한지 체크.
	if (CurrentState == InitTags.InitState_Spawned && DesiredState == InitTags.InitState_DataAvailable)
	{
		// PlayerState가 유효한 경우 전환 가능.
		if (!PS)
		{
			return false;
		}
		return true;
	}

	// DataAvailable -> DataInitialized 상태로 전환 가능한지 체크.
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		// 즉, 모든 feature들이 DataAvailable이 되어 PawnExtensionComponent의 상태가 DataInitialized가 되었을 때 비로소 HeroComponent도 DataInitialized 상태로 전환.
		return PS && Manager->HasFeatureReachedInitState(Pawn, UTIPawnExtensionComponent::NAME_ActorFeatureName, InitTags.InitState_DataInitialized);
	}

	// DataInitialized -> GameplayReady 상태로 전환 가능한지 체크.
	if (CurrentState == InitTags.InitState_DataInitialized && DesiredState == InitTags.InitState_GameplayReady)
	{
		// 현재 상태가 DataInitialized인 경우에 전환 가능.
		return true;
	}

	return false;
}

void UTIHeroComponent::CheckDefaultInitialization()
{
	// 그냥 ContinueInitStateChain() 함수를 호출해서 다음 InitState로 변경 가능한 경우 변경.
	const FTIGameplayTags& InitTags = FTIGameplayTags::Get();
	static const TArray<FGameplayTag> StateChain = { InitTags.InitState_Spawned, InitTags.InitState_DataAvailable, InitTags.InitState_DataInitialized, InitTags.InitState_GameplayReady };
	ContinueInitStateChain(StateChain);
}

void UTIHeroComponent::HandleChangeInitState(UGameFrameworkComponentManager* Manager, FGameplayTag CurrentState, FGameplayTag DesiredState)
{
	const FTIGameplayTags InitTags = FTIGameplayTags::Get();
	// 현재 상태가 DataAvailable이고, 전환 하고자 하는 상태가 DataInitialized이면, 작업을 처리함.
	if (CurrentState == InitTags.InitState_DataAvailable && DesiredState == InitTags.InitState_DataInitialized)
	{
		APawn* Pawn = GetPawn<APawn>();
		ATIPlayerState* PS = GetPlayerState<ATIPlayerState>();
		if (!ensure(Pawn && PS))
		{
			return;
		}

		// PawnExtensionComponent로부터 PawnData 얻어오기.
		const bool bIsLocallyControlled = Pawn->IsLocallyControlled();
		const UTIPawnData* PawnData = nullptr;
		if (UTIPawnExtensionComponent* PawnExtComp = UTIPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
		{
			PawnData = PawnExtComp->GetPawnData<UTIPawnData>();
		}

		// 카메라 모드 설정.
		if (bIsLocallyControlled && PawnData)
		{
			// 현재 Character에 Attach된 CameraComponent를 찾음.
			if (UTICameraComponent* CameraComponent = UTICameraComponent::FindCameraComponent(Pawn))
			{
				// CameraComponent의 delegate에 CameraMode 클래스를 리턴하는 멤버 함수 바인딩.
				CameraComponent->DetermineCameraModeDelegate.BindUObject(this, &ThisClass::DetermineCameraMode);
			}
		}

		// 입력 설정.
		if (ATIPlayerController* PC = GetController<ATIPlayerController>())
		{
			if (Pawn->InputComponent != nullptr)
			{
				// 우리가 프로젝트 설정을 통해 기본 Input Component를 우리가 만든 커스텀 Input Component인 TIInputComponent로 설정 했으므로,
				// Pawn->InputComponent = TIInputComponent가 됨.
				InitializePlayerInput(Pawn->InputComponent);
			}
		}
	}
}

TSubclassOf<UTICameraMode> UTIHeroComponent::DetermineCameraMode() const
{
	if (AbilityCameraMode)
	{
		return AbilityCameraMode;
	}

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return nullptr;
	}

	// PawnExtensionComponent에서 PawnData를 PlayerState에서 가져와 들고 있으므로, PawnExtensionComponent에 접근해 PawnData 가져옴.
	if (UTIPawnExtensionComponent* PawnExtComp = UTIPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UTIPawnData* PawnData = PawnExtComp->GetPawnData<UTIPawnData>())
		{
			return PawnData->DefaultCameraMode;
		}
	}

	return nullptr;
}

void UTIHeroComponent::InitializePlayerInput(UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	const APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	// LocalPlayer를 가져오기 위해.
	const APlayerController* PC = GetController<APlayerController>();
	check(PC);

	// EnhancedInputLocalPlayerSubsystem을 가져오기 위해.
	const ULocalPlayer* LP = PC->GetLocalPlayer();
	check(LP);

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	check(Subsystem);

	// EnhancedInputLocalPlayerSubsystem의 MappingContext를 비워줌.
	Subsystem->ClearAllMappings();

	// PawnExtensionComponent -> PawnData -> InputConfig 존재 유무 판단.
	if (const UTIPawnExtensionComponent* PawnExtComp = UTIPawnExtensionComponent::FindPawnExtensionComponent(Pawn))
	{
		if (const UTIPawnData* PawnData = PawnExtComp->GetPawnData<UTIPawnData>())
		{
			if (const UTIInputConfig* InputConfig = PawnData->InputConfig)
			{
				const FTIGameplayTags& GameplayTags = FTIGameplayTags::Get();

				// HeroComponent가 가지고 있는 Input Mapping Context를 순회하며, EnhancedInputLocalPlayerSubsystem에 추가.
				for (const FTIMappableConfigPair& Pair : DefaultInputConfigs)
				{
					if (Pair.bShouldActivateAutomatically)
					{
						FModifyContextOptions Options = {};
						Options.bIgnoreAllPressedKeysUntilRelease = false;

						// 내부적으로 Input Mapping Context를 추가함.
						Subsystem->AddPlayerMappableConfig(Pair.Config.LoadSynchronous(), Options);
					}
				}

				UTIInputComponent* IC = CastChecked<UTIInputComponent>(PlayerInputComponent);
				
				// InputTag_Move와 InputTag_Look_Mouse에 대해 각각 Input_Move()와 Input_LookMouse() 멤버 함수에 바인딩 시킴.
				// - 바인딩 한 이후, Input 이벤트에 따라 멤버 함수가 트리거 됨.			
				IC->BindNativeAction(InputConfig, GameplayTags.InputTag_Move, ETriggerEvent::Triggered, this, &ThisClass::Input_Move, false);				
				IC->BindNativeAction(InputConfig, GameplayTags.InputTag_Look_Mouse, ETriggerEvent::Triggered, this, &ThisClass::Input_LookMouse, false);
			}
		}
	}

	// GameFeatureAction_AddInputConfig의 HandlePawnExtension 콜백 함수 전달.
	// 현재 이 클래스(HeroComponent)는 마네킹 캐릭터 블루프린트에 추가 되었으므로, 마네킹 캐릭터에서 강제로 NAME_BindInputsNow 이벤트를 보냄.
	// 그러면, AddInputConfig GameFeatureAction 클래스의 콜백 함수인 HandlePawnExtension 함수가 호출됨.
	UGameFrameworkComponentManager::SendGameFrameworkComponentExtensionEvent(const_cast<APawn*>(Pawn), NAME_BindInputsNow);
}

void UTIHeroComponent::Input_Move(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	AController* Controller = Pawn ? Pawn->GetController() : nullptr;

	if (Controller)
	{
		const FVector2D Value = InputActionValue.Get<FVector2D>();
		const FRotator MovementRotation(0.0f, Controller->GetControlRotation().Yaw, 0.0f);

		if (Value.X != 0.0f)
		{
			// Left/Right -> x 값에 들어 있음.
			// MovementDirection은 현재 카메라의 RightVector를 의미함. (World-Space) local -> world
			// 월드 (0,0,0) 기준 right vector를 캐릭터의 회전에 맞춰 회전 시킴.
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);

			// AddMovementInput 함수를 한번 보자.
			// - 내부적으로 MovementDirection * Value.X를 MovementComponent에 적용(더하기)해준다.
			Pawn->AddMovementInput(MovementDirection, Value.X);
		}

		if (Value.Y != 0.0f) // 앞서 우리는 Forward 적용을 위해 swizzle input modifier를 사용 했음.
		{
			// 앞서 Left/Right와 마찬가지로 Forward/Backward를 적용.
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
			Pawn->AddMovementInput(MovementDirection, Value.Y);
		}
	}
}

void UTIHeroComponent::Input_LookMouse(const FInputActionValue& InputActionValue)
{
	APawn* Pawn = GetPawn<APawn>();
	if (!Pawn)
	{
		return;
	}

	const FVector2D Value = InputActionValue.Get<FVector2D>();

	if (Value.X != 0.0f)
	{
		// X에는 Yaw 값이 있음.
		// - Camera에 대해  Yaw 적용.
		Pawn->AddControllerYawInput(Value.X);
	}

	if (Value.Y != 0.0f)
	{
		// Y에는 Pitch 값이 있음.
		double AimInverseValue = -Value.Y;
		Pawn->AddControllerPitchInput(AimInverseValue);
	}
}
