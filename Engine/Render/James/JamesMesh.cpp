#include "JamesMesh.h"
#include "Resource/ModelLoader.h"

namespace Blue
{
	JamesMesh::JamesMesh()
	{
		// 모델 로드.
		std::vector<std::weak_ptr<MeshData>> meshList;
		if (ModelLoader::Get().Load("James.fbx", meshList))
		{
			for (auto const& mesh : meshList)
			{
				meshes.emplace_back(mesh);
			}
		}
	}
}