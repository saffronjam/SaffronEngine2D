#pragma once

#include "Camera.hpp"

class CameraController
{
public:
    CameraController(Graphics &gfx, Camera &camera, class InputUtility &iu);

    void Update(sf::Time dt);

    bool GetEngaged() { return m_engaged; }

private:
    Graphics &m_gfx;
    Camera &m_camera;
    InputUtility &m_iu;

    bool m_engaged;

    float m_zoomFactor;
    float m_rotationSpeed;
};