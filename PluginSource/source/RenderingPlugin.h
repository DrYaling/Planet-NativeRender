#pragma once

#include "PlatformBase.h"
#include "RenderAPI.h"
#include "ResourceManager.h"
#include "Logger/Logger.h"
extern "C" {
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaClearRender();
	// void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API RegisterLogWarning(logger::CPPLogCallback callback);
	// void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API RegisterLogError(logger::CPPLogCallback callback);
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaSetTimeFromUnity(float t);
	// --------------------------------------------------------------------------
	// SetTextureFromUnity, an example function we export which is called by one of the scripts.
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaSetTextureFromUnity(void* textureHandle, int w, int h, uint32_t mesh_index, TextureType tex_type);

	// --------------------------------------------------------------------------
	// SetMeshBuffersFromUnity, an example function we export which is called by one of the scripts.
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaSetMeshBuffersFromUnity(void* vertexBufferHandle, int vertexCount, void* sourceTriangles, int triangleCount, int mesh_type);
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaModifyVertexBuffer(int32_t index, uint32_t mesh_id, int mesh_type);
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API OnRenderEvent(int eventID);
	/// <summary>
	/// initialize resource engine
	/// </summary>
	/// <param name="threadCount"></param>
	/// <param name="resPath"></param>
	/// <returns></returns>
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaInitRender(uint32_t threadCount, const char* resPath);
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaLoadMesh(uint32_t mesh_id);
	// --------------------------------------------------------------------------
	// GetRenderEventFunc, an example function we export which is used to get a rendering event callback function.
	UnityRenderingEvent UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaGetRenderEventFunc();
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaBeginModifyMesh(uint32_t index);
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaGetMeshState(const uint32_t* indexes, uint32_t size, uint32_t* states);
	/// <summary>
	/// 检查单个地图的数据加载状态
	/// </summary>
	/// <param name="mesh_id">mesh id</param>
	/// <returns></returns>
	bool UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaCheckMeshState(const uint32_t mesh_id);
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaPrintRenderMemoryUsed();
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaReleaseMesh(uint32_t mesh_id);



	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaSetLogLevel(uint32_t log_level);
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaInitNative();
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaOnApplicationQuit();
	void UNITY_INTERFACE_EXPORT UNITY_INTERFACE_API CaRegisterLog(logger::CPPLogCallback callback);
	
}
