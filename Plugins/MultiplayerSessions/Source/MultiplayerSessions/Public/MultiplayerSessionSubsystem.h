#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionDelegates.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MultiplayerSessionSubsystem.generated.h"

//声明一个动态多播带有一个bool参数,在Menu中绑定回调
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FMultiplayerOnCreateSessionsComplete,bool,bWasSuccessful);
UCLASS()
class MULTIPLAYERSESSIONS_API UMultiplayerSessionSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	UMultiplayerSessionSubsystem();
	//在UI界面（Menu中）,触发方法：
	//创建
	void CreatSession(int32 NumPublicConnections, FString MatchType);
	//寻找
	void FindSession(int32 MaxSearchResults);
	//加入
	void JoinSession(const FOnlineSessionSearchResult& SearchResult);
	void DestroySession();
	void StartSession();
	//动态多播
	FMultiplayerOnCreateSessionsComplete MultiplayerOnCreateSessionsComplete;
protected:
	//回调
	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);
	void OnFindSessionsComplete(bool bWasSuccessful);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
	void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);
	void OnStartSessionComplete(FName SessionName, bool bWasSuccessful);
private:
	IOnlineSessionPtr SessionInterface;
	//Session Setting 存储最后一次会话设定，方便下次创建
	TSharedPtr<class FOnlineSessionSettings> LastSessionSettings;
	//委托
	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FDelegateHandle CreateSessionCompleteDelegateHandle;
	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	FDelegateHandle FindSessionsCompleteDelegateHandle;
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	FDelegateHandle JoinSessionCompleteDelegateHandle;
	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	FDelegateHandle DestroySessionCompleteDelegateHandle;
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;
	FDelegateHandle StartSessionCompleteDelegateHandle;
};
