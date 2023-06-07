#include "ResourceManager.h"
#include "Logger/Logger.h"
#include <stdio.h>
#include "Threading/ThreadManager.h"

ResourceManager::ResourceManager()
{

}

void ResourceManager::initialize(const char* config_path)
{
	if (nullptr == config_path)
	{
		LogError("config path is empty");
		return;
	}
	m_sResPath = config_path;
}

void ResourceManager::load_texture(const MeshTextureInfo& tex)
{
	const std::string& texture_path = tex.path;
	const auto iter = m_mTextures.find(texture_path);
	if (iter != m_mTextures.end())
		return;

	char file_path[256] = { 0 };
#define EOF_CHECK(f) if(feof(f)) {LogErrorFormat("file %s read fail!, unexpected eof at line %d", file_path, __LINE__); fclose(f); return;}
	//static auto file_path = "E:/WorkSpace/native-render-demo/UnityProject/Assets/MeshData/Meshes/planet_0.data";
	sprintf(file_path, "%s/Textures/%s.data", m_sResPath.c_str(), tex.path.c_str());
	auto file = fopen(file_path, "rb+");
	if (!file)
	{
		LogErrorFormat("open file %s fail!", file_path);
		return;
	}
	unsigned char buff[4] = { 0 };
	size_t size = fread(buff, 1, 4, file);
	EOF_CHECK(file)
		//0(alpha)0000000(size)
		TextureImpl impl;
	impl.ref_count = 0;
	const bool alpha = (buff[0] >> 7) != 0;
	const size_t fileSize = ((buff[0] & 0x7) << 24) | (buff[1] << 16) | (buff[2] << 8) | buff[3];
	TraceFormat("texture %s alpha %d, size %d", file_path, alpha, fileSize);
	if (!alpha)
	{
		std::vector<unsigned char> tmp;
		tmp.resize(fileSize);
		size = fread(&tmp[0], 1, fileSize, file);
		if (size != fileSize)
		{
			LogErrorFormat("none alpha texture %s size invalid!", file_path);
			fclose(file);
			return;
		}
		impl.data.resize(fileSize / 3 * 4);
		const int strip = 3;
		for (size_t i = 0; i < fileSize / strip; i++)
		{
			const int dStrip = 4;
			const int idx = i * strip;
			const int dIdx = i * dStrip;
			impl.data[dIdx] = tmp[idx];
			impl.data[dIdx + 1] = tmp[idx + 1];
			impl.data[dIdx + 2] = tmp[idx + 2];
			impl.data[dIdx + 3] = 255;
		}
	}
	else
	{
		impl.data.resize(fileSize);
		size = fread(&impl.data[0], 1, fileSize, file);
		if (size != fileSize)
		{
			LogErrorFormat("alpha texture %s size invalid!", file_path);
			fclose(file);
			return;
		}
	}
	fclose(file);
	//LogFormat("save texture %d",tex_id);
	//图片暂时不清除
	std::unique_lock<std::mutex> lock(m_mxt);
	m_mTextures.insert(std::make_pair(texture_path, impl));
#undef EOF_CHECK
}

void ResourceManager::reset_load(uint32_t mesh_id, const std::unique_lock<std::mutex>&& lock)
{
	for (auto iter = m_vLoadingMesh.begin(); iter != m_vLoadingMesh.end(); ++iter)
	{
		if ((*iter).mesh_id == mesh_id)
		{

			iter = m_vLoadingMesh.erase(iter);
			break;
		}
	}
	if(m_vLoadingMesh.empty())
	{
		m_bLoading.store(false);
		Trace("exit load state");
	}
	m_condition.notify_all();
}

bool ResourceManager::mesh_is_loading(uint32_t mesh_id) const
{
	for (auto iter = m_vLoadingMesh.begin(); iter != m_vLoadingMesh.end(); ++iter)
	{
		if ((*iter).mesh_id == mesh_id)
		{
			return true;
		}
	}
	return false;
}

void ResourceManager::release_texture(const std::string& texture)
{
	const auto& texture_iter = m_mTextures.find(texture);
	if (texture_iter != m_mTextures.end())
	{
		texture_iter->second.ref_count--;
		if (texture_iter->second.ref_count == 0)
		{
			m_mTextures.erase(texture_iter);
		}
	}
}

const TextureImpl* ResourceManager::get_texture(const std::string& tex_id)
{
	std::lock_guard<std::mutex> lock(m_mxt);
	auto iter = m_mTextures.find(tex_id);
	if (iter != m_mTextures.end())
		return &iter->second;
	return nullptr;
}

void ResourceManager::set_mesh(uint32_t mesh_id, const MeshWithColorImpl&& mesh)
{
	const auto iter = m_mCMeshes.find(mesh_id);
	if (iter != m_mCMeshes.end())
		m_mCMeshes.erase(iter);
	m_mCMeshes.insert(std::make_pair(mesh_id, mesh));
}

void ResourceManager::set_mesh(uint32_t mesh_id, const MeshImpl&& mesh)
{
	const auto iter = m_mMeshes.find(mesh_id);
	if (iter != m_mMeshes.end())
		m_mMeshes.erase(iter);
	m_mMeshes.insert(std::make_pair(mesh_id, mesh));
}

