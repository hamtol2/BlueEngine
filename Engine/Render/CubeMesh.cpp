#include "CubeMesh.h"
#include "Resource/ModelLoader.h"

namespace Blue
{
	CubeMesh::CubeMesh()
	{
		// �� �ε�.
		std::vector<std::weak_ptr<MeshData>> meshList;
		if (ModelLoader::Get().Load("cube.obj", meshList))
		{
			for (auto const& mesh : meshList)
			{
				meshes.emplace_back(mesh);
			}
		}
	}
}