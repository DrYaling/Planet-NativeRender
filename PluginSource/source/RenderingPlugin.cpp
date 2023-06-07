// Example low level rendering Unity plugin


#include <assert.h>
#include <math.h>
#include <vector>

#include "Threading/ThreadManager.h"
#include "RenderingPlugin.h"
struct  TextureInfo
{
	void* texture;
	int height;
	int width;
	TextureType tex_type;	//0 main, 1-sub
};
struct  Vertex
{
	void* buffer;
	size_t vertexCount;
	void* triangleBuffer;
	size_t originTriangleCount;
	std::vector<TextureInfo> textures;
};
static std::vector<Vertex> vertex_buffers;
static IUnityInterfaces* s_UnityInterfaces = nullptr;
static IUnityGraphics* s_Graphics = nullptr;
static RenderAPI* s_CurrentAPI = nullptr;
static UnityGfxRenderer s_DeviceType = kUnityGfxRendererNull;
std::mutex state_mtx;
static std::map<int32_t, bool> vertex_buffer_state;
std::mutex log_mtx;
static std::vector<std::string> _msgLogs;

static void OnGraphicsDeviceEvent(UnityGfxDeviceEventType eventType);
// --------------------------------------------------------------------------
// SetTimeFromUnity, an example function we export which is called by one of the scripts.

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaClearRender()
{
	Trace("clear");
	{
		std::lock_guard<std::mutex> lock(log_mtx);
		_msgLogs.clear();
	}
	vertex_buffer_state.clear();
	vertex_buffers.clear();
	ResourceManager::get_instance()->clear();
	//threading::ThreadManager::Destroy();
}
static float g_Time;

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaRegisterLog(logger::CPPLogCallback callback)
{
	logger::SetLogCallBack(callback);
	std::lock_guard<std::mutex> lock(log_mtx);
	for(const auto& msg: _msgLogs)
	{
		Log(msg.c_str());
	}
	_msgLogs.clear();
}
void CaSetTimeFromUnity (float t) { g_Time = t; }

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaSetTextureFromUnity(void* textureHandle, int w, int h, uint32_t mesh_index, TextureType tex_type)
{
	// A script calls this at initialization time; just remember the texture pointer here.
	// Will update texture pixels each frame from the plugin rendering event (texture update
	// needs to happen on the rendering thread).
	if(mesh_index >= vertex_buffers.size())
	{
		LogErrorFormat("Texture index %d out of mesh range", mesh_index);
		return;
	}
	auto tex = TextureInfo();
	tex.texture = textureHandle;
	tex.height = h;
	tex.width = w;
	tex.tex_type = tex_type;
	vertex_buffers[mesh_index].textures.push_back(tex);
	
	//UNITY_LOG("add texture buffer of index %d, h %d, w %d success", _textures.size(), h,w);
}

// --------------------------------------------------------------------------
// SetMeshBuffersFromUnity, an example function we export which is called by one of the scripts.



extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaSetMeshBuffersFromUnity(void* vertexBufferHandle, int vertexCount, void* sourceTriangles, int triangleCount, int mesh_type)
{
	// A script calls this at initialization time; just remember the pointer here.
	// Will update buffer data each frame from the plugin rendering event (buffer update
	// needs to happen on the rendering thread).


	// The script also passes original source mesh data. The reason is that the vertex buffer we'll be modifying
	// will be marked as "dynamic", and on many platforms this means we can only write into it, but not read its previous
	// contents. In this example we're not creating meshes from scratch, but are just altering original mesh data --
	// so remember it. The script just passes pointers to regular C# array contents.
	if(mesh_type != MeshImplType::MESH_NORMAL)
	{
		LogError("MESH_NORMAL is supported at this moment!");
		return;
	}
	auto buffer = Vertex();
	buffer.buffer = vertexBufferHandle;	
	buffer.vertexCount= vertexCount;
	buffer.originTriangleCount = triangleCount;
	buffer.triangleBuffer = sourceTriangles;
	vertex_buffers.push_back(buffer);
	vertex_buffer_state.insert(std::make_pair<int32_t,bool>(static_cast<int32_t>(vertex_buffers.size()) - 1, false));
}

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginLoad(IUnityInterfaces* unityInterfaces)
{
	s_UnityInterfaces = unityInterfaces;
	s_Graphics = s_UnityInterfaces->Get<IUnityGraphics>();
	s_Graphics->RegisterDeviceEventCallback(OnGraphicsDeviceEvent);
	
#if SUPPORT_VULKAN
	if (s_Graphics->GetRenderer() == kUnityGfxRendererNull)
	{
		extern void RenderAPI_Vulkan_OnPluginLoad(IUnityInterfaces*);
		RenderAPI_Vulkan_OnPluginLoad(unityInterfaces);
	}
#endif // SUPPORT_VULKAN
	{
		std::lock_guard<std::mutex> lock(log_mtx);
		_msgLogs.push_back("plugin load");
	}
	// Run OnGraphicsDeviceEvent(initialize) manually on plugin load
	OnGraphicsDeviceEvent(kUnityGfxDeviceEventInitialize);
}

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API UnityPluginUnload()
{
	{
		std::lock_guard<std::mutex> lock(log_mtx);
		_msgLogs.push_back("plugin unload");
	}
	printf("plugin unload");
	s_Graphics->UnregisterDeviceEventCallback(OnGraphicsDeviceEvent);
}

#if UNITY_WEBGL
typedef void	(UNITY_INTERFACE_API * PluginLoadFunc)(IUnityInterfaces* unityInterfaces);
typedef void	(UNITY_INTERFACE_API * PluginUnloadFunc)();

extern "C" void	UnityRegisterRenderingPlugin(PluginLoadFunc loadPlugin, PluginUnloadFunc unloadPlugin);

extern "C" void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API RegisterPlugin()
{
	UnityRegisterRenderingPlugin(UnityPluginLoad, UnityPluginUnload);
}
#endif

// --------------------------------------------------------------------------
// GraphicsDeviceEvent



void OnGraphicsDeviceEvent(UnityGfxDeviceEventType eventType)
{
	// Create graphics API implementation upon initialization
	if (eventType == kUnityGfxDeviceEventInitialize)
	{
		assert(s_CurrentAPI == NULL);
		s_DeviceType = s_Graphics->GetRenderer();
		s_CurrentAPI = CreateRenderAPI(s_DeviceType);
		{
			std::lock_guard<std::mutex> lock(log_mtx);
			_msgLogs.push_back("plugin create");
		}
	}

	// Let the implementation process the device related events
	if (s_CurrentAPI)
	{
		{
			std::lock_guard<std::mutex> lock(log_mtx);
			_msgLogs.push_back("plugin on event");
		}
		printf("plugin on event");
		s_CurrentAPI->ProcessDeviceEvent(eventType, s_UnityInterfaces);
	}

	// Cleanup graphics API implementation upon shutdown
	if (eventType == kUnityGfxDeviceEventShutdown)
	{
		delete s_CurrentAPI;
		s_CurrentAPI = NULL;
		s_DeviceType = kUnityGfxRendererNull;
		{
			std::lock_guard<std::mutex> lock(log_mtx);
			_msgLogs.push_back("plugin shutdown");
		}
		printf("plugin shutdown");
	}
}



