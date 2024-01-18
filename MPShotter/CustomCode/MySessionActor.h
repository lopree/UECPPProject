// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "MySessionActor.generated.h"

UCLASS()
class MPSHOTTER_API AMySessionActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AMySessionActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	IOnlineSessionPtr OnlineSessionInterface;
protected:
	//创建会话的函数
	UFUNCTION(BlueprintCallable)
	void CreateMySession();
	//成功创建会话的回调
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
private:
	//创建一个会话的委托,这是一个已经封装好的多播委托
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
};
