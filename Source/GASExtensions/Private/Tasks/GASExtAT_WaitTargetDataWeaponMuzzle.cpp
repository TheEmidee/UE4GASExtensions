#include "Tasks/GASExtAT_WaitTargetDataWeaponMuzzle.h"

#include "Targeting/GASExtTargetingHelperLibrary.h"

FGASExtWaitTargetDataMuzzleOptions::FGASExtWaitTargetDataMuzzleOptions()
{
    MaxRange.Value = 999999.0f;
    TargetingSpread.Value = 0.0f;
}

UGASExtAT_WaitTargetDataWeaponMuzzle * UGASExtAT_WaitTargetDataWeaponMuzzle::WaitTargetDataHitWeaponMuzzle( UGameplayAbility * owning_ability, FName task_instance_name, const FGASExtWaitTargetDataReplicationOptions & replication_options, const FGASExtWaitTargetDataMuzzleOptions & muzzle_options )
{
    auto * my_obj = NewAbilityTask< UGASExtAT_WaitTargetDataWeaponMuzzle >( owning_ability, task_instance_name );
    my_obj->ReplicationOptions = replication_options;
    my_obj->WeaponMuzzleOptions = muzzle_options;
    return my_obj;
}

FGameplayAbilityTargetDataHandle UGASExtAT_WaitTargetDataWeaponMuzzle::ProduceTargetData()
{
    const auto muzzle_socket_transform = WeaponMuzzleOptions.WeaponMuzzleLocationInfo.GetTargetingTransform();
    auto trace_start = muzzle_socket_transform.GetLocation();
    FVector trace_end;

    FCollisionQueryParams collision_query_params( SCENE_QUERY_STAT( USWAT_WaitTargetDataWeaponMuzzle ), WeaponMuzzleOptions.CollisionInfo.bUsesTraceComplex );
    collision_query_params.bReturnPhysicalMaterial = WeaponMuzzleOptions.CollisionInfo.bReturnsPhysicalMaterial;

    TArray< FSWTargetingLocationInfo > location_infos;

    for ( auto projectile_index = 0; projectile_index < WeaponMuzzleOptions.AmountOfProjectiles.GetValue(); ++projectile_index )
    {
        const auto muzzle_forward_vector = muzzle_socket_transform.Rotator().Vector();
        trace_end = trace_start + muzzle_forward_vector * WeaponMuzzleOptions.MaxRange.GetValue();

        UGASExtTargetingHelperLibrary::ComputeTraceEndWithSpread(
            trace_end,
            FSWSpreadInfos(
                trace_start,
                WeaponMuzzleOptions.TargetingSpread.GetValue(),
                WeaponMuzzleOptions.MaxRange.GetValue() ) );

        // Don't pass the original muzzle location infos to the server because it's only valid on the owning client

        FSWTargetingLocationInfo location_info;

        location_info.Source.LocationType = EGameplayAbilityTargetingLocationType::LiteralTransform;
        location_info.Source.LiteralTransform = WeaponMuzzleOptions.WeaponMuzzleLocationInfo.GetTargetingTransform();

        location_info.Target.LocationType = EGameplayAbilityTargetingLocationType::LiteralTransform;
        location_info.Target.LiteralTransform = FTransform::Identity;
        location_info.Target.LiteralTransform.SetLocation( trace_end );

        location_infos.Emplace( location_info );
    }

    const auto target_data_handle = UGASExtTargetingHelperLibrary::MakeTargetDataFromMultipleLocationInfos( location_infos );

    return target_data_handle;
}
