#include "Renderer.h"

Renderer::Renderer(RenderBuffer* frameBuffer, Scene* RenderScene) :
	frameBuffer(frameBuffer), RenderScene(RenderScene) {
	Camera = RenderScene->Cameras;
	float aspect = (float)this->frameBuffer->width / (float)this->frameBuffer->height;
	Camera->set_aspect(aspect);
	InitializeShaderInfo();
};

void Renderer::InitializeShaderInfo() {
	ShaderInfo[0].des = "6.NormalMap";
	ShaderInfo[0].shader = new NormalMapShader();
	ShaderInfo[1].des = "5.Blinn";
	ShaderInfo[1].shader = new BlinnShader();
	ShaderInfo[2].des = "4.Texture_Light(no shadow)";
	ShaderInfo[2].shader = new TextureWithLightShader();
	ShaderInfo[3].des = "3.Texture(no shadow)";
	ShaderInfo[3].shader = new TextureShader();
	ShaderInfo[4].des = "2.Toon(no shadow)";
	ShaderInfo[4].shader = new ToonShader();
	ShaderInfo[5].des = "1.Ground(no shadow)";
	ShaderInfo[5].shader = new GroundShader();
}

void Renderer::Render()
{
	frameBuffer->renderbuffer_clear_color(Color::Black);
	frameBuffer->renderbuffer_clear_depth(std::numeric_limits<float>::max());

	input_poll_events();
	Matrix4x4 view_matrix = RenderScene->Cameras->get_view_matrix();
	Matrix4x4 projection_matrix = RenderScene->Cameras->get_proj_matrix();
	Matrix4x4 model_matrix = RenderScene->StaticModels->GetModelMatrix();
	Matrix4x4 model_matrix_I = model_matrix.invert();

	shader_data->view_Pos = RenderScene->Cameras->get_position();
	shader_data->light_dir = (RenderScene->Lights->transform.position - RenderScene->Cameras->get_target_position()).normalize();
	shader_data->light_color = LightColor;
	shader_data->ambient = AMBIENT;
	shader_data->model_matrix = model_matrix;
	shader_data->model_matrix_I = model_matrix_I;
	shader_data->view_matrix = view_matrix;
	shader_data->projection_matrix = projection_matrix;
	float aspect = (float)frameBuffer->width / (float)frameBuffer->height;
	shader_data->light_vp_matrix = orthographic(aspect, 1, 0, 5) * lookat(RenderScene->Lights->transform.position, Camera->get_target_position(), Vector3f(0, 1, 0));
	shader_data->camera_vp_matrix = projection_matrix * view_matrix;
	shader_data->enable_shadow = enable_shadow;

	if (enable_shadow)
	{
		RenderShadow();
	}

	DrawData* draw_data = new DrawData;
	draw_data->render_buffer = frameBuffer;
	draw_data->model = RenderScene->StaticModels->StaticModel;
	draw_data->shader = RenderScene->StaticModels->StaticModelShaderInfo->shader;
	shader_data->material = RenderScene->StaticModels->StaticModelShaderInfo->shader->shader_data->material;
	draw_data->shader->shader_data = shader_data;

	graphics_draw_triangle(draw_data);


	if (enable_shadow)
	{
		ClearShadowMap();
	}

	PrintSceneInfo();
}

void Renderer::RenderShadow() {
	if(!ShadowMap)
	ShadowMap = new RenderBuffer(frameBuffer->width, frameBuffer->height);

	if (!ShadowDrawData) {
		ShadowDrawData = new DrawData;
		ShadowDrawData->shader = new ShadowShader();
	}

	ShadowDrawData->model = RenderScene->StaticModels->StaticModel;
	ShadowDrawData->shader->shader_data = shader_data;
	ShadowDrawData->render_buffer = ShadowMap;

	graphics_draw_triangle(ShadowDrawData);
	shader_data->shadow_map = ShadowMap;
};

void Renderer::ClearShadowMap() {
	ShadowMap->renderbuffer_clear_color(Color::Black);
	ShadowMap->renderbuffer_clear_depth(std::numeric_limits<float>::max());
}

void Renderer::PrintSceneInfo() {
	snprintf(text, 500, "");

	char line[50] = "";
	snprintf(line, 50, "camera pos: (%.1f, %.1f, %.1f)\n", Camera->get_position().x, Camera->get_position().y, Camera->get_position().z);
	strcat(text, line);
	snprintf(line, 50, "camera dir: (%.1f, %.1f, %.1f)\n", TO_DEGREES(Camera->get_forward().x), TO_DEGREES(Camera->get_forward().y), TO_DEGREES(Camera->get_forward().z));
	strcat(text, line);
	snprintf(line, 50, "press mouse [Left] to rotate camera\n");
	strcat(text, line);
	snprintf(line, 50, "press mouse [Right] to move camera\n");
	strcat(text, line);
	snprintf(line, 50, "press key [Space] to reset camera\n\n");
	strcat(text, line);


	snprintf(line, 50, "light dir: (%.1f, %.1f, %.1f)\n", TO_DEGREES((RenderScene->Lights->transform.position - Camera->get_target_position()).normalize().x), TO_DEGREES((RenderScene->Lights->transform.position - Camera->get_target_position()).normalize().y), TO_DEGREES((RenderScene->Lights->transform.position - Camera->get_target_position()).normalize().z));
	strcat(text, line);
	snprintf(line, 50, "press key [A] or [D] to rotate light dir\n\n");
	strcat(text, line);

	snprintf(line, 50, "shadow: %s\n", enable_shadow ? "On" : "Off");
	strcat(text, line);
	snprintf(line, 50, "press key [E] to switch shadow\n\n");
	strcat(text, line);

	snprintf(line, 50, "shader: %s\n", ShaderInfo[ShaderIndex].des);
	strcat(text, line);
	snprintf(line, 50, "press key [Q] to switch shader\n\n");
	strcat(text, line);
}

void Renderer::on_key_input(keycode_t key, int pressed) {
	if (pressed)
	{
		switch (key)
		{
		case KEY_A:
			RenderScene->Lights->transform.position = proj<3>(rotate(TO_RADIANS(-5), 0, 1, 1) * embed<4>(RenderScene->Lights->transform.position, 1.f));
			break;
		case KEY_D:
			RenderScene->Lights->transform.position = proj<3>(rotate(TO_RADIANS(5), 0, 1, 1) * embed<4>(RenderScene->Lights->transform.position, 1.f));
			break;
		case KEY_E:
			enable_shadow = !enable_shadow;
			break;
		case KEY_Q:
			ShaderIndex = (ShaderIndex + 1) % 6;
			shader_data = RenderScene->StaticModels->StaticModelShaderInfo->shader->shader_data;
			RenderScene->StaticModels->StaticModelShaderInfo->shader = ShaderInfo[ShaderIndex].shader;
			RenderScene->StaticModels->StaticModelShaderInfo->shader->shader_data = shader_data;
		default:
			break;
		}
	}
}