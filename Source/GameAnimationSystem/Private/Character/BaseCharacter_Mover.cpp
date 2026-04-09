// Copyright Soatori Games, Inc. All Rights Reserved.

#include "Character/BaseCharacter_Mover.h"

#include "Camera/CameraComponent.h"
#include "Character/TraversalLogicComponent.h"
#include "Data/GASPDataTypes.h"
#include "DefaultMovementSet/CharacterMoverComponent.h"
#include "GameFramework/GameplayCameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

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
	FTraversalCheckInputs Result;

	// Get current movement mode
	const EGASPMovementMode MovementMode = Get_CurrentMovementMode();

	// Get CharacterMover component
	UCharacterMoverComponent* MoverComp = GetCharacterMover();
	if (!MoverComp)
	{
		return Result;
	}

	switch (MovementMode)
	{
		case EGASPMovementMode::OnGround:
		case EGASPMovementMode::Sliding:
		case EGASPMovementMode::Traversing:
		{
			// OnGround/Sliding/Traversing logic
			const FVector Velocity = MoverComp->GetVelocity();
			const float SpeedXY = Velocity.Size2D();

			// TraceForwardDirection: velocity direction or target orientation
			if (SpeedXY > 50.0f)
			{
				Result.TraceForwardDirection = Velocity.GetSafeNormal();
			}
			else
			{
				Result.TraceForwardDirection = MoverComp->GetTargetOrientation().Vector();
			}

			// TraceForwardDistance: MapRangeClamped(local velocity X, 0-375 → 75-300)
			const FRotator ActorRotation = GetActorRotation();
			const FVector LocalVelocity = ActorRotation.UnrotateVector(Velocity);
			const float LocalVelocityX = LocalVelocity.X;
			Result.TraceForwardDistance = FMath::GetMappedRangeValueClamped(
				FVector2D(0.0f, 375.0f),
				FVector2D(75.0f, 300.0f),
				LocalVelocityX
			);

			Result.TraceRadius = 30.0f;
			Result.TraceHalfHeight = 60.0f;
			break;
		}

		case EGASPMovementMode::InAir:
		{
			// InAir logic - use move input
			const FVector MoveInput = MoverDefaultInputs_PostSim.GetMoveInput();

			if (!MoveInput.IsNearlyZero())
			{
				Result.TraceForwardDirection = MoveInput.GetSafeNormal();
			}
			else
			{
				Result.TraceForwardDirection = GetActorForwardVector();
			}

			Result.TraceForwardDistance = 75.0f;
			Result.TraceEndOffset = FVector(0.0f, 0.0f, 50.0f);
			Result.TraceRadius = 30.0f;
			Result.TraceHalfHeight = 86.0f;
			break;
		}
	}

	return Result;
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

bool ABaseCharacter_Mover::Jump_Implementation()
{
	// Check if not currently doing traversal action
	if (!TraversalLogic || TraversalLogic->bDoingTraversalAction)
	{
		return false;
	}

	// Get traversal check inputs
	FTraversalCheckInputs Inputs = GetTraversalCheckInputs();

	// Try traversal action
	bool TraversalCheckFailed = false;
	bool MontageSelectionFailed = false;
	TraversalLogic->TryTraversalAction(Inputs, EDrawDebugTrace::ForOneFrame, TraversalCheckFailed, MontageSelectionFailed);

	// Return true if either check failed (jump failed)
	return TraversalCheckFailed || MontageSelectionFailed;
}

FVector2D ABaseCharacter_Mover::Get_Move_Implementation() const
{
	// TODO: Implement move input retrieval logic
	return FVector2D::ZeroVector;
}

FVector2D ABaseCharacter_Mover::Get_TwinStick_AimDirection_Implementation() const
{
	// TODO: Implement twin-stick aim direction retrieval logic
	return FVector2D::ZeroVector;
}

