/*
 * created by zxb
 * planet mesh config
 */
using UnityEngine;
using System.Collections.Generic;
using System.IO;

namespace Game
{
    [System.Serializable]
    public struct PlanetMeshInfo
    {
        public uint meshId;
        /// <summary>
        /// mesh 类型,0-六边形,1-五边形
        /// </summary>
        public byte meshType;
        public Vector3 pos;
        public uint[] neighbors;
        public Vector3[] cors;

        public float Distance(Vector3 position)
        {
            float min = Vector3.Distance(position, pos);
            foreach (var cor in cors)
            {
                var d = Vector3.Distance(position, cor);
                if (d < min)
                    min = d;
            }
            return min;
        }
    }

    public class PlanetMeshConfig
    {
        private readonly Dictionary<uint, PlanetMeshInfo> _meshes = new Dictionary<uint, PlanetMeshInfo>();
        static PlanetMeshConfig _instance;

        public static PlanetMeshConfig instance
        {
            get
            {
                if (null == _instance)
                {
                    _instance = new PlanetMeshConfig();
                    _instance.Load();
                }
                return _instance;
            }
        }

        public static void Destroy()
        {
            _instance = null;
        }

        public List<uint> GetMeshes()
        {
            List<uint> list = new List<uint>();
            foreach (var kv in _meshes)
            {
                list.Add(kv.Key);
            }
            return list;
        }
        public void Load()
        {
            TextAsset asset = Resources.Load<TextAsset>("PlanetConfigs/config");
            foreach (var text in asset.text.Replace("\r","").Split('\n'))
            {
                if (text.Length <= 5)
                    continue;
                string[] splits = text.Split('\t');
                if (splits.Length != 5)
                    throw new System.Exception($"text {text} invalid");
                uint id = uint.Parse(splits[0].Trim());
                float[] pos = new float[3];
                string[] poss = splits[1].Trim().Split('|');
                for (int i = 0; i < 3; i++)
                {
                    pos[i] = float.Parse(poss[i]);
                }

                string[] neighbors = splits[2].Trim().Split('|');
                List<uint> ns = new List<uint>();
                foreach (var ne in neighbors)
                {
                    ns.Add(uint.Parse(ne));
                }

                string[] cors = splits[4].Trim().Split('|');
                List<Vector3> cs = new List<Vector3>();
                foreach (var cl in cors)
                {
                    var xyz = cl.Split(',');
                    cs.Add(new Vector3(float.Parse(xyz[0]), float.Parse(xyz[1]), float.Parse(xyz[2])));
                }
                _meshes.Add(id, new PlanetMeshInfo
                {
                    meshId = id,
                    pos = new Vector3(pos[0],pos[1],pos[2]),
                    neighbors = ns.ToArray(),
                    meshType = byte.Parse(splits[3].Trim()),
                    cors = cs.ToArray()
                });
            }
        }

        public PlanetMeshInfo GetMesh(uint meshId)
        {
            _meshes.TryGetValue(meshId, out var mesh);
            return mesh;
        }
#if UNITY_EDITOR
        public static void SaveConfig(Dictionary<uint, PlanetMeshInfo> meshes)
        {
            System.Text.StringBuilder builder = new System.Text.StringBuilder();
            foreach (var kv in meshes)
            {
                var value = kv.Value;
                string line = $"{kv.Key}\t{value.pos.x}|{value.pos.y}|{value.pos.z}\t";
                for (int i = 0; i < value.neighbors.Length; i++)
                {
                    if (i != value.neighbors.Length - 1)
                    {
                        line += $"{value.neighbors[i]}|";
                    }
                    else
                    {
                        line += $"{value.neighbors[i]}";
                    }
                }
                line += $"\t{value.meshType}\t";

                for (int i = 0; i < value.cors.Length; i++)
                {
                    var v = value.cors[i];
                    if (i != value.cors.Length - 1)
                    {
                        line += $"{v.x},{v.y},{v.z}|";
                    }
                    else
                    {
                        line += $"{v.x},{v.y},{v.z}";
                    }
                }
                builder.AppendLine(line);
            }

            var filePath = Application.dataPath + "/Resources/PlanetConfigs/config.bytes";
            if (File.Exists(filePath))
                File.Delete(filePath);
            var file = File.Create(filePath, builder.Length);
            file.Write(System.Text.Encoding.UTF8.GetBytes(builder.ToString()), 0, builder.Length);
            file.Flush();
            file.Close();
        }
#endif
    }
}