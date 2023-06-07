using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.IO;
using System.Reflection;
using System.Runtime.InteropServices;
using AOT;
using Game;
using UnityEngine.Networking;

public enum TextureType
{
    MAIN_TEX_0 = 0, //第一个材质的主贴图
    MAIN_TEX_1 = 1, //第二个材质的主贴图
    SUB_TEX_0 = 2, //第一个材质的副贴图
    SUB_TEX_1 = 3, //第二个材质的副贴图
};
public static class CppLogger
{
    internal static int mainThreadId;
#if UNITY_IOS
    private const string dllName = "__Internal";
#else
    private const string dllName = "RenderingPlugin";
#endif

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void Log(string message, int level);

    [DllImport(dllName)]
    static extern void CaRegisterLog(Log callback);
    static List<string> _logs = new List<string>();
    static List<string> _logErrs = new List<string>();
    static List<string> _logWarns = new List<string>();

    public static void ShowLog()
    {
        CaRegisterLog(NativeLog);
    }

    public static void FlushLog()
    {
        lock (_logs)
        {
            foreach (var log in _logs)
            {
                Debug.Log(log);
            }
            _logs.Clear();
        }

        lock (_logWarns)
        {
            foreach (var log in _logWarns)
            {
                Debug.LogWarning(log);
            }
            _logWarns.Clear();
        }

        lock (_logErrs)
        {
            foreach (var log in _logErrs)
            {
                Debug.LogError(log);
            }
            _logErrs.Clear();
        }
    }

    [MonoPInvokeCallback(typeof(Log))]
    private static void NativeLog(string message, int level)
    {
        switch (level)
        {
            case 1:
                if (System.Threading.Thread.CurrentThread.ManagedThreadId == mainThreadId)
                    Debug.LogError(message);
                else
                {
                    lock (_logErrs)
                        _logErrs.Add(message);
                }
                break;
            case 2:
                if (System.Threading.Thread.CurrentThread.ManagedThreadId == mainThreadId)
                    Debug.LogWarning(message);
                else
                {
                    lock (_logWarns)
                        _logWarns.Add(message);
                }
                break;
            default:
                if (System.Threading.Thread.CurrentThread.ManagedThreadId == mainThreadId)
                    Debug.Log(message);
                else
                {
                    lock (_logs)
                        _logs.Add(message);
                }
                break;
        }
    }
    
}

public class MeshRenderInfo
{
    public int index;
    public bool active;
    public uint meshId;
    public GameObject mesh;
}
public class UseRenderingPlugin : MonoBehaviour
{
#if (UNITY_IOS || UNITY_TVOS || UNITY_WEBGL) && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
    [DllImport("RenderingPlugin")]
#endif
    private static extern void CaOnApplicationQuit();
    // Native plugin rendering events are only called if a plugin is used
    // by some script. This means we have to DllImport at least
    // one function in some active script.
    // For this example, we'll call into plugin's SetTimeFromUnity
    // function and pass the current time so the plugin can animate.

#if (UNITY_IOS || UNITY_TVOS || UNITY_WEBGL) && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
    [DllImport ("RenderingPlugin")]
#endif
	private static extern void CaSetTimeFromUnity(float t);


#if (UNITY_IOS || UNITY_TVOS || UNITY_WEBGL) && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
    [DllImport("RenderingPlugin")]
#endif
    private static extern void CaInitRender(uint threadCount, string resPath);
    // We'll also pass native pointer to a texture in Unity.
    // The plugin will fill texture data from native code.
#if (UNITY_IOS || UNITY_TVOS || UNITY_WEBGL) && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
    [DllImport ("RenderingPlugin")]
#endif
	private static extern void CaSetTextureFromUnity(System.IntPtr texture, int w, int h, uint meshIndex, int texType);

