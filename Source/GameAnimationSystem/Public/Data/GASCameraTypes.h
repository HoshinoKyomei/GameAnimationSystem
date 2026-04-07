// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GASCameraTypes.generated.h"

/**
 * Camera movement mode types.
 */
UENUM(BlueprintType)
enum class ECameraMode : uint8
{
	FreeCam,
	Strafe,
	Aim,
	TwinStick
};

/**
 * Camera distance/style types.
 */
UENUM(BlueprintType)
enum class ECameraStyle : uint8
{
	Close,
	Medium,
	Far,
	Debug
};

/**
 * Character properties for camera system.
 */
USTRUCT(BlueprintType)
struct FCharacterPropertiesForCamera
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECameraStyle CameraStyle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ECameraMode CameraMode;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Gait; // EGait - stored as uint8 to avoid dependency on GASMovementTypes

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 Stance; // EStance - stored as uint8 to avoid dependency on GASMovementTypes
};