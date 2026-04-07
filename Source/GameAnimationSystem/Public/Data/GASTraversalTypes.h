// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "MotionWarpingComponent.h"

#include "GASMovementTypes.h"
#include "GASAnimationTypes.h"

#include "GASTraversalTypes.generated.h"

/**
 * Input parameters for traversal check.
 */
USTRUCT(BlueprintType)
struct FTraversalCheckInputs
{
	GENERATED_BODY()

	/** Direction to trace forward. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TraceForwardDirection;

	/** Distance to trace forward. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TraceForwardDistance;

	/** Offset for trace origin. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TraceOriginOffset;

	/** Offset for trace end. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TraceEndOffset;

	/** Radius for trace capsule. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TraceRadius;

	/** Half height for trace capsule. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TraceHalfHeight;
};

/**
 * Result of traversal check.
 */
USTRUCT(BlueprintType)
struct FTraversalCheckResult
{
	GENERATED_BODY()

	/** Type of traversal action. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETraversalActionType ActionType;

	/** Whether there is a front ledge. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasFrontLedge;

	/** Location of the front ledge. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector FrontLedgeLocation;

	/** Normal of the front ledge. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector FrontLedgeNormal;

	/** Whether there is a back ledge. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasBackLedge;

	/** Location of the back ledge. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector BackLedgeLocation;

	/** Normal of the back ledge. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector BackLedgeNormal;

	/** Whether there is back floor. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasBackFloor;

	/** Location of the back floor. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector BackFloorLocation;

	/** Height of the obstacle. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ObstacleHeight;

	/** Depth of the obstacle. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ObstacleDepth;

	/** Height of the back ledge. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BackLedgeHeight;

	/** Component that was hit. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UPrimitiveComponent* HitComponent;

	/** Chosen animation montage. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* ChosenMontage;

	/** Start time for the animation. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StartTime;

	/** Play rate for the animation. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayRate;
};

/**
 * Input parameters for traversal chooser.
 */
USTRUCT(BlueprintType)
struct FTraversalChooserInputs
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETraversalActionType ActionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasFrontLedge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasBackLedge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasBackFloor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ObstacleHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ObstacleDepth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BackLedgeHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DistanceToLedge;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGASPMovementMode MovementMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGait Gait;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPoseHistory PoseHistory;
};

/**
 * Output from traversal chooser.
 */
USTRUCT(BlueprintType)
struct FTraversalChooserOutputs
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETraversalActionType ActionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MontageStartTime;
};

/**
 * Character properties for traversal system.
 */
USTRUCT(BlueprintType)
struct FCharacterPropertiesForTraversal
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCapsuleComponent* Capsule;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UMotionWarpingComponent* MotionWarping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGASPMovementMode MovementMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGait Gait;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Speed;
};