void ModifyTexturePixels(TextureInfo& texture, std::string tex_id)
{
	TraceFormat("ModifyTexturePixels id %s", tex_id.c_str());
	const auto tex = ResourceManager::get_instance()->get_texture(tex_id);
	if (!tex)
		return;
	void* textureHandle = texture.texture;
	int width = texture.width;
	int height = texture.height;
	if (!textureHandle || tex->data.size() != width * height * 4)
		return;

	int textureRowPitch;
	void* textureDataPtr = s_CurrentAPI->BeginModifyTexture(textureHandle, width, height, &textureRowPitch);
	TraceFormat("ModifyTexturePixels buffer w %d, h %d, size %d, textureRowPitch %d, tex data size %d", width, height, width * height * 4, textureRowPitch, tex->data.size());

	//openGlES, metal, dx11
	if (!textureDataPtr)
	{
		s_CurrentAPI->EndModifyTexture(textureHandle, width, height, textureRowPitch, (void*)&tex->data[0]);
		Trace("texture modify finish opengl");
	}
	//vulkan dx12
	else
	{
		const auto copySize = tex->data.size();
		auto dst = static_cast<unsigned char*>(textureDataPtr);
		auto src = &tex->data[0];
		memcpy(dst, &tex->data[0], copySize);
		s_CurrentAPI->EndModifyTexture(textureHandle, width, height, textureRowPitch, textureDataPtr);
		Trace("texture modify finish dx12");
	}
}
void ModifyTriangles(void* triangleBuffer, const uint16_t* indexes, size_t size, size_t oldSize) {
	size_t bufferSize;

	TraceFormat("modify triangle buffer ptr %d", triangleBuffer);;
	void* indexBufferPtr = s_CurrentAPI->BeginModifyVertexBuffer(triangleBuffer, &bufferSize);
	if (!indexBufferPtr)
	{
		LogError("fail to load index buffer from gfx device");
		return;
	}
	TraceFormat("set index buffer count %d, triangleBufferSize %d, size of i32 %d, triangle size %d", size, bufferSize, sizeof(uint16_t), oldSize);
	auto newIndexSize = size * sizeof(uint16_t);
	auto* indexBuffer = static_cast<unsigned char*>(indexBufferPtr);
	if (newIndexSize > bufferSize)
	{
		LogFormat("indexes cut size %d", newIndexSize - bufferSize);
		newIndexSize = bufferSize;
	}
	memcpy(indexBuffer, indexes, newIndexSize);
	if (newIndexSize < bufferSize)
	{
		const auto lastIndex = indexes[size - 1];
		auto leftBuffer = (uint16_t*)(indexBuffer + newIndexSize);
		memset(leftBuffer, lastIndex, bufferSize - newIndexSize);
	}
	s_CurrentAPI->EndModifyVertexBuffer(triangleBuffer);
	Trace("modify EndModifyVertexBuffer");;

}
void ModifyVertex(uint32_t mesh_id, int32_t index)
{
	TraceFormat("ModifyVertex mesh_id %d, index %d", mesh_id, index);
	const auto mesh = ResourceManager::get_instance()->get_mesh(mesh_id);
	if (!mesh)
	{
		LogErrorFormat("mesh id %d outof range", mesh_id);
		return;
	}
	auto& handler = vertex_buffers[index];
	void* bufferHandle = handler.buffer;
	//如果还在加载,则继续等待
	for (const auto& tex : mesh->textures)
	{
		for (auto& texture : handler.textures)
		{
			if(tex.type == texture.tex_type)
			{
				ModifyTexturePixels(texture, tex.path);
			}
		}
	}
	//修改索引
	ModifyTriangles(handler.triangleBuffer, &mesh->triangles[0], mesh->triangles.size(), handler.originTriangleCount);
	size_t bufferSize;
	const int vertexCount = handler.vertexCount;
	TraceFormat("ModifyVertexBuffer buffer vcount %d, mesh count %d, size of vertex %d", vertexCount, mesh->vertex.size(), sizeof(MeshVertex));
	if (!bufferHandle || mesh->vertex.size() > vertexCount || mesh->vertex.empty())
	{
		LogError("invalid mesh data");
		return;
	}

	void* bufferDataPtr = s_CurrentAPI->BeginModifyVertexBuffer(bufferHandle, &bufferSize);
	TraceFormat("modify mesh buffer ptr %d", bufferDataPtr);;
	if (!bufferDataPtr)
		return;
	const int vertexStride = int(bufferSize / vertexCount);
	if (vertexStride != sizeof(MeshVertex)) {
		LogErrorFormat("invalid vertex buffer, GPU indexes %d, MeshVertex size %d", vertexStride, sizeof(MeshVertex));
		return;
	}

	auto bufferPtr = static_cast<char*>(bufferDataPtr);
	
	const auto copySize = mesh->vertex.size() * vertexStride;
	memcpy(bufferPtr, &mesh->vertex[0], copySize);
	/*if (copySize < bufferSize) {
		const auto last = mesh->vertex.rbegin();
		bufferPtr += copySize;
		for (int i = copySize / vertexStride; i < vertexCount; ++i)
		{
			MeshVertex& dst = *reinterpret_cast<MeshVertex*>(bufferPtr);
			dst.pos[0] = last->pos[0];
			dst.pos[1] = last->pos[1];
			dst.pos[2] = last->pos[2];
			dst.normal[0] = last->normal[0];
			dst.normal[1] = last->normal[1];
			dst.normal[2] = last->normal[2];
			dst.uv[0] = last->uv[0];
			dst.uv[1] = last->uv[1];
			bufferPtr += vertexStride;
		}
	}*/
	s_CurrentAPI->EndModifyVertexBuffer(bufferHandle);
	Trace("modify mesh success");;
	std::lock_guard<std::mutex> lock(state_mtx);
	vertex_buffer_state[index] = true;
	
}
void ModifyVertexWithColor(uint32_t mesh_id, int32_t index)
{
	auto& handler = vertex_buffers[index];
	void* bufferHandle = handler.buffer;
	const auto mesh = ResourceManager::get_instance()->get_mesh_with_color(mesh_id);
	if (!mesh)
	{
		LogErrorFormat("mesh id %d outof range", mesh_id);
		return;
	}
	ModifyTriangles(handler.triangleBuffer, &mesh->triangles[0], mesh->triangles.size(), handler.originTriangleCount);
	
	size_t bufferSize;


	const int vertexCount = handler.vertexCount;
	TraceFormat("ModifyVertexBuffer buffer vcount %d, mesh count %d, size of vertex %d", vertexCount, mesh->vertex.size(), sizeof(MeshVertexWithColor));
	if (!bufferHandle || mesh->vertex.size() > vertexCount || mesh->triangles.size() > handler.originTriangleCount || mesh->vertex.size() == 0)
	{
		LogError("invalid mesh data");
		return;
	}
	void* bufferDataPtr = s_CurrentAPI->BeginModifyVertexBuffer(bufferHandle, &bufferSize);
	if (!bufferDataPtr)
		return;
	const int vertexStride = static_cast<int>(bufferSize / vertexCount);
	if (vertexStride != sizeof(MeshVertexWithColor)) {
		LogErrorFormat("invalid vertex buffer, GPU indexes %d, MeshVertex size %d", vertexStride, sizeof(MeshVertexWithColor));
		return;
	}

	char* bufferPtr = static_cast<char*>(bufferDataPtr);

	const auto copySize = mesh->vertex.size() * vertexStride;
	memcpy(bufferPtr, &mesh->vertex[0], copySize);
	if (copySize < bufferSize) {
		const auto last = mesh->vertex.rbegin();
		bufferPtr += copySize;
		for (int i = copySize / vertexStride; i < vertexCount; ++i)
		{
			MeshVertexWithColor& dst = *reinterpret_cast<MeshVertexWithColor*>(bufferPtr);
			dst.pos[0] = last->pos[0];
			dst.pos[1] = last->pos[1];
			dst.pos[2] = last->pos[2];
			dst.normal[0] = last->normal[0];
			dst.normal[1] = last->normal[1];
			dst.normal[2] = last->normal[2];
			dst.color[0] = last->color[0];
			dst.color[1] = last->color[1];
			dst.color[2] = last->color[2];
			dst.color[3] = last->color[3];
			dst.uv[0] = last->uv[0];
			dst.uv[1] = last->uv[1];
			bufferPtr += vertexStride;
		}
	}
	s_CurrentAPI->EndModifyVertexBuffer(bufferHandle);
	TraceFormat("lock state buffer %d", index);
	std::lock_guard<std::mutex> lock(state_mtx);
	vertex_buffer_state[index] = true;
}

