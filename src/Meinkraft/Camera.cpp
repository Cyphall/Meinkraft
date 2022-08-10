#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Toolbox.h"

glm::dvec3 Camera::getOrientation()
{
	if (_orientationChanged) recalculateOrientation();
	return _orientation;
}

glm::dvec3 Camera::getSideOrientation()
{
	if (_orientationChanged) recalculateOrientation();
	return _sideOrientation;
}

glm::mat4 Camera::getView()
{
	if (_viewChanged) recalculateView();
	return _view;
}

glm::mat4 Camera::getProjection()
{
	if (_projectionChanged) recalculateProjection();
	return _projection;
}

glm::dvec3 Camera::getPosition() const
{
	return _position;
}

void Camera::setPosition(glm::dvec3 position)
{
	_position = position;
	_viewChanged = true;
	_frustumChanged = true;
}

glm::dvec2 Camera::getSphericalCoords() const
{
	return _sphericalCoords;
}

void Camera::setSphericalCoords(glm::dvec2 sphericalCoords)
{
	_sphericalCoords = sphericalCoords;
	_orientationChanged = true;
	_viewChanged = true;
	_frustumChanged = true;
}

double Camera::getSpeed() const
{
	return _speed;
}

void Camera::setSpeed(double speed)
{
	_speed = speed;
}

double Camera::getExposure() const
{
	return _exposure;
}

void Camera::setExposure(double exposure)
{
	_exposure = exposure;
}

double Camera::getFov() const
{
	return _fov;
}

void Camera::setFov(double fovX)
{
	_fov = fovX;
	_projectionChanged = true;
	_frustumChanged = true;
}

Camera::Camera(glm::dvec3 position, glm::dvec2 sphericalCoords):
_position(position), _sphericalCoords(sphericalCoords)
{
	glfwGetCursorPos(Toolbox::window, &_previousMousePos.x, &_previousMousePos.y);
	glfwGetWindowSize(Toolbox::window, &_windowSize.x, &_windowSize.y);
	setFov(100);
}

void Camera::update(double deltaTime)
{
	double ratio = deltaTime * _speed;
	
	if (glfwGetKey(Toolbox::window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
	{
		ratio /= 20;
	}
	if (glfwGetKey(Toolbox::window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
	{
		ratio *= 10;
	}
	
	if (glfwGetKey(Toolbox::window, GLFW_KEY_W) == GLFW_PRESS)
	{
		setPosition(getPosition() + getOrientation() * ratio);
	}
	if (glfwGetKey(Toolbox::window, GLFW_KEY_S) == GLFW_PRESS)
	{
		setPosition(getPosition() - getOrientation() * ratio);
	}
	if (glfwGetKey(Toolbox::window, GLFW_KEY_A) == GLFW_PRESS)
	{
		setPosition(getPosition() + getSideOrientation() * ratio);
	}
	if (glfwGetKey(Toolbox::window, GLFW_KEY_D) == GLFW_PRESS)
	{
		setPosition(getPosition() - getSideOrientation() * ratio);
	}
	
	glm::dvec2 currentMousePos;
	glfwGetCursorPos(Toolbox::window, &currentMousePos.x, &currentMousePos.y);
	
	glm::dvec2 mouseOffset = currentMousePos - _previousMousePos;
	setSphericalCoords(getSphericalCoords() - mouseOffset / 12.0);
	
	_previousMousePos = currentMousePos;
}

void Camera::recalculateOrientation()
{
	_sphericalCoords = glm::dvec2(
		glm::mod(_sphericalCoords.x, 360.0),
		glm::clamp(_sphericalCoords.y, -89.0, 89.0)
	);
	
	glm::dvec2 sphericalCoordsRadians = glm::radians(_sphericalCoords);
	
	_orientation = glm::dvec3(
		glm::cos(sphericalCoordsRadians.y) * glm::sin(sphericalCoordsRadians.x),
		glm::sin(sphericalCoordsRadians.y),
		glm::cos(sphericalCoordsRadians.y) * glm::cos(sphericalCoordsRadians.x)
	);
	
	_sideOrientation = glm::normalize(glm::cross(glm::dvec3(0, 1, 0), _orientation));
	
	_orientationChanged = false;
}

void Camera::recalculateView()
{
	_view = glm::lookAt(getPosition(), getPosition() + getOrientation(), glm::dvec3(0, 1, 0));
	
	_viewChanged = false;
}

std::array<FrustumPlane, 4>& Camera::getFrustumPlanes()
{
	if (_frustumChanged) recalculateFrustumPlanes();
	return _frustumPlanes;
}

void Camera::recalculateProjection()
{
	double aspect = (double)_windowSize.x / (double)_windowSize.y;
	double fovY = 2.0 * glm::atan(glm::tan(glm::radians(_fov) * 0.5) / aspect);
	_projection = glm::perspective(fovY, aspect, NEAR_DISTANCE, FAR_DISTANCE);
	
	_projectionChanged = false;
}

void Camera::recalculateFrustumPlanes()
{
	double aspect = (double)_windowSize.x / (double)_windowSize.y;
	double fovY = 2.0 * glm::atan(glm::tan(glm::radians(_fov) * 0.5) / aspect);
	
	glm::dvec2 nearHalfSize;
	nearHalfSize.y = glm::tan(fovY / 2) * NEAR_DISTANCE;
	nearHalfSize.x = nearHalfSize.y * aspect;
	
	glm::dvec3 X = getSideOrientation();
	glm::dvec3 Z = getOrientation();
	glm::dvec3 Y = glm::cross(X, Z);
	
	glm::dvec3 pos = getPosition();
	
	glm::dvec3 nearCenter = pos + Z * NEAR_DISTANCE;
	
	_frustumPlanes[0] = {.position = pos, .normal = glm::cross(Y, glm::normalize((nearCenter + X * nearHalfSize.x) - pos))}; // Left plane
	_frustumPlanes[1] = {.position = pos, .normal = glm::cross(glm::normalize((nearCenter - X * nearHalfSize.x) - pos), Y)}; // Right plane
	
	_frustumPlanes[2] = {.position = pos, .normal = glm::cross(X, glm::normalize((nearCenter - Y * nearHalfSize.y) - pos))}; // Bottom plane
	_frustumPlanes[3] = {.position = pos, .normal = glm::cross(glm::normalize((nearCenter + Y * nearHalfSize.y) - pos), X)}; // Top plane
	
	_frustumChanged = false;
}
