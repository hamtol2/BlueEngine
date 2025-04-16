#include "ModelLoader.h"
#include "Core/Common.h"
#include <vector>
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Render/Vertex.h"
#include "Render/Mesh.h"

namespace Blue
{
	ModelLoader* ModelLoader::instance = nullptr;

	ModelLoader::ModelLoader()
	{
		instance = this;
	}

	bool ModelLoader::Load(const std::string& name, std::weak_ptr<MeshData>& outData)
	{
		// 이미 가지고 있는지 확인.
		auto result = meshes.find(name);
		if (result != meshes.end())
		{
			outData = result->second;
			return true;
		}

		// 파일로드.
		char path[512] = {};
		sprintf_s(path, 512, "../Assets/Meshes/%s", name.c_str());

		FILE* file = nullptr;
		fopen_s(&file, path, "rb");
		if (file == nullptr)
		{
			ThrowIfFailed(E_FAIL, TEXT("Failed to open mesh file."));
			return false;
		}

		std::vector<Vector3> positions;
		std::vector<Vector2> texCoords;
		std::vector<Vector3> normals;
		std::vector<Vertex> vertices;

		// 파싱(Parcing, 해석).
		char line[512] = {};
		while (!feof(file))
		{
			// 한 줄씩 읽기.
			if (!fgets(line, 512, file))
			{
				break;
			}

			// 헤더 읽기.
			char header[3] = {};
			sscanf_s(line, "%s", header, 3);

			// 각 타입 별로 데이터 저장.
			if (strcmp(header, "v") == 0)
			{
				Vector3 position;
				sscanf_s(line, "v %f %f %f", &position.x, &position.y, &position.z);
				positions.emplace_back(position);
			}
			else if (strcmp(header, "vt") == 0)
			{
				Vector2 texCoord;
				sscanf_s(line, "vt %f %f", &texCoord.x, &texCoord.y);
				texCoords.emplace_back(texCoord);
			}
			else if (strcmp(header, "vn") == 0)
			{
				Vector3 normal;
				sscanf_s(line, "vn %f %f %f", &normal.x, &normal.y, &normal.z);
				normals.emplace_back(normal);
			}
			else if (strcmp(header, "f") == 0)
			{
				int v1, v2, v3;
				int t1, t2, t3;
				int n1, n2, n3;
				
				sscanf_s(line, "f %d/%d/%d %d/%d/%d %d/%d/%d",
					&v1, &t1, &n1, &v2, &t2, &n2, &v3, &t3, &n3);

				vertices.emplace_back(
					positions[v1 - 1], 
					Vector3::One, 
					texCoords[t1 - 1],
					normals[n1 - 1]);
				
				vertices.emplace_back(
					positions[v2 - 1], Vector3::One, texCoords[t2 - 1],
					normals[n2 - 1]);
				vertices.emplace_back(
					positions[v3 - 1], Vector3::One, texCoords[t3 - 1],
					normals[n3 - 1]);
			}
		}

		// 파일 닫기.
		fclose(file);

		// 인덱스 배열.
		std::vector<uint32> indices;
		indices.reserve(vertices.size());

		for (uint32 ix = 0; ix < (uint32)vertices.size(); ++ix)
		{
			indices.emplace_back(ix);
		}

		// 탄젠트 / 바이탄젠트(바이노멀) 계산.
		for (uint32 ix = 0; ix < (uint32)vertices.size(); ix += 3)
		{
			// 면을 이루는 3개의 정점 가져오기.
			Vertex& v0 = vertices[ix + 0];
			Vertex& v1 = vertices[ix + 1];
			Vertex& v2 = vertices[ix + 2];

			// 간선 구하기.
			Vector3 edge1 = v1.position - v0.position;
			Vector3 edge2 = v2.position - v0.position;

			// UV 차이 구하기.
			Vector2 deltaUV1 = v1.texCoord - v0.texCoord;
			Vector2 deltaUV2 = v2.texCoord - v0.texCoord;

			// 스케일 구하기.
			float denominator = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

			// 탄젠트.
			Vector3 tangent = (edge1 * deltaUV2.y - edge2 * deltaUV1.y) * denominator;
			Vector3 bitangent = (edge2 * deltaUV1.x - edge1 * deltaUV2.x) * denominator;

			v0.tangent += tangent;
			v1.tangent += tangent;
			v2.tangent += tangent;

			v0.bitangent += bitangent;
			v1.bitangent += bitangent;
			v2.bitangent += bitangent;
		}

		// 앞에서 구한 탄젠트/바이 탄젠트/노멀의 직교성 보장하도록 계산.
		// 그람-슈미트 알고리즘 & 외적.
		for (auto& vertex : vertices)
		{
			vertex.tangent = (vertex.tangent - vertex.normal * Dot(vertex.normal, vertex.tangent)).Normalized();
			vertex.tangent = vertex.tangent.Normalized();
			vertex.bitangent = Cross(vertex.normal, vertex.tangent);
		}

		// 메시 데이터 생성 및 리소스 등록.
		std::shared_ptr<MeshData> newData = std::make_shared<MeshData>(vertices, indices);
		meshes.insert(std::make_pair(name, newData));
		outData = newData;
		return true;
	}

	ModelLoader& ModelLoader::Get()
	{
		return *instance;
	}
}