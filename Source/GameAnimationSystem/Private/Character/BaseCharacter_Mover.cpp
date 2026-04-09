// Copyright Soatori Games, Inc. All Rights Reserved.

#include "Character/BaseCharacter_Mover.h"

#include "Camera/CameraComponent.h"
#include "Character/TraversalLogicComponent.h"
#include "DefaultMovementSet/CharacterMoverComponent.h"
#include "GameFramework/GameplayCameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

ABaseCharacter_Mover::ABaseCharacter_Mover(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Create Motion Warping component
	MotionWarping = CreateDefaultSubobject<UMotionWarpingComponent>(TEXT("MotionWarping"));

	// Create Traversal Logic component
	TraversalLogic = CreateDefaultSubobject<UTraversalLogicComponent>(TEXT("TraversalLogic"));

	// Create Gameplay Camera component (attached to Mesh, auto-activate off, run in editor false)
	GameplayCamera = CreateDefaultSubobject<UGameplayCameraComponent>(TEXT("GameplayCamera"));
	GameplayCamera->SetupAttachment(GetMesh());
	GameplayCamera->SetAutoActivate(false);
	GameplayCamera->bRunInEditor = false;

	// Create Spring Arm component (attached to Capsule)
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());

	// Create Camera component (attached to SpringArm)
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
}

void ABaseCharacter_Mover::BeginPlay()
{
	Super::BeginPlay();

	UCharacterMoverComponent* MoverComp = GetCharacterMover();
	if (MoverComp)
	{
		// Bind OnMovementModeChanged (AssignDelegate = replace existing bindings)
		MoverComp->OnMovementModeChanged.Clear();
		MoverComp->OnMovementModeChanged.AddDynamic(this, &ABaseCharacter_Mover::OnMovementModeChanged);

		// Add OnPreSimulationTick binding (AddDelegate = add to existing bindings)
		MoverComp->OnPreSimulationTick.AddDynamic(this, &ABaseCharacter_Mover::OnPreSimulateTick);
	}

	// Set Tick prerequisite order: SkeletalMesh depends on CharacterMover
	if (GetMesh() && MoverComp)
	{
		GetMesh()->AddTickPrerequisiteComponent(MoverComp);
	}

	// Set Tick prerequisite order: SkeletalMesh depends on this Actor
	if (GetMesh())
	{
		GetMesh()->AddTickPrerequisiteActor(this);
	}

	// Initialize MoverDefaultInputs_PreSim.OrientationIntent to Actor forward vector
	MoverDefaultInputs_PreSim.OrientationIntent = GetActorForwardVector();
}

void ABaseCharacter_Mover::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Call update functions in sequence (BlueprintNativeEvent pattern)
	CacheInputsFromMover();
	Update_FloorValues();
	Update_ControlRotationRate();
	Update_SlidingAudio();
	Update_TargetedActor();
	Update_TwinStickMode();
	DebugDraws();
}

void ABaseCharacter_Mover::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Call client RPC to run setup on owning client
	ClientPossessed();
}

void ABaseCharacter_Mover::ClientPossessed_Implementation()
{
	// Runs on owning client only - setup camera and input
	SetupCamera();
	SetupInput();
}

void ABaseCharacter_Mover::ProduceInput_Implementation(int32 SimTimeMs, FMoverInputCmdContext& InputCmdResult)
{
	// Default implementation - can be extended in Blueprint or subclass
}

void ABaseCharacter_Mover::Set_CharacterInputState_Implementation(const FPlayerInputState& InInputState)
{
	PlayerInputState = InInputState;
}

FCharacterPropertiesForAnimation ABaseCharacter_Mover::Get_PropertiesForAnimation_Implementation() const
{
	return FCharacterPropertiesForAnimation();
}

FCharacterPropertiesForCamera ABaseCharacter_Mover::Get_PropertiesForCamera_Implementation() const
{
	return FCharacterPropertiesForCamera();
}

FCharacterPropertiesForTraversal ABaseCharacter_Mover::Get_PropertiesForTraversal_Implementation() const
{
	return FCharacterPropertiesForTraversal();
}

