class MonoBehaviourLike
{
public:
    // Lifecycle events
    virtual void Awake() {}
    virtual void Start() {}
    virtual void FixedUpdate() {}
    virtual void Update() {}
    virtual void LateUpdate() {}
    virtual void OnDestroy() {}

    // Render-related methods
    virtual void OnGUI() {}
    virtual void OnRenderObject() {}

    // Collision-related events
    virtual void OnCollisionEnter() {}
    virtual void OnCollisionExit() {}
    virtual void OnCollisionStay() {}

    // Trigger-related events
    virtual void OnTriggerEnter() {}
    virtual void OnTriggerExit() {}
    virtual void OnTriggerStay() {}

    // Other common MonoBehaviour methods can be added here, such as:
    virtual void OnEnable() {}
    virtual void OnDisable() {}

    // ... and many others depending on the level of detail you need

    // Destructor to handle cleanup if necessary
    virtual ~MonoBehaviourLike() {}
};