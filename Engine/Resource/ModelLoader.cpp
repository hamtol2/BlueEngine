#include "ModelLoader.h"
#include "Core/Common.h"
#include <vector>
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Render/Vertex.h"
#include "Render/Mesh.h"

#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>
#include <cimport.h>

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

		// 확장자 확인.
		char nameBuffer[128] = {};
		strcpy_s(nameBuffer, 128, name.c_str());
		char* extension = nullptr;
		char* nameOnly = strtok_s(nameBuffer, ".", &extension);

		// 확장자에 따른 모델링 로드 처리.
		if (strcmp(extension, "obj") == 0)
		{
			std::shared_ptr<MeshData> newMesh;
			if (LoadOBJ(name, newMesh))
			{
				outData = newMesh;
				return true;
			}

			return false;
		}
		else if (strcmp(extension, "fbx") == 0)
		{
			std::shared_ptr<MeshData> newMesh;
			if (LoadFBX(name, newMesh, 0.02f))
			{
				outData = newMesh;
				return true;
			}

			return false;
		}

		return false;
	}

	ModelLoader& ModelLoader::Get()
	{
		return *instance;
	}

	bool ModelLoader::LoadOBJ(const std::string& name, std::shared_ptr<MeshData>& outData)
	{
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
				texCoord.y = 1.0f - texCoord.y;
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
			// 정사영 -> 직교 방향벡터 구하기.
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

	bool ModelLoader::LoadFBX(const std::string& name, std::shared_ptr<MeshData>& outData, float baseScale)
	{
		// 파일로드.
		char path[512] = {};
		sprintf_s(path, 512, "../Assets/Meshes/%s", name.c_str());

		// fbx scene 임포트.
		const aiScene* fbxScene = aiImportFile(path, aiProcess_ConvertToLeftHanded);

		// 씬 임포트에 실패하거나 씬에 메시가 없는 경우 실패 처리.
		if (!fbxScene || !fbxScene->HasMeshes())
		{
			return false;
		}

		// fbxScene에서 루트 노드 가져오기.
		aiNode* rootNode = fbxScene->mRootNode;
		
		// 메시를 가진 노드 검색.
		aiNode* meshNode = nullptr;
		for (uint32 ix = 0; ix < rootNode->mNumChildren; ++ix)
		{
			aiNode* currentNode = rootNode->mChildren[ix];
			if (currentNode->mNumMeshes > 0)
			{
				meshNode = currentNode;
				break;
			}
		}

		// 메시를 가진 노드가 없는 경우 실패.
		if (!meshNode)
		{
			return false;
		}

		// 메시 처리.
		std::vector<std::shared_ptr<MeshData>> meshes;
		for (uint32 meshIndex = 0; meshIndex < meshNode->mNumMeshes; ++meshIndex)
		{
			// aiMesh 가져오기.
			aiMesh* mesh = fbxScene->mMeshes[meshNode->mMeshes[meshIndex]];

			// 정점 처리.
			std::vector<Vertex> vertices;
			for (uint32 ix = 0; ix < mesh->mNumVertices; ++ix)
			{
				// 위치 설정.
				Vector3 position(mesh->mVertices[ix].x, mesh->mVertices[ix].y, mesh->mVertices[ix].z);
				
				// 기본으로 설정된 스케일 적용 (FBX의 경우 너무 크게 적용되는 경우가 있음).
				position *= baseScale;

				// UV 설정.
				Vector2 texCoord;
				if (mesh->HasTextureCoords(0))
				{
					texCoord.x = mesh->mTextureCoords[0][ix].x;
					texCoord.y = 1.0f - mesh->mTextureCoords[0][ix].y;
				}

				// 노멀 설정.
				Vector3 normal;
				if (mesh->HasNormals())
				{
					normal.x = mesh->mNormals[ix].x;
					normal.y = mesh->mNormals[ix].y;
					normal.z = mesh->mNormals[ix].z;
				}

				// 탄젠트/바이탄젠트 설정.
				Vector3 tangent;
				Vector3 bitangent;
				if (mesh->HasTangentsAndBitangents())
				{
					tangent.x = mesh->mTangents[ix].x;
					tangent.y = mesh->mTangents[ix].y;
					tangent.z = mesh->mTangents[ix].z;

					bitangent.x = mesh->mBitangents[ix].x;
					bitangent.y = mesh->mBitangents[ix].y;
					bitangent.z = mesh->mBitangents[ix].z;
				}

				// 정점 생성.
				Vertex vertex(position, Vector3::One, texCoord, normal);
				vertex.tangent = tangent;
				vertex.bitangent = bitangent;

				// 정점 배열에 추가.
				vertices.emplace_back(vertex);
			}

			// 인덱스 생성.
			std::vector<uint32> indices;
			indices.reserve(mesh->mNumFaces * 3);
			for (uint32 ix = 0; ix < mesh->mNumFaces; ++ix)
			{
				// 면(Face) 가져오기.
				const aiFace& face = mesh->mFaces[ix];

				// 인덱스 설정.
				indices.emplace_back(mesh->mFaces[ix].mIndices[0]);
				indices.emplace_back(mesh->mFaces[ix].mIndices[1]);
				indices.emplace_back(mesh->mFaces[ix].mIndices[2]);
			}

			// MeshData 생성 및 배열에 추가.
			std::shared_ptr<MeshData> meshData = std::make_shared<MeshData>(vertices, indices);
			meshes.emplace_back(meshData);
		}

		// fbx import 해제.
		aiReleaseImport(fbxScene);

		// 해시 테이블에 메시 저장.
		this->meshes.insert(std::make_pair(name, meshes[0]));

		// 출력.
		outData = meshes[0];

		// 성공 반환.
		return true;
	}
}