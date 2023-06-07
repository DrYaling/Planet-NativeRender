using System;
using System.IO;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using Game;
using UnityEngine;
using UnityEditor;
namespace Planet
{
    [StructLayout(LayoutKind.Sequential)]
    public struct MeshVertex
    {
        public const int Size = 32;
        /// <summary>
        /// 坐标
        /// </summary>
        public Vector3 vertex;
        /// <summary>
        /// 法线
        /// </summary>
        public Vector3 normal;

        /// <summary>
        /// uv
        /// </summary>
        public Vector2 uv;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct MeshVertexWithColor
    {
        public const int Size = 48;
        /// <summary>
        /// 坐标
        /// </summary>
        public Vector3 vertex;

        /// <summary>
        /// 法线
        /// </summary>
        public Vector3 normal;

        public int color;

        /// <summary>
        /// uv
        /// </summary>
        public Vector2 uv;
    }

    [StructLayout(LayoutKind.Sequential)]
    public struct MeshInfo<T> where T: struct
    {
        /// <summary>
        /// 位置
        /// </summary>
        public Vector3 position;
        /// <summary>
        /// id
        /// </summary>
        public int id;
        /// <summary>
        /// mesh类型,0-单uv不带color,1-单uv带color
        /// </summary>
        public int meshType;
        /// <summary>
        /// 纹理信息
        /// </summary>
        public byte[][] textures;
        /// <summary>
        /// 索引
        /// </summary>
        public ushort[] indexes;
        public T[] vertexes;
    }
    public class PlanetExporter
    {
        //20 bytes HEADER SIZE,fixed header info
        //  0000  |  0     000 |  0 0 0 0     |     0 0 0 0     |           0              0          0         0  
        // version  type   id     vertex count  triangle count      texture count(0-3)  t1 size,   t2 size,  t3 size, t4 size (一个纹理名字长度占3位)
        private const int headerSize = 20;
        /// <summary>
        /// end of mesh
        /// fixed data buffer 
        /// </summary>
        private const int tailSize = 4;

        private static readonly byte[] End = new byte[] {(byte) 'm', (byte) 'e', (byte) 's', (byte) 'h'};

        private static readonly byte[] Version = new byte[] {(byte)'0', (byte) '0', (byte) '0', (byte) '1'};
        private static readonly int SubTexture = Shader.PropertyToID("_subTexture");

        private static void SerializeMesh(byte[] buffer, MeshVertex vertex, ref int index)
        {
            var old = index;
            for (int i = 0; i < 3; i++)
            {
                var value = vertex.vertex[i];
                unsafe
                {
                    byte* pdata = (byte*) &value;
                    byte[] byteArray = new byte[sizeof(float)];
                    for (int x = 0; x < sizeof(float); ++x)
                        buffer[index++] = pdata[x];
                }
            }

            for (int i = 0; i < 3; i++)
            {
                var value = vertex.normal[i];
                unsafe
                {
                    byte* pdata = (byte*) &value;
                    byte[] byteArray = new byte[sizeof(float)];
                    for (int x = 0; x < sizeof(float); ++x)
                        buffer[index++] = pdata[x];
                }
            }

            for (int i = 0; i < 2; i++)
            {
                var value = vertex.uv[i];
                unsafe
                {
                    byte* pdata = (byte*) &value;
                    byte[] byteArray = new byte[sizeof(float)];
                    for (int x = 0; x < sizeof(float); ++x)
                        buffer[index++] = pdata[x];
                }
            }

            Debug.Assert(index - old == MeshVertex.Size, $" differ {index - old}");
        }
        public static byte[] SerializeMesh(MeshInfo<MeshVertex> mesh)
        {
            if (mesh.indexes == null)
            {
                throw new Exception("mesh 索引数据错误");
            }

            if (mesh.vertexes == null)
            {
                throw new Exception("mesh 顶点数据错误");
            }

            int size = headerSize + MeshVertex.Size * mesh.vertexes.Length + mesh.indexes.Length * 2 + tailSize;
            foreach (var texture in mesh.textures)
            {
                if (texture.Length > byte.MaxValue / 4)
                {
                    throw new Exception("纹理的文件名不能超过48,且必须为英文或下划线");
                }
                size += texture.Length;
            }

            Debug.Log($"first {(mesh.vertexes[0].vertex, mesh.vertexes[0].normal, mesh.vertexes[0].uv)}, indexes {(mesh.indexes[0], mesh.indexes[1], mesh.indexes[2], mesh.indexes[3], mesh.indexes[4])}");
            byte[] buffer = new byte[size];
            int index = 0;
            Array.Copy(Version, buffer, Version.Length);
            index += 4;
            // type id
            buffer[index++] = 0;
            buffer[index++] = (byte) ((mesh.id & 0x00ff0000) >> 16);
            buffer[index++] = (byte) ((mesh.id & 0x0000ff00) >> 8);
            buffer[index++] = (byte) (mesh.id & 0x000000ff);
            var vertexCount = mesh.vertexes.Length;
            //vertexCount
            buffer[index++] = (byte) ((vertexCount & 0x00ff0000) >> 24);
            buffer[index++] = (byte) ((vertexCount & 0x00ff0000) >> 16);
            buffer[index++] = (byte) ((vertexCount & 0x0000ff00) >> 8);
            buffer[index++] = (byte) (vertexCount & 0x000000ff);
            var indexCount = mesh.indexes.Length;
            //indexCount
            buffer[index++] = (byte) ((indexCount & 0x00ff0000) >> 24);
            buffer[index++] = (byte) ((indexCount & 0x00ff0000) >> 16);
            buffer[index++] = (byte) ((indexCount & 0x0000ff00) >> 8);
            buffer[index++] = (byte) (indexCount & 0x000000ff);
            //每张纹理占6位,, 纹理名字最多 2^6-2也就是 30个英文字母,其中第一个字母是类型标志,0为main,1为sub
            //mesh
            buffer[index++] = (byte) mesh.textures.Length;
            int textureInfo = 0;

            for (int i = 0; i < mesh.textures.Length; i++)
            {
                textureInfo |= mesh.textures[i].Length << (6 * 3 - i);
            }
            //4张纹理占24位
            buffer[index++] = (byte) ((textureInfo >> 16) & 0xff);
            buffer[index++] = (byte) ((textureInfo >> 8) & 0xff);
            buffer[index++] = (byte) (textureInfo & 0xff);
            for (int i = 0; i < mesh.indexes.Length; i++)
            {
                var value = mesh.indexes[i];
                buffer[index++] = (byte) (value & 0xff);
                buffer[index++] = (byte)((value & 0xff00) >> 8);
            }

            foreach (var vertex in mesh.vertexes)
            {
                SerializeMesh(buffer, vertex, ref index);
            }
            foreach (var texture in mesh.textures)
            {
                foreach (var b in texture)
                {
                    buffer[index++] = b;
                }
            }
            foreach (var value in End)
            {
                buffer[index++] = value;
            }
            return buffer;
        }