    // We'll pass native pointer to the mesh vertex buffer.
    // Also passing source unmodified mesh data.
    // The plugin will fill vertex data from native code.
#if (UNITY_IOS || UNITY_TVOS || UNITY_WEBGL) && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
    [DllImport ("RenderingPlugin")]
#endif
	private static extern unsafe void CaSetMeshBuffersFromUnity (IntPtr vertexBuffer, int vertexCount, IntPtr sourceTriangles, int triangleCount, int meshType);
    /// <summary>
    /// 设置mesh资源
    /// </summary>
    /// <param name="vertexCount"></param>
    /// <param name="sourceVertices"></param>
    /// <param name="sourceNormals"></param>
    /// <param name="sourceUVs"></param>
    /// <param name="sourceTriangles"></param>
    /// <param name="triangleCount"></param>
    /// <param name="meshId"></param>
    /// <param name="meshType"></param>
#if (UNITY_IOS || UNITY_TVOS || UNITY_WEBGL) && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
    [DllImport("RenderingPlugin")]
#endif
    private static extern void CaSetMeshData(int vertexCount, IntPtr sourceVertices,
        IntPtr sourceNormals, IntPtr sourceUVs, IntPtr sourceTriangles, int triangleCount, uint meshId, int meshType);
	/// <summary>
	/// 修改图片
	/// </summary>
	/// <param name="index"></param>
	/// <param name="textureId"></param>
#if (UNITY_IOS || UNITY_TVOS || UNITY_WEBGL) && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport("RenderingPlugin")]
#endif
	private static extern void CaModifyTexturePixels(int index, uint textureId);
    /// <summary>
    /// 修改mesh
    /// 修改的mesh最多支持2个材质,主副共4个贴图,mesh顶点数尽量接近;纹理尽量使用相同尺寸,如512或者1024或者2048,不能超过2048
    /// </summary>
    /// <param name="index"></param>
    /// <param name="meshId"></param>
    /// <param name="meshType"></param>
#if (UNITY_IOS || UNITY_TVOS || UNITY_WEBGL) && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
    [DllImport("RenderingPlugin")]
#endif
    private static extern void CaModifyVertexBuffer(int index, uint meshId, int meshType);
#if (UNITY_IOS || UNITY_TVOS || UNITY_WEBGL) && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
	[DllImport("RenderingPlugin")]
#endif
    private static extern IntPtr CaGetRenderEventFunc();
    /// <summary>
    /// load mesh with mesh_id, load was multithreading
    /// </summary>
    /// <param name="meshId"></param>
#if (UNITY_IOS || UNITY_TVOS || UNITY_WEBGL) && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
    [DllImport("RenderingPlugin")]
#endif
    private static extern void CaLoadMesh(uint meshId);
    /// <summary>
    /// 开始修改顶点
    /// </summary>
    /// <param name="index"></param>
#if (UNITY_IOS || UNITY_TVOS || UNITY_WEBGL) && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
    [DllImport("RenderingPlugin")]
#endif
    private static extern void CaBeginModifyMesh(uint index);

