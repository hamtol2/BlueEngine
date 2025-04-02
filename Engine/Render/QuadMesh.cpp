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
		// 정점 배열.
		std::vector<Vertex> vertices =
		{
			Vertex(Vector3(-0.5f, 0.5f, 0.5f), Vector3(1.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f)),
			Vertex(Vector3(0.5f, 0.5f, 0.5f), Vector3(0.0f, 1.0f, 0.0f), Vector2(1.0f, 0.0f)),
			Vertex(Vector3(0.5f, -0.5f, 0.5f), Vector3(0.0f, 0.0f, 1.0f), Vector2(1.0f, 1.0f)),
			Vertex(Vector3(-0.5f, -0.5f, 0.5f), Vector3(1.0f, 1.0f, 0.0f), Vector2(0.0f, 1.0f)),
		};

		// 인덱스 배열.
		std::vector<uint32> indices = { 0, 1, 3, 1, 2, 3 };

		// 변환.
		//vertices[0].position = vertices[0].position * Matrix4::Scale(0.5f);
		//vertices[1].position = vertices[1].position * Matrix4::Scale(0.5f);
		//vertices[2].position = vertices[2].position * Matrix4::Scale(0.5f);
		//vertices[3].position = vertices[3].position * Matrix4::Scale(0.5f);

		//meshes.emplace_back(std::make_shared<MeshData>(vertices, indices));
		// 모델 로드.
		std::weak_ptr<MeshData> mesh;
		if (ModelLoader::Get().Load("quad.obj", mesh))
		{
			meshes.emplace_back(mesh);
		}

		//shaders.emplace_back(std::make_shared<TextureMappingShader>("T_coord.png"));
		//std::weak_ptr<TextureMappingShader> shader;
		//if (ShaderLoader::Get().Load<TextureMappingShader>(shader, "T_coord.png"))
		//{
		//	shaders.emplace_back(shader);
		//}
	}

	void QuadMesh::Update(float deltaTime)
	{
		// 회전 처리.
		//static float angle = 0.0f;
		//angle += 90.0f * deltaTime;

		// 회전 적용.
		//transform.rotation.z = angle;

		//Rotate(angle);
	}

	void QuadMesh::Rotate(float angle)
	{
		// 정점 배열.
		std::vector<Vertex> vertices =
		{
			Vertex(Vector3(-0.5f, 0.5f, 0.5f), Vector3(1.0f, 0.0f, 0.0f), Vector2(0.0f, 0.0f)),
			Vertex(Vector3(0.5f, 0.5f, 0.5f), Vector3(0.0f, 1.0f, 0.0f), Vector2(1.0f, 0.0f)),
			Vertex(Vector3(0.5f, -0.5f, 0.5f), Vector3(0.0f, 0.0f, 1.0f), Vector2(1.0f, 1.0f)),
			Vertex(Vector3(-0.5f, -0.5f, 0.5f), Vector3(1.0f, 1.0f, 0.0f), Vector2(0.0f, 1.0f)),
		};

		static std::vector<Vertex> result;
		static bool hasInitialized = false;
		if (!hasInitialized)
		{
			hasInitialized = true;
			result.assign(vertices.begin(), vertices.end());
		}

		// 회전.
		Matrix4 rotation = Matrix4::RotationZ(angle);
		result[0].position = vertices[0].position * rotation;
		result[1].position = vertices[1].position * rotation;
		result[2].position = vertices[2].position * rotation;
		result[3].position = vertices[3].position * rotation;

		// 메시의 정점 버퍼 업데이트.
		meshes[0].lock()->UpdateVertexBuffer(result);
	}
}