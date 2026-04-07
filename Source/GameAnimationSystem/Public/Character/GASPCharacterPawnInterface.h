// Copyright Soatori Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Data/GASPDataTypes.h"

#include "GASPCharacterPawnInterface.generated.h"

/**
 * Interface for character pawns to provide animation data.
 */
UINTERFACE(Blueprintable)
class UGASPCharacterPawnInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for character pawns to provide animation data.
 */
class GAMEANIMATIONSYSTEM_API IGASPCharacterPawnInterface
{
	GENERATED_BODY()

public:
	/**
	 * Set character input state.
	 * @param InInputState The input state to set.
	*/
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Setters")
	void Set_CharacterInputState(const FPlayerInputState& InInputState);
	virtual void Set_CharacterInputState_Implementation(const FPlayerInputState& InInputState) { }

	/**
	 * Get character properties for animation system.
	 * @return The character properties struct containing all animation-relevant data.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Getters")
	FCharacterPropertiesForAnimation Get_PropertiesForAnimation() const;
	virtual FCharacterPropertiesForAnimation Get_PropertiesForAnimation_Implementation() const { return FCharacterPropertiesForAnimation(); }

	/**
	 * Get character properties for camera system.
	 * @return The character properties struct containing all camera-relevant data.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Getters")
	FCharacterPropertiesForCamera Get_PropertiesForCamera() const;
	virtual FCharacterPropertiesForCamera Get_PropertiesForCamera_Implementation() const { return FCharacterPropertiesForCamera(); }

	/**
	 * Get character properties for traversal system.
	 * @return The character properties struct containing all traversal-relevant data.
	 */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Getters")
	FCharacterPropertiesForTraversal Get_PropertiesForTraversal() const;
	virtual FCharacterPropertiesForTraversal Get_PropertiesForTraversal_Implementation() const { return FCharacterPropertiesForTraversal(); }
};