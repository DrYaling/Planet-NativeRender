using UnityEditor;
using UnityEngine;
namespace Editor
{
    [CustomEditor(typeof(Neighbor))]
    public class NeighborEditor : UnityEditor.Editor
    {
        public override void OnInspectorGUI()
        {
            Neighbor neighbor = target as Neighbor;
            base.OnInspectorGUI();
            if (neighbor != null && GUILayout.Button("重置顶点"))
            {
                neighbor.ResetCors();
            }
        }
    }
}