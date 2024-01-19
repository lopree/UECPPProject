#include "MultiplayerSessionSubsystem.h"

UMultiplayerSessionSubsystem::UMultiplayerSessionSubsystem()
{
	//绑定
	OnCreateSessionCompleteDelegate.BindUObject(this, &UMultiplayerSessionSubsystem::OnCreateSessionComplete);
	OnFindSessionsCompleteDelegate.BindUObject(this, &UMultiplayerSessionSubsystem::OnFindSessionsComplete);
	OnJoinSessionCompleteDelegate.BindUObject(this, &UMultiplayerSessionSubsystem::OnJoinSessionComplete);
	OnDestroySessionCompleteDelegate.BindUObject(this, &UMultiplayerSessionSubsystem::OnDestroySessionComplete);
	OnStartSessionCompleteDelegate.BindUObject(this, &UMultiplayerSessionSubsystem::OnStartSessionComplete);
	if (const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get())
	{
		SessionInterface = OnlineSubsystem->GetSessionInterface();
	}
}

void UMultiplayerSessionSubsystem::CreatSession(int32 NumPublicConnections, FString MatchType)
{
	
}

void UMultiplayerSessionSubsystem::FindSession(int32 MaxSearchResults)
{
}

void UMultiplayerSessionSubsystem::JoinSession(const FOnlineSessionSearchResult& SearchResult)
{
}

void UMultiplayerSessionSubsystem::DestroySession()
{
}

void UMultiplayerSessionSubsystem::StartSession()
{
}

void UMultiplayerSessionSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
}

void UMultiplayerSessionSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
}

void UMultiplayerSessionSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
}

void UMultiplayerSessionSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
}

void UMultiplayerSessionSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
}
