// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"

#include "GASAnimationTypes.generated.h"

/**
 * Experimental state machine states for animation system.
 */
UENUM(BlueprintType)
enum class EExperimentalStateMachineState : uint8
{
	IdleLoop,
	TransitionToIdleLoop,
	LocomotionLoop,
	TransitionToLocomotionLoop,
	InAirLoop,
	TransitionToInAirLoop,
	IdleBreak,
	TransitionToSlide,
	SlideLoop
};

/**
 * Traversal action types for obstacle navigation.
 */
UENUM(BlueprintType)
enum class ETraversalActionType : uint8
{
	None,
	/** Traverse over a thin object and end on the ground at a similar level (Low fence). */
	Hurdle,
	/** Traverse over a thin object and end in a falling state (Tall fence, or elevated obstacle with no floor on the other side). */
	Vault,
	/** Traverse up and onto an object without passing over it. */
	Mantle
};

/**
 * Struct used in Experimental State Machine to drive Blend Stack inputs.
 */
USTRUCT(BlueprintType)
struct FBlendStackInputs
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimationAsset* Anim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Loop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BlendTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBlendProfile* BlendProfile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> Tags;
};

/**
 * Outputs for animation chooser.
 */
USTRUCT(BlueprintType)
struct FChooserOutputs
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool UseMM;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MMCostLimit;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BlendTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName BlendProfile;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FName> Tags;
};

/**
 * Debug graph line properties.
 */
USTRUCT(BlueprintType)
struct FDebugGraphLineProperties
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor Color;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<float> Values;
};

/**
 * Pose history entry for motion matching.
 */
USTRUCT(BlueprintType)
struct FPoseHistoryEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform Transform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Timestamp;
};

/**
 * Pose history for motion matching and traversal.
 */
USTRUCT(BlueprintType)
struct FPoseHistory
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPoseHistoryEntry> Entries;
};

/**
 * Auto idle state configuration.
 */
USTRUCT(BlueprintType)
struct FAutoIdleState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Auto Idle")
	bool IsAutoIdle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Auto Idle")
	float InTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Auto Idle")
	float LastMoveTime;
};