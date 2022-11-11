// TechnoDarkness
// Author: Alexey Faradjev (myndbyrd)

#pragma once

#include "CoreMinimal.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CombinedSkeletalMeshComponent.generated.h"

/////////////////////////////////////////////////////////////////////

/** Character body parts. */
UENUM(BlueprintType)
enum class EBodyPart : uint8
{
	FullBody,
	
	Hair,
	Eyes,
	Glasses,
	Head,
	Torso,
	Arms,
	Legs,
	Feet,

	MAX UMETA(Hidden)
};

/////////////////////////////////////////////////////////////////////

/**
* Blueprint equivalent of FSkeleMeshMergeSectionMapping
* Info to map all the sections from a single source skeletal mesh to
* a final section entry in the merged skeletal mesh.
*/
USTRUCT(BlueprintType)
struct /*TECHNODARKNESS_API*/ FSkelMeshMergeSectionMapping_BP
{
	GENERATED_BODY()
	
	/** Indices to final section entries of the merged skeletal mesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Merge Params")
	TArray<int32> SectionIDs;
};

/////////////////////////////////////////////////////////////////////

/**
* Used to wrap a set of UV Transforms for one mesh.
*/
USTRUCT(BlueprintType)
struct /*TECHNODARKNESS_API*/ FSkelMeshMergeUVTransform
{
	GENERATED_BODY()
	
	/** A list of how UVs should be transformed on a given mesh, where index represents a specific UV channel. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Merge Params")
	TArray<FTransform> UVTransforms;
};

/////////////////////////////////////////////////////////////////////

/**
* Blueprint equivalent of FSkelMeshMergeUVTransforms
* Info to map all the sections about how to transform their UVs
*/
USTRUCT(BlueprintType)
struct /*TECHNODARKNESS_API*/ FSkelMeshMergeUVTransformMapping
{
	GENERATED_BODY()
	
	/** For each UV channel on each mesh, how the UVS should be transformed. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Merge Params")
	TArray<FSkelMeshMergeUVTransform> UVTransformsPerMesh;
};

/////////////////////////////////////////////////////////////////////

/**
* Struct containing all parameters used to perform a Skeletal Mesh merge.
*/
USTRUCT(BlueprintType)
struct /*TECHNODARKNESS_API*/ FSkeletalMeshMergeParams
{
	GENERATED_BODY()
	
	FSkeletalMeshMergeParams()
	{
		StripTopLODS = 0;
		bNeedsCpuAccess = false;
		bSkeletonBefore = false;
		Skeleton = nullptr;
	}
	
	// An optional array to map sections from the source meshes to merged section entries
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray <FSkelMeshMergeSectionMapping_BP> MeshSectionMappings;
	
	// An optional array to transform the UVs in each mesh
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray <FSkelMeshMergeUVTransformMapping> UVTransformsPerMesh;
	
	// The list of skeletal meshes to merge.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray <USkeletalMesh*> MeshesToMerge;
	
	// The number of high LODs to remove from input meshes
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StripTopLODS;
	
	// Whether or not the resulting mesh needs to be accessed by the CPU for any reason (e.g. for spawning particle effects).
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bNeedsCpuAccess : 1;
	
	// Update skeleton before merge. Otherwise, update after.
	// Skeleton must also be provided.
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 bSkeletonBefore : 1;
	
	// Skeleton that will be used for the merged mesh.
	// Leave empty if the generated skeleton is OK.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class USkeleton* Skeleton;
};

/////////////////////////////////////////////////////////////////////

/** Body part mesh info. */
USTRUCT(BlueprintType)
struct FMeshBodyPart
{
	GENERATED_BODY()

	// Body part skeletal mesh.
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	USkeletalMesh* Mesh;
};

/////////////////////////////////////////////////////////////////////

/**
* Utils for merging meshes.
*/
UCLASS()
class TECHNODARKNESS_API UMeshMergeFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/**
	* Merges the given meshes into a single mesh.
	* @return The merged mesh (will be invalid if the merge failed).
	*/
	UFUNCTION(BlueprintCallable, Category = "Mesh Merge", meta = (UnsafeDuringActorConstruction = "true"))
	static class USkeletalMesh* MergeMeshes(const FSkeletalMeshMergeParams& Params);
};

/////////////////////////////////////////////////////////////////////

/**
 * CombinedSkeletalMeshComponent is used to create an instance of an animated combined SkeletalMesh asset from separated skeletal meshes.
 */
UCLASS(Blueprintable, ClassGroup = (Rendering, Common), HideCategories = Object, EditInlineNew, meta = (BlueprintSpawnableComponent))
class TECHNODARKNESS_API UCombinedSkeletalMeshComponent : public USkeletalMeshComponent
{
	GENERATED_BODY()

public:
	/**
	 * Set mesh to the body part.
	 * @param InBodyPart Body part for which we want to set mesh.
	 * @param BodyMesh Mesh that we want to set.
	 */
	UFUNCTION(BlueprintCallable)
	void SetBodyPart(EBodyPart InBodyPart, USkeletalMesh* BodyMesh);

	/**
	 * Clear body part mesh.
	 * @param InBodyPart Body part that we want to clear.
	 */
	UFUNCTION(BlueprintCallable)
	void ClearBodyPart(EBodyPart InBodyPart);
	
	UFUNCTION(BlueprintPure)
	FORCEINLINE USkeletalMesh* GetBodyPartSkeletalMesh(EBodyPart InBodyPart) const
	{
		return BodyParts[static_cast<int32>(InBodyPart)].Mesh;
	}

protected:
	virtual void OnRegister() override;

	void GenerateMesh(const TArray<FMeshBodyPart>& MeshParts);

	// List of the body parts.
	UPROPERTY(EditAnywhere, Category = "Combined Mesh", meta = (ArraySizeEnum="EBodyPart"))
	FMeshBodyPart BodyParts[EBodyPart::MAX]; 

	// Character physics asset.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combined Mesh")
	UPhysicsAsset* PhysicsAsset;

	// Character skeleton.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combined Mesh")
	USkeleton* Skeleton;

private:
	TArray<FMeshBodyPart> GetConvertedBodyParts() const;
};
