// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "GASInputTypes.generated.h"

/**
 * Player input state.
 */
USTRUCT(BlueprintType)
struct FPlayerInputState
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool WantsToSprint;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool WantsToWalk;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool WantsToStrafe;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool WantsToAim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool WantsToCrouch;
};