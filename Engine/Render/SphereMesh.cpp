#include "SphereMesh.h"
#include "Resource/ModelLoader.h"

namespace Blue
{
	SphereMesh::SphereMesh()
	{
		// 모델 로드.
		std::vector<std::weak_ptr<MeshData>> meshList;
		//if (ModelLoader::Get().Load("sphere.obj", meshList))
		if (ModelLoader::Get().Load("sphere.fbx", meshList))
		{
			for (auto const& mesh : meshList)
			{
				meshes.emplace_back(mesh);
			}
			//meshes.emplace_back(meshList);
		}
	}
}