extern "C" void	UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaModifyVertexBuffer(int32_t index, uint32_t mesh_id, int mesh_type)
{
	TraceFormat("ModifyVertexBuffer id %d, index %d", mesh_id, index);
	if (index >= vertex_buffers.size())
	{
		LogError("mesh index outof range");
		return;
	}
	if(mesh_type == MeshImplType::MESH_NORMAL)
	{
		ModifyVertex(mesh_id, index);
	}
	else
	{
		ModifyVertexWithColor(mesh_id, index);
	}

}

extern "C" void	UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API OnRenderEvent(int eventID)
{
	// Unknown / unsupported graphics device type? Do nothing
	if (s_CurrentAPI == nullptr)
	{
		printf("device not found");
		return;
	}
	const auto mesh_index = eventID >> 24;
	const auto mesh_id = eventID & 0x000fffff;
	//printf("OnRenderEvent mesh_index %d, mesh_id %d", mesh_index, mesh_id);
	CaModifyVertexBuffer(mesh_index, mesh_id, 0);
}
/// <summary>
/// initialize resource engine
/// </summary>
/// <param name="threadCount"></param>
/// <param name="resPath"></param>
/// <returns></returns>
extern "C" void	UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaInitRender(uint32_t threadCount, const char* resPath)
{
	//threading::ThreadManager::Destroy();
	vertex_buffer_state.clear();
	vertex_buffers.clear();
	sResource->clear();
	threading::ThreadManager::SetThreadCount(threadCount);
	sResource->initialize(resPath);
	//create thread manager, ensure pool initialized before task enqueue
	sThreadManager;
	LogFormat("initialize render plugin, thread count %d, res path %s", threadCount, resPath);
}