        private static bool ParseMesh(Mesh mesh, uint meshId, out MeshInfo<MeshVertex> meshInfo)
        {
            meshInfo = default;
            meshInfo.id = (int)meshId;
            meshInfo.position = Vector3.zero;
            meshInfo.meshType = 0;
            meshInfo.indexes = new ushort[mesh.triangles.Length];
            for (int i = 0; i < mesh.triangles.Length; i++)
            {
                meshInfo.indexes[i] = (ushort)mesh.triangles[i];
            }

            meshInfo.vertexes = new MeshVertex[mesh.vertexCount];
            var vertexes = mesh.vertices;
            var normals = mesh.normals;
            var uvs = mesh.uv;
            for (int i = 0; i < mesh.vertexCount; i++)
            {
                MeshVertex vertex = new MeshVertex();
                vertex.vertex = vertexes[i];
                vertex.normal = normals[i];
                vertex.uv = uvs[i];
                meshInfo.vertexes[i] = vertex;
            }
            return true;
        }
        [MenuItem("Assets/导出Mesh二进制")]
        public static void ExportSelected()
        {
            if (null == Selection.gameObjects)
                return;
            float totalCount = Selection.gameObjects.Length;
            EditorUtility.DisplayCancelableProgressBar("","开始加载",0);
            try
            {
                Dictionary<uint, PlanetMeshInfo> meshes = new Dictionary<uint, PlanetMeshInfo>();
                for (int goIndex = 0; goIndex < Selection.gameObjects.Length; goIndex++)
                {
                    var go = Selection.gameObjects[goIndex];
                    var neighbor = go.GetComponent<Neighbor>();
                    if (null == neighbor)
                        throw new Exception("Neighbor Component is required");
                    PlanetMeshInfo meshNeighbor = new PlanetMeshInfo();
                    meshNeighbor.meshId = neighbor.meshId;
                    meshNeighbor.pos = neighbor.logicPosition;
                    meshNeighbor.neighbors = new uint[neighbor.neighbors.Count];
                    meshNeighbor.meshType = neighbor.meshType;
                    meshNeighbor.cors = neighbor.cors.ToArray();
                    for (int i = 0; i < neighbor.neighbors.Count; i++)
                    {
                        meshNeighbor.neighbors[i] = neighbor.neighbors[i].neighbor.GetComponent<Neighbor>().meshId;
                    }

                    meshes.Add(neighbor.meshId, meshNeighbor);
                    EditorUtility.DisplayCancelableProgressBar("请等候...", "数据导出中", goIndex / totalCount);
                    var mesh = go.GetComponent<MeshFilter>()?.sharedMesh;
                    if (null == mesh)
                        continue;

                    if (ParseMesh(mesh, neighbor.meshId, out var meshInfo))
                    {
                        var render = Selection.activeGameObject.GetComponent<MeshRenderer>();
                        int size = 0;
                        foreach (var mat in render.sharedMaterials)
                        {
                            var main = mat.mainTexture;
                            if (main != null)
                                size += 1;
                            var sub = mat.HasProperty(SubTexture) ? mat.GetTexture(SubTexture) : null;
                            if (sub != null)
                                size += 1;
                        }
                        meshInfo.textures = new byte[size][];
                        if (size > 4)
                        {
                            throw new Exception("当前版本仅支持最多4张贴图");
                        }

                        int index = 0;
                        int matIndex = 0;
                        foreach (var mat in render.sharedMaterials)
                        {
                            if (matIndex >= 2)
                                throw new Exception("仅支持2个材质,且所有mesh均使用这两个材质");
                            var main = mat.mainTexture;
                            var sub = mat.HasProperty(SubTexture) ? mat.GetTexture(SubTexture) : null;
                            if (main != null)
                            {
                                meshInfo.textures[index] = new byte[main.name.Length + 1];
                                //main flag
                                meshInfo.textures[index][0] = matIndex == 0 ? (byte)TextureType.MAIN_TEX_0 : (byte)TextureType.MAIN_TEX_1;
                                for (int i = 0; i < main.name.Length; i++)
                                {
                                    if ((uint)main.name[i] > 255)
                                    {
                                        throw new Exception("仅支持英文纹理名");
                                    }
                                    meshInfo.textures[index][i + 1] = (byte)main.name[i];
                                }

                                var t2d = main as Texture2D;
                                if (null == t2d)
                                    throw new Exception("当前仅支持texture2D");
                                var pixelSize = main.width * main.height;
                                bool alpha = t2d.alphaIsTransparency;
                                int totalSize = pixelSize * (alpha ? 4 : 3);
                                var texBuffer = new byte[totalSize + 4];
                                var texPath = Application.dataPath + "/PlanetData/Textures/" + main.name + ".data";
                                int tIdx = 0;
                                texBuffer[tIdx++] = alpha ? (byte)1 : (byte)0;
                                texBuffer[tIdx++] = (byte)((totalSize >> 16) & 0xff);
                                texBuffer[tIdx++] = (byte)((totalSize >> 8) & 0xff);
                                texBuffer[tIdx++] = (byte)(totalSize & 0xff);
                                for (int j = 0; j < t2d.height; j++)
                                {
                                    for (int i = 0; i < t2d.width; i++)
                                    {
                                        var pixel = (Color32)t2d.GetPixel(i, j);
                                        texBuffer[tIdx++] = pixel[0];
                                        texBuffer[tIdx++] = pixel[1];
                                        texBuffer[tIdx++] = pixel[2];
                                        if (alpha)
                                            texBuffer[tIdx++] = pixel[3];
                                    }
                                }
                                File.WriteAllBytes(texPath, texBuffer);
                                index++;
                            }

                            if (sub != null)
                            {
                                meshInfo.textures[index] = new byte[sub.name.Length + 1];
                                //sub flag
                                meshInfo.textures[index][0] = matIndex == 0 ? (byte)TextureType.SUB_TEX_0 : (byte)TextureType.SUB_TEX_1;
                                for (int i = 0; i < sub.name.Length; i++)
                                {
                                    if ((uint)sub.name[i] > 255)
                                    {
                                        throw new Exception("仅支持英文纹理名");
                                    }

                                    meshInfo.textures[index][i + 1] = (byte)sub.name[i];
                                }

                                index++;
                            }

                            matIndex++;
                        }
                        var bytes = SerializeMesh(meshInfo);
                        if (null == bytes)
                        {
                            Debug.LogError($"mesh {mesh.name} 序列化错误");
                            continue;
                        }
                        var dataPath = Application.dataPath + "/PlanetData/Meshes/" + meshInfo.id + ".data";
                        File.WriteAllBytes(dataPath, bytes);
                        AssetDatabase.Refresh(ImportAssetOptions.ForceSynchronousImport);
                        Resources.UnloadUnusedAssets();
                        GC.Collect();
                    }
                }

                Game.PlanetMeshConfig.SaveConfig(meshes);
            }
            catch (Exception e)
            {
                Console.WriteLine(e);
                throw;
            }
            finally
            {
                EditorUtility.ClearProgressBar();
            }
        }
    }
}