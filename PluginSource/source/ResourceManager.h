#pragma once
/**
 *	native插件mesh和图片管理器
 * 资源管理器
 */
#include<map>
#include <vector>
#include <string>

#include "define.h"
#include<mutex>
#include<atomic>
#include<condition_variable>
enum TextureType
{
	MAIN_TEX_0 = 0,	//第一个材质的主贴图
	MAIN_TEX_1 = 1,	//第二个材质的主贴图
	MAIN_TEX_2 = 2,	//第三个材质的主贴图
	MAIN_TEX_3 = 3,	//第四个材质的主贴图
	SUB_TEX_0  = 4,	//第一个材质的副贴图
	SUB_TEX_1  = 5,	//第二个材质的副贴图
	SUB_TEX_2  = 6,	//第三个材质的副贴图
	SUB_TEX_3  = 7,	//第四个材质的副贴图
};
struct  Vector2
{
	float x;
	float y;
};
struct  Vector3
{
	float x;
	float y;
	float z;
};
struct TextureImpl
{
	uint32_t ref_count;
	std::vector<unsigned char> data;
};
enum MeshImplType
{
	MESH_NORMAL = 0,
	MESH_WITH_COLOR = 1,
};
/// <summary>
/// 带顶点色顶点信息
/// </summary>
struct MeshVertexWithColor
{
	float pos[3];
	float normal[3];
	unsigned char color[4];
	float uv[2];
	float uv1[2];
};
struct MeshWithColorImpl
{
	std::vector<MeshVertexWithColor> vertex;
	std::vector<uint16_t> triangles;
};
/// <summary>
/// 不带顶点色顶点信息
/// </summary>
struct MeshVertex
{
	float pos[3];
	float normal[3];
	float uv[2];
	//float uv1[2];
};
struct  MeshTextureInfo
{
	TextureType type;
	std::string path;
};
struct MeshImpl
{
	std::vector<MeshVertex> vertex;
	std::vector<uint16_t> triangles;
	std::vector<MeshTextureInfo> textures;
};
struct MeshLoadInfo
{
	uint32_t mesh_id;
	//如果在加载过程中释放了,这个标记为true
	bool quit;
};
class ResourceManager
{
public:
	static ResourceManager* get_instance()
	{
		static ResourceManager instance;
		return &instance;
	}
	void initialize(const char* config_path);
	///get texture of id, this api should be called after mesh data was got
	const TextureImpl* get_texture(const std::string& tex_id);
	void check_mesh(uint32_t mesh_id);
	/// <summary>
	/// check assets is loading
	/// </summary>
	/// <returns></returns>
	inline bool	 is_loading() const
	{
		return m_bLoading.load();
	}
	const MeshImpl* get_mesh(const uint32_t mesh_id);
	const MeshWithColorImpl* get_mesh_with_color(const uint32_t mesh_id);
	/// <summary>
	/// 释放mesh
	/// </summary>
	/// <returns></returns>
	void release_mesh(uint32_t mesh_id);
	void clear();
	size_t memory_used();
	bool mesh_loaded(uint32_t mesh_id);
private:
	void set_mesh(uint32_t mesh_id, const MeshWithColorImpl&& mesh);
	void set_mesh(uint32_t mesh_id, const MeshImpl&& mesh);
	void load_mesh(uint32_t mesh_id);
	void load_texture(const MeshTextureInfo&);
	//reset load state
	void reset_load(uint32_t mesh_id, const std::unique_lock<std::mutex>&& lock);
	inline bool mesh_is_loading(uint32_t mesh_id) const;
	///释放纹理
	///
	///内部不加锁,外部调用需要加锁保证内存访问不冲突
	void release_texture(const std::string& texture);

private:
	ResourceManager();
	~ResourceManager() = default;
	std::map<std::string, TextureImpl> m_mTextures;
	/// <summary>
	/// 不带顶点色顶点缓冲区
	/// </summary>
	std::map<uint32_t, MeshImpl> m_mMeshes;
	std::vector<MeshLoadInfo> m_vLoadingMesh;
	std::atomic<bool> m_bLoading;
	//带顶点色顶点缓冲区
	std::map<uint32_t, MeshWithColorImpl> m_mCMeshes;
	std::string m_sResPath;
	std::mutex m_mxt;
	std::condition_variable m_condition;
};
#define sResource ResourceManager::get_instance()