    /// <summary>
    /// 查询mesh状态
    /// </summary>
    /// <param name="indexes"></param>
    /// <param name="size"></param>
    /// <param name="states"></param>
#if (UNITY_IOS || UNITY_TVOS || UNITY_WEBGL) && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
    [DllImport("RenderingPlugin")]
#endif
    private static extern unsafe void CaGetMeshState(uint* indexes, uint size, uint* states);

#if UNITY_WEBGL && !UNITY_EDITOR
	[DllImport ("__Internal")]
	private static extern void RegisterPlugin();
#endif
    /// <summary>
    ///  print native memory used in bytes
    /// </summary>
    /// <param name="meshId"></param>
#if (UNITY_IOS || UNITY_TVOS || UNITY_WEBGL) && !UNITY_EDITOR
	[DllImport ("__Internal")]
#else
    [DllImport("RenderingPlugin")]
#endif
    public static extern void CaPrintRenderMemoryUsed();
    private static readonly int SubTexture = Shader.PropertyToID("_subTexture");
    private static readonly int MainTex = Shader.PropertyToID("_MainTex");
    private readonly List<MeshRenderInfo> _activeMeshes = new List<MeshRenderInfo>();
    private readonly List<uint> _loadingMeshList = new List<uint>(10);
    private uint[] _loadingMeshes = new uint[10];
    private uint[] _loadingMeshStates = new uint[10];
    private bool _initialized;
    public bool initialized => _initialized;
    private void Awake()
    {
        _initialized = false;
        CppLogger.mainThreadId = System.Threading.Thread.CurrentThread.ManagedThreadId;
        CppLogger.ShowLog();
#if UNITY_EDITOR
        CaInitRender(1, Application.dataPath + "/PlanetData");
#else
        Initialize(2, Application.persistentDataPath+"/PlanetData");
#endif
    }
    IEnumerator Start()
    {
        //TEST
#if !UNITY_EDITOR
        Debug.Log($"persistentDataPath {Application.persistentDataPath}");
        if (!Directory.Exists(Application.persistentDataPath + "/PlanetData"))
        {
            Directory.CreateDirectory(Application.persistentDataPath + "/PlanetData");
        }

        if (!Directory.Exists(Application.persistentDataPath + "/PlanetData/Meshes"))
        {
            Directory.CreateDirectory(Application.persistentDataPath + "/PlanetData/Meshes");
        }

        if (!Directory.Exists(Application.persistentDataPath + "/PlanetData/Textures"))
        {
            Directory.CreateDirectory(Application.persistentDataPath + "/PlanetData/Textures");
        }

        foreach (var meshId in PlanetMeshConfig.instance.GetMeshes())
        {
            string meshPath = Application.persistentDataPath + $"/PlanetData/Meshes/{meshId}.data";
            var sourcePath = Application.streamingAssetsPath + $"/PlanetData/Meshes/{meshId}.data";
            var uri = new System.Uri(sourcePath);
            UnityWebRequest request = UnityWebRequest.Get(uri.AbsoluteUri);
            yield return request.SendWebRequest();
            if (request.isNetworkError || request.isHttpError)
            {
                Debug.Log(": Error: " + request.error);
            }

            if (!string.IsNullOrEmpty(request.error))
            {
                Debug.LogFormat("request error {0}:{1}", sourcePath, request.error);
                throw new Exception("err ");
            }

            if (File.Exists(meshPath))
                continue;
            try
            {
                FileStream fsDes = File.Create(meshPath);
                fsDes.Write(request.downloadHandler.data, 0, request.downloadHandler.data.Length);
                fsDes.Flush();
                fsDes.Close();
            }
            catch (System.Exception e)
            {
                Debug.LogException(e);
                throw;
            }

            string texPath = Application.persistentDataPath + "/PlanetData/Textures/texture (1).data";
            sourcePath = Application.streamingAssetsPath + "/PlanetData/Textures/texture (1).data";
            uri = new System.Uri(sourcePath);
            request = UnityWebRequest.Get(uri.AbsoluteUri);
            yield return request.SendWebRequest();
            if (request.isNetworkError)
            {
                Debug.Log(": Error: " + request.error);
            }

            //因为第一次可能还没加载完，返回的是0没有数据，所以需要判断一下，但是如果加载不出来就会死循环，可以自己做处理，我这里暂时就不写了
            while (request.responseCode != 200)
            {
                yield return null;
            }

            if (!string.IsNullOrEmpty(request.error))
            {
                Debug.LogFormat("copy file error {0}:{1}", sourcePath, request.error);
                throw new Exception("err ");
            }

            if (File.Exists(texPath))
                continue;
            try
            {
                FileStream fsDes = File.Create(texPath);
                fsDes.Write(request.downloadHandler.data, 0, request.downloadHandler.data.Length);
                fsDes.Flush();
                fsDes.Close();
            }
            catch (System.Exception e)
            {
                Debug.LogException(e);
                throw;
            }
        }
#endif
        Debug.Log($"Demo Start {sizeof(char)}, {sizeof(byte)}");
        Screen.SetResolution(1920, 1080, true);
        Application.targetFrameRate = 120;
        yield return null;
#if UNITY_WEBGL && !UNITY_EDITOR
		RegisterPlugin();
#endif
        SendMeshBuffersToPlugin();
        _initialized = true;
    }

