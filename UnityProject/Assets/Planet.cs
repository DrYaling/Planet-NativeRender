using UnityEngine;
using System.Collections;
using System.Collections.Generic;
using  Game;
//using UnityEditor;

//[ExecuteInEditMode]
public class Planet : MonoBehaviour
{

	public struct Coeff
	{
		public float factor;
		public int period;
		public Coeff(float factor, int period) { this.factor = factor; this.period = period; }
	}

    [System.NonSerialized]
	public float mass = 10.0f;
	[System.NonSerialized]
	public float baseRadius = 10000;

    [SerializeField] private float fieldOfView = 1000;
    [SerializeField] private float hideView = 5000;
    [SerializeField]
    UseRenderingPlugin plugin;
    PlanetMeshInfo _currentMesh;
    private List<PlanetMeshInfo> _previousActiveNeighbors = new List<PlanetMeshInfo>();
    private void Awake()
    {
        mass = 9999999.0f;
        //baseRadius = GetComponent<SphereCollider>().radius * transform.lossyScale.x;

    }

    private void LoadCurrentMap(Vector3 pos)
    {
        float minDis = float.MaxValue;
        uint nearestMesh = 0;
        foreach (var meshId in PlanetMeshConfig.instance.GetMeshes())
        {
            var mesh = PlanetMeshConfig.instance.GetMesh(meshId);
            if (mesh.meshId > 0)
            {
                var dis = Vector3.Distance(mesh.pos, pos);
                if (dis < minDis)
                {
                    minDis = dis;
                    nearestMesh = meshId;
                }
            }
        }

        if (nearestMesh == 0)
        {
            throw new System.Exception($"fail!no map found for pos {pos}");
        }
        _currentMesh = PlanetMeshConfig.instance.GetMesh(nearestMesh);
        plugin.ShowMap(_currentMesh.meshId, _currentMesh.pos);
    }
    /// <summary>
    /// 更新当前格子
    /// </summary>
    /// <param name="pos"></param>
    private void UpdateCurrentMap(Vector3 pos)
    {
        float minDis = Vector3.Distance(pos, _currentMesh.pos);
        uint nearestMesh = 0;
        foreach (var meshId in _currentMesh.neighbors)
        {
            var mesh = PlanetMeshConfig.instance.GetMesh(meshId);
            if (mesh.meshId > 0)
            {
                var dis = Vector3.Distance(mesh.pos, pos);
                if (dis < minDis)
                {
                    minDis = dis;
                    nearestMesh = meshId;
                }
            }
        }

        if (nearestMesh != 0)
        {
            //切换地图时记录上个地图的邻居
            foreach (var neighbor in _currentMesh.neighbors)
            {
                var neighborInfo = PlanetMeshConfig.instance.GetMesh(neighbor);
                if (neighborInfo.meshId > 0)
                {
                    _previousActiveNeighbors.Add(neighborInfo);
                }
            }
            _currentMesh = PlanetMeshConfig.instance.GetMesh(nearestMesh);
            //移除相同邻居地图
            _previousActiveNeighbors.RemoveAll((n) =>
            {
                foreach (var mid in _currentMesh.neighbors)
                {
                    if (mid == n.meshId)
                        return true;
                }
                return false;
            });
        }
    }
    public void UpdatePlayer(PlayControl player)
    {
        if (null == plugin || !plugin.initialized)
            return;
        var playerTransform = player.transform;
        var position = playerTransform.position;
        var forward = playerTransform.forward;
        if (_currentMesh.meshId == 0)
        {
            //show current mesh
            LoadCurrentMap(position);
        }
        else
        {
            UpdateCurrentMap(position);
        }

        for (int i = _previousActiveNeighbors.Count - 1; i >= 0; i--)
        {
            var neighborInfo = _previousActiveNeighbors[i];
            var dis = neighborInfo.Distance(position);
            //一旦隐藏了,就从列表移除
            if (dis > hideView)
            {
                plugin.HideMap(neighborInfo.meshId);
                _previousActiveNeighbors.RemoveAt(i);
            }
        }
        //先不考虑方向,只考虑周围
        foreach (var neighbor in _currentMesh.neighbors)
        {
            var neighborInfo = PlanetMeshConfig.instance.GetMesh(neighbor);
            if (neighborInfo.meshId > 0)
            {
                var dis = neighborInfo.Distance(position);
                if (dis <= fieldOfView)
                {
                    plugin.ShowMap(neighbor, neighborInfo.pos);
                }
                else if (dis < fieldOfView + 2000)
                {
                    plugin.TryLoadMap(neighbor);
                }
                else if (dis > hideView)
                {
                    plugin.HideMap(neighbor);
                }
            }
        }
    }
    
}