void ABaseCharacter_Mover::OnRotationIdle_Implementation()
{
	// Early exit if auto idle is not enabled
	if (!IdleSet.IsAutoIdle)
	{
		return;
	}

	// Only process OnGround movement mode
	const EGASPMovementMode MovementMode = Get_CurrentMovementMode();
	if (MovementMode != EGASPMovementMode::OnGround)
	{
		return;
	}

	// Get current game time
	const float CurrentTime = GetWorld()->GetTimeSeconds();

	// Check 1: If there is move input, update last move time and enable strafe
	const FVector MoveInput = MoverDefaultInputs_PreSim.GetMoveInput();
	if (!MoveInput.IsNearlyZero())
	{
		IdleSet.LastMoveTime = CurrentTime;
		PlayerInputState.WantsToStrafe = true;
		return;
	}

	// Check 2: If aiming, update last move time and enable strafe
	if (PlayerInputState.WantsToAim)
	{
		IdleSet.LastMoveTime = CurrentTime;
		PlayerInputState.WantsToStrafe = true;
		return;
	}

	// Check 3: If idle for longer than InTime threshold, disable strafe
	const float ElapsedTime = CurrentTime - IdleSet.LastMoveTime;
	if (ElapsedTime > IdleSet.InTime)
	{
		PlayerInputState.WantsToStrafe = false;
		return;
	}

	// Check 4: If rotation difference exceeds threshold (>10° yaw), enable strafe
	const FRotator ActorRotation = GetActorRotation();
	const FRotator AimingRotation = Get_AimingRotation();
	const FRotator DeltaRotation = UKismetMathLibrary::NormalizedDeltaRotator(ActorRotation, AimingRotation);
	const float YawDiff = FMath::Abs(DeltaRotation.Yaw);

	if (YawDiff > 10.0f)
	{
		IdleSet.LastMoveTime = CurrentTime;
		PlayerInputState.WantsToStrafe = true;
	}
}

void ABaseCharacter_Mover::DebugDraws_Implementation()
{
}

void ABaseCharacter_Mover::Look_Implementation(const FVector2D& ActionValue)
{
	AddControllerYawInput(ActionValue.X);
	AddControllerPitchInput(ActionValue.Y);
}

void ABaseCharacter_Mover::LookGamepad_Implementation(const FVector2D& ActionValue)
{
	// Only process if NOT in TwinStick mode
	if (!bTwinStickMode)
	{
		const float DeltaTime = GetWorld()->GetDeltaSeconds();
		const FVector2D ScaledInput = ActionValue * DeltaTime;
		AddControllerYawInput(ScaledInput.X);
		AddControllerPitchInput(ScaledInput.Y);
	}
}

void ABaseCharacter_Mover::Walk_Implementation()
{
	// Only toggle walk if not sprinting
	if (!PlayerInputState.WantsToSprint)
	{
		PlayerInputState.WantsToWalk = !PlayerInputState.WantsToWalk;
	}
}

void ABaseCharacter_Mover::Sprint_Implementation(bool ActionValue)
{
	// Set sprint state
	PlayerInputState.WantsToSprint = ActionValue;

	// If starting sprint while crouching, stop crouching
	if (ActionValue)
	{
		UCharacterMoverComponent* MoverComp = GetCharacterMover();
		if (MoverComp && MoverComp->IsCrouching())
		{
			PlayerInputState.WantsToCrouch = false;
		}
	}
}

void ABaseCharacter_Mover::JumpFailed_Implementation(bool IsFailed)
{
	if (IsFailed)
	{
		UCharacterMoverComponent* MoverComp = GetCharacterMover();
		if (MoverComp && MoverComp->IsCrouching())
		{
			// If crouching, stop crouching
			PlayerInputState.WantsToCrouch = false;
		}
		else
		{
			// Set jump pressed flag for one frame
			bJump_JustPressed = true;
			// Reset next frame via latent action
			GetWorldTimerManager().SetTimerForNextTick([this]()
			{
				bJump_JustPressed = false;
			});
		}
	}
}

void ABaseCharacter_Mover::Crouch_Implementation()
{
	// Get current movement mode
	const EGASPMovementMode MovementMode = Get_CurrentMovementMode();

	// Check if OnGround OR InAir
	if (MovementMode == EGASPMovementMode::OnGround || MovementMode == EGASPMovementMode::Sliding)
	{
		// Use GetCharacterMover() to access the private CharacterMover member
		if (UCharacterMoverComponent* MoverComp = GetCharacterMover())
		{
			if (MoverComp->IsCrouching())
			{
				// Already crouching -> stop crouching
				PlayerInputState.WantsToCrouch = false;
			}
			else
			{
				// Not crouching -> start crouching
				PlayerInputState.WantsToCrouch = true;
			}
		}
	}
}

void ABaseCharacter_Mover::Strafe_Implementation()
{
	// Toggle strafe state
	PlayerInputState.WantsToStrafe = !PlayerInputState.WantsToStrafe;
}

void ABaseCharacter_Mover::Aim_Implementation(bool ActionValue)
{
	// Set aim state
	PlayerInputState.WantsToAim = ActionValue;
}

void ABaseCharacter_Mover::CameraDown_Implementation()
{
}

void ABaseCharacter_Mover::CameraUp_Implementation()
{
}