	private void SendMeshBuffersToPlugin ()
	{
        for (int index = 0; index < 12; index++)
		{
			if (index >= transform.childCount)
                return;
			var child = transform.GetChild(index);
            if (child.GetComponent<MeshFilter>() == null)
                return;
            var filter = child.GetComponent<MeshFilter>();
            var mesh = filter.mesh;
            // The plugin will want to modify the vertex buffer -- on many platforms
            // for that to work we have to mark mesh as "dynamic" (which makes the buffers CPU writable --
            // by default they are immutable and only GPU-readable).
            mesh.MarkDynamic();
            // Make sure to have vertex colors so that the plugin can rely on a known
            // vertex layout (position+normal+color+UV). Since Unity 2019.3 it's easier
            // since there are APIs to query all that info.
            var colors = mesh.colors;
            mesh.Optimize();
            mesh.MarkModified();
            if (colors.Length > 0)
            {
                Debug.LogError($"带顶点色的mesh暂不支持");
                mesh.colors = new Color[0];
            }
            CaSetMeshBuffersFromUnity(mesh.GetNativeVertexBufferPtr(0), mesh.vertexCount,mesh.GetNativeIndexBufferPtr(),
                (int)mesh.GetIndexCount(0), 0);
            if (child.GetComponent<Renderer>() == null)
                continue;
            int matIndex = 0;
            foreach (var mat in child.GetComponent<Renderer>().materials)
            {
                if (matIndex >= 2)
                    throw new Exception("一个mesh仅支持2个材质");
                Texture2D tex = (Texture2D) mat.GetTexture(MainTex);
                if (null == tex)
                {
                    matIndex++;
                    continue;
                }
                // Set point filtering just so we can see the pixels clearly
                tex.filterMode = FilterMode.Bilinear;
                // Call Apply() so it's actually uploaded to the GPU
                tex.Apply();

                // Pass texture pointer to the plugin
                CaSetTextureFromUnity(tex.GetNativeTexturePtr(), tex.width, tex.height, (uint) index, matIndex == 0 ? (int) TextureType.MAIN_TEX_0 : (int) TextureType.MAIN_TEX_1);
                if(mat.HasProperty(SubTexture))
                {
                   tex = (Texture2D)mat.GetTexture(SubTexture);
                   if (null != tex)
                   {
                       // Set point filtering just so we can see the pixels clearly
                       tex.filterMode = FilterMode.Bilinear;
                       // Call Apply() so it's actually uploaded to the GPU
                       tex.Apply();

                       // Pass texture pointer to the plugin
                       CaSetTextureFromUnity(tex.GetNativeTexturePtr(), tex.width, tex.height, (uint) index,
                           matIndex == 0 ? (int) TextureType.SUB_TEX_0 : (int) TextureType.SUB_TEX_1);
                    }
                }
                matIndex++;
            }

            _activeMeshes.Add(new MeshRenderInfo
            {
                index = index,
                active = false,
                mesh = child.gameObject,
            });
        }

        foreach (var mesh in _activeMeshes)
        {
            mesh.mesh.SetActive(false);
        }
	}

    private float _touchInterval;
    private int _mapIndex;
    private int _meshIndex;

