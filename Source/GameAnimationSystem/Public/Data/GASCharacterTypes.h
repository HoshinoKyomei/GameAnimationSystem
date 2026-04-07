// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "GASMovementTypes.h"
#include "GASInputTypes.h"
#include "GASCameraTypes.h"

#include "GASCharacterTypes.generated.h"

/**
 * Character properties for animation system.
 */
USTRUCT(BlueprintType)
struct FCharacterPropertiesForAnimation
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlayerInputState InputState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EMovementMode> MovementMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EStance Stance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERotationMode RotationMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGait Gait;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMovementDirection MovementDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FTransform ActorTransform;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector InputAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentMaxAcceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float CurrentMaxDeceleration;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator OrientationIntent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator AimingRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bJustLanded;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector LandVelocity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SteeringTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector GroundNormal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector GroundLocation;
};

/**
 * Input parameters for rotation offset curve chooser.
 */
USTRUCT(BlueprintType)
struct FRotationOffsetCurveChooser_Inputs
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGASPMovementMode MovementMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EMovementDirection MovementDirection;
};