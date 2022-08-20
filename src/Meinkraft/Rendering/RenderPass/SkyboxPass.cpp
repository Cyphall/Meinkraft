#include "SkyboxPass.h"

#include "Meinkraft/Chunk.h"
#include "Meinkraft/World.h"
#include "Meinkraft/ChunkBufferManager.h"
#include "Meinkraft/Toolbox.h"
#include "Meinkraft/GL/ShaderProgramCreateInfo.h"
#include "Meinkraft/GL/ShaderProgram.h"
#include "Meinkraft/Camera.h"

SkyboxPass::SkyboxPass()
{
	// ========== SHADERS ==========
	
	ShaderProgramCreateInfo createInfo;
	createInfo.shadersFiles[GL_VERTEX_SHADER].emplace_back("skybox");
	createInfo.shadersFiles[GL_FRAGMENT_SHADER].emplace_back("skybox");
	
	_skyboxShader = std::make_unique<ShaderProgram>(createInfo);
	
	// ========== VAOS ==========
	
	glCreateVertexArrays(1, &_skyboxVao);
	
	glVertexArrayAttribBinding(_skyboxVao, 0, 0);
	glVertexArrayAttribIFormat(_skyboxVao, 0, 3, GL_BYTE, 0);
	glEnableVertexArrayAttrib(_skyboxVao, 0);
	
	// ========== VBOS ==========
	
	std::vector<glm::i8vec3> skyboxData = {
		{-1,  1, -1},
		{-1, -1, -1},
		{ 1, -1, -1},
		{ 1, -1, -1},
		{ 1,  1, -1},
		{-1,  1, -1},
		
		{-1, -1,  1},
		{-1, -1, -1},
		{-1,  1, -1},
		{-1,  1, -1},
		{-1,  1,  1},
		{-1, -1,  1},
		
		{ 1, -1, -1},
		{ 1, -1,  1},
		{ 1,  1,  1},
		{ 1,  1,  1},
		{ 1,  1, -1},
		{ 1, -1, -1},
		
		{-1, -1,  1},
		{-1,  1,  1},
		{ 1,  1,  1},
		{ 1,  1,  1},
		{ 1, -1,  1},
		{-1, -1,  1},
		
		{-1,  1, -1},
		{ 1,  1, -1},
		{ 1,  1,  1},
		{ 1,  1,  1},
		{-1,  1,  1},
		{-1,  1, -1},
		
		{-1, -1, -1},
		{-1, -1,  1},
		{ 1, -1, -1},
		{ 1, -1, -1},
		{-1, -1,  1},
		{ 1, -1,  1}
	};
	
	glCreateBuffers(1, &_skyboxVbo);
	glNamedBufferStorage(_skyboxVbo, skyboxData.size() * sizeof(glm::i8vec3), skyboxData.data(), 0);
	
	glVertexArrayVertexBuffer(_skyboxVao, 0, _skyboxVbo, 0, sizeof(glm::i8vec3));
}

SkyboxPass::~SkyboxPass()
{
	glDeleteBuffers(1, &_skyboxVbo);
	glDeleteVertexArrays(1, &_skyboxVao);
}

void SkyboxPass::render()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_FALSE);
	
	glBindVertexArray(_skyboxVao);
	
	_skyboxShader->bind();
	
	glm::mat4 mvp =
		Toolbox::camera->getProjection() *
		glm::mat4(glm::mat3(Toolbox::camera->getView()));
	
	_skyboxShader->setUniform("u_mvp", mvp);
	
	glDrawArrays(GL_TRIANGLES, 0, 36);
	
	glDisable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
}