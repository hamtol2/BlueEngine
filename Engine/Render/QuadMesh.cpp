#include "QuadMesh.h"
#include "Vertex.h"
#include "Shader/Shader.h"
#include "Shader/TextureMappingShader.h"

#include "Resource/ShaderLoader.h"
#include "Resource/ModelLoader.h"

#include "Math/Matrix4.h"
#include "Math/Vector3.h"

namespace Blue
{
	QuadMesh::QuadMesh()
	{
		// �� �ε�.
		std::weak_ptr<MeshData> mesh;
		if (ModelLoader::Get().Load("quad.obj", mesh))
		{
			meshes.emplace_back(mesh);
		}
	}

	void QuadMesh::Update(float deltaTime)
	{
		// ȸ�� ó��.
		//static float angle = 0.0f;
		//angle += 90.0f * deltaTime;

		// ȸ�� ����.
		//transform.rotation.z = angle;

		//Rotate(angle);
	}

	void QuadMesh::Rotate(float angle)
	{
		//// ���� �迭.
		//std::vector<Vertex> vertices =
		//{
		//	Vertex(Vector3(-0.5f, 0.5f, 0.5f), Vector3(1.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f)),
		//	Vertex(Vector3(0.5f, 0.5f, 0.5f), Vector3(0.0f, 1.0f, 0.0f), Vector2(1.0f, 0.0f)),
		//	Vertex(Vector3(0.5f, -0.5f, 0.5f), Vector3(0.0f, 0.0f, 1.0f), Vector2(1.0f, 1.0f)),
		//	Vertex(Vector3(-0.5f, -0.5f, 0.5f), Vector3(1.0f, 1.0f, 0.0f), Vector2(0.0f, 1.0f)),
		//};
		//
		//static std::vector<Vertex> result;
		//static bool hasInitialized = false;
		//if (!hasInitialized)
		//{
		//	hasInitialized = true;
		//	result.assign(vertices.begin(), vertices.end());
		//}
		//
		//// ȸ��.
		//Matrix4 rotation = Matrix4::RotationZ(angle);
		//result[0].position = vertices[0].position * rotation;
		//result[1].position = vertices[1].position * rotation;
		//result[2].position = vertices[2].position * rotation;
		//result[3].position = vertices[3].position * rotation;
		//
		//// �޽��� ���� ���� ������Ʈ.
		//meshes[0].lock()->UpdateVertexBuffer(result);
	}
}