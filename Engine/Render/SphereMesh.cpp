#include "SphereMesh.h"
#include "Resource/ModelLoader.h"

namespace Blue
{
	SphereMesh::SphereMesh()
	{
		// �� �ε�.
		std::weak_ptr<MeshData> mesh;
		if (ModelLoader::Get().Load("sphere.obj", mesh))
		{
			meshes.emplace_back(mesh);
		}
	}
}