void ResourceManager::load_mesh(uint32_t mesh_id)
{
	TraceFormat("load mesh %d",mesh_id);
	{
		std::lock_guard<std::mutex> lock(m_mxt);
		//已经加载成功
		if (m_mMeshes.find(mesh_id) != m_mMeshes.end())
		{
			for (auto iter = m_vLoadingMesh.begin(); iter != m_vLoadingMesh.end(); ++iter)
			{
				if ((*iter).mesh_id == mesh_id)
				{

					iter = m_vLoadingMesh.erase(iter);
					break;
				}
			}
			if (m_vLoadingMesh.empty())
			{
				m_bLoading.store(false);
				Trace("exit loading state");
			}
			TraceFormat("load mesh %d exit, mesh was loaded", mesh_id);
			return;
		}
	}
	char file_path[128] = { 0 };
#define EOF_CHECK(f) if(feof(f)) {LogErrorFormat("file %s read fail!, unexpected eof at line %d", file_path, __LINE__); fclose(f); return;}
	//static auto file_path = "E:/WorkSpace/native-render-demo/UnityProject/Assets/MeshData/Meshes/planet_0.data";
	sprintf(file_path, "%s/Meshes/%d.data", m_sResPath.c_str(), mesh_id);
	const auto file = fopen(file_path, "rb+");
	if (!file)
	{
		LogErrorFormat("open file %s fail!", file_path);
		reset_load(mesh_id, std::unique_lock<std::mutex>(m_mxt));
		return;
	}
	unsigned char buff[20] = { 0 };
	auto size = fread(buff, 1, 20, file);
	if (size != 20 || feof(file))
	{
		LogErrorFormat("file content invalid %s, read size %d!", file_path, size);
		fclose(file);
		reset_load(mesh_id, std::unique_lock<std::mutex>(m_mxt));
		return;
	}
	uint32_t vertexSize(0), indexCount(0), textureCount(0);
	if (textureCount > 4)
	{
		LogErrorFormat("file content invalid %s, texture size %d!", file_path, textureCount);
		fclose(file);
		reset_load(mesh_id, std::unique_lock<std::mutex>(m_mxt));
		return;
	}
	vertexSize = (buff[8] << 24) | (buff[9] << 16) | (buff[10] << 8) | buff[11];
	indexCount = (buff[12] << 24) | (buff[13] << 16) | (buff[14] << 8) | buff[15];
	uint32_t textures[4] = { 0 };
	textureCount = buff[16];
	const uint32_t textureInfo = (buff[17] << 16) + (buff[18] << 8) + buff[19];
	textures[0] = (textureInfo >> 18) & 0x3f;
	textures[1] = (textureInfo >> 12) & 0x3f;
	textures[2] = (textureInfo >> 6) & 0x3f;
	textures[3] = textureInfo & 0x3f;
	TraceFormat("mesh vertexSize %d, indexCount %d, tc %d", vertexSize, indexCount, textureCount);
	MeshImpl mesh;
	mesh.triangles.resize(indexCount);
	mesh.vertex.resize(vertexSize);
	mesh.textures.resize(textureCount);
	size = fread(&mesh.triangles[0], 1, indexCount * sizeof(uint16_t), file);
	EOF_CHECK(file);
	if (size != indexCount * sizeof(uint16_t))
	{
		LogErrorFormat("size of index read error, read %d bytes, expected %d bytes", size, indexCount * sizeof(MeshVertex));
		fclose(file);
		reset_load(mesh_id, std::unique_lock<std::mutex>(m_mxt));
		return;
	}
	size = fread(&mesh.vertex[0], 1, vertexSize * sizeof(MeshVertex), file);
	//EOF_CHECK(file);
	if (size != vertexSize * sizeof(MeshVertex))
	{
		LogErrorFormat("size of vertex read error, read %d bytes, expected %d bytes", size, vertexSize * sizeof(MeshVertex));
		fclose(file);
		reset_load(mesh_id, std::unique_lock<std::mutex>(m_mxt));
		return;
	}
	auto& textureList = mesh.textures;
	for (size_t i = 0; i < textureCount; i++)
	{
		EOF_CHECK(file);
		textureList[i].type = static_cast<TextureType>(fgetc(file));
		EOF_CHECK(file);
		const auto pathSize = textures[i] - 1;
		textureList[i].path.resize(pathSize);
		size = fread(&textureList[i].path[0], 1, pathSize, file);
		if (size != pathSize)
		{
			LogErrorFormat("read texture path %d fail", i);
			fclose(file);
			reset_load(mesh_id, std::unique_lock<std::mutex>(m_mxt));
			return;
		}
		//UNITY_LOG("texture %d path %s,size %d-%d", textureList[i].texture_id, textureList[i].path.c_str(), textures[i], textureList[i].path.size());
	}
	fclose(file);
	for (const auto& tex : mesh.textures)
	{
		load_texture(tex);
	}

	std::unique_lock<std::mutex> lock(m_mxt);
	//如果不需要加载了,则清除
	const auto liter = std::find_if(m_vLoadingMesh.begin(), m_vLoadingMesh.end(), [mesh_id](MeshLoadInfo val)
		{
			return val.mesh_id == mesh_id;
		});
	if (!liter->quit)
		set_mesh(mesh_id, static_cast<const MeshImpl>(mesh));
	else
	{
		//如果释放了,相应图片引用减1
		for (const auto& texture : mesh.textures)
		{
			release_texture(texture.path);
		}
	}
	reset_load(mesh_id, std::move(lock));
#undef EOF_CHECK
}