extern "C" void	UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaLoadMesh(uint32_t mesh_id)
{
	sResource->check_mesh(mesh_id);
}
UnityRenderingEvent CaGetRenderEventFunc()
{
	return OnRenderEvent;
}


extern "C" void	UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaBeginModifyMesh(uint32_t index)
{
	std::lock_guard<std::mutex> lock(state_mtx);
	if(vertex_buffer_state.size() <= index)
	{
		LogErrorFormat("buffer index %d out of range",index);
		return;
	}
	vertex_buffer_state[index] = false;
}
extern "C" void	UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaGetMeshState(const uint32_t* indexes, uint32_t size, uint32_t* states)
{
	std::lock_guard<std::mutex> lock(state_mtx);
	for (size_t index = 0; index < size; index++)
	{
		const auto idx = indexes[index];
		if (vertex_buffer_state.size() <= idx)
		{
			LogErrorFormat("buffer index %d out of range", idx);
			states[index] = false;
			continue;
		}
		states[index] = vertex_buffer_state[idx] ? 1 : 0;
	}
}
/// <summary>
/// 检查单个地图的数据加载状态
/// </summary>
/// <param name="mesh_id">mesh id</param>
/// <returns></returns>
extern "C" bool	UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaCheckMeshState(const uint32_t mesh_id)
{
	return sResource->mesh_loaded(mesh_id);
}
extern "C" void	UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaPrintRenderMemoryUsed()
{
	LogFormat("native plugin memory used %d kbs",sResource->memory_used() / 1024);
}

extern "C" void	UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaReleaseMesh(uint32_t mesh_id)
{
	sResource->release_mesh(mesh_id);
}

void CaSetLogLevel(uint32_t log_level)
{
	logger::SetLogLevel(log_level);
}
void CaInitNative()
{
	logger::ClearLogger();
}

void CaOnApplicationQuit()
{
	CaClearRender();
	logger::ClearLogger();
}
