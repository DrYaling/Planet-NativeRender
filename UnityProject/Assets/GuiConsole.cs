using System;
using System.Collections.Generic;
using UnityEngine;

class GuiConsole : MonoBehaviour {

    private static readonly int MAX_LOG = 2000;
    private readonly Queue<string>[] _logList;
    private readonly Vector2[] _scrollPos;
    private readonly string[] _logTypeNames;

    private GuiConsole ( ) {
        this._logTypeNames = Enum.GetNames( typeof( LogType ) );
        this._logList = new Queue<string>[this._logTypeNames.Length];
        this._scrollPos = new Vector2[this._logTypeNames.Length];
        for ( int i = 0; i < _logList.Length; ++i ) {
            this._logList[i] = new Queue<string>( MAX_LOG );
            this._scrollPos[i] = new Vector2( 0, 1 );
        }

        _visible = false;
    }

    void Start ( ) {
        Application.logMessageReceived += LogCallback;
    }

    void Update ( ) {
        if ( Input.GetKeyDown( KeyCode.Home ) ) {
            _visible = !_visible;
        }

        if ( Input.GetKeyDown( KeyCode.End ) ) {
            _show = !_show;
        }
    }

    void Enqueue ( Queue<string> queue, string text, string stackTrace ) {
        while ( queue.Count >= MAX_LOG ) {
            queue.Dequeue( );
        }
        queue.Enqueue( text );
        if ( !string.IsNullOrEmpty( stackTrace ) ) {
            queue.Enqueue( stackTrace );
        }
    }

    void LogCallback ( string condition, string stackTrace, LogType type ) {
        int index = (int)type;
        var queue = this._logList[index];
        switch ( type ) {
            case LogType.Exception:
            case LogType.Error:
            case LogType.Warning:
                Enqueue( queue, condition, stackTrace );
                break;
            default:
                Enqueue( queue, condition, null );
                break;
        }
        this._scrollPos[index] = new Vector2( 0, 100000f );
    }

    private static readonly float EDGE_X = 24, EDGE_Y = 16;
    private int _logTypeChoose = (int)LogType.Log;
    private Rect _rcWindow;
    private bool _visible = true;
	private bool _show =
#if !UNITY_EDITOR
        true
#else
            true
#endif
        ;

    void OnGUI ( ) {

        if ( !_show )
            return;

        GUI.skin = Resources.Load( "ConsoleSkin" ) as GUISkin;

        if ( !_visible ) {
            if ( GUI.Button( new Rect( 240, 120, 200, 80 ), "Console" ) ) {
                _visible = true;
                return;
            }
            return;
        }

        EventType et = Event.current.type;
        if ( et == EventType.Repaint || et == EventType.Layout ) {
            this._rcWindow = new Rect( EDGE_X, EDGE_Y, Screen.width - EDGE_X * 2, Screen.height - EDGE_Y * 2 );
            GUI.Window( 0x1234, _rcWindow, WindowFunc, string.Empty );
        }
    }

    void WindowFunc ( int id ) {
        try {
            if ( GUILayout.Button( "关闭", new GUIStyle()
            {
                fontSize = 36
            }, GUILayout.Height( 50 ) ) ) {
                _visible = false;
            }

            GUILayout.BeginVertical( );
            try {
                _logTypeChoose = GUILayout.Toolbar( _logTypeChoose, this._logTypeNames, new GUIStyle()
                {
                    fontSize = 36
                });
                var queue = this._logList[_logTypeChoose];
                if ( queue.Count > 0 ) {
                    _scrollPos[_logTypeChoose] = GUILayout.BeginScrollView( _scrollPos[_logTypeChoose] );
                    try {
                        foreach ( var s in queue ) {
                            GUILayout.Label( s , new GUIStyle()
                            {
                                fontSize = 36
                            });
                        }
                    } finally {
                        GUILayout.EndScrollView( );
                    }
                }
            } finally {
                GUILayout.EndVertical( );
            }
        } catch ( Exception ex ) {
            UnityEngine.Debug.LogException( ex );
        }
    }

}

