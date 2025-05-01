#pragma once

#include <string>
#include <unordered_map>
#include <memory>

namespace Blue
{
	struct MeshData;
	class ModelLoader
	{
	public:
		ModelLoader();
		~ModelLoader() = default;

		bool Load(const std::string& name, std::weak_ptr<MeshData>& outData);

		static ModelLoader& Get();

	private:
		bool LoadOBJ(const std::string& name, std::shared_ptr<MeshData>& outData);
		bool LoadFBX(const std::string& name, std::shared_ptr<MeshData>& outData, float baseScale = 1.0f);

	private:
		static ModelLoader* instance;

		std::unordered_map<std::string, std::shared_ptr<MeshData>> meshes;
	};
}