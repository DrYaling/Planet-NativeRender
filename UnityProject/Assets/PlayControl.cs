using System.Globalization;
using UnityEngine;
using UnityEngine.UI;
namespace Game
{
    //[ExecuteInEditMode]
    public class PlayControl: MonoBehaviour
    {
        private Gravity _gravity;
        private Vector2 _lastVelocity;

        [SerializeField] private float speed;
        [SerializeField] private Planet planet;
        private float time = 0;
        private bool _start = false;
        [SerializeField]private Text text;
        public Vector3 forward { get; private set; }
        private Vector3 origin;
        private void Awake()
        {
            if (null == planet)
                planet = GameObject.FindObjectOfType<Planet>();
            forward = transform.forward;
            origin = transform.position;
            //transform.position = planet.transform.position + new Vector3(0, planet.baseRadius + 10, 0);
        }
        private void Start()
        {
            _gravity = new Gravity(planet) {distance = planet.baseRadius + 10};
        }

        private float _tick = 0;
        private int _tickSec = 0;
        private void Update()
        {
            _tick += Time.deltaTime;
            if (Input.GetKey(KeyCode.W) || Input.touchCount > 0)
            {
                if (Input.GetKey(KeyCode.A))
                {
                    _gravity.MoveTurn(this, -5, speed * Time.deltaTime, out var f);
                }
                else if (Input.GetKey(KeyCode.D))
                {
                    _gravity.MoveTurn(this, 5, speed * Time.deltaTime, out var f);
                }
                else
                {
                    _gravity.MoveStrait(this, speed * Time.deltaTime, out var f);
                    forward = f;
                }

            }

            if (_start)
            {
                time += Time.deltaTime;
                text.text = time.ToString(CultureInfo.InvariantCulture);
            }

            planet.UpdatePlayer(this);
            int tick = (int) _tick;
            if (tick != _tickSec && tick % 5 == 0)
            {
                UseRenderingPlugin.CaPrintRenderMemoryUsed();
            }
            _tickSec = tick;
        }
        private void OnCollisionEnter(Collision collision)
        {
            Debug.Log("OnCollisionEnter ");
        }
        private void OnTriggerEnter(Collider collision)
        {
            Debug.Log("OnTriggerEnter ");
            if (collision.GetComponent<Collider>().name == "Plane")
            {
                if (_start)
                {
                    Debug.Log($"cost time {time}");
                    time = 0;
                    _start = false;
                }
                else
                {
                    _start = true;
                    time = 0;
                    text.text = "0";
                }
            }
        }
    }
}