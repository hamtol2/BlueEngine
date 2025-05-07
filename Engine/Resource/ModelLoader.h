#pragma once

#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>
#include <cimport.h>

#include <string>
#include <unordered_map>
#include <memory>
#include <vector>

namespace Blue
{
	struct MeshData;
	class ModelLoader
	{
	public:
		ModelLoader();
		~ModelLoader() = default;

		bool Load(const std::string& name, std::vector<std::weak_ptr<MeshData>>& outData, float baseScale = 1.0f);

		static ModelLoader& Get();

	private:
		bool LoadOBJ(const std::string& name, std::vector<std::shared_ptr<MeshData>>& outData);
		bool LoadFBX(const std::string& name, std::vector<std::shared_ptr<MeshData>>& outData, float baseScale = 1.0f);

		void ProcessMesh(aiMesh* mesh, float baseScale, std::vector<std::shared_ptr<MeshData>>& meshes);

	private:
		static ModelLoader* instance;

		std::unordered_map<std::string, std::vector<std::shared_ptr<MeshData>>> meshes;
	};
}