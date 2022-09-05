#pragma once

#include "GASExtGameFeatureAction_AddAbilities.h"

#include <CoreMinimal.h>
#include <GameFeatureAction.h>
#include <GameFeatureStateChangeObserver.h>

#include "GASExtGameFeatureAction_AddGameplayCuePath.generated.h"

UCLASS( MinimalAPI, meta = ( DisplayName = "Add Gameplay Cue Path" ) )
class UGASExtGameFeatureAction_AddGameplayCuePath final : public UGameFeatureAction
{
    GENERATED_BODY()

public:
    const TArray< FDirectoryPath > & GetDirectoryPathsToAdd() const;

#if WITH_EDITOR
    EDataValidationResult IsDataValid( TArray< FText > & validation_errors ) override;
#endif
private:
    /** List of paths to register to the gameplay cue manager. These are relative tot he game content directory */
    UPROPERTY( EditAnywhere, Category = "Game Feature | Gameplay Cues", meta = ( RelativeToGameContentDir, LongPackageName ) )
    TArray< FDirectoryPath > DirectoryPathsToAdd;
};

UCLASS()
class GASEXTENSIONS_API UGASExtGameFeatureObserver_AddGameplayCuePath : public UObject, public IGameFeatureStateChangeObserver
{
    GENERATED_BODY()

public:
    void OnGameFeatureRegistering( const UGameFeatureData * game_feature_data, const FString & plugin_name ) override;
    void OnGameFeatureUnregistering( const UGameFeatureData * game_feature_data, const FString & plugin_name ) override;
};

FORCEINLINE const TArray< FDirectoryPath > & UGASExtGameFeatureAction_AddGameplayCuePath::GetDirectoryPathsToAdd() const
{
    return DirectoryPathsToAdd;
}
