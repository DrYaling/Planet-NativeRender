using System.Collections.Generic;
using System.IO;
using UnityEditor;
using UnityEngine;
namespace Editor
{
    public class ExploretMesh
    {
        [MenuItem("Assets/导出Mesh")]
        static void Export()
        {
            if (null == Selection.activeGameObject)
                return;
            var mesh = Selection.activeGameObject.GetComponent<MeshFilter>()?.sharedMesh;
            if (null == mesh)
                return;
            List<Vector3> vs = new List<Vector3>();
            mesh.GetVertices(vs);
            var new_mesh = new Mesh();
            new_mesh.vertices = vs.ToArray();
            mesh.GetNormals(vs);
            new_mesh.normals = vs.ToArray();
            List<Vector2> ns = new List<Vector2>();
            mesh.GetUVs(0,ns);
            var cs = new List<Color>();
            mesh.GetColors(cs);
            new_mesh.colors = cs.ToArray();
            new_mesh.uv = ns.ToArray();
            var ts = mesh.triangles;
            new_mesh.triangles = ts;
            new_mesh.Optimize();
            new_mesh.MarkDynamic();
            var name = "Assets/Resources/Meshes/" + mesh.name + "_export.asset";
            for (int i = 0; i < 1000; i++)
            {
                var ni = Application.dataPath + "/Resources/Meshes/" + mesh.name + "_export"+ i.ToString() + ".asset";
                if (!File.Exists(ni))
                {
                    name = "Assets/Resources/Meshes/" + mesh.name + "_export" + i.ToString() + ".asset";
                    break;
                }
            }
            AssetDatabase.CreateAsset(new_mesh, name);
            AssetDatabase.Refresh(ImportAssetOptions.ForceUpdate);
        }
    }
}