void ABaseCharacter_Mover::SetupInput_Implementation()
{
}

void ABaseCharacter_Mover::SetupCamera_Implementation()
{
}

void ABaseCharacter_Mover::Update_SlidingAudio_Implementation()
{
}

void ABaseCharacter_Mover::Update_TwinStickMode_Implementation()
{
}

void ABaseCharacter_Mover::Update_TargetedActor_Implementation()
{
}

void ABaseCharacter_Mover::Update_ControlRotationRate_Implementation()
{
}

void ABaseCharacter_Mover::Update_FloorValues_Implementation()
{
}

FTraversalCheckInputs ABaseCharacter_Mover::GetTraversalCheckInputs_Implementation() const
{
	return FTraversalCheckInputs();
}

EGASPMovementMode ABaseCharacter_Mover::Get_CurrentMovementMode_Implementation() const
{
	return EGASPMovementMode::OnGround;
}

FRotator ABaseCharacter_Mover::Get_AimingRotation_Implementation() const
{
	return FRotator::ZeroRotator;
}

ERotationMode ABaseCharacter_Mover::Get_RotationMode_Implementation() const
{
	return ERotationMode::OrientToMovement;
}

FVector ABaseCharacter_Mover::Get_OrientationIntent_Implementation() const
{
	return FVector::ZeroVector;
}

EGait ABaseCharacter_Mover::Get_Gait_Implementation() const
{
	return EGait::Walk;
}

float ABaseCharacter_Mover::Get_Speed_Implementation() const
{
	return 0.0f;
}

FMovementDirectionThresholds ABaseCharacter_Mover::Get_MovementDirectionThresholds_Implementation() const
{
	return FMovementDirectionThresholds();
}

EMovementDirection ABaseCharacter_Mover::Get_MovementDirectionFromThresholds_Implementation(
	const FMovementDirectionThresholds& Thresholds, float Direction) const
{
	return EMovementDirection::F;
}

void ABaseCharacter_Mover::Get_MovementDirectionAndOffset_Implementation(EMovementDirection& MovementDirection, float& RotationOffset)
{
	MovementDirection = EMovementDirection::F;
	RotationOffset = 0.0f;
}

void ABaseCharacter_Mover::CacheInputsFromMover_Implementation()
{
}

void ABaseCharacter_Mover::OnMovementModeChanged_Implementation(const FName& PreviousMovementModeName, const FName& NewMovementModeName)
{
}

void ABaseCharacter_Mover::OnPreSimulateTick_Implementation(const FMoverTimeStep& TimeStep, const FMoverInputCmdContext& InputCmd)
{
}

bool ABaseCharacter_Mover::Jump_Implementation() const
{
	return false;
}

FVector2D ABaseCharacter_Mover::Get_Move_Implementation() const
{
	return FVector2D::ZeroVector;
}

FVector2D ABaseCharacter_Mover::Get_TwinStick_AimDirection_Implementation() const
{
	return FVector2D::ZeroVector;
}

void ABaseCharacter_Mover::OnRotationIdle_Implementation()
{
}

void ABaseCharacter_Mover::DebugDraws_Implementation()
{
}

void ABaseCharacter_Mover::Look_Implementation(const FVector2D& ActionValue)
{
}

void ABaseCharacter_Mover::LookGamepad_Implementation(const FVector2D& ActionValue)
{
}

void ABaseCharacter_Mover::Walk_Implementation()
{
}

void ABaseCharacter_Mover::Sprint_Implementation()
{
}

void ABaseCharacter_Mover::JumpFailed_Implementation(bool IsFailed)
{
}

void ABaseCharacter_Mover::Crouch_Implementation()
{
}

void ABaseCharacter_Mover::Strafe_Implementation()
{
}

void ABaseCharacter_Mover::Aim_Implementation()
{
}

void ABaseCharacter_Mover::CameraDown_Implementation()
{
}

void ABaseCharacter_Mover::CameraUp_Implementation()
{
}