    private void Update()
    {
        if (!_initialized)
            return;
        _touchInterval += Time.deltaTime;
        //if (Input.GetKeyDown(KeyCode.N) || Input.touchCount > 0 || Input.GetMouseButtonDown(0))
        //{
        //    if (_touchInterval < 0.3f)
        //        return;
        //    _touchInterval = 0;
        //    var meshes = PlanetMeshConfig.instance.GetMeshes();
        //    var mesh = PlanetMeshConfig.instance.GetMesh(meshes[_meshIndex]);
        //    ShowMap(mesh.meshId, mesh.pos);
        //    _mapIndex++;
        //    _mapIndex %= 6;
        //    _meshIndex++;
        //    _meshIndex %= meshes.Count;
        //}
        CppLogger.FlushLog();
    }
    /// <summary>
    /// 隐藏已经显示的mesh
    /// </summary>
    /// <param name="meshId"></param>
    public void HideMap(uint meshId)
    {
        var meshInfo = _activeMeshes.Find((m) => m.active && m.meshId == meshId);
        if (null != meshInfo)
        {
            meshInfo.active = false;
            meshInfo.mesh.SetActive(false);
            meshInfo.meshId = 0;
        }    
    }
    public void FixedUpdate()
    {
        if (_loadingMeshList.Count > 0)
        {
            int size = _loadingMeshList.Count;
            Array.Clear(_loadingMeshes, 0, size);
            Array.Clear(_loadingMeshStates,0, size);
            for (int i = 0; i < size; i++)
            {
                _loadingMeshes[i] = _loadingMeshList[i];
            }
            unsafe
            {
                fixed(uint* ptr1 = _loadingMeshes)
                {
                    fixed (uint* ptr2 = _loadingMeshStates)
                    {
                        CaGetMeshState(ptr1, (uint) size, ptr2);
                    }
                }
            }

            for (int i = 0; i < size; i++)
            {
                if (_loadingMeshStates[i] == 1)
                {
                    int meshIndex = (int)_loadingMeshes[i];
                    if (meshIndex >= _activeMeshes.Count)
                    {
                        Debug.LogError("数据异常,加载的索引超过了缓存");
                        continue;
                    }
                    _activeMeshes[meshIndex].mesh.SetActive(true);
                    Debug.Log($"mesh {_loadingMeshes[i]}, meshId {_activeMeshes[meshIndex].meshId} load success");
                    _loadingMeshList.Remove(_loadingMeshes[i]);
                }
            }
        }
    }
    public void ShowMap(uint meshId, Vector3 position)
    {
        //已经显示了地图
        if (_activeMeshes.Find((m) => m.active && m.meshId == meshId) != null)
        {
            return;
        }
        var meshInfo = _activeMeshes.Find((m) => !m.active);
        if (meshInfo == null)
        {
            Debug.LogError($"fail to show map meshId {meshId}, pool was empty");
            return;
        }

        if (!_loadedMeshes.Contains(meshId))
            CaLoadMesh(meshId);
        meshInfo.meshId = meshId;
        meshInfo.mesh.transform.position = position;
        meshInfo.active = true;
        meshInfo.mesh.SetActive(false);
        _loadingMeshList.Add((uint)meshInfo.index);
        CaBeginModifyMesh((uint)meshInfo.index);
        //LoadMesh(meshId);
        UnityEngine.Profiling.Profiler.BeginSample("ModifyVertexBuffer");
#if UNITY_EDITOR

        CaModifyVertexBuffer(meshInfo.index, meshId, 0);
#else
        Debug.Log($"index {meshInfo.index}, meshId {meshId}");
        GL.IssuePluginEvent(GetRenderEventFunc(), meshInfo.index << 24 | (int) meshId);
#endif
        UnityEngine.Profiling.Profiler.EndSample();
    }

    private readonly List<uint> _loadedMeshes = new List<uint>();
    public void TryLoadMap(uint meshId)
    {
        if (_loadedMeshes.Contains(meshId))
            return;
        _loadedMeshes.Add(meshId);
        CaLoadMesh(meshId);
    }
    private void OnApplicationQuit()
    {
        CaOnApplicationQuit();
        _activeMeshes.Clear();
        CppLogger.FlushLog();
    }
}
