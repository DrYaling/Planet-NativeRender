using System;
using System.Collections.Generic;
using UnityEditor;
using UnityEngine;
namespace Editor
{
    struct V3: IComparable
    {
        public float x;
        public float y;
        public float z;
        public int CompareTo(object obj)
        {
            var v = (V3)obj;
            var d0 = x * x + y * y + z * z;
            var d1 = v.x * v.x + v.y * v.y + v.z * v.z;
            if (d0 > d1)
                return -1;
            return 1;
        }
    }
    [CustomEditor(typeof(DrawHexasphere))]
    public class HexaSphereEditor : UnityEditor.Editor
    {
        private float scale = 1;
        private Transform transform;
        /// <summary>
        /// 计算mesh顶点数
        /// </summary>
        /// <param name="root"></param>
        /// <param name="boundMesh"></param>
        /// <param name="targetMesh"></param>
        /// <param name="referenceMesh"></param>
        private void BindBound(Transform root, Mesh boundMesh, Mesh targetMesh, Mesh referenceMesh)
        {
            float sqrt3 = Mathf.Sqrt(3);
            //模型空间计算射线
            Matrix4x4 modelMatrix = root.worldToLocalMatrix;
            var col = root.gameObject.AddComponent<MeshCollider>();
            var vertexes = boundMesh.vertices;
            //float a = Vector3.Distance(vertexes[0], vertexes[1]);
            var refVertexes = referenceMesh.vertices;
            var middle = Vector3.zero;
            foreach (var v in vertexes)
            {
                middle += v;
            }

            middle /= (float)vertexes.Length;
            //框处球面的对应顶点和坐标
            var vertexesOnBall = new Vector3[vertexes.Length];
            var dss = new float[vertexes.Length];
            for (int i = 0; i < vertexes.Length; i++)
            {
                dss[i] = float.MaxValue;
            }
            float mds = float.MaxValue;
            Vector3 spMiddle = middle;
            float minX = float.MaxValue;
            foreach (var vertex in vertexes)
            {
                if (minX > vertex.x)
                    minX = vertex.x;
            }
            float maxX = -minX;
            List<V3> newVertexes = new List<V3>(2048*256);
            for (int i = 0; i < refVertexes.Length; i++)
            {
                var vertex = refVertexes[i] * scale;
                RaycastHit hitInfo;
                if (Physics.Raycast(vertex, Vector3.zero - vertex, out hitInfo, float.MaxValue))
                {
                    if (hitInfo.collider.transform == root)
                    {
                        var p = vertex - root.position;
                        newVertexes.Add(new V3{x = p.x, y = p.y, z = p.z});
                    }
                }

                //var local = root.InverseTransformPoint(vertex);
                //var up = root.up;
                //var dis = Vector3.Dot(local, up);
                //local = local - up * dis;
                //var localVertex = (modelMatrix * local);
                //if (a - localVertex.z <= localVertex.x / sqrt3)
                //{
                //    continue;
                //}

                //Debug.Log($"point {vertex} ok");
            }

            newVertexes.Sort();
            int[] indexes = new int[newVertexes.Count * 3];
            int size = newVertexes.Count;
            //int tIndex = 2;
            //indexes[0] = 0;
            //Vector3 one = newVertexes[0];
            //int FindNearest(Vector3 a, Vector3 b)
            //{
            //    float dis = float.MaxValue;
            //    int idx = 0;
            //    for (int i = 0; i < newVertexes.Count; i++)
            //    {
            //        var p = newVertexes[i];
            //        if (p == a || p == b)
            //            continue;
            //        var d = Vector3.Distance(p, a);
            //        if (d < dis)
            //        {
            //            dis = d;
            //            idx = i;
            //        }
            //    }
            //    return idx;
            //}
            //indexes[1] = FindNearest(one, one);
            //Vector3 two = newVertexes[indexes[1]];
            //while (tIndex < indexes.Length)
            //{
            //    indexes[tIndex] = FindNearest(one, two);
            //    one = two;
            //    two = newVertexes[indexes[tIndex]];
            //    tIndex++;
            //}
            newVertexes.Sort();
            for (int i = 0; i < newVertexes.Count; i++)
            {
                indexes[i * 3] = i;
                if (i == size - 1)
                {
                    indexes[i * 3 + 1] = 0;
                    indexes[i * 3 + 2] = 1;
                }
                else if (i == size - 2)
                {
                    indexes[i * 3 + 1] = i + 1;
                    indexes[i * 3 + 2] = 0;
                }
                else
                {
                    indexes[i * 3 + 1] = i + 1;
                    indexes[i * 3 + 2] = i + 2;
                }
            }

            var vtxes = new Vector3[newVertexes.Count];
            for (int i = 0; i < newVertexes.Count; i++)
            {
                var p = newVertexes[i];
                vtxes[i] = new Vector3(p.x,p.y,p.z);
            }
            targetMesh.vertices = vtxes;
            targetMesh.triangles = indexes;
            targetMesh.RecalculateNormals();
        }
        public override void OnInspectorGUI()
        {
            DrawHexasphere sphere = this.target as DrawHexasphere;
            if (null == sphere)
                return;
            transform = sphere.transform;
            base.OnInspectorGUI();
            if (GUILayout.Button("导出mesh"))
            {
                var root = sphere.transform.Find("Root");
                root.localScale = new Vector3(0.9f, 0.9f, 0.9f);
                int index = 0;
                var refMesh = sphere.sphere.mesh;
                scale = sphere.sphere.transform.lossyScale.x;
                refMesh.MarkModified();
                var meshes = sphere.GetMeshes();
                var total = (float) meshes.Count;
                try
                {
                    foreach (var (mesh, name) in sphere.GetMeshes())
                    {
                        EditorUtility.DisplayProgressBar("请稍等", $"正在生成第{index + 1}个mesh", index / total);
                        var meshPath = $"Assets/Planet/diqiu_{index + 1}.asset";
                        var newMesh = new Mesh();
                        var prefabGo = root.GetChild(index).gameObject;
                        BindBound(prefabGo.transform, mesh, newMesh, refMesh);
                        AssetDatabase.CreateAsset(newMesh, meshPath);
                        var prefabPath = $"Assets/Planet/diqiu_{index + 1}.prefab";
                        var meshAssets = AssetDatabase.LoadAssetAtPath<Mesh>(meshPath);
                        prefabGo.GetComponent<MeshFilter>().mesh = meshAssets;
                        PrefabUtility.SaveAsPrefabAsset(prefabGo, prefabPath);
                        index++;
                        throw new Exception("finish");
                    }
                }
                catch (Exception e)
                {
                    Debug.LogException(e);
                }
                finally
                {
                    EditorUtility.ClearProgressBar();
                }

                AssetDatabase.Refresh(ImportAssetOptions.ForceSynchronousImport);
            }
        }
    }
}