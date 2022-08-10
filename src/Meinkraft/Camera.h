#pragma once

#include <glm/glm.hpp>
#include <array>

struct FrustumPlane
{
	glm::dvec3 position;
	glm::dvec3 normal;
};

class Camera
{
public:
	Camera(glm::dvec3 position = glm::dvec3(0), glm::dvec2 sphericalCoords = glm::dvec2(0));
	
	void update(double deltaTime);
	
	glm::dvec3 getOrientation();
	
	glm::dvec3 getSideOrientation();
	
	glm::mat4 getView();
	
	glm::mat4 getProjection();
	
	glm::dvec3 getPosition() const;
	void setPosition(glm::dvec3 position);
	
	glm::dvec2 getSphericalCoords() const;
	void setSphericalCoords(glm::dvec2 sphericalCoords);
	
	double getSpeed() const;
	void setSpeed(double speed);
	
	double getExposure() const;
	void setExposure(double exposure);
	
	double getFov() const;
	void setFov(double fovX);
	
	std::array<FrustumPlane, 4>& getFrustumPlanes();
	
private:
	static constexpr double NEAR_DISTANCE = 0.02;
	static constexpr double FAR_DISTANCE = 1000.0;
	static constexpr double CHUNK_FRUSTUM_TEST_OFFSET = 13.8564064606; // sqrt(8^2 + 8^2 + 8^2)
	
	bool _orientationChanged = true;
	glm::dvec3 _orientation = glm::dvec3(0);
	glm::dvec3 _sideOrientation = glm::dvec3(0);
	
	bool _viewChanged = true;
	glm::mat4 _view = glm::mat4(0);
	
	bool _projectionChanged = true;
	glm::mat4 _projection = glm::mat4(0);
	
	glm::dvec3 _position;
	
	// x: phi (horizontal) 0 to 360
	// y: theta (vertical) -89 to 89
	glm::dvec2 _sphericalCoords;
	
	glm::dvec2 _previousMousePos;
	
	double _speed = 10;
	double _exposure = 1;
	double _fov;
	
	bool _frustumChanged = true;
	std::array<FrustumPlane, 4> _frustumPlanes;
	
	glm::ivec2 _windowSize;
	
	void recalculateOrientation();
	void recalculateView();
	void recalculateProjection();
	void recalculateFrustumPlanes();
};
