#pragma once
#include "Component/Component.h"
#include "Core/ShaderProgram.h"
#include <Core/Texture.h>
#include "Component/Camera.h"
#include "Core/Application.h"


Skybox::Skybox()
{
	shader = new ShaderProgram(::Application::contentPath / "Shaders/Skybox/skybox.vs", ::Application::contentPath / "Shaders/Skybox/skybox.fs");

	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}


void Skybox::Draw(Camera* camera)
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindVertexArray(0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	shader->Use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapTex->ID);
	shader->SetUniform("cubeMap", 0);

	auto view = glm::mat4(glm::mat3(camera->GetViewMatrix()));
	auto proj = camera->GetProjection();

	shader->SetUniform("view", view);
	shader->SetUniform("projection", proj);

	glDrawArrays(GL_TRIANGLES, 0, 36);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void Skybox::SetCubeMap(Texture* tex)
{
	this->cubeMapTex = tex;
}
Texture* Skybox::GetCubeMap() { return cubeMapTex; }

