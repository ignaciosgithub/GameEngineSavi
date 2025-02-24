class Raycast
{
public:
    Vector3 start;
    Vector3 direction;
   
    Raycast() : start(0, 0, 0), direction(0, 0, -1) {}
   
    Raycast(Vector3 _start, Vector3 _direction) : start(_start), direction(_direction) {}
};