// TechnoDarkness
// Author: Alexey Faradjev (myndbyrd)

#include "Components/SkeletalMeshComponents/CombinedSkeletalMeshComponent.h"

#include "SkeletalMeshMerge.h"

///////////////////////////////////////////////////////////////////////////
// UMeshMergeFunctionLibrary

static void ToMergeParams(const TArray<FSkelMeshMergeSectionMapping_BP>& InSectionMappings, TArray<FSkelMeshMergeSectionMapping>& OutSectionMappings)
{
	if (InSectionMappings.Num() > 0)
	{
		OutSectionMappings.AddUninitialized(InSectionMappings.Num());
		for (int32 i = 0; i < InSectionMappings.Num(); ++i)
		{
			OutSectionMappings[i].SectionIDs = InSectionMappings[i].SectionIDs;
		}
	}
};

static void ToMergeParams(const TArray<FSkelMeshMergeUVTransformMapping>& InUVTransformsPerMesh, TArray<FSkelMeshMergeUVTransforms>& OutUVTransformsPerMesh)
{
	if (InUVTransformsPerMesh.Num() > 0)
	{
		OutUVTransformsPerMesh.Empty();
		OutUVTransformsPerMesh.AddUninitialized(InUVTransformsPerMesh.Num());
		for (int32 i = 0; i < InUVTransformsPerMesh.Num(); ++i)
		{
			TArray<TArray<FTransform>>& OutUVTransforms = OutUVTransformsPerMesh[i].UVTransformsPerMesh;
			const TArray<FSkelMeshMergeUVTransform>& InUVTransforms = InUVTransformsPerMesh[i].UVTransformsPerMesh;
			if (InUVTransforms.Num() > 0)
			{
				OutUVTransforms.Empty();
				OutUVTransforms.AddUninitialized(InUVTransforms.Num());
				for (int32 j = 0; j < InUVTransforms.Num(); j++)
				{
					OutUVTransforms[i] = InUVTransforms[i].UVTransforms;
				}
			}
		}
	}
};

USkeletalMesh* UMeshMergeFunctionLibrary::MergeMeshes(const FSkeletalMeshMergeParams& Params)
{
	TArray<USkeletalMesh*> MeshesToMergeCopy = Params.MeshesToMerge;
	MeshesToMergeCopy.RemoveAll([](const USkeletalMesh* InMesh)
	{
		return InMesh == nullptr;
	});
    
	if (MeshesToMergeCopy.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Must provide multiple valid Skeletal Meshes in order to perform a merge."));
		return nullptr;
	}

	const EMeshBufferAccess BufferAccess = Params.bNeedsCpuAccess ? EMeshBufferAccess::ForceCPUAndGPU : EMeshBufferAccess::Default;
    
	TArray<FSkelMeshMergeSectionMapping> SectionMappings;
	TArray<FSkelMeshMergeUVTransforms> UvTransforms;
	ToMergeParams(Params.MeshSectionMappings, SectionMappings);
	ToMergeParams(Params.UVTransformsPerMesh, UvTransforms);
    
	USkeletalMesh* BaseMesh = NewObject<USkeletalMesh>();
    
	if (Params.Skeleton && Params.bSkeletonBefore)
	{
		BaseMesh->SetSkeleton(Params.Skeleton);
	}
    
	FSkeletalMeshMerge Merger(BaseMesh, MeshesToMergeCopy, SectionMappings, Params.StripTopLODS, BufferAccess, UvTransforms.GetData());
	if (!Merger.DoMerge())
	{
		UE_LOG(LogTemp, Warning, TEXT("Merge failed!"));
		return nullptr;
	}
    
	if (Params.Skeleton && !Params.bSkeletonBefore)
	{
		BaseMesh->SetSkeleton(Params.Skeleton);
	}
    
	return BaseMesh;
}

///////////////////////////////////////////////////////////////////////////
// UCombinedSkeletalMeshComponent

void UCombinedSkeletalMeshComponent::SetBodyPart(EBodyPart InBodyPart, USkeletalMesh* BodyMesh)
{
	BodyParts[static_cast<int32>(InBodyPart)].Mesh = BodyMesh;
	GenerateMesh(GetConvertedBodyParts());
}

void UCombinedSkeletalMeshComponent::ClearBodyPart(EBodyPart InBodyPart)
{
	BodyParts[static_cast<int32>(InBodyPart)].Mesh = nullptr;
	GenerateMesh(GetConvertedBodyParts());
}

void UCombinedSkeletalMeshComponent::OnRegister()
{
	Super::OnRegister();
	
	GenerateMesh(GetConvertedBodyParts());
}

void UCombinedSkeletalMeshComponent::GenerateMesh(const TArray<FMeshBodyPart>& MeshParts)
{
	FSkeletalMeshMergeParams SkeletalMeshMergeParams;
	SkeletalMeshMergeParams.Skeleton = Skeleton;
	SkeletalMeshMergeParams.bSkeletonBefore = true;
	SkeletalMeshMergeParams.bNeedsCpuAccess = true;
	
	for (const FMeshBodyPart& MeshPart : MeshParts)
	{
		if (MeshPart.Mesh)
		{
			SkeletalMeshMergeParams.MeshesToMerge.Add(MeshPart.Mesh);
		}
	}

	USkeletalMesh* NewSkeletalMesh = UMeshMergeFunctionLibrary::MergeMeshes(SkeletalMeshMergeParams);
	if (NewSkeletalMesh)
	{
		NewSkeletalMesh->SetPhysicsAsset(PhysicsAsset);
	}
	SetSkeletalMesh(NewSkeletalMesh);
}

TArray<FMeshBodyPart> UCombinedSkeletalMeshComponent::GetConvertedBodyParts() const
{
	constexpr int32 BodyPartsCount = static_cast<int32>(EBodyPart::MAX);
	
	TArray<FMeshBodyPart> Result;
	Result.Reserve(BodyPartsCount);

	for (int32 i = 0; i < BodyPartsCount; ++i)
	{
		Result.Emplace(BodyParts[i]);
	}

	return Result;
}
