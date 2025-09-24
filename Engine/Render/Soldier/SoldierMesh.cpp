#include "SoldierMesh.h"
#include "Resource/ModelLoader.h"

namespace Blue
{
	SoldierMesh::SoldierMesh()
	{
		// 모델 로드.
		std::vector<std::weak_ptr<MeshData>> meshList;
		if (ModelLoader::Get().Load("Soldier.fbx", meshList))
		{
			for (auto const& mesh : meshList)
			{
				meshes.emplace_back(mesh);
			}
		}
	}
}