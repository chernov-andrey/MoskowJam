// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UW_MainMenu.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class MOSKOWJAM_API UUW_MainMenu : public UUserWidget
{
	GENERATED_BODY()
protected:
	UPROPERTY(BlueprintReadOnly,meta = (BindWidget))
	TObjectPtr<UButton> Exit_Button;

	virtual void NativeConstruct()override;

	

};
