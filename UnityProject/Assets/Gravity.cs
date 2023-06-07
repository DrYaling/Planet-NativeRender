using UnityEngine;

namespace Game
{
    public class Gravity
    {
        private readonly Planet _planet;
        private Vector2 _gravity;
        public AnimationCurve gravityCurve = new AnimationCurve(); // evolution of gravity for the distance
        public float maxGravityDist = 1f; // ratio between gravity's max distance and planet's radius
        public double distance = 0;
        private Gravity()
        {
        }

        public Gravity(Planet planet)
        {
            _planet = planet;
        }
        /// <summary>
        /// 计算重力方向和forward,仅在直线移动生效
        /// </summary>
        /// <param name="player"></param>
        /// <param name="deltaDistance"></param>
        /// <param name="forward"></param>
        /// <returns></returns>
        public Vector3 MoveStrait(PlayControl player, float deltaDistance, out Vector3 forward)
        {
            forward = player.forward;
            var oldForward = forward;
            Transform transform = player.transform;
            var position = transform.position;
            var planetPosition = _planet.transform.position;
            var down = (planetPosition - position).normalized;
            var up = -down;
            //在移动过程中,右方是不会改变的
            var right = Vector3.Cross(forward, up);

            forward = Vector3.Cross(right, down).normalized;
            position += forward * deltaDistance;
            //position = up * _planet.radiusDiff + planetPosition + new Vector3(0, 10, 0);
            var dis = Vector3.Distance(position, planetPosition);
            if (dis > distance + 1.5 || dis < distance - 1.5)
            {
                var old = position;
                position = (position - planetPosition).normalized * (float)distance + planetPosition;
                Debug.Log($"dis {dis}, expected {distance}, position: old {old}, new {position}, cur {transform.position}");
            }

            //重新计算上方和前方
            down = (planetPosition - position).normalized;
            up = -down;
            forward = Vector3.Cross(right, down).normalized;
            if (Vector3.Dot(oldForward, forward) > 15)
            {
                Debug.LogError($"forward error!{oldForward}, {forward}, degree {Vector3.Dot(oldForward, forward)}");
            }
            transform.LookAt(position, up);

            transform.position = position;
            //前方为
            return _gravity;
        }

        /// <summary>
        /// 计算重力方向和forward,转弯生效
        /// </summary>
        /// <param name="player"></param>
        /// <param name="turnAngle"></param>
        /// <param name="deltaDistance"></param>
        /// <param name="forward"></param>
        /// <returns></returns>
        public Vector3 MoveTurn(PlayControl player, float turnAngle, float deltaDistance, out Vector3 forward)
        {
            //求近似值
            //
            forward = player.forward;
            Transform transform = player.transform;
            var position = transform.position;
            var planetPosition = _planet.transform.position;
            Vector3 df = deltaDistance * Mathf.Cos(turnAngle) * forward;
            Vector3 posDiff = planetPosition - position;
            var down = posDiff.normalized;
            var up = -down;
            var right = Vector3.Cross(forward, up);
            Vector3 da = deltaDistance * Mathf.Sin(turnAngle) * right;
            Vector3 newForward = (df + da).normalized;
            Vector3 newGravity = new Vector3();
            //如果是多个星球,则求和即可
            float planetDis = posDiff.magnitude;
            float factor = 1;//_planet.mass * gravityCurve.Evaluate((distance - _planet.baseRadius) / (maxGravityDist * _planet.baseRadius));
            newGravity += new Vector3(posDiff.x, posDiff.y, posDiff.z) / planetDis * factor;
            _gravity = newGravity;
            right = Vector3.Cross(newForward, up);
            forward = Vector3.Cross(right, newGravity).normalized;
            position += forward * deltaDistance;
            var dis = Vector3.Distance(position, planetPosition);
            if (dis > planetDis + 1.5 || dis < planetDis - 1.5)
            {
                var old = position;
                position = (position - planetPosition).normalized * (float) planetDis + planetPosition;
                Debug.Log(
                    $"dis {dis}, expected {planetDis}, position: old {old}, new {position}, cur {transform.position}");
            }

            //重新计算上方和前方
            down = (planetPosition - position).normalized;
            up = -down;
            forward = Vector3.Cross(right, down).normalized;
            transform.LookAt(position, up);

            transform.position = position;
            //前方为
            return _gravity;
        }
    }
}