void ResourceManager::check_mesh(uint32_t mesh_id)
{
	{
		std::lock_guard<std::mutex> lock(m_mxt);
		//已经加载成功
		if (m_mMeshes.find(mesh_id) != m_mMeshes.end())
		{
			TraceFormat("mesh %d  exist", mesh_id);
			return;
		}
		if (mesh_is_loading(mesh_id))
		{
			TraceFormat("mesh %d is loading", mesh_id);
			return;
		}
		m_bLoading.store(true);
		m_vLoadingMesh.push_back(MeshLoadInfo{ mesh_id, false });
	}
	TraceFormat("mesh %d need load", mesh_id);
	sThreadManager->AddTask(threading::ThreadTask([this, mesh_id]
		{
			load_mesh(mesh_id);
		}));
}

const MeshImpl* ResourceManager::get_mesh(const uint32_t mesh_id)
{
	//如果正在loading,则等待到loading结束
	while (is_loading())
	{
		std::unique_lock<std::mutex> lock(m_mxt);
		if (!mesh_is_loading(mesh_id))
		{
			TraceFormat("mesh %d is not loading", mesh_id);
			break;
		}
		TraceFormat("mesh %d wait for notify", mesh_id);
		m_condition.wait(lock, [this, mesh_id]
			{
				const bool loading = this->mesh_is_loading(mesh_id);
				return !loading;
			});
		break;
	}
	std::unique_lock<std::mutex> lock(m_mxt);
	auto iter = m_mMeshes.find(mesh_id);
	if (iter != m_mMeshes.end())
		return &iter->second;
	return nullptr;
}

const MeshWithColorImpl* ResourceManager::get_mesh_with_color(const uint32_t mesh_id)
{
	auto iter = m_mCMeshes.find(mesh_id);
	if (iter != m_mCMeshes.end())
		return &iter->second;
	return nullptr;
}

void ResourceManager::release_mesh(uint32_t mesh_id)
{
	std::unique_lock<std::mutex> lock(m_mxt);
	const auto iter = m_mMeshes.find(mesh_id);
	if (iter != m_mMeshes.end())
	{
		auto impl = iter->second;
		for (const auto& tex : impl.textures)
		{
			release_texture(tex.path);
		}
		m_mMeshes.erase(iter);
	}
	//如果正在加载,则标记为需要释放
	const auto liter = std::find_if(m_vLoadingMesh.begin(), m_vLoadingMesh.end(), [mesh_id](MeshLoadInfo val)
		{
			return val.mesh_id == mesh_id;
		});
	if (liter != m_vLoadingMesh.end())
	{
		liter->quit = true;
	}
	//如果正在加载,则停止加载
}

void ResourceManager::clear()
{
	std::unique_lock<std::mutex> lock(m_mxt);
	release_map(m_mCMeshes);
	release_map(m_mMeshes);
	release_map(m_mTextures);
	//release_vector(m_vLoadingMesh, uint32_t);
	m_vLoadingMesh.clear();
	m_condition.notify_all();
	m_bLoading.store(false);
	Trace("resource clear");
}

size_t ResourceManager::memory_used()
{
	std::unique_lock<std::mutex> lock(m_mxt);
	size_t value = 0;
	for (const auto& iter : m_mTextures)
	{
		const auto size = sizeof(iter) + iter.second.data.size();
		//LogFormat("texture %d take memory %d",iter.first, size);
		value += size;
	}
	for (const auto& iter : m_mMeshes)
	{
		const auto txtSize = sizeof(iter) + iter.second.textures.size() * (sizeof(MeshTextureInfo) * 4);
		//LogFormat("mesh %d textures take memory %d", iter.first, txtSize);
		const auto idxSize = iter.second.triangles.size() * (sizeof(uint16_t));
		//LogFormat("mesh %d indexes take memory %d", iter.first, idxSize);
		const auto vtxSize = iter.second.vertex.size() * (sizeof(MeshVertex));
		//LogFormat("mesh %d vertex take memory %d", iter.first, vtxSize);
		value += txtSize + idxSize + vtxSize;
	}
	for (const auto& iter : m_mCMeshes)
	{
		value += sizeof(iter) + iter.second.triangles.size() * (sizeof(uint16_t));
		value += iter.second.vertex.size() * (sizeof(MeshVertexWithColor));
	}
	//LogFormat("total memory used %d",value);
	return value;
}

bool ResourceManager::mesh_loaded(uint32_t mesh_id)
{
	std::lock_guard<std::mutex> lock(m_mxt);
	return m_mMeshes.find(mesh_id) != m_mMeshes.end();
}
