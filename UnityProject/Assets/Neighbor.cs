using System.Collections;
using System.Collections.Generic;
using UnityEngine;
[System.Serializable]
public class NeighborInfo
{
    public Vector3 dir;
    public GameObject neighbor;
}
[ExecuteInEditMode]
public class Neighbor : MonoBehaviour
{
    /// <summary>
    /// 自身Info
    /// </summary>
    NeighborInfo neighborInfo = new NeighborInfo(); 
    public List<NeighborInfo> neighbors;
    public Vector3 logicPosition;
    /// <summary>
    /// 多边形顶点数据
    /// </summary>
    public List<Vector3> cors = new List<Vector3>();

    public List<Vector3> corsL = new List<Vector3>();
    public uint meshId;
    /// <summary>
    /// mesh 类型,0-六边形,1-五边形
    /// </summary>
    public byte meshType;
    private void Awake()
    {
        logicPosition = transform.position;
        meshId = uint.Parse(gameObject.name.Replace("diqiu_", ""));
        for (int i = transform.childCount - 1; i >= 0; i--)
        {
            if(Application.isPlaying)
                Object.Destroy(transform.GetChild(i).gameObject);
            else
                Object.DestroyImmediate(transform.GetChild(i).gameObject);
        }
    }
    private IEnumerator Start()
    {
        yield return new WaitForSeconds(1);
        SetNeighbors();
    }
    private void Update()
    {
        if (null == neighbors)
            return;
        for (int i = 0; i < neighbors.Count; i++)
        {
            if (neighbors[i].neighbor == null)
                continue;

            var dir = (neighbors[i].neighbor.GetComponent<Neighbor>().logicPosition - logicPosition).normalized;
            neighbors[i].dir = dir;
        }
    }
    private void SetNeighbors()
    {
        if (null == neighbors)
            neighbors = new List<NeighborInfo>();
        Transform parent = transform.parent;
        var planets = GetAllPanet(parent);
        Dictionary<Neighbor, float> distances = new Dictionary<Neighbor, float>();

        for (int i = 0; i < planets.Length; i++)
        {
            var planet = planets[i];
            //忽略自身
            if (logicPosition == planet.logicPosition)
                continue;

            //计算两个中心距离，并加入集合
            float distance = (logicPosition - planet.logicPosition).sqrMagnitude;
            distances.Add(planet, distance);
        }

        List<KeyValuePair<Neighbor, float>> lst = new List<KeyValuePair<Neighbor, float>>(distances);
        //自身为六边形时，最近的6个为它的邻区块
        lst.Sort(delegate (KeyValuePair<Neighbor, float> s1, KeyValuePair<Neighbor, float> s2)
        {
            return s1.Value.CompareTo(s2.Value);
        });
        distances.Clear();

        for (int m = 0; m < 6; m++)
        {
            if (lst[m].Value > 4.05000E+07)
                continue;

            var planet = lst[m];
            GameObject neighborObj = planet.Key.gameObject;
            NeighborInfo info = planet.Key.neighborInfo;
            //序列化Obj
            info.neighbor = neighborObj;
            neighbors.Add(info);
        }

        meshType = (byte) (neighbors.Count == 6 ? 0 : 1);
        //var cos = new float[neighbors.Count];
        //for (int i = 0; i < cos.Length; i++)
        //{
        //    cos[i] = 1000;
        //    cors.Add(logicPosition);
        //    corsL.Add(Vector3.zero);
        //}
        var root = GameObject.Find($"PlanetAll").transform;
        var dis = float.MaxValue;
        Transform refMap = null;

        for (int i = 0; i < root.childCount; i++)
        {
            var child = root.GetChild(i);
            var d = Vector3.Distance(logicPosition, child.position);
            if (d < dis)
            {
                dis = d;
                refMap = child;
            }
        }
        var vertexes = refMap.GetComponent<MeshFilter>().sharedMesh.vertices;
        foreach (var vertex in vertexes)
        {
            var pos = refMap.TransformPoint(vertex);
            cors.Add(pos);
        }
        foreach (var v in cors)
        {
            var go = new GameObject(v.ToString());
            go.transform.SetParent(transform, false);
            go.transform.position = v;
        }
    }

    public void ResetCors()
    {
        for (int i = 0; i < transform.childCount; i++)
        {
            cors[i] = transform.GetChild(i).position;
        }
    }
    /// <summary>
    /// 获取节点下所有六边形子节点
    /// </summary>
    /// <returns></returns>
    public Neighbor[] GetAllPanet(Transform parent)
    {
        var planets = parent.GetComponentsInChildren<Neighbor>();

        return planets;
    }
}
