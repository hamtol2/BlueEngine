#include "SphereMesh.h"
#include "Resource/ModelLoader.h"

namespace Blue
{
	SphereMesh::SphereMesh()
	{
		// ¸ðµ¨ ·Îµå.
		std::vector<std::weak_ptr<MeshData>> meshList;
		//if (ModelLoader::Get().Load("sphere.obj", mesh))
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