class Pyramid
{
public:
    std::vector<Face> faces;
    Vector3 rotation; // in degrees
    Vector3 position;

    Pyramid() {}

    Pyramid(std::vector<Face> _faces, Vector3 _rotation, Vector3 _position)
     : faces(_faces), rotation(_rotation), position(_position) {}
void rotate(float x_deg, float y_deg, float z_deg);
   void rotatef(float x_deg, float y_deg, float z_deg);

    void translate(float x, float y, float z);

    void draw(Vector3 finalColor);
};