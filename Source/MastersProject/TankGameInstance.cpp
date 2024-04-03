// Masters Project - Owen S Atkinson


#include "TankGameInstance.h"

#include "MoviePlayer.h"
#include "Blueprint/UserWidget.h"

void UTankGameInstance::Init()
{
	Super::Init();
	//Add Delegates - One for Preloading and one once the loading is complete
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UTankGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UTankGameInstance::EndLoadingScreen);
}

void UTankGameInstance::BeginLoadingScreen(const FString& MapName)
{
	if (!IsRunningDedicatedServer())
	{
		//Set up Loading screen attributes and give it to MoviePlayer to player. Should auto terminate once EndLoadingScreen is Called
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
		//MinLoading Screen should be only used for testing unless we decide to add hints or something else we want the player to see,
		//in which case it could be useful.
		LoadingScreen.MinimumLoadingScreenDisplayTime = 0.5f;
		//Set Widget Loading Screen here. Can also use a movie file instead.
		if(LoadingWidget != nullptr)
		{
			UUserWidget* LoadWidget = CreateWidget<UUserWidget>(this,LoadingWidget);
			LoadingScreen.WidgetLoadingScreen = LoadWidget->TakeWidget();
		}
 
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}
}

void UTankGameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{
}
