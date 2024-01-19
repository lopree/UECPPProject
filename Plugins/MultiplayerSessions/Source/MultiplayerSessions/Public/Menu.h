#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Menu.generated.h"

class UButton;
UCLASS()
class MULTIPLAYERSESSIONS_API UMenu : public UUserWidget
{
	GENERATED_BODY()
public:
	//关卡蓝图中调用
	UFUNCTION(BlueprintCallable)
	void MenuSetUp(int NumPublicConnections = 4, FString MatchType = TEXT("FreeForAll"),FString LObbyPath = TEXT("/Game/Maps/LobbyMap"));
protected:
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;
	//自定义动态多播的回调
	UFUNCTION()
	void OnCreateSession(bool bWasSuccessful);
	void OnFindSession(const TArray<FOnlineSessionSearchResult>& SearchResults, bool bWasSuccessful);
	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result);
	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);
	UFUNCTION()
	void OnStartSession(bool bWasSuccessful);
private:
	UPROPERTY(meta=(BindWidget))
	UButton* HostButton;
	UPROPERTY(meta=(BindWidget))
	UButton* JoinButton;

	UFUNCTION()
	void HostBtnClicked();
	UFUNCTION()
	void JoinBtnClicked();
	//移除UI
	void MenuTearDown();
	//管理所有的Session
	class UMultiplayerSessionSubsystem* MultiplayerSessionSubsystem;

	int32 CustomNumPublicConnections{4};
	FString MatchType{TEXT("FreeForAll")};
	FString PathToLobby{TEXT("")};
};
