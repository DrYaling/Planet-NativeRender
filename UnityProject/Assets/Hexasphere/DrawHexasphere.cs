using Code.Hexasphere;
using UnityEngine;
using System.Collections.Generic;

[RequireComponent(typeof(MeshFilter), typeof(MeshRenderer))]
public class DrawHexasphere : MonoBehaviour
{
    [Min(5f)]
    [SerializeField] private float radius = 10f;
    [Range(1, 15)]
    [SerializeField] private int divisions = 4;
    [Range(0.1f, 1f)]
    [SerializeField] private float hexSize = 1f;
    [SerializeField] public MeshFilter sphere;

    //1
    private Mesh _mesh;
    private MeshFilter _meshFilter;
    //2
    private GameObject _rootGO=null;
    public Material _hexagonMat;
    public Material _pentagonMat;
    private Hexasphere _hexasphere;

    private float _oldRadius;
    private int _oldDivisions;
    private float _oldHexSize;
    private float _lastUpdated;

    private void Start()
    {
        _meshFilter = GetComponent<MeshFilter>();
        DrawHexasphereMesh2();
    }

    private void Update()
    {
        _lastUpdated += Time.deltaTime;
        if (_lastUpdated < 1f) return;
        if (Mathf.Abs(_oldRadius - radius) > 0.001f || _oldDivisions != divisions ||
            Mathf.Abs(_oldHexSize - hexSize) > 0.001f)
        {
            DrawHexasphereMesh2();
        }
    }

    private void DrawHexasphereMesh()
    {
        _oldRadius = radius;
        _oldDivisions = divisions;
        _oldHexSize = hexSize;
        _lastUpdated = 0f;
        
        _hexasphere = new Hexasphere(radius, divisions, hexSize);

        _mesh = new Mesh();
        _meshFilter.mesh = _mesh;
        _mesh.vertices = _hexasphere.MeshDetails.Vertices.ToArray();
        _mesh.triangles = _hexasphere.MeshDetails.Triangles.ToArray();
        _mesh.RecalculateNormals();

        _hexasphere.Tiles.ForEach(tile =>
        {
            //tile.Points contains all the points for the given tile
            //tile.Faces contains all the faces for the given tile
            //tile.Neighbours contains all the neighbouring tiles 
        });
        Debug.Log(_hexasphere.Tiles.Count+" "+_hexasphere.MeshDetails.Triangles.Count+" "+_hexasphere.MeshDetails.Vertices.Count);
        //Debug.Log(_hexasphere.ToObj()); //Hexasphere export as waveform .obj file 
        //Debug.Log(_hexasphere.ToJson()); //Hexasphere export as a JSON object
    }

    private void DrawHexasphereMesh2()
    {
        _oldRadius = radius;
        _oldDivisions = divisions;
        _oldHexSize = hexSize;
        _lastUpdated = 0f;

        _hexasphere = new Hexasphere(radius, divisions, hexSize);

        //for temp
        if (_rootGO != null)
        {
            DestroyImmediate(_rootGO);
        }
        _rootGO = new GameObject("Root");
        _rootGO.transform.SetParent(this.transform, false);

        List<Mesh> meshList = _hexasphere.GetMeshList();
        int temp = 0;
        foreach (var m in meshList)
        {
            Vector3 center = Vector3.zero;
            int num = m.vertices.Length;
            foreach (var v in m.vertices)
            {
                center += v;
            }
            if(num>0)
                center = center / num;
            List<Vector3> vList = new List<Vector3>();
            for(int i=0; i<num; ++i)
            {
                Vector3 vec = m.vertices[i];
                vec -= center;
                vList.Add(vec);
            }
            m.SetVertices(vList);

            ++temp;
            GameObject go = new GameObject(temp.ToString());
            var mf = go.AddComponent<MeshFilter>();
            m.RecalculateNormals();
            m.RecalculateBounds();
            mf.mesh = m;
            var mr = go.AddComponent<MeshRenderer>();
            mr.receiveShadows = false;
            mr.shadowCastingMode = UnityEngine.Rendering.ShadowCastingMode.Off;
            mr.material = (temp > 12) ? _hexagonMat : _pentagonMat;

            go.transform.SetParent(_rootGO.transform, false);
            go.transform.localPosition = center;
        }
    }

    public List<(Mesh, string)> GetMeshes()
    {
        List<(Mesh, string)> list = new List<(Mesh, string)>(_rootGO.transform.childCount);
        for (int i = 0; i < _rootGO.transform.childCount; i++)
        {
            var mf = _rootGO.transform.GetChild(i).GetComponent<MeshFilter>();
            if (null != mf)
                list.Add((mf.mesh, mf.gameObject.name));
        }
        return list;
    }
}
